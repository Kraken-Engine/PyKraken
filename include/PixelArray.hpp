#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <pybind11/pybind11.h>
#include <string>

#include "_globals.hpp"

namespace py = pybind11;

class Vec2;
struct Color;
class Rect;

namespace pixel_array
{
void _bind(py::module_& module);
} // namespace pixel_array

// enum class ScrollType
// {
//     SCROLL_SMEAR,
//     SCROLL_ERASE,
//     SCROLL_REPEAT,
// };

class PixelArray
{
  public:
    PixelArray() = default;
    PixelArray(SDL_Surface* sdlSurface);
    PixelArray(const Vec2& size);
    PixelArray(const std::string& filePath);
    ~PixelArray();

    void fill(const Color& color) const;

    void blit(const PixelArray& other, const Vec2& pos, Anchor anchor, py::object srcRect) const;

    void blit(const PixelArray& other, const Rect& dstRect, py::object srcRect) const;

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

    std::unique_ptr<PixelArray> copy() const;

  private:
    SDL_Surface* m_surface = nullptr;
};
