#include "Text.hpp"

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "Camera.hpp"
#include "Font.hpp"
#include "Log.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"

namespace kn
{
static TTF_TextEngine* _textEngine = nullptr;
static std::vector<Text*> _textInstances;
static std::mutex _textsMutex;

Text::Text(const Font& font)
{
    if (!_textEngine)
    {
        throw std::runtime_error("Text engine not initialized; create a window first");
    }

    m_text = TTF_CreateText(_textEngine, font._get(), "", 0);
    if (!m_text)
    {
        throw std::runtime_error(std::string("Failed to create text: ") + SDL_GetError());
    }

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

void Text::setFont(const Font& font) const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    if (!TTF_SetTextFont(m_text, font._get()))
    {
        throw std::runtime_error(std::string("Failed to set text font: ") + SDL_GetError());
    }
}

void Text::draw(Vec2 pos, const Vec2& anchor) const
{
    if (!renderer::_get())
        throw std::runtime_error("Renderer not initialized");
    if (!TTF_GetTextFont(m_text))
        throw std::runtime_error("Text font is not set or has gone out of scope");

    const Vec2 cameraPos = camera::getActivePos();
    pos.x -= cameraPos.x;
    pos.y -= cameraPos.y;

    // Get text size so we can offset based on the anchor
    int textW = 0, textH = 0;
    TTF_GetTextSize(m_text, &textW, &textH);

    pos.x -= textW * anchor.x;
    pos.y -= textH * anchor.y;

    const int drawX = static_cast<int>(std::round(pos.x));
    const int drawY = static_cast<int>(std::round(pos.y));

    // Draw shadow if applicable
    if (shadowColor.a > 0 && !shadowOffset.isZero())
    {
        Color originalColor = getColor();
        setColor(shadowColor);

        const int shadowX = drawX + static_cast<int>(std::round(shadowOffset.x));
        const int shadowY = drawY + static_cast<int>(std::round(shadowOffset.y));
        TTF_DrawRendererText(m_text, shadowX, shadowY);

        setColor(originalColor);
    }

    TTF_DrawRendererText(m_text, drawX, drawY);
}

void Text::setWrapWidth(int wrapWidth) const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    if (wrapWidth < 0)
        wrapWidth = 0;
    if (!TTF_SetTextWrapWidth(m_text, wrapWidth))
    {
        throw std::runtime_error(std::string("Failed to set text wrap width: ") + SDL_GetError());
    }
}

int Text::getWrapWidth() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    int wrapWidth;
    if (!TTF_GetTextWrapWidth(m_text, &wrapWidth))
    {
        throw std::runtime_error(std::string("Failed to get text wrap width: ") + SDL_GetError());
    }
    return wrapWidth;
}

void Text::setText(const std::string& text) const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    if (!TTF_SetTextString(m_text, text.c_str(), 0))
    {
        throw std::runtime_error(std::string("Failed to set text string: ") + SDL_GetError());
    }
}

std::string Text::getText() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    return m_text->text ? std::string(m_text->text) : "";
}

void Text::setColor(const Color& color) const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    if (!TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a))
    {
        throw std::runtime_error(std::string("Failed to set text color: ") + SDL_GetError());
    }
}

Color Text::getColor() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    Color color;
    if (!TTF_GetTextColor(m_text, &color.r, &color.g, &color.b, &color.a))
    {
        throw std::runtime_error(std::string("Failed to get text color: ") + SDL_GetError());
    }
    return color;
}

Rect Text::getRect() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    int w, h;
    if (!TTF_GetTextSize(m_text, &w, &h))
    {
        throw std::runtime_error(std::string("Failed to get text size: ") + SDL_GetError());
    }
    return {0, 0, w, h};
}

Vec2 Text::getSize() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    int w, h;
    if (!TTF_GetTextSize(m_text, &w, &h))
    {
        throw std::runtime_error(std::string("Failed to get text size: ") + SDL_GetError());
    }
    return {w, h};
}

int Text::getWidth() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    int w;
    if (!TTF_GetTextSize(m_text, &w, nullptr))
    {
        throw std::runtime_error(std::string("Failed to get text width: ") + SDL_GetError());
    }
    return w;
}

int Text::getHeight() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    int h;
    if (!TTF_GetTextSize(m_text, nullptr, &h))
    {
        throw std::runtime_error(std::string("Failed to get text height: ") + SDL_GetError());
    }
    return h;
}

namespace text
{
void _init()
{
    _textEngine = TTF_CreateRendererTextEngine(renderer::_get());
    if (!_textEngine)
    {
        throw std::runtime_error(std::string("Failed to create text engine: ") + SDL_GetError());
    }
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
        .def(
            py::init<const Font&>(), py::arg("font"),
            R"doc(
Create a Text object.

Args:
    font (Font): The font to use for rendering this text.

Raises:
    RuntimeError: If text creation fails.
    )doc"
        )

        .def_readwrite("shadow_color", &Text::shadowColor, R"doc(
Get or set the shadow color for the text.
        )doc")
        .def_readwrite("shadow_offset", &Text::shadowOffset, R"doc(
Get or set the shadow offset for the text.
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
            [](const Text& self, const py::object& posObj, const py::object& anchorObj) -> void
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

                Vec2 anchor{};
                if (!anchorObj.is_none())
                {
                    try
                    {
                        anchor = anchorObj.cast<Vec2>();
                    }
                    catch (const py::cast_error&)
                    {
                        throw py::type_error("Invalid type for 'anchor', expected Vec2");
                    }
                }

                self.draw(pos, anchor);
            },
            py::arg("pos") = py::none(), py::arg("anchor") = py::none(), R"doc(
Draw the text to the renderer at the specified position with alignment.
A shadow is drawn if shadow_color.a > 0 and shadow_offset is not (0, 0).

Args:
    pos (Vec2 | None): The position in pixels. Defaults to (0, 0).
    anchor (Vec2 | None): The anchor point for alignment (0.0-1.0). Defaults to top left (0, 0).

Raises:
    RuntimeError: If the renderer is not initialized or text drawing fails.
    RuntimeError: If the text font is not set or has gone out of scope.
        )doc"
        )
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
}  // namespace text
}  // namespace kn
