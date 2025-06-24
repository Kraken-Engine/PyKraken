#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct SDL_Window;
namespace math
{
class Vec2;
}

void init();

void quit();

namespace window
{
void _bind(py::module_& module);

SDL_Window* getWindow();

void create(const std::string& title, bool scaled, const py::object& sizeObj);

bool isOpen();

void close();

py::tuple getSize();

void setFullscreen(bool fullscreen);

bool isFullscreen();

void setTitle(const std::string& title);

std::string getTitle();
} // namespace window
