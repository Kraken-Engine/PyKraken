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
class Font;

namespace text
{
void _bind(const py::module_& module);
void _init();
void _quit();
void _cleanupTexts();  // Clean up all text objects before text engine is destroyed
}  // namespace text

class Text
{
  public:
    Color shadowColor{};
    Vec2 shadowOffset{};

    Text(const Font& font);
    ~Text();

    void draw(Vec2 pos = {}, Anchor anchor = Anchor::TopLeft) const;

    void setFont(const Font& font) const;

    void setWrapWidth(int wrapWidth) const;
    int getWrapWidth() const;

    void setText(const std::string& text) const;
    std::string getText() const;

    void setColor(const Color& color) const;
    Color getColor() const;

    Rect getRect() const;
    Vec2 getSize() const;
    int getWidth() const;
    int getHeight() const;

  private:
    TTF_Text* m_text = nullptr;

    friend void text::_cleanupTexts();  // Allow text::_cleanupTexts to access private members
};
}  // namespace kn
