#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif // KRAKEN_ENABLE_PYTHON

#include <filesystem>
#include <string>

#include "Color.hpp"
#include "Math.hpp"
#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Rect;

namespace font
{
enum class Hinting
{
    Normal,
    Mono,
    Light,
    LightSubpixel,
    None,
};

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(const nb::module_& module);
#endif // KRAKEN_ENABLE_PYTHON

void _init();  // Initialize TTF library
void _quit();  // Clean up all fonts and shut down TTF
}  // namespace font

class Font
{
  public:
    Font(const std::filesystem::path& fileDir, int ptSize);
    ~Font();

    void setAlignment(TextAlign alignment) const;
    TextAlign getAlignment() const;

    void setHinting(font::Hinting hinting) const;
    font::Hinting getHinting() const;

    void setPtSize(int pt) const;
    int getPtSize() const;

    void setBold(bool on) const;
    void setItalic(bool on) const;
    void setUnderline(bool on) const;
    void setStrikethrough(bool on) const;

    bool isBold() const;
    bool isItalic() const;
    bool isUnderline() const;
    bool isStrikethrough() const;

    int getHeight() const;
    int getAscent() const;
    int getDescent() const;

    void setLineSpacing(int lineSpacing) const;
    int getLineSpacing() const;

    void setOutline(int outline) const;
    int getOutline() const;

    void setKerning(bool enabled) const;
    bool getKerning() const;

    /* NOT VALID UNTIL SDL_TTF 3.4.0
    void setCharSpacing(int charSpacing) const;
    int getCharSpacing() const;
    */

    TTF_Font* _get() const
    {
        return m_font;
    }

  private:
    TTF_Font* m_font = nullptr;

    friend void font::_quit();  // Allow font::_quit to access private members for cleanup
};
}  // namespace kn
