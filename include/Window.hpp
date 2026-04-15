#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Vec2;

namespace window
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

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

void setIcon(const std::filesystem::path& path);

void saveScreenshot(const std::filesystem::path& filePath);
}  // namespace window
}  // namespace kn
