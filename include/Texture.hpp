#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <string>

#include "_globals.hpp"
#include "Rect.hpp"
#include "Math.hpp"

namespace py = pybind11;

namespace kn
{
class PixelArray;
struct Color;

class Texture
{
public:
    double angle = 0.0;
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

    void render(Rect dstRect, const Rect& srcRect = {}) const;

    void render(Vec2 pos = {}, Anchor anchor = Anchor::Center) const;

    [[nodiscard]] SDL_Texture* getSDL() const;

private:
    SDL_Texture* m_texPtr = nullptr;
};

namespace texture
{
void _bind(const py::module_& module);
} // namespace texture
} // namespace kn
