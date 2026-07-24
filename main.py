"""Benchmark nanobind wrapper allocation with and without instance pooling.

Build PyKraken once with ``KRAKEN_PYTHON_POOL_CAPACITY=0`` and save that
result as the baseline. Rebuild with the default capacity of 128, then pass the
baseline back to this script to produce a release-note-ready comparison. Use
``uv sync --reinstall-package kraken-engine`` after changing the CMake option
so the benchmark imports the newly built extension.
"""

from __future__ import annotations

import argparse
import gc
import json
import math
import platform
import statistics
import sys
import timeit
from collections.abc import Callable
from datetime import UTC, datetime
from pathlib import Path
from typing import Any

import pykraken as kn
import pykraken._pykraken as native


Factory = Callable[[], object]


def benchmark_cases() -> dict[str, Factory]:
    point_a = kn.Vec2(1.0, 2.0)
    point_b = kn.Vec2(3.0, 4.0)

    return {
        "Vec2": lambda: kn.Vec2(1.0, 2.0),
        "PolarCoordinate": lambda: kn.PolarCoordinate(0.5, 10.0),
        "Color": lambda: kn.Color(32, 64, 128, 255),
        "Transform": lambda: kn.Transform(),
        "Rect": lambda: kn.Rect(1.0, 2.0, 3.0, 4.0),
        "Circle": lambda: kn.Circle(point_a, 5.0),
        "Capsule": lambda: kn.Capsule(point_a, point_b, 2.0),
        "Line": lambda: kn.Line(point_a, point_b),
        "Vertex": lambda: kn.Vertex(point_a),
    }


def measure(factory: Factory, iterations: int, repeat: int, warmup: int) -> dict[str, Any]:
    for _ in range(warmup):
        factory()

    timer = timeit.Timer(factory)
    was_enabled = gc.isenabled()
    gc.disable()
    try:
        samples = timer.repeat(repeat=repeat, number=iterations)
    finally:
        if was_enabled:
            gc.enable()

    samples_ns = [elapsed * 1e9 / iterations for elapsed in samples]
    median_ns = statistics.median(samples_ns)
    return {
        "best_ns": min(samples_ns),
        "median_ns": median_ns,
        "million_per_second": 1e3 / median_ns,
        "samples_ns": samples_ns,
    }


def metadata(iterations: int, repeat: int, warmup: int) -> dict[str, Any]:
    try:
        import nanobind

        nanobind_version = nanobind.__version__
    except ImportError:
        nanobind_version = None

    return {
        "timestamp_utc": datetime.now(UTC).isoformat(),
        "python": platform.python_version(),
        "implementation": platform.python_implementation(),
        "platform": platform.platform(),
        "nanobind": nanobind_version,
        "extension": str(Path(native.__file__).resolve()),
        "pool_capacity": native.__pool_capacity__,
        "iterations": iterations,
        "repeat": repeat,
        "warmup": warmup,
    }


def load_result(path: Path) -> dict[str, Any]:
    with path.open(encoding="utf-8") as file:
        result = json.load(file)

    if result.get("schema_version") != 1:
        raise ValueError(f"Unsupported benchmark result in {path}")
    return result


