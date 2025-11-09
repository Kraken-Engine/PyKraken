#include "Text.hpp"
#include "Font.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <vector>

namespace kn
{
static TTF_TextEngine* _textEngine = nullptr;
static std::vector<Text*> _textInstances;
static std::mutex _textsMutex;

Text::Text(const Font& font)
{
    m_text = TTF_CreateText(_textEngine, font._get(), "", 0);
    TTF_SetTextColor(m_text, 255, 255, 255, 255);

    // Register this text for cleanup
    std::lock_guard g(_textsMutex);
    _textInstances.push_back(this);
}

Text::~Text()
{
    // Remove from registry if still present
    {
        std::lock_guard g(_textsMutex);
        auto it = std::find(_textInstances.begin(), _textInstances.end(), this);
        if (it != _textInstances.end())
        {
            _textInstances.erase(it);
        }
    }

    // Only clean up if text hasn't been freed by _cleanupTexts
    if (m_text != nullptr)
    {
        TTF_DestroyText(m_text);
        m_text = nullptr;
    }
}

void Text::setFont(const Font& font) const { TTF_SetTextFont(m_text, font._get()); }

void Text::draw(const Vec2& pos, const Anchor anchor) const
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

void Text::setWrapWidth(int wrapWidth) const
{
    if (wrapWidth < 0)
        wrapWidth = 0;
    TTF_SetTextWrapWidth(m_text, wrapWidth);
}

int Text::getWrapWidth() const
{
    int wrapWidth;
    TTF_GetTextWrapWidth(m_text, &wrapWidth);
    return wrapWidth;
}

void Text::setText(const std::string& text) const { TTF_SetTextString(m_text, text.c_str(), 0); }

std::string Text::getText() const { return std::string(m_text->text); }

void Text::setColor(const Color& color) const
{
    TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
}

Color Text::getColor() const
{
    Color color;
    TTF_GetTextColor(m_text, &color.r, &color.g, &color.b, &color.a);
    return color;
}

Rect Text::getRect() const
{
    int w, h;
    TTF_GetTextSize(m_text, &w, &h);
    return {0, 0, w, h};
}

Vec2 Text::getSize() const
{
    int w, h;
    TTF_GetTextSize(m_text, &w, &h);
    return {w, h};
}

int Text::getWidth() const
{
    int w;
    TTF_GetTextSize(m_text, &w, nullptr);
    return w;
}

int Text::getHeight() const
{
    int h;
    TTF_GetTextSize(m_text, nullptr, &h);
    return h;
}

namespace text
{
void _init()
{
    _textEngine = TTF_CreateRendererTextEngine(renderer::_get());
    if (!_textEngine)
        throw std::runtime_error("Failed to create text engine: " + std::string(SDL_GetError()));
}

void _cleanupTexts()
{
    // Clean up all text objects before text engine is destroyed
    std::lock_guard g(_textsMutex);
    for (Text* text : _textInstances)
    {
        if (text->m_text != nullptr)
        {
            TTF_DestroyText(text->m_text);
            text->m_text = nullptr;
        }
    }
    _textInstances.clear();
}

void _quit()
{
    // Clean up all text objects first
    _cleanupTexts();

    if (_textEngine)
        TTF_DestroyRendererTextEngine(_textEngine);
    _textEngine = nullptr;
}

void _bind(const py::module_& module)
{
    py::classh<Text>(module, "Text", R"doc(
A text object for rendering text to the active renderer.

This class handles the rendered text instance. You must provide a Font object
when creating a Text instance.

Note:
    A window/renderer must be created before using text. Typically you should
    call kn.window.create(...) first, which initializes the text engine.
    )doc")
        .def(py::init<const Font&>(), py::arg("font"),
             R"doc(
Create a Text object.

Args:
    font (Font): The font to use for rendering this text.

Raises:
    RuntimeError: If text creation fails.
    )doc")

        .def_property("wrap_width", &Text::getWrapWidth, &Text::setWrapWidth, R"doc(
Get or set the wrap width in pixels for text wrapping.

Set to 0 to disable wrapping. Negative values are clamped to 0.
        )doc")
        .def_property("text", &Text::getText, &Text::setText, R"doc(
Get or set the text string to be rendered.
        )doc")
        .def_property("color", &Text::getColor, &Text::setColor, R"doc(
Get or set the color of the rendered text.
        )doc")
        .def_property_readonly("size", &Text::getSize, R"doc(
Get the size (width, height) of the current text as a Vec2.

Returns:
    Vec2: The text dimensions.
        )doc")
        .def_property_readonly("width", &Text::getWidth, R"doc(
Get the width in pixels of the current text.

Returns:
    int: The text width.
        )doc")
        .def_property_readonly("height", &Text::getHeight, R"doc(
Get the height in pixels of the current text.

Returns:
    int: The text height.
        )doc")

        .def(
            "draw",
            [](const Text& self, const py::object& posObj, const Anchor anchor) -> void
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
        .def("set_font", &Text::setFont, py::arg("font"), R"doc(
Set the font to use for rendering this text.

Args:
    font (Font): The font to use.
        )doc")
        .def("get_rect", &Text::getRect, R"doc(
Get the bounding rectangle of the current text.

Returns:
    Rect: A rectangle with x=0, y=0, and width/height of the text.
    )doc");
}
} // namespace text
} // namespace kn
