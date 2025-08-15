#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Vec2;
enum class Anchor;
struct Color;
class Texture;
class Rect;

namespace renderer
{
void _bind(py::module_& module);

void init(SDL_Window* window, const Vec2& resolution);
void quit();

void clear(const Color& color);
void present();

Vec2 getResolution();

void drawTexture(const Texture& texture, Rect dstRect, const Rect& srcRect);
void drawTexture(const Texture& texture, Vec2 pos, Anchor anchor);

SDL_Renderer* get();
} // namespace renderer
