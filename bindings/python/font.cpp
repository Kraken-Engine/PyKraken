#include "kraken/graphics/Font.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "bindings/python/bindings.hpp"
#include "graphics/assets/SpaceGrotesk.h"
#include "graphics/assets/minecraftia.h"
#include "kraken/core/Log.hpp"

namespace kn::font
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::enum_<font::Hinting>(module, "FontHint", R"doc(
Font hinting modes for controlling how fonts are rendered.

Hinting is the process of fitting font outlines to the pixel grid to improve
readability at small sizes.
    )doc")
        .value("NORMAL", Hinting::Normal, "Default hinting")
        .value("MONO", Hinting::Mono, "Monochrome hinting")
        .value("LIGHT", Hinting::Light, "Light hinting")
        .value("LIGHT_SUBPIXEL", Hinting::LightSubpixel, "Light subpixel hinting")
        .value("NONE", Hinting::None, "No hinting");

    nb::class_<Font>(module, "Font", R"doc(
A font typeface for rendering text.

This class wraps an SDL_ttf font and manages font properties like size,
style, and alignment. You can load fonts from a file path or use one of
the built-in typefaces:

- "kraken-modern": A modern sans-serif font bundled with the engine.
- "kraken-retro": A pixel/retro font bundled with the engine. Point size is
                  rounded to the nearest multiple of 8 for crisp rendering.

Note:
    A window/renderer must be created before using fonts. Typically you should
    call kn.window.create(...) first, which initializes the font engine.
    )doc")
        .def(
            nb::init<const std::filesystem::path&, int>(), "file_dir"_a, "pt_size"_a,
            R"doc(
Create a Font.

Args:
    file_dir (str | os.PathLike[str]): Path to a .ttf font file, or one of the built-in names
                    "kraken-modern" or "kraken-retro".
    pt_size (int): The point size. Values below 8 are clamped to 8. For
                   "kraken-retro", the size is rounded to the nearest multiple
                   of 8 to preserve pixel alignment.

Raises:
    RuntimeError: If the font fails to load.
    )doc"
        )

        .def_prop_rw("alignment", &Font::getAlignment, &Font::setAlignment, R"doc(
Get or set the text alignment for wrapped text.
        )doc")
        .def_prop_rw("hinting", &Font::getHinting, &Font::setHinting, R"doc(
Get or set the font hinting mode.
        )doc")
        .def_prop_rw("pt_size", &Font::getPtSize, &Font::setPtSize, R"doc(
Get or set the point size of the font. Values below 8 are clamped to 8.
        )doc")
        .def_prop_rw("bold", &Font::isBold, &Font::setBold, R"doc(
Get or set whether bold text style is enabled.
        )doc")
        .def_prop_rw("italic", &Font::isItalic, &Font::setItalic, R"doc(
Get or set whether italic text style is enabled.
        )doc")
        .def_prop_rw("underline", &Font::isUnderline, &Font::setUnderline, R"doc(
Get or set whether underline text style is enabled.
        )doc")
        .def_prop_rw("strikethrough", &Font::isStrikethrough, &Font::setStrikethrough, R"doc(
Get or set whether strikethrough text style is enabled.
        )doc")
        .def_prop_rw("line_spacing", &Font::getLineSpacing, &Font::setLineSpacing, R"doc(
Get or set the spacing between lines of text in pixels.
        )doc")
        .def_prop_rw("outline", &Font::getOutline, &Font::setOutline, R"doc(
Get or set the outline width in pixels (0 for no outline).
        )doc")
        .def_prop_rw("kerning", &Font::getKerning, &Font::setKerning, R"doc(
Get or set whether kerning is enabled.
        )doc")

        .def_prop_ro("height", &Font::getHeight, R"doc(
Get the maximum pixel height of all glyphs in the font.

Returns:
    int: The font height in pixels.
        )doc")
        .def_prop_ro("ascent", &Font::getAscent, R"doc(
Get the pixel ascent of the font.

Returns:
    int: The font ascent in pixels.
        )doc")
        .def_prop_ro("descent", &Font::getDescent, R"doc(
Get the pixel descent of the font.

Returns:
    int: The font descent in pixels.
        )doc");
    /*
    .def_property("char_spacing", &Font::getCharSpacing, &Font::setCharSpacing, R"doc(
Get or set the additional spacing between characters in pixels.
        )doc");
    */
}
}  // namespace kn::font
