#include "Font.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "misc/SpaceGrotesk.h"
#include "misc/minecraftia.h"

#include <cmath>
#include <stdexcept>

namespace kn
{
static TTF_TextEngine* _textEngine = nullptr;

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

    m_text = TTF_CreateText(_textEngine, m_font, "", 0);
    TTF_SetTextColor(m_text, 255, 255, 255, 255);
}

Font::~Font()
{
    if (m_text)
        TTF_DestroyText(m_text);
    m_text = nullptr;

    if (m_font)
        TTF_CloseFont(m_font);
    m_font = nullptr;
}

void Font::draw(const Vec2& pos, const Anchor anchor) const
{
    // Round incoming position to the nearest pixel
    int x = static_cast<int>(std::lround(pos.x));
    int y = static_cast<int>(std::lround(pos.y));

    // Get text size so we can offset based on the anchor
    int textW = 0, textH = 0;
    TTF_GetTextSize(m_text, &textW, &textH);

    switch (anchor)
    {
    case Anchor::TopLeft:
        // no offset
        break;
    case Anchor::TopMid:
        x -= textW / 2;
        break;
    case Anchor::TopRight:
        x -= textW;
        break;
    case Anchor::MidLeft:
        y -= textH / 2;
        break;
    case Anchor::Center:
        x -= textW / 2;
        y -= textH / 2;
        break;
    case Anchor::MidRight:
        x -= textW;
        y -= textH / 2;
        break;
    case Anchor::BottomLeft:
        y -= textH;
        break;
    case Anchor::BottomMid:
        x -= textW / 2;
        y -= textH;
        break;
    case Anchor::BottomRight:
        x -= textW;
        y -= textH;
        break;
    }

    TTF_DrawRendererText(m_text, x, y);
}

void Font::setWrapWidth(int wrapWidth) const
{
    if (wrapWidth < 0)
        wrapWidth = 0;
    TTF_SetTextWrapWidth(m_text, wrapWidth);
}

int Font::getWrapWidth() const
{
    int wrapWidth;
    TTF_GetTextWrapWidth(m_text, &wrapWidth);
    return wrapWidth;
}

void Font::setText(const std::string& text) const { TTF_SetTextString(m_text, text.c_str(), 0); }

std::string Font::getText() const { return std::string(m_text->text); }

void Font::setColor(const Color& color) const
{
    TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
}

Color Font::getColor() const
{
    Color color;
    TTF_GetTextColor(m_text, &color.r, &color.g, &color.b, &color.a);
    return color;
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

Rect Font::getRect() const
{
    int w, h;
    TTF_GetTextSize(m_text, &w, &h);
    return {0, 0, w, h};
}

Vec2 Font::getSize() const
{
    int w, h;
    TTF_GetTextSize(m_text, &w, &h);
    return {w, h};
}

int Font::getWidth() const
{
    int w;
    TTF_GetTextSize(m_text, &w, nullptr);
    return w;
}

int Font::getHeight() const
{
    int h;
    TTF_GetTextSize(m_text, nullptr, &h);
    return h;
}

namespace font
{
void _init()
{
    if (!TTF_Init())
        throw std::runtime_error("Failed to initialize SDL_ttf");

    _textEngine = TTF_CreateRendererTextEngine(renderer::_get());
    if (!_textEngine)
        throw std::runtime_error("Failed to create text engine: " + std::string(SDL_GetError()));
}

void _quit()
{
    if (_textEngine)
        TTF_DestroyRendererTextEngine(_textEngine);
    _textEngine = nullptr;

    TTF_Quit();
}

void _bind(const py::module_& module)
{
    py::classh<Font>(module, "Font", R"doc(
A font object for rendering text to the active renderer.

This class wraps an SDL_ttf font and an internal text object for efficient
rendering. You can load fonts from a file path or use one of the built-in
typefaces:

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
        .def(
            "draw",
            [](const Font& self, const py::object& posObj, const Anchor anchor) -> void
            {
                Vec2 pos{};
                if (!posObj.is_none())
                {
                    try
                    {
                        pos = posObj.cast<Vec2>();
                    }
                    catch (const py::cast_error&)
                    {
                        throw py::type_error("Invalid type for 'pos', expected Vec2");
                    }
                }
                self.draw(pos, anchor);
            },
            py::arg("pos") = py::none(), py::arg("anchor") = Anchor::TopLeft, R"doc(
Draw the text to the renderer at the specified position with alignment.

Args:
    pos (Vec2 | None): The position in pixels. Defaults to (0, 0).
    anchor (Anchor): The anchor point for alignment. Defaults to TopLeft.
        )doc")
        .def("get_rect", &Font::getRect, R"doc(
Get the bounding rectangle of the current text.

Returns:
    Rect: A rectangle with x=0, y=0, and width/height of the text.
        )doc")
        .def_property("wrap_width", &Font::getWrapWidth, &Font::setWrapWidth, R"doc(
Get or set the wrap width in pixels for text wrapping.

Set to 0 to disable wrapping. Negative values are clamped to 0.
        )doc")
        .def_property("text", &Font::getText, &Font::setText, R"doc(
Get or set the text string to be rendered.
        )doc")
        .def_property("color", &Font::getColor, &Font::setColor, R"doc(
Get or set the color of the rendered text.
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
        .def_property_readonly("size", &Font::getSize, R"doc(
Get the size (width, height) of the current text as a Vec2.

Returns:
    Vec2: The text dimensions.
        )doc")
        .def_property_readonly("width", &Font::getWidth, R"doc(
Get the width in pixels of the current text.

Returns:
    int: The text width.
        )doc")
        .def_property_readonly("height", &Font::getHeight, R"doc(
Get the height in pixels of the current text.

Returns:
    int: The text height.
        )doc");
}
} // namespace font
} // namespace kn
