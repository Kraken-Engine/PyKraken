#!/usr/bin/env python3
"""
build.py — configure/build PyKraken with CMake (no `pip install .`).

Typical usage:
  python build.py
  python build.py --config Debug
  python build.py --build-dir build
  python build.py -- --trace-expand   # passes extra args to CMake configure step

Notes:
- On MSVC multi-config generators (Visual Studio), --config matters for build.
- On Ninja/Makefile single-config generators, --config is mostly ignored (safe to pass).
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import List


def run(cmd: List[str], cwd: Path | None = None) -> None:
    print("\n>", " ".join(cmd))
    subprocess.run(cmd, cwd=str(cwd) if cwd else None, check=True)


def is_windows() -> bool:
    return os.name == "nt"


def default_generator() -> str | None:
    """
    Pick a sensible default generator:
    - If Ninja is available, use Ninja (fast, great for contributors).
    - Otherwise let CMake pick.
    """
    if shutil.which("ninja"):
        return "Ninja"
    return None


def cmake_exe() -> str:
    exe = shutil.which("cmake")
    if not exe:
        raise SystemExit("cmake not found on PATH. Install CMake and try again.")
    return exe


def main() -> int:
    parser = argparse.ArgumentParser(description="CMake build helper for PyKraken (no pip).")
    parser.add_argument("--build-dir", default="build", help="Build directory (default: build)")
    parser.add_argument("--config", default="Release", choices=["Debug", "Release", "RelWithDebInfo", "MinSizeRel"])
    parser.add_argument("--generator", default=None, help='CMake generator (e.g. "Ninja", "Visual Studio 17 2022")')
    parser.add_argument("--fresh", action="store_true", help="Delete build dir before configuring")
    parser.add_argument("--no-configure", action="store_true", help="Skip CMake configure step")
    parser.add_argument("--no-build", action="store_true", help="Skip build step")

    # Dependency toggles (match your CMake options)
    parser.add_argument("--vendor-all", action="store_true", help="Enable vendoring for pybind11/SDL/tmxlite")
    parser.add_argument("--vendor-pybind11", action="store_true", help="Vendor pybind11 (FetchContent)")
    parser.add_argument("--vendor-sdl", action="store_true", help="Vendor SDL3/SDL_image/SDL_ttf (FetchContent)")
    parser.add_argument("--vendor-tmxlite", action="store_true", help="Vendor tmxlite (FetchContent)")
    parser.add_argument("--vendor-box2d", action="store_true", help="Vendor Box2D (FetchContent)")

    # Everything after `--` goes to CMake configure step
    parser.add_argument("cmake_args", nargs=argparse.REMAINDER, help="Extra args after `--` go to CMake configure")

    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parent
    build_dir = (repo_root / args.build_dir).resolve()

    if args.fresh and build_dir.exists():
        print(f"Deleting build dir: {build_dir}")
        def onexc(func, path, exc_info):
            import stat
            if not os.access(path, os.W_OK):
                os.chmod(path, stat.S_IWUSR)
                func(path)
            else:
                raise
        shutil.rmtree(build_dir, onexc=onexc)

    build_dir.mkdir(parents=True, exist_ok=True)

    cmake = cmake_exe()

    # Decide generator
    gen = args.generator or default_generator()

    # Decide vendoring
    vendor_pybind11 = args.vendor_all or args.vendor_pybind11
    vendor_sdl = args.vendor_all or args.vendor_sdl
    vendor_tmxlite = args.vendor_all or args.vendor_tmxlite
    vendor_box2d = args.vendor_all or args.vendor_box2d

    # If user didn't specify any vendor flags, default to ON for all (contributor-friendly).
    if not (args.vendor_all or args.vendor_pybind11 or args.vendor_sdl or args.vendor_tmxlite or args.vendor_box2d):
        vendor_pybind11 = True
        vendor_sdl = True
        vendor_tmxlite = True
        vendor_box2d = True

    # CMake configure args
    configure_cmd = [
        cmake,
        "-S",
        str(repo_root),
        "-B",
        str(build_dir),
        f"-DPYK_VENDOR_PYBIND11={'ON' if vendor_pybind11 else 'OFF'}",
        f"-DPYK_VENDOR_SDL={'ON' if vendor_sdl else 'OFF'}",
        f"-DPYK_VENDOR_TMXLITE={'ON' if vendor_tmxlite else 'OFF'}",
        f"-DPYK_VENDOR_BOX2D={'ON' if vendor_box2d else 'OFF'}",
    ]

    # Helpful for IDEs/tools (and harmless elsewhere)
    configure_cmd += ["-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]

    if gen:
        configure_cmd += ["-G", gen]

    # Strip a leading "--" if present (common when passing through args)
    extra = args.cmake_args
    if extra and extra[0] == "--":
        extra = extra[1:]
    configure_cmd += extra

    # Build + install commands
    build_cmd = [cmake, "--build", str(build_dir), "--config", args.config]

    # Run steps
    if not args.no_configure:
        run(configure_cmd)

    if not args.no_build:
        run(build_cmd)

    print("\nDone.")
    print(f"Build dir: {build_dir}")

    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except subprocess.CalledProcessError as e:
        print(f"\nCommand failed with exit code {e.returncode}", file=sys.stderr)
        return_code = e.returncode
        raise SystemExit(return_code)
