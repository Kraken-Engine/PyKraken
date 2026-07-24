import argparse
import itertools
import os
from pathlib import Path
import platform
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
from importlib import resources
from importlib.metadata import PackageNotFoundError, version
from urllib.error import HTTPError, URLError
from urllib.request import urlretrieve


try:
    VERSION = version("kraken-engine")
except PackageNotFoundError:
    VERSION = "1.7.4"
REPOSITORY = "https://github.com/Kraken-Engine/PyKraken"


class DotAnimator:
    def __init__(self, message):
        self.message = message
        self.dots = itertools.cycle(("", ".", "..", "..."))
        self.running = False
        self.thread = None

    def _animate(self):
        while self.running:
            sys.stdout.write(f"\r{self.message}{next(self.dots)}   ")
            sys.stdout.flush()
            time.sleep(0.4)

        sys.stdout.write(f"\r{self.message}...   \n")
        sys.stdout.flush()

    def __enter__(self):
        self.running = True
        self.thread = threading.Thread(target=self._animate)
        self.thread.start()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.running = False
        if self.thread:
            self.thread.join()


def _project_identifier(name):
    identifier = re.sub(r"[^A-Za-z0-9_]", "_", name)
    if not identifier or identifier[0].isdigit():
        identifier = f"Kraken_{identifier}"
    return identifier


def _template_files(language, demo=False, sdk=False):
    common = {
        "gitignore.txt": ".gitignore",
        "README.md.txt": "README.md",
    }
    if language == "python":
        common["main.demo.py.txt" if demo else "main.py.txt"] = "main.py"
    else:
        common.update(
            {
                "CMakeLists.sdk.txt" if sdk else "CMakeLists.txt": "CMakeLists.txt",
                "CMakePresets.json.txt": "CMakePresets.json",
                "main.demo.cpp.txt" if demo else "main.cpp.txt": "src/main.cpp",
            }
        )
    return common


def _render_template(text, project_name, sdk=False):
    replacements = {
        "{{PROJECT_NAME}}": project_name,
        "{{PROJECT_IDENTIFIER}}": _project_identifier(project_name),
        "{{KRAKEN_VERSION}}": VERSION,
        "{{KRAKEN_TAG}}": f"v{VERSION}",
        "{{BUILD_TYPE}}": "Release" if sdk else "Debug",
    }
    for placeholder, value in replacements.items():
        text = text.replace(placeholder, value)
    return text


def _write_project(target_dir, language, demo=False, sdk=False, force=False):
    template_dir = resources.files("pykraken").joinpath("templates", language)
    files = _template_files(language, demo=demo, sdk=sdk)
    collisions = [target_dir / destination for destination in files.values() if (target_dir / destination).exists()]

    if collisions and not force:
        paths = ", ".join(str(path) for path in collisions)
        raise FileExistsError(f"Refusing to overwrite existing files: {paths}. Use --force to replace them.")

    target_dir.mkdir(parents=True, exist_ok=True)
    for source_name, destination_name in files.items():
        destination = target_dir / destination_name
        destination.parent.mkdir(parents=True, exist_ok=True)
        source = template_dir.joinpath(source_name)
        rendered = _render_template(source.read_text(encoding="utf-8"), target_dir.name, sdk=sdk)
        destination.write_text(rendered, encoding="utf-8")


def _sdk_asset():
    system = platform.system()
    machine = platform.machine().lower()

    if system == "Darwin" and machine in {"arm64", "aarch64"}:
        return f"kraken-sdk-v{VERSION}-macos-arm64.tar.gz"
    if system == "Windows" and machine in {"amd64", "x86_64"}:
        return f"kraken-sdk-v{VERSION}-windows-x64.zip"
    if system == "Windows" and machine in {"arm64", "aarch64"}:
        return f"kraken-sdk-v{VERSION}-windows-arm64.zip"

    raise RuntimeError(
        f"No prebuilt Kraken SDK is published for {system} {platform.machine()}. "
        "Create the project without --sdk to use the portable bundled source build."
    )


def _download_sdk(target_dir):
    asset = _sdk_asset()
    url = f"{REPOSITORY}/releases/download/v{VERSION}/{asset}"

    with tempfile.TemporaryDirectory(prefix="kraken-sdk-") as temporary_dir:
        archive = Path(temporary_dir) / asset
        extracted = Path(temporary_dir) / "extracted"
        extracted.mkdir()

        try:
            with DotAnimator(f"Downloading {asset}"):
                urlretrieve(url, archive)
        except (HTTPError, URLError) as error:
            raise RuntimeError(f"Unable to download {url}: {error}") from error

        shutil.unpack_archive(archive, extracted)
        sdk_dir = target_dir / ".kraken" / "sdk"
        if sdk_dir.exists():
            shutil.rmtree(sdk_dir)
        sdk_dir.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(extracted, sdk_dir)


