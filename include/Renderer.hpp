#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include "Color.hpp"

namespace py = pybind11;

namespace kn
{
class Vec2;

namespace renderer
{
void _bind(py::module_& module);
void _init(SDL_Window* window, const Vec2& resolution);
void _quit();
SDL_Renderer* _get();

void clear(const Color& color = {});
void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
void present();
Vec2 getResolution();
} // namespace renderer
} // namespace kn
