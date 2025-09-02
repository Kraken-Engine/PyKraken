#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <pybind11/pybind11.h>
#include <string>

#include "_globals.hpp"
#include "Rect.hpp"

namespace py = pybind11;

namespace kn
{
class Vec2;
struct Color;

class PixelArray
{
public:
    PixelArray() = default;
    explicit PixelArray(SDL_Surface* sdlSurface);
    explicit PixelArray(const Vec2& size);
    explicit PixelArray(const std::string& filePath);
    ~PixelArray();

    void fill(const Color& color) const;

    void blit(const PixelArray& other, const Vec2& pos, Anchor anchor,
              const Rect& srcRect = {}) const;

    void blit(const PixelArray& other, const Rect& dstRect, const Rect& srcRect = {}) const;

    void setColorKey(const Color& color) const;

    [[nodiscard]] Color getColorKey() const;

    void setAlpha(uint8_t alpha) const;

    [[nodiscard]] int getAlpha() const;

    [[nodiscard]] Color getAt(const Vec2& coord) const;

    void setAt(const Vec2& coord, const Color& color) const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] Vec2 getSize() const;

    [[nodiscard]] Rect getRect() const;

    [[nodiscard]] SDL_Surface* getSDL() const;

    [[nodiscard]] std::unique_ptr<PixelArray> copy() const;

private:
    SDL_Surface* m_surface = nullptr;
};

namespace pixel_array
{
void _bind(const py::module_& module);

// enum class ScrollType
// {
//     SCROLL_SMEAR,
//     SCROLL_ERASE,
//     SCROLL_REPEAT,
// };
} // namespace pixel_array
} // namespace kn
