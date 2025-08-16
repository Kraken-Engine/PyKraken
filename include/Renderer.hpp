#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Vec2;
struct Color;

namespace renderer
{
void _bind(py::module_& module);

void init(SDL_Window* window, const Vec2& resolution);

void quit();

void clear(py::object color);

void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void present();

Vec2 getResolution();

SDL_Renderer* get();
} // namespace renderer
