#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <pybind11/pybind11.h>
#include <string>

#include "Color.hpp"
#include "Math.hpp"

namespace py = pybind11;

namespace kn
{
class Font
{
  public:
    Font(const std::string& fileDir, int ptSize);
    ~Font();

    void render(const std::string& text, const Vec2& pos = {}, const Color& color = {255, 255, 255},
                int wrapWidth = 0) const;

    void setBold(bool on) const;
    void setItalic(bool on) const;
    void setUnderline(bool on) const;
    void setStrikethrough(bool on) const;
    void setPtSize(int pt) const;

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