def _run_bake(args):
    import pykraken as kn

    if args.verbose:
        kn.log.enable()

    try:
        with DotAnimator(f"Baking: {args.input}"):
            kn.shaders.bake(args.input, args.out)

        outputs = {
            "spv": args.out + ".spv",
            "dxil": args.out + ".dxil",
            "msl": args.out + ".msl",
        }
        missing = [name for name, path in outputs.items() if not os.path.exists(path)]
        if missing:
            print(f"Some shader outputs failed: {', '.join(missing)}")
        else:
            print("Shaders generated successfully.")
    except Exception as error:
        print(f"Error baking shader: {error}")
    finally:
        kn.log.disable()


def _run_build(args):
    import pykraken

    name = args.name if args.name else os.path.splitext(args.entry)[0]
    kraken_package_dir = os.path.dirname(pykraken.__file__)
    hook_dir = os.path.join(kraken_package_dir, "__pyinstaller")
    command = [
        "pyinstaller",
        "--onefile",
        "--noconsole",
        f"--name={name}",
        f"--additional-hooks-dir={hook_dir}",
        args.entry,
    ]
    if args.icon:
        command.append(f"--icon={args.icon}")

    try:
        if args.verbose:
            print(f"Building bundle: {name}...")
            subprocess.run(command, check=True)
        else:
            with DotAnimator(f"Building bundle: {name}"):
                subprocess.run(command, check=True, capture_output=True, text=True)
        print(f"Executable created successfully in ./dist/{name}")
    except FileNotFoundError:
        print("PyInstaller was not found. Install it with: pip install pyinstaller")
    except subprocess.CalledProcessError as error:
        print(f"Build failed with error code {error.returncode}")
        if not args.verbose and error.stderr:
            print(f"\n--- Error Details ---\n{error.stderr}")


def _run_init(args, parser):
    selected_flags = int(args.cpp) + int(args.python)
    if args.language and selected_flags:
        parser.error("--language cannot be combined with --cpp or --python")
    if selected_flags > 1:
        parser.error("--cpp and --python are mutually exclusive")

    language = args.language or ("cpp" if args.cpp else "python")
    if args.sdk and language != "cpp":
        parser.error("--sdk can only be used for a C++ project")

    target_dir = Path(args.path).expanduser().resolve()
    try:
        _write_project(target_dir, language, demo=args.demo, sdk=args.sdk, force=args.force)
        if args.sdk:
            _download_sdk(target_dir)
    except (FileExistsError, RuntimeError) as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1

    print(f"Created {language.upper()} Kraken project at {target_dir}")
    relative = os.path.relpath(target_dir)
    if language == "cpp":
        print(f"Next: cd {relative} && cmake --preset dev && cmake --build --preset dev")
    else:
        print(f"Next: cd {relative} && python main.py")
    return 0


def _create_parser():
    parser = argparse.ArgumentParser(prog="kraken", description="Kraken Engine developer tools")
    subparsers = parser.add_subparsers(dest="command", help="Available commands", required=True)

    bake_parser = subparsers.add_parser("bake", help="Compile HLSL/GLSL to SPV, DXIL, and MSL")
    bake_parser.add_argument("input", help="Path to the input shader file")
    bake_parser.add_argument("-o", "--out", default=".", help="Output directory (default: current)")
    bake_parser.add_argument("-v", "--verbose", action="store_true", help="Show debug logging")

    build_parser = subparsers.add_parser("build", help="Bundle a Python game into an executable")
    build_parser.add_argument("entry", help="Path to the main Python script")
    build_parser.add_argument("-n", "--name", default="Platformer", help="Name of the executable")
    build_parser.add_argument("-i", "--icon", help="Path to the icon file (.ico)")
    build_parser.add_argument("-v", "--verbose", action="store_true", help="Show detailed output")

    init_parser = subparsers.add_parser("init", help="Create a Python or C++ Kraken project")
    init_parser.add_argument("path", nargs="?", default=".", help="Project directory")
    language_group = init_parser.add_mutually_exclusive_group()
    language_group.add_argument("--cpp", action="store_true", help="Create a C++20 CMake project")
    language_group.add_argument("--python", action="store_true", help="Create a Python project (default)")
    init_parser.add_argument("--language", choices=("python", "cpp"), help="Project language")
    init_parser.add_argument("--sdk", action="store_true", help="Use the matching prebuilt C++ SDK")
    init_parser.add_argument("-f", "--force", action="store_true", help="Overwrite generated files")
    init_parser.add_argument("--demo", action="store_true", help="Create a rendering demo")

    subparsers.add_parser("docs", help="Open Kraken documentation in a browser")
    return parser


def main(argv=None):
    parser = _create_parser()
    args = parser.parse_args(argv)

    if args.command == "bake":
        _run_bake(args)
    elif args.command == "build":
        _run_build(args)
    elif args.command == "init":
        return _run_init(args, parser)
    elif args.command == "docs":
        import webbrowser

        try:
            webbrowser.open("https://krakenengine.org/")
            print("Opening documentation...")
        except Exception as error:
            print(f"Failed to open browser: {error}")
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
