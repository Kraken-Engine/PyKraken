import pathlib
import sys

def main():
    # Take the stub directory as a command line argument
    stub_dir = pathlib.Path(sys.argv[1])

    for p in stub_dir.rglob('*.pyi'):
        content = p.read_text('utf-8')

        # 1. Fix the generic PathLike missing type across ALL files
        content = content.replace('os.PathLike,', 'os.PathLike[str],')
        content = content.replace('os.PathLike)', 'os.PathLike[str])')
        content = content.replace('os.PathLike |', 'os.PathLike[str] |')

        # 2. Remove the internal prefix ONLY from the root __init__.pyi
        if p.name == '__init__.pyi':
            content = content.replace('_pykraken.', '')

        p.write_text(content, 'utf-8')

if __name__ == '__main__':
    main()