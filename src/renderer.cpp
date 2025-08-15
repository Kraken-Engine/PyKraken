#include "Renderer.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "_globals.hpp"

static SDL_Renderer* _renderer = nullptr;
static SDL_Texture* _target = nullptr;

namespace renderer
{
void _bind(py::module_& module)
{
    auto subRenderer = module.def_submodule("renderer", "Functions for rendering graphics");

    subRenderer.def("clear", &clear, py::arg("color") = Color{0, 0, 0, 255}, R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
    )doc");

    subRenderer.def("present", &present, R"doc(
Present the rendered content to the screen.

This finalizes the current frame and displays it. Should be called after
all drawing operations for the frame are complete.
    )doc");

    subRenderer.def("get_resolution", &getResolution, R"doc(
Get the resolution of the renderer.

Returns:
    Vec2: The current rendering resolution as (width, height).
    )doc");

    subRenderer.def("draw_texture",
                    py::overload_cast<const Texture&, Rect, const Rect&>(&drawTexture),
                    py::arg("texture"), py::arg("dst_rect"), py::arg("src_rect") = Rect(), R"doc(
Draw a texture with specified destination and source rectangles.

Args:
    texture (Texture): The texture to draw.
    dst_rect (Rect): The destination rectangle on the renderer.
    src_rect (Rect, optional): The source rectangle from the texture. 
                              Defaults to entire texture if not specified.
    )doc");

    subRenderer.def("draw_texture", py::overload_cast<const Texture&, Vec2, Anchor>(&drawTexture),
                    py::arg("texture"), py::arg("pos") = Vec2(), py::arg("anchor") = Anchor::CENTER,
                    R"doc(
Draw a texture at the specified position with anchor alignment.

Args:
    texture (Texture): The texture to draw.
    pos (Vec2, optional): The position to draw at. Defaults to (0, 0).
    anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    )doc");
}

void init(SDL_Window* window, const Vec2& resolution)
{
    _renderer = SDL_CreateRenderer(window, nullptr);
    if (_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    SDL_SetRenderLogicalPresentation(_renderer, resolution.x, resolution.y,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    _target = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
                                resolution.x, resolution.y);
    SDL_SetTextureScaleMode(_target, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderTarget(_renderer, _target);
}

void quit()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
}

void clear(const Color& color)
{
    if (!color._isValid())
        throw std::invalid_argument("Color values must be between 0 and 255");

    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(_renderer);
}

Vec2 getResolution()
{
    int w, h;
    SDL_GetRenderLogicalPresentation(_renderer, &w, &h, nullptr);
    return {w, h};
}

void drawTexture(const Texture& texture, Rect dstRect, const Rect& srcRect)
{
    SDL_Texture* sdlTexture = texture.getSDL();

    Vec2 cameraPos = camera::getActivePos();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    dstRect.x -= cameraPos.x;
    dstRect.y -= cameraPos.y;
    SDL_FRect dstSDLRect = dstRect;
    SDL_FRect srcSDLRect = (srcRect.getSize() == Vec2()) ? texture.getRect() : srcRect;

    SDL_RenderTextureRotated(_renderer, sdlTexture, &srcSDLRect, &dstSDLRect, texture.angle,
                             nullptr, flipAxis);
}

void drawTexture(const Texture& texture, Vec2 pos, const Anchor anchor)
{
    SDL_Texture* sdlTexture = texture.getSDL();

    Vec2 cameraPos = camera::getActivePos();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    pos -= cameraPos;
    Rect rect = texture.getRect();
    switch (anchor)
    {
    case Anchor::TOP_LEFT:
        rect.setTopLeft(pos);
        break;
    case Anchor::TOP_MID:
        rect.setTopMid(pos);
        break;
    case Anchor::TOP_RIGHT:
        rect.setTopRight(pos);
        break;
    case Anchor::MID_LEFT:
        rect.setMidLeft(pos);
        break;
    case Anchor::CENTER:
        rect.setCenter(pos);
        break;
    case Anchor::MID_RIGHT:
        rect.setMidRight(pos);
        break;
    case Anchor::BOTTOM_LEFT:
        rect.setBottomLeft(pos);
        break;
    case Anchor::BOTTOM_MID:
        rect.setBottomMid(pos);
        break;
    case Anchor::BOTTOM_RIGHT:
        rect.setBottomRight(pos);
        break;
    }

    SDL_FRect dstSDLRect = rect;
    SDL_RenderTextureRotated(_renderer, sdlTexture, nullptr, &dstSDLRect, texture.angle, nullptr,
                             flipAxis);
}

void present()
{
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_RenderTexture(_renderer, _target, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
    SDL_SetRenderTarget(_renderer, _target);
}

SDL_Renderer* get() { return _renderer; }

} // namespace renderer
