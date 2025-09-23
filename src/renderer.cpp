#include "Renderer.hpp"
#include "Camera.hpp"
#include "Texture.hpp"

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

    subRenderer.def(
        "draw",
        [](const Texture& texture, const Rect& dstRect, const py::object& srcObj)
        {
            try
            {
                srcObj.is_none() ? draw(texture, dstRect)
                                 : draw(texture, dstRect, srcObj.cast<Rect>());
            }
            catch (const py::cast_error&)
            {
                throw std::invalid_argument("'src' must be a Rect");
            }
        },
        py::arg("texture"), py::arg("dst"), py::arg("src") = py::none(), R"doc(
    Render a texture with specified destination and source rectangles.

    Args:
        texture (Texture): The texture to render.
        dst (Rect): The destination rectangle on the renderer.
        src (Rect, optional): The source rectangle from the texture. Defaults to entire texture if not specified.
        )doc");

    subRenderer.def(
        "draw",
        [](const Texture& texture, const py::object& pos, const Anchor anchor)
        {
            try
            {
                const auto posVec = pos.is_none() ? Vec2() : pos.cast<Vec2>();
                draw(texture, posVec, anchor);
            }
            catch (const py::cast_error&)
            {
                throw std::invalid_argument("'pos' must be a Vec2 and 'src' must be a Rect");
            }
        },
        py::arg("texture"), py::arg("pos") = py::none(), py::arg("anchor") = Anchor::Center, R"doc(
    Render a texture at the specified position with anchor alignment.

    Args:
        texture (Texture): The texture to render.
        pos (Vec2, optional): The position to draw at. Defaults to (0, 0).
        anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
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

void draw(const Texture& texture, Rect dstRect, const Rect& srcRect)
{
    const Vec2 cameraPos = camera::getActivePos();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    dstRect.x -= cameraPos.x;
    dstRect.y -= cameraPos.y;
    const SDL_FRect dstSDLRect = {
        std::roundf(static_cast<float>(dstRect.x)),
        std::roundf(static_cast<float>(dstRect.y)),
        std::roundf(static_cast<float>(dstRect.w)),
        std::roundf(static_cast<float>(dstRect.h)),
    };
    const auto srcSDLRect =
        static_cast<SDL_FRect>(srcRect.w == 0.0 && srcRect.h == 0.0 ? texture.getRect() : srcRect);
    SDL_RenderTextureRotated(_renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, texture.angle,
                             nullptr, flipAxis);
}

void draw(const Texture& texture, Vec2 pos, const Anchor anchor)
{
    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    pos -= camera::getActivePos();
    Rect rect = texture.getRect();
    switch (anchor)
    {
    case Anchor::TopLeft:
        rect.setTopLeft(pos);
        break;
    case Anchor::TopMid:
        rect.setTopMid(pos);
        break;
    case Anchor::TopRight:
        rect.setTopRight(pos);
        break;
    case Anchor::MidLeft:
        rect.setMidLeft(pos);
        break;
    case Anchor::Center:
        rect.setCenter(pos);
        break;
    case Anchor::MidRight:
        rect.setMidRight(pos);
        break;
    case Anchor::BottomLeft:
        rect.setBottomLeft(pos);
        break;
    case Anchor::BottomMid:
        rect.setBottomMid(pos);
        break;
    case Anchor::BottomRight:
        rect.setBottomRight(pos);
        break;
    }

    const SDL_FRect dstSDLRect = {
        std::roundf(static_cast<float>(rect.x)),
        std::roundf(static_cast<float>(rect.y)),
        std::roundf(static_cast<float>(rect.w)),
        std::roundf(static_cast<float>(rect.h)),
    };
    SDL_RenderTextureRotated(_renderer, texture.getSDL(), nullptr, &dstSDLRect, texture.angle,
                             nullptr, flipAxis);
}

SDL_Renderer* _get() { return _renderer; }
} // namespace kn::renderer
