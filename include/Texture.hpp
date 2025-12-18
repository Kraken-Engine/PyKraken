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

class Texture
{
  public:
    struct Flip
    {
        bool h = false;
        bool v = false;
    } flip;

    explicit Texture(SDL_Texture* sdlTexture);
    explicit Texture(const PixelArray& pixelArray);
    explicit Texture(const std::string& filePath);
    ~Texture();

    void loadFromSDL(SDL_Texture* sdlTexture);

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
};

namespace texture
{
void _bind(const py::module_& module);
} // namespace texture
} // namespace kn
