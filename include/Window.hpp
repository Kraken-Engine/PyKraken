#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct Color;
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

SDL_Renderer* getRenderer();

void create(const math::Vec2& resolution, const std::string& title, bool scaled);

bool isOpen();

void close();

void clear(const Color& color);

void flip();

py::tuple getSize();

float getScale();

void setFullscreen(bool fullscreen);

bool isFullscreen();
void setWindowIcon(SDL_Window* window, SDL_Surface* icon);
void setTitle(const std::string& title);

std::string getTitle();
} // namespace window
