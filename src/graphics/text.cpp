#include "kraken/graphics/Text.hpp"

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "kraken/core/Log.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Font.hpp"
#include "kraken/graphics/Renderer.hpp"

namespace kn
{
static TTF_TextEngine* _textEngine = nullptr;
static std::vector<Text*> _textInstances;
static std::mutex _textsMutex;

Text::Text(const Font& font, const std::string& text)
{
    if (!_textEngine)
    {
        throw std::runtime_error("Text engine not initialized; create a window first");
    }

    m_text = TTF_CreateText(_textEngine, font._get(), text.c_str(), 0);
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

    if (m_text)
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

    pos = camera::worldToScreen(pos);

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
        throw std::runtime_error(std::string("Failed to set text wrap width: ") + SDL_GetError());
}

int Text::getWrapWidth() const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    int wrapWidth;
    if (!TTF_GetTextWrapWidth(m_text, &wrapWidth))
        throw std::runtime_error(std::string("Failed to get text wrap width: ") + SDL_GetError());

    return wrapWidth;
}

void Text::setText(const std::string& text) const
{
    if (!m_text)
        throw std::runtime_error("Text is destroyed or uninitialized");

    if (!TTF_SetTextString(m_text, text.c_str(), 0))
        throw std::runtime_error(std::string("Failed to set text string: ") + SDL_GetError());
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
        throw std::runtime_error(std::string("Failed to set text color: ") + SDL_GetError());
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
        if (text->m_text)
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

}  // namespace text
}  // namespace kn
