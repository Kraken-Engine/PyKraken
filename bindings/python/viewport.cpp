#include "kraken/graphics/Viewport.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Renderer.hpp"

namespace kn::viewport
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::enum_<ViewportMode>(module, "ViewportMode", R"doc(
Viewport layout mode for split-screen layouts.
    )doc")
        .value("VERTICAL", ViewportMode::VERTICAL, "Split viewports vertically")
        .value("HORIZONTAL", ViewportMode::HORIZONTAL, "Split viewports horizontally");

    auto subViewport = module.def_submodule("viewport", "Viewport management functions");

    subViewport.def(
        "layout", &layout, "count"_a, "mode"_a = ViewportMode::VERTICAL,
        R"doc(
Layout the screen into multiple viewports.
The viewports are created with the current renderer target resolution in mind.

Args:
    count (int): The number of viewports to create (between 2 and 4).
    mode (ViewportMode, optional): The layout mode for 2 viewports (VERTICAL or HORIZONTAL).
                              Defaults to VERTICAL.

Returns:
    list[Rect]: A list of Rects representing the viewports.
                    )doc"
    );

    subViewport.def("set", &set, "rect"_a, R"doc(
Set the current viewport to the given rectangle.

Args:
    rect (Rect): The rectangle defining the viewport.
                    )doc");

    subViewport.def("unset", &unset, R"doc(
Unset the current viewport, reverting to the full rendering area.
                    )doc");
}
}  // namespace kn::viewport
