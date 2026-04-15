#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>
#include <string>

#include "Rect.hpp"
#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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
    explicit PixelArray(const std::filesystem::path& filePath);
    ~PixelArray();

    // Move semantics
    PixelArray(const PixelArray&) = delete;
    PixelArray& operator=(const PixelArray&) = delete;
    PixelArray(PixelArray&& other) noexcept;
    PixelArray& operator=(PixelArray&& other) noexcept;

    void fill(const Color& color) const;

    void blit(
        const PixelArray& other, const Vec2& pos, const Vec2& anchor = Anchor::TOP_LEFT,
        const Rect& srcRect = {}
    ) const;

    void blit(const PixelArray& other, const Rect& dstRect, const Rect& srcRect = {}) const;

    void setColorKey(const Color& color) const;

    [[nodiscard]] Color getColorKey() const;

    void setAlpha(uint8_t alpha) const;

    [[nodiscard]] int getAlpha() const;

    [[nodiscard]] Color getAt(int x, int y) const;

    void setAt(int x, int y, const Color& color) const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] Vec2 getSize() const;

    [[nodiscard]] Rect getRect() const;

    [[nodiscard]] SDL_Surface* getSDL() const;

    [[nodiscard]] PixelArray copy() const;

    void scroll(int dx, int dy, ScrollMode scrollMode) const;

  private:
    SDL_Surface* m_surface = nullptr;
};

namespace pixel_array
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

PixelArray flip(const PixelArray& pixelArray, bool flipX, bool flipY);
PixelArray scaleTo(const PixelArray& pixelArray, const Vec2& size);
PixelArray scaleBy(const PixelArray& pixelArray, double factor);
PixelArray scaleBy(const PixelArray& pixelArray, const Vec2& factor);
PixelArray rotate(const PixelArray& pixelArray, double angle);
PixelArray boxBlur(const PixelArray& pixelArray, int radius, bool repeatEdgePixels = true);
PixelArray gaussianBlur(const PixelArray& pixelArray, int radius, bool repeatEdgePixels = true);
PixelArray invert(const PixelArray& pixelArray);
PixelArray grayscale(const PixelArray& pixelArray);

}  // namespace pixel_array
}  // namespace kn
