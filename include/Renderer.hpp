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
    Renderer() = default;
    explicit Renderer(const math::Vec2& resolution);
    ~Renderer();

    // Renderer(const Renderer& other);
    // Renderer& operator=(const Renderer&) = delete;

    // Renderer(Renderer&&) noexcept = default;
    // Renderer& operator=(Renderer&&) noexcept = default;

    void clear(const Color& color);

    void present();

    void draw(const Texture& texture);

    SDL_Renderer* getSDL() const;

  private:
    SDL_Renderer* m_renderer = nullptr;
};