#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>

#include "Rect.hpp"
#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
class Vec2;
struct Color;

enum class ScrollMode
{
    SMEAR,
    ERASE,
    REPEAT,
};

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

    void scroll(int dx, int dy, ScrollMode scrollMode) const;

  private:
    SDL_Surface* m_surface = nullptr;
};

namespace pixel_array
{
void _bind(py::module_& module);

std::unique_ptr<PixelArray> flip(const PixelArray& pixelArray, bool flipX, bool flipY);

std::unique_ptr<PixelArray> scaleTo(const PixelArray& pixelArray, const Vec2& size);

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, double factor);

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, const Vec2& factor);

std::unique_ptr<PixelArray> rotate(const PixelArray& pixelArray, double angle);

std::unique_ptr<PixelArray> boxBlur(const PixelArray& pixelArray, int radius,
                                    bool repeatEdgePixels = true);

std::unique_ptr<PixelArray> gaussianBlur(const PixelArray& pixelArray, int radius,
                                         bool repeatEdgePixels = true);

std::unique_ptr<PixelArray> invert(const PixelArray& pixelArray);

std::unique_ptr<PixelArray> grayscale(const PixelArray& pixelArray);
}  // namespace pixel_array
}  // namespace kn
