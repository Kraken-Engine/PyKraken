"""Rewrite absolute _pykraken imports to relative imports in generated stubs.

nanobind emits ``import _pykraken`` and ``_pykraken.Vec2`` style references.
Inside the ``_pykraken`` package these absolute imports don't resolve, so we
rewrite them to relative imports from the package ``__init__``.
"""

import re
import sys
from pathlib import Path


def fix_stub(path: Path) -> None:
    text = path.read_text(encoding="utf-8")
    original = text

    # Drop "import _pykraken" and "import _pykraken.<sub>" lines
    text = re.sub(r"^import _pykraken(\.\w+)*\n", "", text, flags=re.MULTILINE)

    # Collect all _pykraken.X references (the first dotted name after _pykraken.)
    names = sorted(set(re.findall(r"(?<!\w)_pykraken\.([A-Za-z_]\w*)", text)))

    # Replace _pykraken.X with just X  (handles _pykraken.Vec2 and _pykraken.tilemap.Layer)
    text = re.sub(r"(?<!\w)_pykraken\.", "", text)

    # Build a relative import for collected names
    if names:
        import_block = "from . import (\n"
        for name in names:
            import_block += f"    {name},\n"
        import_block += ")\n"

        # Insert after existing top-level imports
        lines = text.split("\n")
        insert_idx = 0
        for i, line in enumerate(lines):
            if line.startswith(("import ", "from ")):
                insert_idx = i + 1
        lines.insert(insert_idx, import_block)
        text = "\n".join(lines)

    # Ensure Callable is imported when used in nb::sig annotations
    if "Callable" in text and "import Callable" not in text:
        text = "from collections.abc import Callable\n" + text

    if text != original:
        path.write_text(text, encoding="utf-8")


def main() -> None:
    stub_dir = Path(sys.argv[1])
    for pyi in sorted(stub_dir.glob("*.pyi")):
        if pyi.name == "__init__.pyi":
            # __init__.pyi: just strip self-referencing _pykraken. prefixes
            text = pyi.read_text(encoding="utf-8")
            fixed = re.sub(r"(?<!\w)_pykraken\.([A-Za-z_]\w*)", r"\1", text)
            # Ensure Callable is imported when used in nb::sig annotations
            if "Callable" in fixed and "import Callable" not in fixed:
                fixed = re.sub(
                    r"(from collections\.abc import .+)",
                    r"\1, Callable",
                    fixed,
                )
            if fixed != text:
                pyi.write_text(fixed, encoding="utf-8")
        else:
            fix_stub(pyi)


if __name__ == "__main__":
    main()
