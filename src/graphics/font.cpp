#include "kraken/graphics/Font.hpp"

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "graphics/assets/SpaceGrotesk.h"
#include "graphics/assets/minecraftia.h"
#include "kraken/core/Log.hpp"

namespace kn
{
namespace font
{
// Static registry to track all font instances for proper cleanup
static std::vector<Font*> _fontInstances;
static std::mutex _fontsMutex;
}  // namespace font

Font::Font(const std::filesystem::path& fileDir, int ptSize)
{
    if (ptSize < 8)
        ptSize = 8;

    if (fileDir.string() == "kraken-modern")
    {
        SDL_IOStream* rw = SDL_IOFromMem(SpaceGrotesk_ttf, SpaceGrotesk_ttf_len);
        m_font = TTF_OpenFontIO(rw, true, static_cast<float>(ptSize));
    }
    else if (fileDir.string() == "kraken-retro")
    {
        SDL_IOStream* rw = SDL_IOFromMem(Minecraftia_Regular_ttf, Minecraftia_Regular_ttf_len);
        const int ptSizeFixed = (ptSize + 4) / 8 * 8;  // Round to the nearest multiple of 8
        m_font = TTF_OpenFontIO(rw, true, static_cast<float>(ptSizeFixed));
    }
    else
    {
        m_font = TTF_OpenFont(fileDir.string().c_str(), static_cast<float>(ptSize));
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
            font::_fontInstances.erase(it);
    }

    // Only clean up if font hasn't been freed by _quit
    if (!m_font)
    {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}

void Font::setAlignment(const TextAlign alignment) const
{
    switch (alignment)
    {
    case TextAlign::Left:
        TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_LEFT);
        break;
    case TextAlign::Center:
        TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_CENTER);
        break;
    case TextAlign::Right:
        TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_RIGHT);
        break;
    }
}

TextAlign Font::getAlignment() const
{
    const TTF_HorizontalAlignment align = TTF_GetFontWrapAlignment(m_font);
    switch (align)
    {
    case TTF_HORIZONTAL_ALIGN_LEFT:
        return TextAlign::Left;
    case TTF_HORIZONTAL_ALIGN_CENTER:
        return TextAlign::Center;
    case TTF_HORIZONTAL_ALIGN_RIGHT:
        return TextAlign::Right;
    default:
        return TextAlign::Left;
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

int Font::getPtSize() const
{
    return static_cast<int>(TTF_GetFontSize(m_font));
}

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

int Font::getHeight() const
{
    return TTF_GetFontHeight(m_font);
}

int Font::getAscent() const
{
    return TTF_GetFontAscent(m_font);
}

int Font::getDescent() const
{
    return TTF_GetFontDescent(m_font);
}

void Font::setLineSpacing(const int lineSpacing) const
{
    TTF_SetFontLineSkip(m_font, lineSpacing);
}

int Font::getLineSpacing() const
{
    return TTF_GetFontLineSkip(m_font);
}

void Font::setOutline(const int outline) const
{
    TTF_SetFontOutline(m_font, outline);
}

int Font::getOutline() const
{
    return TTF_GetFontOutline(m_font);
}

void Font::setKerning(const bool enabled) const
{
    TTF_SetFontKerning(m_font, enabled);
}

bool Font::getKerning() const
{
    return TTF_GetFontKerning(m_font) != 0;
}

/*
void Font::setCharSpacing(const int charSpacing) const
{
    TTF_SetFontCharSpacing(m_font, charSpacing);
}

int Font::getCharSpacing() const { return TTF_GetFontCharSpacing(m_font); }
*/

namespace font
{
void _init()
{
    if (!TTF_Init())
        throw std::runtime_error("Failed to initialize SDL_ttf");

    log::info(
        "SDL_ttf version: {}.{}.{}", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,
        SDL_TTF_MICRO_VERSION
    );
}

void _quit()
{
    // Clean up all fonts before TTF is shut down
    {
        std::lock_guard g(_fontsMutex);
        for (Font* font : _fontInstances)
        {
            if (font->m_font)
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

}  // namespace font
}  // namespace kn
