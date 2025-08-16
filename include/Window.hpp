#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

struct SDL_Window;
class Vec2;

void init();

void quit();

namespace window
{
void _bind(py::module_& module);

SDL_Window* get();

void create(const std::string& title, const Vec2& res, bool scaled);

bool isOpen();

void close();

Vec2 getSize();

float getScale();

void setFullscreen(bool fullscreen);

bool isFullscreen();

void setTitle(const std::string& title);

std::string getTitle();
} // namespace window