def compare(current: dict[str, Any], baseline: dict[str, Any]) -> dict[str, Any]:
    baseline_info = baseline["metadata"]
    current_info = current["metadata"]
    if baseline_info["pool_capacity"] == current_info["pool_capacity"]:
        raise ValueError(
            "Baseline and current results use the same pool capacity; rebuild "
            "PyKraken with a different KRAKEN_PYTHON_POOL_CAPACITY"
        )

    for setting in ("iterations", "repeat", "warmup"):
        if baseline_info[setting] != current_info[setting]:
            raise ValueError(
                f"Baseline uses {setting}={baseline_info[setting]}, but the current "
                f"run uses {current_info[setting]}"
            )

    names = current["results"].keys() & baseline["results"].keys()
    if not names:
        raise ValueError("The benchmark results do not contain any matching cases")

    cases = {}
    ratios = []
    for name in current["results"]:
        if name not in names:
            continue
        before_ns = baseline["results"][name]["median_ns"]
        after_ns = current["results"][name]["median_ns"]
        speedup = before_ns / after_ns
        ratios.append(speedup)
        cases[name] = {
            "baseline_median_ns": before_ns,
            "current_median_ns": after_ns,
            "speedup": speedup,
            "percent_faster": (speedup - 1.0) * 100.0,
        }

    geometric_mean = math.exp(statistics.fmean(math.log(ratio) for ratio in ratios))
    return {
        "baseline_pool_capacity": baseline_info["pool_capacity"],
        "current_pool_capacity": current_info["pool_capacity"],
        "geometric_mean_speedup": geometric_mean,
        "geometric_mean_percent_faster": (geometric_mean - 1.0) * 100.0,
        "cases": cases,
    }


def print_results(result: dict[str, Any]) -> None:
    info = result["metadata"]
    print(
        f"Python {info['python']} | nanobind {info['nanobind'] or 'unknown'} | "
        f"pool capacity {info['pool_capacity']}"
    )
    print(f"{info['iterations']:,} constructions × {info['repeat']} repeats per case\n")

    comparison = result.get("comparison")
    if comparison is None:
        print(f"{'Case':<20} {'Best (ns)':>12} {'Median (ns)':>14} {'M objects/s':>14}")
        print("-" * 64)
        for name, values in result["results"].items():
            print(
                f"{name:<20} {values['best_ns']:>12.2f} "
                f"{values['median_ns']:>14.2f} {values['million_per_second']:>14.2f}"
            )
        return

    before_capacity = comparison["baseline_pool_capacity"]
    after_capacity = comparison["current_pool_capacity"]
    print(
        f"{'Case':<20} {f'Pool {before_capacity} (ns)':>16} "
        f"{f'Pool {after_capacity} (ns)':>16} {'Speedup':>10}"
    )
    print("-" * 66)
    for name, values in comparison["cases"].items():
        print(
            f"{name:<20} {values['baseline_median_ns']:>16.2f} "
            f"{values['current_median_ns']:>16.2f} {values['speedup']:>9.2f}×"
        )

    speedup = comparison["geometric_mean_speedup"]
    percent = comparison["geometric_mean_percent_faster"]
    print(f"\nGeometric-mean speedup: {speedup:.2f}× ({percent:.1f}% faster)")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Benchmark PyKraken's pooled nanobind value objects.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""Example:
  CMAKE_ARGS=-DKRAKEN_PYTHON_POOL_CAPACITY=0 \\
      uv sync --reinstall-package kraken-engine
  uv run --no-sync python main.py --output benchmark-unpooled.json

  CMAKE_ARGS=-DKRAKEN_PYTHON_POOL_CAPACITY=128 \\
      uv sync --reinstall-package kraken-engine
  uv run --no-sync python main.py --baseline benchmark-unpooled.json \\
      --output benchmark-pooled.json
""",
    )
    parser.add_argument("--iterations", type=int, default=1_000_000)
    parser.add_argument("--repeat", type=int, default=7)
    parser.add_argument("--warmup", type=int, default=10_000)
    parser.add_argument("--baseline", type=Path)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()

    if args.iterations < 1 or args.repeat < 1 or args.warmup < 0:
        parser.error("iterations/repeat must be positive and warmup must be non-negative")
    return args


def main() -> int:
    args = parse_args()
    result = {
        "schema_version": 1,
        "metadata": metadata(args.iterations, args.repeat, args.warmup),
        "results": {
            name: measure(factory, args.iterations, args.repeat, args.warmup)
            for name, factory in benchmark_cases().items()
        },
    }

    if args.baseline:
        result["comparison"] = compare(result, load_result(args.baseline))

    print_results(result)
    if args.output:
        with args.output.open("w", encoding="utf-8") as file:
            json.dump(result, file, indent=2)
            file.write("\n")
        print(f"\nSaved result to {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
