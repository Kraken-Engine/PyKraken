#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include <string>

#include "Math.hpp"
#include "Rect.hpp"

namespace py = pybind11;

namespace kn
{
class PixelArray;
struct Color;

enum class TextureAccess
{
    STATIC = SDL_TEXTUREACCESS_STATIC,
    TARGET = SDL_TEXTUREACCESS_TARGET,
};

enum class TextureScaleMode
{
    NEAREST = SDL_SCALEMODE_NEAREST,
    LINEAR = SDL_SCALEMODE_LINEAR,
    PIXELART = SDL_SCALEMODE_PIXELART,
    DEFAULT
};

class Texture
{
  public:
    struct Flip
    {
        bool h = false;
        bool v = false;
    } flip;

    Texture(const Vec2& size, TextureScaleMode scaleMode = TextureScaleMode::DEFAULT);
    Texture(
        const PixelArray& pixelArray, TextureScaleMode scaleMode = TextureScaleMode::DEFAULT,
        TextureAccess access = TextureAccess::STATIC
    );
    Texture(
        const std::string& filePath, TextureScaleMode scaleMode = TextureScaleMode::DEFAULT,
        TextureAccess access = TextureAccess::STATIC
    );
    ~Texture();

    [[nodiscard]] double getWidth() const;

    [[nodiscard]] double getHeight() const;

    [[nodiscard]] Vec2 getSize() const;

    [[nodiscard]] Rect getRect() const;

    void setTint(const Color& tint) const;

    [[nodiscard]] Color getTint() const;

    void setAlpha(float alpha) const;

    [[nodiscard]] float getAlpha() const;

    void makeAdditive() const;

    void makeMultiply() const;

    void makeNormal() const;

    [[nodiscard]] SDL_Texture* getSDL() const;

  private:
    SDL_Texture* m_texPtr = nullptr;
    double m_width = 0.0;
    double m_height = 0.0;
};

namespace texture
{
void _bind(const py::module_& module);
}  // namespace texture
}  // namespace kn
