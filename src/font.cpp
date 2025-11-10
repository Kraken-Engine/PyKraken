#include "Font.hpp"
#include "misc/SpaceGrotesk.h"
#include "misc/minecraftia.h"

#include <algorithm>
#include <cmath>
#include <mutex>
#include <pybind11/native_enum.h>
#include <stdexcept>
#include <vector>

namespace kn
{
namespace font
{
// Static registry to track all font instances for proper cleanup
static std::vector<Font*> _fontInstances;
static std::mutex _fontsMutex;
} // namespace font

Font::Font(const std::string& fileDir, int ptSize)
{
    if (ptSize < 8)
        ptSize = 8;

    if (fileDir == "kraken-clean")
    {
        SDL_IOStream* rw = SDL_IOFromMem(SpaceGrotesk_ttf, SpaceGrotesk_ttf_len);
        m_font = TTF_OpenFontIO(rw, true, static_cast<float>(ptSize));
    }
    else if (fileDir == "kraken-retro")
    {
        SDL_IOStream* rw = SDL_IOFromMem(Minecraftia_Regular_ttf, Minecraftia_Regular_ttf_len);
        const int ptSizeFixed = (ptSize + 4) / 8 * 8; // Round to the nearest multiple of 8
        m_font = TTF_OpenFontIO(rw, true, static_cast<float>(ptSizeFixed));
    }
    else
    {
        m_font = TTF_OpenFont(fileDir.c_str(), static_cast<float>(ptSize));
    }

    if (!m_font)
        throw std::runtime_error("Failed to load font: " + std::string(SDL_GetError()));

    // Register this font for cleanup
    std::lock_guard g(font::_fontsMutex);
    font::_fontInstances.push_back(this);
}

Font::~Font()
{
    // Remove from registry if still present
    {
        std::lock_guard g(font::_fontsMutex);
        auto it = std::find(font::_fontInstances.begin(), font::_fontInstances.end(), this);
        if (it != font::_fontInstances.end())
        {
            font::_fontInstances.erase(it);
        }
    }

    // Only clean up if font hasn't been freed by _quit
    if (m_font != nullptr)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}

void Font::setAlignment(const Align alignment) const
{
    switch (alignment)
    {
    case Align::Left:
        TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_LEFT);
        break;
    case Align::Center:
        TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_CENTER);
        break;
    case Align::Right:
        TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_RIGHT);
        break;
    }
}

Align Font::getAlignment() const
{
    const TTF_HorizontalAlignment align = TTF_GetFontWrapAlignment(m_font);
    switch (align)
    {
    case TTF_HORIZONTAL_ALIGN_LEFT:
        return Align::Left;
    case TTF_HORIZONTAL_ALIGN_CENTER:
        return Align::Center;
    case TTF_HORIZONTAL_ALIGN_RIGHT:
        return Align::Right;
    default:
        return Align::Left;
    }
}

void Font::setHinting(const font::Hinting hinting) const
{
    switch (hinting)
    {
    case font::Hinting::Normal:
        TTF_SetFontHinting(m_font, TTF_HINTING_NORMAL);
        break;
    case font::Hinting::Light:
        TTF_SetFontHinting(m_font, TTF_HINTING_LIGHT);
        break;
    case font::Hinting::Mono:
        TTF_SetFontHinting(m_font, TTF_HINTING_MONO);
        break;
    case font::Hinting::LightSubpixel:
        TTF_SetFontHinting(m_font, TTF_HINTING_LIGHT_SUBPIXEL);
        break;
    case font::Hinting::None:
        TTF_SetFontHinting(m_font, TTF_HINTING_NONE);
        break;
    }
}

font::Hinting Font::getHinting() const
{
    const TTF_HintingFlags hinting = TTF_GetFontHinting(m_font);
    switch (hinting)
    {
    case TTF_HINTING_NORMAL:
        return font::Hinting::Normal;
    case TTF_HINTING_LIGHT:
        return font::Hinting::Light;
    case TTF_HINTING_MONO:
        return font::Hinting::Mono;
    case TTF_HINTING_LIGHT_SUBPIXEL:
        return font::Hinting::LightSubpixel;
    case TTF_HINTING_NONE:
        return font::Hinting::None;
    default:
        return font::Hinting::Normal;
    }
}

void Font::setPtSize(int pt) const
{
    if (pt < 8)
        pt = 8;
    TTF_SetFontSize(m_font, static_cast<float>(pt));
}

int Font::getPtSize() const { return static_cast<int>(TTF_GetFontSize(m_font)); }

void Font::setBold(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_BOLD) : (s & ~TTF_STYLE_BOLD));
}

void Font::setItalic(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_ITALIC) : (s & ~TTF_STYLE_ITALIC));
}

void Font::setUnderline(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_UNDERLINE) : (s & ~TTF_STYLE_UNDERLINE));
}

void Font::setStrikethrough(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_STRIKETHROUGH) : (s & ~TTF_STYLE_STRIKETHROUGH));
}

bool Font::isBold() const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    return (s & TTF_STYLE_BOLD) != 0;
}

bool Font::isItalic() const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    return (s & TTF_STYLE_ITALIC) != 0;
}

bool Font::isUnderline() const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    return (s & TTF_STYLE_UNDERLINE) != 0;
}

bool Font::isStrikethrough() const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    return (s & TTF_STYLE_STRIKETHROUGH) != 0;
}

int Font::getHeight() const { return TTF_GetFontHeight(m_font); }

int Font::getAscent() const { return TTF_GetFontAscent(m_font); }

int Font::getDescent() const { return TTF_GetFontDescent(m_font); }

