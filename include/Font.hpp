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

class Font
{
  public:
    Font(const std::string& fileDir, int ptSize);
    ~Font();

    void draw(const Vec2& pos = {}, Anchor anchor = Anchor::TopLeft) const;

    void setWrapWidth(int wrapWidth) const;
    int getWrapWidth() const;

    void setText(const std::string& text) const;
    std::string getText() const;

    void setColor(const Color& color) const;
    Color getColor() const;

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

    Rect getRect() const;
    Vec2 getSize() const;
    int getWidth() const;
    int getHeight() const;

  private:
    TTF_Font* m_font = nullptr;
    TTF_Text* m_text = nullptr;
};

namespace font
{
void _bind(const py::module_& module);
void _init();
void _quit();
} // namespace font
} // namespace kn
