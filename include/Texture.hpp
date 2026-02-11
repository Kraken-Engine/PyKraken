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

    Texture(int width, int height, TextureScaleMode scaleMode = TextureScaleMode::DEFAULT);
    Texture(
        const PixelArray& pixelArray, TextureScaleMode scaleMode = TextureScaleMode::DEFAULT,
        TextureAccess access = TextureAccess::STATIC
    );
    Texture(
        const std::string& filePath, TextureScaleMode scaleMode = TextureScaleMode::DEFAULT,
        TextureAccess access = TextureAccess::STATIC
    );
    ~Texture();

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] Vec2 getSize() const;

    [[nodiscard]] Rect getClipArea() const;
    void setClipArea(const Rect& area);

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
    int m_width = 0;
    int m_height = 0;
    Rect m_clipArea{};
};

namespace texture
{
void _bind(const py::module_& module);
}  // namespace texture
}  // namespace kn
