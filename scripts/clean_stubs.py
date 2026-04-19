import pathlib
import sys

def main():
    # Take the stub directory as a command line argument
    stub_dir = pathlib.Path(sys.argv[1])

    for p in stub_dir.rglob('*.pyi'):
        content = ""
        content = p.read_text('utf-8')

        # 1. Fix the generic PathLike missing type across ALL files
        content = content.replace('os.PathLike,', 'os.PathLike[str],')
        content = content.replace('os.PathLike)', 'os.PathLike[str])')
        content = content.replace('os.PathLike |', 'os.PathLike[str] |')

        if p.name == '__init__.pyi':
            # 2. Remove the _pykraken prefix from __init__.pyi
            content = content.replace('_pykraken.', '')
        else:
            # 3. Remove the underscore from _pykraken in other files
            content = content.replace('_pykraken', 'pykraken')

        p.write_text(content, 'utf-8')

if __name__ == '__main__':
    main()
