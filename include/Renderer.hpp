#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

class Vec2;

enum class Anchor;
struct SDL_Renderer;
struct SDL_Texture;
class Color;
class Texture;
class Circle;
class Line;
class Rect;

namespace renderer
{
void _bind(py::module_& module);
}

class Renderer
{
  public:
    explicit Renderer(const Vec2& resolution);
    ~Renderer();

    void clear(const Color& color);

    void present();

    Vec2 toViewport(const Vec2& windowCoord) const;

    Vec2 getResolution() const;

    void draw(const Vec2& point, const Color& color);
    void draw(const Texture& texture, Rect dstRect, const Rect& srcRect);
    void draw(const Texture& texture, Vec2 pos, Anchor anchor);
    void draw(const Circle& circle, const Color& color, int thickness);
    void draw(const Line& line, const Color& color, int thickness);
    void draw(Rect rect, const Color& color, int thickness);

    SDL_Renderer* getSDL() const;

  private:
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_target = nullptr;
};