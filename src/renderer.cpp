#include "Math.hpp"

#include "Renderer.hpp"

namespace kn::renderer
{
static SDL_Renderer* _renderer = nullptr;
static SDL_Texture* _target = nullptr;

void _bind(py::module_& module)
{
    auto subRenderer = module.def_submodule("renderer", "Functions for rendering graphics");

    subRenderer.def(
        "clear",
        [](const py::object& colorObj)
        {
            try
            {
                clear(colorObj.is_none() ? Color() : colorObj.cast<Color>());
            }
            catch (const py::cast_error&)
            {
                throw std::invalid_argument("Expected Color object, convertible sequence, or None");
            }
        },
        py::arg("color") = py::none(), R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
    )doc");

    subRenderer.def("clear", py::overload_cast<uint8_t, uint8_t, uint8_t, uint8_t>(&clear),
                    py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a") = 255, R"doc(
Clear the renderer with the specified color.

Args:
    r (int): Red component (0-255).
    g (int): Green component (0-255).
    b (int): Blue component (0-255).
    a (int, optional): Alpha component (0-255). Defaults to 255.
    )doc");

    subRenderer.def("present", &present, R"doc(
Present the rendered content to the screen.

This finalizes the current frame and displays it. Should be called after
all drawing operations for the frame are complete.
    )doc");

    subRenderer.def("get_res", &getResolution, R"doc(
Get the resolution of the renderer.

Returns:
    Vec2: The current rendering resolution as (width, height).
    )doc");
}

void _init(SDL_Window* window, const Vec2& resolution)
{
    _renderer = SDL_CreateRenderer(window, nullptr);
    if (_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    SDL_SetRenderLogicalPresentation(_renderer, static_cast<int>(resolution.x),
                                     static_cast<int>(resolution.y),
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    _target = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
                                static_cast<int>(resolution.x), static_cast<int>(resolution.y));
    SDL_SetTextureScaleMode(_target, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderTarget(_renderer, _target);
}

void _quit()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
}

void clear(const Color& color)
{
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(_renderer);
}

void clear(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderClear(_renderer);
}

Vec2 getResolution() { return {_target->w, _target->h}; }

void present()
{
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_RenderTexture(_renderer, _target, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
    SDL_SetRenderTarget(_renderer, _target);
}

SDL_Renderer* _get() { return _renderer; }
} // namespace kn::renderer
