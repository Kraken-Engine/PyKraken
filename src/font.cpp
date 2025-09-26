#include "Font.hpp"
#include "Renderer.hpp"
#include "misc/minecraftia.h"
#include "misc/titillium.h"

#include <stdexcept>

static TTF_TextEngine* _textEngine = nullptr;

namespace kn
{
void _init()
{
    _textEngine = TTF_CreateRendererTextEngine(renderer::_get());
    if (!_textEngine)
        throw std::runtime_error("Failed to create text engine: " + std::string(SDL_GetError()));
}

void _quit()
{
    if (_textEngine)
        TTF_DestroyRendererTextEngine(_textEngine);
    _textEngine = nullptr;
}

Font::Font(const std::string& fileDir, int ptSize)
{
    if (ptSize < 8)
        ptSize = 8;

    if (fileDir == "kraken-clean")
    {
        SDL_IOStream* rw = SDL_IOFromMem(TitilliumWeb_Regular_ttf, TitilliumWeb_Regular_ttf_len);
        m_font = TTF_OpenFontIO(rw, true, ptSize);
    }
    else if (fileDir == "kraken-retro")
    {
        SDL_IOStream* rw = SDL_IOFromMem(Minecraftia_Regular_ttf, Minecraftia_Regular_ttf_len);
        const int ptSizeFixed = (ptSize + 4) / 8 * 8; // Round to nearest multiple of 8
        m_font = TTF_OpenFontIO(rw, true, ptSizeFixed);
    }
    else
    {
        m_font = TTF_OpenFont(fileDir.c_str(), ptSize);
    }

    if (!m_font)
        throw std::runtime_error("Failed to load font: " + std::string(SDL_GetError()));

    m_text = TTF_CreateText(_textEngine, m_font, "", 0);
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

void Font::render(const std::string& text, const Vec2& pos, const Color& color,
                  const uint32_t wrapWidth) const
{
    TTF_SetTextString(m_text, text.c_str(), 0);
    if (pos.x != 0.0 && pos.y != 0.0)
    {
        const auto x = static_cast<int>(std::round(pos.x));
        const auto y = static_cast<int>(std::round(pos.y));
        TTF_SetTextPosition(m_text, x, y);
    }
    if (color != WHITE)
        TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
    if (wrapWidth > 0)
        TTF_SetTextWrapWidth(m_text, wrapWidth);

    TTF_DrawRendererText(m_text, 0, 0);
}
} // namespace kn
