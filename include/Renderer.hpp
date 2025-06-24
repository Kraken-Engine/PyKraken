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

class Renderer
{
  public:
    explicit Renderer(const math::Vec2& resolution);
    ~Renderer() = default;

    void clear(const Color& color);

    void present();

    void draw(const Texture& texture);

    void destroy();

    SDL_Renderer* getSDL() const;

  private:
    SDL_Renderer* m_renderer = nullptr;
};