#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Vec2;

void init();
void quit();

namespace window
{
void _bind(py::module_& module);
SDL_Window* _get();

void create(const std::string& title, const Vec2& res, bool scaled);

bool isOpen();

void close();

Vec2 getSize();

int getScale();

void setFullscreen(bool fullscreen);

bool isFullscreen();

void setTitle(const std::string& title);

std::string getTitle();
} // namespace window
} // namespace kn
