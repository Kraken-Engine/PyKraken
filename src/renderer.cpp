#include "Renderer.hpp"

#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/vector.h>

#include "Camera.hpp"
#include "Log.hpp"
#include "PixelArray.hpp"
#include "Texture.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr double CONVERSION = 180.0 / M_PI;
#define TO_DEGREES(x) (x * CONVERSION)

namespace kn::renderer
{
static SDL_Renderer* _renderer = nullptr;
static SDL_GPUDevice* _gpuDevice = nullptr;
static TextureScaleMode _defaultScaleMode = TextureScaleMode::LINEAR;

void _init(SDL_Window* window, const int width, const int height)
{
    _renderer = SDL_CreateGPURenderer(nullptr, window);
    if (_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    _gpuDevice = SDL_GetGPURendererDevice(_renderer);

    SDL_SetRenderLogicalPresentation(_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    const SDL_PropertiesID gpuProperties = SDL_GetGPUDeviceProperties(_gpuDevice);
    const char* gpuName =
        SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown Device");
    const char* driverName = SDL_GetStringProperty(
        gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING, "Unknown Driver"
    );
    const char* driverVersion = SDL_GetStringProperty(
        gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "Unknown Version"
    );
    const char* driverInfo =
        SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, "No Info");

    log::info("GPU Device: {}", gpuName);
    log::info("GPU Driver: {}", driverName);
    log::info("GPU Driver Version: {}", driverVersion);
    log::info("GPU Driver Info: {}", driverInfo);
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
    if (!SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set render draw color: " + std::string(SDL_GetError()));
    if (!SDL_RenderClear(_renderer))
        throw std::runtime_error("Failed to clear renderer: " + std::string(SDL_GetError()));
}

void clear(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    if (!SDL_SetRenderDrawColor(_renderer, r, g, b, a))
        throw std::runtime_error("Failed to set render draw color: " + std::string(SDL_GetError()));
    if (!SDL_RenderClear(_renderer))
        throw std::runtime_error("Failed to clear renderer: " + std::string(SDL_GetError()));
}

Vec2 getTargetResolution()
{
    if (SDL_Texture* target = SDL_GetRenderTarget(_renderer); target)
    {
        float w, h;
        if (!SDL_GetTextureSize(target, &w, &h))
            throw std::runtime_error(
                "Failed to get render target size: " + std::string(SDL_GetError())
            );
        return {w, h};
    }

    int w, h;
    if (!SDL_GetRenderLogicalPresentation(_renderer, &w, &h, nullptr))
        throw std::runtime_error(
            "Failed to get logical presentation size: " + std::string(SDL_GetError())
        );
    return {w, h};
}

void setTarget(const std::shared_ptr<Texture>& target)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not yet initialized");

    if (!target)
    {
        if (!SDL_SetRenderTarget(_renderer, nullptr))
            throw std::runtime_error(
                "Failed to unset render target: " + std::string(SDL_GetError())
            );
        return;
    }

    SDL_Texture* targetSDL = target->getSDL();

    const auto textureProperties = SDL_GetTextureProperties(targetSDL);
    if (textureProperties == 0)
        throw std::runtime_error(
            "Failed to get texture properties: " + std::string(SDL_GetError())
        );

    const auto access =
        SDL_GetNumberProperty(textureProperties, SDL_PROP_TEXTURE_ACCESS_NUMBER, -1);
    if (access == -1)
        throw std::runtime_error(
            "Failed to get texture access property: " + std::string(SDL_GetError())
        );

    if (access != SDL_TEXTUREACCESS_TARGET)
        throw std::runtime_error("Texture is not created with TARGET access");

    if (!SDL_SetRenderTarget(_renderer, targetSDL))
        throw std::runtime_error("Failed to set render target: " + std::string(SDL_GetError()));
}

void setDefaultScaleMode(const TextureScaleMode scaleMode)
{
    _defaultScaleMode = scaleMode;
}

TextureScaleMode getDefaultScaleMode()
{
    return _defaultScaleMode;
}

void present()
{
    if (!SDL_RenderPresent(_renderer))
        throw std::runtime_error("Failed to present renderer: " + std::string(SDL_GetError()));
}

std::unique_ptr<PixelArray> readPixels(const Rect& src)
{
    const auto sdlRect = static_cast<SDL_Rect>(src);
    SDL_Surface* surface = SDL_RenderReadPixels(_renderer, &sdlRect);
    if (!surface)
        throw std::runtime_error("Failed to read pixels: " + std::string(SDL_GetError()));

    return std::make_unique<PixelArray>(surface);
}

void draw(const Texture& texture, const Transform& transform, const Vec2& anchor, const Vec2& pivot)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not yet initialized");
    if (!texture.getSDL())
        throw std::runtime_error("Invalid texture provided for drawing");

    Rect clipArea = texture.getClipArea();
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;
    if (transform.scale.isZero() || texture.getAlpha() == 0.0f)
        return;

    Rect dstRect{0.0, 0.0, clipArea.getSize() * transform.scale};

    // Position based on anchor
    const Vec2 pos = transform.pos - camera::getActivePos();
    dstRect.setTopLeft(pos - (dstRect.getSize() * anchor));

    // cull if completely outside the screen
    const Vec2 targetRes = getTargetResolution();
    if (dstRect.getRight() < 0.0 || dstRect.x >= targetRes.x || dstRect.getBottom() < 0.0 ||
        dstRect.y >= targetRes.y)
    {
        return;
    }

    const auto dstSDLRect = static_cast<SDL_FRect>(dstRect);
    const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);