void Font::setLineSkip(const int lineSkip) const { TTF_SetFontLineSkip(m_font, lineSkip); }

int Font::getLineSkip() const { return TTF_GetFontLineSkip(m_font); }

void Font::setOutline(const int outline) const { TTF_SetFontOutline(m_font, outline); }

int Font::getOutline() const { return TTF_GetFontOutline(m_font); }

void Font::setKerning(const bool enabled) const { TTF_SetFontKerning(m_font, enabled); }

bool Font::getKerning() const { return TTF_GetFontKerning(m_font) != 0; }

void Font::setCharSpacing(const int charSpacing) const { TTF_SetFontCharSpacing(m_font, charSpacing); }

int Font::getCharSpacing() const { return TTF_GetFontCharSpacing(m_font); }

namespace font
{
void _init()
{
    if (!TTF_Init())
        throw std::runtime_error("Failed to initialize SDL_ttf");
}

void _quit()
{
    // Clean up all fonts before TTF is shut down
    {
        std::lock_guard g(_fontsMutex);
        for (Font* font : _fontInstances)
        {
            if (font->m_font != nullptr)
            {
                TTF_CloseFont(font->m_font);
                font->m_font = nullptr;
            }
        }
        _fontInstances.clear();
    }

    // Shut down TTF
    if (TTF_WasInit())
        TTF_Quit();
}

void _bind(const py::module_& module)
{
    py::native_enum<font::Hinting>(module, "FontHint", "enum.IntEnum", R"doc(
Font hinting modes for controlling how fonts are rendered.

Hinting is the process of fitting font outlines to the pixel grid to improve
readability at small sizes.
    )doc")
        .value("NORMAL", Hinting::Normal, "Default hinting")
        .value("MONO", Hinting::Mono, "Monochrome hinting")
        .value("LIGHT", Hinting::Light, "Light hinting")
        .value("LIGHT_SUBPIXEL", Hinting::LightSubpixel, "Light subpixel hinting")
        .value("NONE", Hinting::None, "No hinting")
        .finalize();

    py::classh<Font>(module, "Font", R"doc(
A font typeface for rendering text.

This class wraps an SDL_ttf font and manages font properties like size,
style, and alignment. You can load fonts from a file path or use one of
the built-in typefaces:

- "kraken-clean": A clean sans-serif font bundled with the engine.
- "kraken-retro": A pixel/retro font bundled with the engine. Point size is
                  rounded to the nearest multiple of 8 for crisp rendering.

Note:
    A window/renderer must be created before using fonts. Typically you should
    call kn.window.create(...) first, which initializes the font engine.
    )doc")
        .def(py::init<const std::string&, int>(), py::arg("file_dir"), py::arg("pt_size"),
             R"doc(
Create a Font.

Args:
    file_dir (str): Path to a .ttf font file, or one of the built-in names
                    "kraken-clean" or "kraken-retro".
    pt_size (int): The point size. Values below 8 are clamped to 8. For
                   "kraken-retro", the size is rounded to the nearest multiple
                   of 8 to preserve pixel alignment.

Raises:
    RuntimeError: If the font fails to load.
    )doc")
        .def_property("alignment", &Font::getAlignment, &Font::setAlignment, R"doc(
Get or set the text alignment for wrapped text.

Valid values: Align.LEFT, Align.CENTER, Align.RIGHT
        )doc")
        .def_property("hinting", &Font::getHinting, &Font::setHinting, R"doc(
Get or set the font hinting mode.

Valid values: FontHinting.NORMAL, FontHinting.MONO, FontHinting.LIGHT,
              FontHinting.LIGHT_SUBPIXEL, FontHinting.NONE
        )doc")
        .def_property("pt_size", &Font::getPtSize, &Font::setPtSize, R"doc(
Get or set the point size of the font. Values below 8 are clamped to 8.
        )doc")
        .def_property("bold", &Font::isBold, &Font::setBold, R"doc(
Get or set whether bold text style is enabled.
        )doc")
        .def_property("italic", &Font::isItalic, &Font::setItalic, R"doc(
Get or set whether italic text style is enabled.
        )doc")
        .def_property("underline", &Font::isUnderline, &Font::setUnderline, R"doc(
Get or set whether underline text style is enabled.
        )doc")
        .def_property("strikethrough", &Font::isStrikethrough, &Font::setStrikethrough,
                      R"doc(
Get or set whether strikethrough text style is enabled.
        )doc")
        .def_property_readonly("height", &Font::getHeight, R"doc(
Get the maximum pixel height of all glyphs in the font.

Returns:
    int: The font height in pixels.
        )doc")
        .def_property_readonly("ascent", &Font::getAscent, R"doc(
Get the pixel ascent of the font.

Returns:
    int: The font ascent in pixels.
        )doc")
        .def_property_readonly("descent", &Font::getDescent, R"doc(
Get the pixel descent of the font.

Returns:
    int: The font descent in pixels.
        )doc")
        .def_property("line_spacing", &Font::getLineSkip, &Font::setLineSkip, R"doc(
Get or set the spacing between lines of text in pixels.

        )doc")
        .def_property("outline", &Font::getOutline, &Font::setOutline, R"doc(
Get or set the outline width in pixels.

Returns:
    int: The outline width in pixels (0 for no outline).
        )doc")
        .def_property("kerning", &Font::getKerning, &Font::setKerning, R"doc(
Get or set whether kerning is enabled.

Returns:
    bool: True if kerning is enabled, False otherwise.
        )doc")
        .def_property("char_spacing", &Font::getCharSpacing, &Font::setCharSpacing, R"doc(
Get or set the additional spacing between characters in pixels.


        )doc");
}
} // namespace font
} // namespace kn
