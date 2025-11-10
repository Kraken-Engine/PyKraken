#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <pybind11/pybind11.h>
#include <string>

#include "Color.hpp"
#include "Math.hpp"
#include "_globals.hpp"

namespace py = pybind11;

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

void _bind(const py::module_& module);
void _init(); // Initialize TTF library
void _quit(); // Clean up all fonts and shut down TTF
} // namespace font

class Font
{
  public:
    Font(const std::string& fileDir, int ptSize);
    ~Font();

    void setAlignment(Align alignment) const;
    Align getAlignment() const;

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

    void setLineSkip(int lineSkip) const;
    int getLineSkip() const;

    void setOutline(int outline) const;
    int getOutline() const;

    void setKerning(bool enabled) const;
    bool getKerning() const;

    void setCharSpacing(int charSpacing) const;
    int getCharSpacing() const;

    TTF_Font* _get() const { return m_font; }

  private:
    TTF_Font* m_font = nullptr;

    friend void font::_quit(); // Allow font::_quit to access private members for cleanup
};
} // namespace kn
