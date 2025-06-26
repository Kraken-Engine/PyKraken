#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace renderer
{
void _bind(py::module_& module);
}

namespace math
{
class Vec2;
}

class Color;
struct SDL_Renderer;

class Texture;
class Circle;
class Line;
class Rect;

class Renderer
{
  public:
    explicit Renderer(const math::Vec2& resolution);
    ~Renderer();

    void clear(const Color& color);

    void present();

    math::Vec2 toView(const math::Vec2& windowCoord) const;

    void draw(const math::Vec2& point, const Color& color);
    void draw(const Texture& texture);
    void draw(const Circle& circle, const Color& color, int thickness);
    void draw(const Line& line, const Color& color, int thickness);
    void draw(const Rect& rect, const Color& color, int thickness);

    SDL_Renderer* getSDL() const;

  private:
    SDL_Renderer* m_renderer = nullptr;
};