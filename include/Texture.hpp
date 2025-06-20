#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <string>

struct Color;
class Rect;
class Renderer;

namespace py = pybind11;

namespace math
{
class Vec2;
}

namespace texture
{
void _bind(py::module_& module);
}

class Texture final
{
  public:
    double angle = 0.0;
    struct Flip
    {
        bool h = false;
        bool v = false;
    } flip;

    explicit Texture(SDL_Texture* sdlTexture);
    explicit Texture(const Renderer& renderer, const std::string& filePath);
    ~Texture();

    void loadFromSDL(SDL_Texture* sdlTexture);

    py::tuple getSize() const;

    Rect getRect() const;

    void setTint(const Color& tint) const;

    Color getTint() const;

    void setAlpha(float alpha) const;

    float getAlpha() const;

    void makeAdditive() const;

    void makeMultiply() const;

    void makeNormal() const;

    SDL_Texture* getSDL() const;

  private:
    SDL_Texture* m_texPtr = nullptr;
};
