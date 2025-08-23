#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <pybind11/pybind11.h>
#include <string>

struct Color;
class Rect;
class Renderer;
class PixelArray;
enum class Anchor;
class Vec2;

namespace py = pybind11;

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
    Texture(const PixelArray& pixelArray);
    Texture(const std::string& filePath);
    ~Texture();

    void loadFromSDL(SDL_Texture* sdlTexture);

    Vec2 getSize() const;

    Rect getRect() const;

    void setTint(const Color& tint) const;

    Color getTint() const;

    void setAlpha(float alpha) const;

    float getAlpha() const;

    void makeAdditive() const;

    void makeMultiply() const;

    void makeNormal() const;

    void render(Rect dstRect, py::object srcRect);

    void render(py::object pos, Anchor anchor);

    SDL_Texture* getSDL() const;

  private:
    SDL_Texture* m_texPtr = nullptr;
};