    // Pivot is normalized 0..1 relative to dstRect, for rotation center
    const auto pivotPoint = static_cast<SDL_FPoint>(dstRect.getSize() * pivot);

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    if (!SDL_RenderTextureRotated(
            _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(transform.angle),
            &pivotPoint, flipAxis
        ))
    {
        throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
    }
}

void drawBatch(
    const Texture& texture, const std::vector<Transform>& transforms, const Vec2& anchor,
    const Vec2& pivot
)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not yet initialized");
    if (!texture.getSDL())
        throw std::runtime_error("Invalid texture provided for drawing");
    if (transforms.empty())
        return;

    const Rect clipArea = texture.getClipArea();
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;
    if (texture.getAlpha() == 0.0f)
        return;

    const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);
    const Vec2 clipSize = clipArea.getSize();
    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = getTargetResolution();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    for (const auto& transform : transforms)
    {
        if (transform.scale.isZero())
            continue;

        Rect dstRect{0.0, 0.0, clipSize * transform.scale};
        const Vec2 pos = transform.pos - cameraPos;
        dstRect.setTopLeft(pos - (dstRect.getSize() * anchor));

        if (dstRect.getRight() < 0.0 || dstRect.x >= targetRes.x || dstRect.getBottom() < 0.0 ||
            dstRect.y >= targetRes.y)
            continue;

        const auto dstSDLRect = static_cast<SDL_FRect>(dstRect);
        const auto pivotPoint = static_cast<SDL_FPoint>(dstRect.getSize() * pivot);

        if (!SDL_RenderTextureRotated(
                _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(transform.angle),
                &pivotPoint, flipAxis
            ))
        {
            throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
        }
    }
}

SDL_Renderer* _get()
{
    return _renderer;
}

SDL_GPUDevice* _getGPUDevice()
{
    return _gpuDevice;
}

void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subRenderer = module.def_submodule("renderer", "Functions for rendering graphics");

    subRenderer.def("set_default_scale_mode", &setDefaultScaleMode, "scale_mode"_a, R"doc(
Set the default TextureScaleMode for new textures.

Args:
    scale_mode (TextureScaleMode): The default scaling/filtering mode to use for new textures.
    )doc");

    subRenderer.def("get_default_scale_mode", &getDefaultScaleMode, R"doc(
Get the current default TextureScaleMode for new textures.

Returns:
    TextureScaleMode: The current default scaling/filtering mode.
    )doc");

    subRenderer.def("clear", nb::overload_cast<const Color&>(&clear), "color"_a = Color{}, R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
        )doc");

    subRenderer.def(
        "clear", nb::overload_cast<uint8_t, uint8_t, uint8_t, uint8_t>(&clear), "r"_a, "g"_a, "b"_a,
        "a"_a = 255, R"doc(
Clear the renderer with the specified color.

Args:
    r (int): Red component (0-255).
    g (int): Green component (0-255).
    b (int): Blue component (0-255).
    a (int, optional): Alpha component (0-255). Defaults to 255.
    )doc"
    );

    subRenderer.def("present", &present, nb::call_guard<nb::gil_scoped_release>(), R"doc(
Present the rendered content to the screen.

This finalizes the current frame and displays it. Should be called after
all drawing operations for the frame are complete.
    )doc");

    subRenderer.def("get_target_resolution", &getTargetResolution, R"doc(
Get the resolution of the current render target.
If no target is set, returns the logical presentation resolution.

Returns:
    Vec2: The width and height of the render target.
    )doc");

    subRenderer.def("set_target", &setTarget, "target"_a, R"doc(
Set the current render target to the provided Texture, or unset if None.

Args:
    target (Texture, optional): Texture created with TextureAccess.TARGET, or None to unset.

Raises:
    RuntimeError: If the renderer is not initialized or the texture is not a TARGET texture.
        )doc");

    subRenderer.def(
        "draw", &draw, "texture"_a, "transform"_a = Transform{}, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER,
        R"doc(
Render a texture.

Args:
    texture (Texture): The texture to render.
    transform (Transform, optional): The transform (position, rotation, scale).
    anchor (Vec2, optional): The anchor point (0.0-1.0). Defaults to top left (0, 0).
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
        )doc"
    );

    subRenderer.def("read_pixels", &readPixels, "src"_a = Rect{}, R"doc(
Read pixel data from the renderer within the specified rectangle.

Args:
    src (Rect, optional): The rectangle area to read pixels from. Defaults to entire renderer if None.

Returns:
    PixelArray: An array containing the pixel data.

Raises:
    RuntimeError: If reading pixels fails.
        )doc");

    subRenderer.def(
        "draw_batch", &drawBatch, "texture"_a, "transforms"_a, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, nb::call_guard<nb::gil_scoped_release>(), R"doc(
Render a texture multiple times with different transforms in a single batch call.

This is significantly faster than calling draw() in a loop because it avoids
per-call Python/C++ dispatch overhead.

Args:
    texture (Texture): The texture to render.
    transforms (Sequence[Transform]): A list of transforms (position, rotation, scale).
    anchor (Vec2, optional): The anchor point (0.0-1.0). Defaults to top left (0, 0).
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
        )doc"
    );
}
}  // namespace kn::renderer
