#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <string>

#include "_globals.hpp"

namespace py = pybind11;

class Vec2;
struct Color;
class Rect;

namespace surface
{
void _bind(py::module_& module);
} // namespace surface

// enum class ScrollType
// {
//     SCROLL_SMEAR,
//     SCROLL_ERASE,
//     SCROLL_REPEAT,
// };

class Surface
{
  public:
    Surface() = default;
    Surface(SDL_Surface* sdlSurface);
    Surface(const Vec2& size);
    Surface(const std::string& filePath);
    ~Surface();

    void fill(const Color& color) const;

    void blit(const Surface& other, const Vec2& pos, Anchor anchor, const Rect& srcRect) const;
    void blit(const Surface& other, const Rect& dstRect, const Rect& srcRect) const;

    void setColorKey(const Color& color) const;

    Color getColorKey() const;

    void setAlpha(uint8_t alpha) const;

    int getAlpha() const;

    Color getAt(const Vec2& coord) const;

    void setAt(const Vec2& coord, const Color& color) const;

    int getWidth() const;

    int getHeight() const;

    Vec2 getSize() const;

    Rect getRect() const;

    SDL_Surface* getSDL() const;

    Surface copy() const;

  private:
    SDL_Surface* m_surface = nullptr;

    void setSDL(SDL_Surface* surface);
};
