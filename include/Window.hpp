#pragma once

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace kn
{
class Vec2;

namespace window
{
void _bind(nb::module_& module);
void _quit();
bool _handlesClose();

SDL_Window* _get();

void create(const std::string& title, int width, int height, bool handleClose = true);

bool isOpen();

void close();

Vec2 getSize();

int getScale();

void setFullscreen(bool fullscreen);

bool isFullscreen();

void setTitle(const std::string& title);

std::string getTitle();

void setIcon(const std::string& path);

void saveScreenshot(const std::string& filePath);
}  // namespace window
}  // namespace kn
