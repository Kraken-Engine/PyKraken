#include "kraken/graphics/Window.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>

#include <stdexcept>

#include "bindings/python/bindings.hpp"
#include "graphics/assets/kraken_icon.h"
#include "kraken/animation/AnimationController.hpp"
#include "kraken/animation/Ease.hpp"
#include "kraken/animation/Orchestrator.hpp"
#include "kraken/audio/Mixer.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/Font.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Text.hpp"
#include "kraken/math/Math.hpp"
#include "kraken/physics/World.hpp"

namespace kn::window
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subWindow = module.def_submodule("window", "Window related functions");

    subWindow
        .def("create", &create, "title"_a, "width"_a, "height"_a, "handle_close"_a = true, R"doc(
Create a window with the requested title and resolution.

Args:
    title (str): Non-empty title no longer than 255 characters.
    width (int): The window width, must be positive.
    height (int): The window height, must be positive.
    handle_close (bool): Whether to automatically handle the window quit event.

Raises:
    RuntimeError: If a window already exists or SDL window creation fails.
    ValueError: If the title is invalid or any dimension is non-positive.
    )doc");

    subWindow.def("is_open", &isOpen, R"doc(
Report whether the window is currently open.

Returns:
    bool: True if the window is open and active.
    )doc");

    subWindow.def("close", &close, R"doc(
Close the window.

Marks the window as closed, typically used to signal the main loop to exit.
This doesn't destroy the window immediately but sets the close flag.
    )doc");

    subWindow.def("set_fullscreen", &setFullscreen, "fullscreen"_a, R"doc(
Set the fullscreen mode of the window.

Args:
    fullscreen (bool): True to enable fullscreen mode, False for windowed mode.

Raises:
    RuntimeError: If the window is not initialized or fullscreen mode cannot be changed.
    )doc");

    subWindow.def("is_fullscreen", &isFullscreen, R"doc(
Check if the window is in fullscreen mode.

Returns:
    bool: True if the window is currently in fullscreen mode.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");

    subWindow.def(
        "get_size", &getSize,
        R"doc(
Get the current size of the window.

Returns:
    Vec2: The window size as (width, height).

Raises:
    RuntimeError: If the window is not initialized.
    )doc"
    );

    subWindow.def("get_scale", &getScale, R"doc(
Get the scale of the window relative to the renderer resolution.

Returns:
    int: The window's scale

Raises:
    RuntimeError: If the window is not initialized.
    )doc");

    subWindow.def("get_title", &getTitle, R"doc(
Get the current title of the window.

Returns:
    str: The current window title.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");

    subWindow.def("set_title", &setTitle, "title"_a, R"doc(
Set the title of the window.

Args:
    title (str): The new window title. Must be non-empty and <= 255 characters.

Raises:
    RuntimeError: If the window is not initialized or title setting fails.
    ValueError: If title is empty or exceeds 255 characters.
    )doc");

    subWindow.def("set_icon", &setIcon, "path"_a, R"doc(
Set the window icon from an image file.

Args:
    path (str | os.PathLike[str]): The file path to the image to use as the icon.

Raises:
    RuntimeError: If the window is not initialized or icon setting fails.
    )doc");

    subWindow.def("save_screenshot", &saveScreenshot, "path"_a, R"doc(
Save a screenshot of the current frame to a file.

Args:
    path (str | os.PathLike[str]): The path to save the screenshot to.

Raises:
    RuntimeError: If the window is not initialized or the screenshot cannot be saved.
)doc");
}
}  // namespace kn::window
