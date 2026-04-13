import argparse
import subprocess
import os
import pykraken

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

    args = parser.parse_args()

    if args.command == "bake":
        print(f"⏲️ Baking: {args.input}...")
        try:
            pykraken.shaders.bake(args.input, args.out)
            print("✅ Shaders generated successfully.")
        except Exception as e:
            print(f"❌ Error baking shader: {e}")

    elif args.command == "build":
        print(f"📦 Building Bundle: {args.name}...")

        # Locate the __pyinstaller directory dynamically so students don't need to configure paths
        kraken_pkg_dir = os.path.dirname(pykraken.__file__)
        hook_dir = os.path.join(kraken_pkg_dir, "__pyinstaller")

        # Construct the standard PyInstaller command
        cmd = [
            "pyinstaller",
            "--onefile",
            "--noconsole",
            f"--name={args.name}",
            f"--additional-hooks-dir={hook_dir}",
            args.entry
        ]

        if args.icon:
            cmd.append(f"--icon={args.icon}")

        try:
            subprocess.run(cmd, check=True)
            print(f"✅ Executable created successfully in ./dist/{args.name}")
        except FileNotFoundError:
            print("❌ Error: PyInstaller not found. Please run 'pip install pyinstaller'.")
        except subprocess.CalledProcessError as e:
            print(f"❌ Build failed with error code: {e.returncode}")

if __name__ == "__main__":
    main()
