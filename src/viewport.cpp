#include "Viewport.hpp"

#include <pybind11/native_enum.h>
#include <pybind11/stl.h>

#include "Rect.hpp"
#include "Renderer.hpp"

namespace kn
{
namespace viewport
{
std::vector<Rect> layout(const uint8_t count, const ViewportMode mode)
{
    if (count > 4 || count < 2)
        throw std::runtime_error("'count' must be between 2 and 4");

    const Vec2 rRes = renderer::getTargetResolution();
    std::vector<Rect> viewports;
    viewports.reserve(count);

    switch (count)
    {
    case 4:
    {
        const Vec2 vpSize = rRes * 0.5;
        viewports.push_back({0.0, 0.0, vpSize});
        viewports.push_back({vpSize.x, 0.0, vpSize});
        viewports.push_back({0.0, vpSize.y, vpSize});
        viewports.push_back({vpSize, vpSize});
        break;
    }
    case 3:
    {
        const Vec2 vpSize = rRes * 0.5;
        viewports.push_back({0.0, 0.0, vpSize});
        viewports.push_back({vpSize.x, 0.0, vpSize});
        viewports.push_back({0.0, vpSize.y, {rRes.x, vpSize.y}});
        break;
    }
    case 2:
    {
        switch (mode)
        {
        case ViewportMode::HORIZONTAL:
        {
            const Vec2 vpSize{rRes.x, rRes.y * 0.5};
            viewports.push_back({0.0, 0.0, vpSize});
            viewports.push_back({0.0, vpSize.y, vpSize});
            break;
        }
        case ViewportMode::VERTICAL:
        {
            const Vec2 vpSize{rRes.x * 0.5, rRes.y};
            viewports.push_back({0.0, 0.0, vpSize});
            viewports.push_back({vpSize.x, 0.0, vpSize});
            break;
        }
        }
        break;
    }
    }

    return viewports;
}

void set(const Rect& rect)
{
    if (rect.w == 0 || rect.h == 0)
        throw std::runtime_error("Viewport width and height must be greater than zero");

    const SDL_Rect sdlRect = static_cast<SDL_Rect>(rect);
    if (!SDL_SetRenderViewport(renderer::_get(), &sdlRect))
        throw std::runtime_error(std::string("viewport::set failed: ") + SDL_GetError());
}

void unset()
{
    if (!SDL_SetRenderViewport(renderer::_get(), nullptr))
        throw std::runtime_error(std::string("viewport::unset failed: ") + SDL_GetError());
}

void _bind(py::module_& module)
{
    py::native_enum<ViewportMode>(module, "ViewportMode", "enum.IntEnum", R"doc(
Viewport layout mode for split-screen layouts.
    )doc")
        .value("VERTICAL", ViewportMode::VERTICAL, "Split viewports vertically")
        .value("HORIZONTAL", ViewportMode::HORIZONTAL, "Split viewports horizontally")
        .finalize();

    py::module_ subViewport = module.def_submodule("viewport", "Viewport management functions");

    subViewport.def(
        "layout", &layout, py::arg("count"), py::arg("mode") = ViewportMode::VERTICAL,
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

    subViewport.def("set", &set, py::arg("rect"), R"doc(
Set the current viewport to the given rectangle.

Args:
    rect (Rect): The rectangle defining the viewport.
                    )doc");

    subViewport.def("unset", &unset, R"doc(
Unset the current viewport, reverting to the full rendering area.
                    )doc");
}
}  // namespace viewport
}  // namespace kn
