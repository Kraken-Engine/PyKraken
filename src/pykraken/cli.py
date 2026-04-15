import argparse
import subprocess
import os
import pykraken

import sys
import time
import threading
import itertools

class DotAnimator:
    def __init__(self, message):
        self.message = message
        # Cycle through 0 to 3 dots
        self.dots = itertools.cycle(['', '.', '..', '...'])
        self.running = False
        self.thread = None

    def _animate(self):
        while self.running:
            # \r moves the cursor back to the start of the line
            # The extra spaces at the end ensure shrinking strings (from '...' to '') overwrite old dots
            sys.stdout.write(f"\r{self.message}{next(self.dots)}   ")
            sys.stdout.flush()
            time.sleep(0.4)

        # Clear the line when finished so the success/error message prints cleanly
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

def main():
    parser = argparse.ArgumentParser(prog="pykraken", description="Kraken Engine Developer Tools")
    subparsers = parser.add_subparsers(dest="command", help="Available commands", required=True)

    # --- BAKE COMMAND ---
    bake_parser = subparsers.add_parser("bake", help="Compile HLSL/GLSL to SPV, DXIL, and MSL")
    bake_parser.add_argument("input", help="Path to the input shader file")
    bake_parser.add_argument("-o", "--out", default=".", help="Output directory (default: current)")

    # --- BUILD COMMAND ---
    build_parser = subparsers.add_parser("build", help="Bundle game into an executable")
    build_parser.add_argument("entry", help="Path to the main Python script (e.g., main.py)")
    build_parser.add_argument("-n", "--name", default="Platformer", help="Name of the executable")
    build_parser.add_argument("-i", "--icon", help="Path to the icon file (.ico)", default=None)
    build_parser.add_argument("-v", "--verbose", action="store_true", help="Show detailed PyInstaller output")

    args = parser.parse_args()

    if args.command == "bake":
        try:
            # 1. Start the animation and do the work
            with DotAnimator(f"⏲️  Baking: {args.input}"):
                pykraken.shaders.bake(args.input, args.out)

            # 2. Print success OUTSIDE the with block so the line is cleared first
            print("✅ Shaders generated successfully.")

        except Exception as e:
            # 3. If it crashes, it jumps out of the with block, clears the line, and prints this
            print(f"❌ Error baking shader: {e}")

    elif args.command == "build":
        name = args.name if args.name else os.path.splitext(args.entry)[0]

        # Locate the __pyinstaller directory dynamically so students don't need to configure paths
        kraken_pkg_dir = os.path.dirname(pykraken.__file__)
        hook_dir = os.path.join(kraken_pkg_dir, "__pyinstaller")

        # Construct the standard PyInstaller command
        cmd = [
            "pyinstaller",
            "--onefile",
            "--noconsole",
            f"--name={name}",
            f"--additional-hooks-dir={hook_dir}",
            args.entry
        ]

        if args.icon:
            cmd.append(f"--icon={args.icon}")

        try:
            if args.verbose:
                print(f"📦 Building Bundle: {name}...")
                subprocess.run(cmd, check=True)
            else:
                with DotAnimator(f"📦 Building Bundle: {name}"):
                    subprocess.run(cmd, check=True, capture_output=True, text=True)

            print(f"✅ Executable created successfully in ./dist/{name}")

        except FileNotFoundError:
            print("❌ Error: PyInstaller not found. Please run 'pip install pyinstaller'.")
        except subprocess.CalledProcessError as e:
            print(f"❌ Build failed with error code: {e.returncode}")

            if not args.verbose and e.stderr:
                print("\n--- Error Details ---")
                print(e.stderr)

if __name__ == "__main__":
    main()
