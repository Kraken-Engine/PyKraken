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
    bake_parser.add_argument("-v", "--verbose", action="store_true", help="Show debug logging")

    # --- BUILD COMMAND ---
    build_parser = subparsers.add_parser("build", help="Bundle game into an executable")
    build_parser.add_argument("entry", help="Path to the main Python script (e.g., main.py)")
    build_parser.add_argument("-n", "--name", default="Platformer", help="Name of the executable")
    build_parser.add_argument("-i", "--icon", help="Path to the icon file (.ico)", default=None)
    build_parser.add_argument("-v", "--verbose", action="store_true", help="Show detailed PyInstaller output")

    # --- INIT COMMAND ---
    init_parser = subparsers.add_parser("init", help="Create a starter main.py file")
    init_parser.add_argument(
        "path",
        nargs="?",
        default=".",
        help="Folder to create the project in (default: current directory)"
    )
    init_parser.add_argument(
        "-f", "--force",
        action="store_true",
        help="Overwrite existing main.py if it exists"
    )
    init_parser.add_argument(
        "--demo",
        action="store_true",
        help="Generate a starter file with text and shape rendering"
    )

    # --- DOCS COMMAND ---
    subparsers.add_parser("docs", help="Open PyKraken documentation in browser")

    args = parser.parse_args()

    if args.command == "bake":
        if args.verbose:
            pykraken.log.enable()

        try:
            out_base = args.out

            # 1. Start the animation and do the work
            with DotAnimator(f"⏲️  Baking: {args.input}"):
                pykraken.shaders.bake(args.input, out_base)

            # Check which outputs were produced (the baker now logs instead of throwing).
            outputs = {
                "spv": out_base + ".spv",
                "dxil": out_base + ".dxil",
                "msl": out_base + ".msl",
            }

            missing = [name for name, path in outputs.items() if not os.path.exists(path)]

            if missing:
                print(f"⚠️ Some shader outputs failed: {', '.join(missing)}")
            else:
                print("✅ Shaders generated successfully.")

        except Exception as e:
            # If the baker raises (e.g., can't open input file), show the error.
            print(f"❌ Error baking shader: {e}")

        pykraken.log.disable()

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

    elif args.command == "init":
        target_dir = os.path.abspath(args.path)
        target = os.path.join(target_dir, "main.py")

        minimal_template = """import pykraken as kn

kn.init()
kn.window.create("Kraken Window", 800, 600)

while kn.window.is_open():
    kn.event.poll()
    kn.renderer.clear()
    kn.renderer.present()

kn.quit()
"""

        demo_template = """import pykraken as kn
from random import randint, choice

WIN_SIZE = kn.Vec2(800, 600)
WIN_WIDTH, WIN_HEIGHT = WIN_SIZE.as_ints()

kn.init()
kn.window.create("Hello, world!", WIN_WIDTH, WIN_HEIGHT)
kn.time.set_target(60)

font = kn.Font("kraken-modern", 32)
label = kn.Text(font, "Hello, world!")

SHAPE_COUNT = 40

shapes = [
    kn.Polygon(
        n=randint(3, 5),
        radius=randint(6, 12),
        centroid=kn.Vec2(
            randint(10, WIN_WIDTH - 10),
            randint(10, WIN_HEIGHT - 10)
        )
    )
    for _ in range(SHAPE_COUNT)
]

spin_speeds = [
    randint(30, 60) * kn.math.DEG2RAD * choice((-1, 1))
    for _ in range(SHAPE_COUNT)
]

shape_colors = [
    kn.color.from_hsv(randint(0, 359), 0.4, 0.6)
    for _ in range(SHAPE_COUNT)
]

while kn.window.is_open():
    kn.event.poll()

    dt = kn.time.get_delta()

    kn.renderer.clear(kn.Color.DARK_GRAY)
    for shape, spin_speed, color in zip(shapes, spin_speeds, shape_colors):
        shape.rotate(spin_speed * dt)
        kn.draw.polygon(shape, color, filled=False)
    label.draw(WIN_SIZE / 2, kn.Anchor.CENTER)

    kn.renderer.present()

kn.quit()
"""
        template = demo_template if args.demo else minimal_template

        try:
            os.makedirs(target_dir, exist_ok=True)

            if os.path.exists(target) and not args.force:
                print(f"❌ {target} already exists. Use --force to overwrite.")
                return

            with open(target, "w", encoding="utf-8") as f:
                f.write(template)

            rel_target = os.path.relpath(target)
            print(f"✅ Created {rel_target} successfully.")

            if os.path.abspath(args.path) == os.getcwd():
                print(f"👉 Next: python main.py")
            else:
                print(f"👉 Next: cd {args.path} && python main.py")

        except Exception as e:
            print(f"❌ Failed to create main.py: {e}")

    elif args.command == "docs":
        import webbrowser

        URL = "https://krakenengine.org/"
        try:
            webbrowser.open(URL)
            print("🌐 Opening documentation...")
        except Exception as e:
            print(f"❌ Failed to open browser: {e}")


if __name__ == "__main__":
    main()
