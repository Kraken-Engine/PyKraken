#include "Renderer.hpp"

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
static Texture* _primaryTarget = nullptr;

static Vec2 _size{};

void _init(SDL_Window* window, const int width, const int height)
{
    _size = {width, height};

    _renderer = SDL_CreateGPURenderer(nullptr, window);
    if (_renderer == nullptr)
    {
        log::warn(
            "SDL_GPU backend failed to initialize, falling back to legacy renderer. Reason: {}",
            SDL_GetError()
        );
        _renderer = SDL_CreateRenderer(window, nullptr);
    }

    if (_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    _gpuDevice = SDL_GetGPURendererDevice(_renderer);

    SDL_SetRenderLogicalPresentation(_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    if (_gpuDevice)
    {
        const SDL_PropertiesID gpuProperties = SDL_GetGPUDeviceProperties(_gpuDevice);
        const char* gpuName =
            SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown");
        const char* driverName =
            SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING, "Unknown");
        const char* driverVersion = SDL_GetStringProperty(
            gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "Unknown"
        );
        const char* driverInfo =
            SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, "None");

        log::info("GPU Device: {}", gpuName);
        log::info("GPU Driver: {}", driverName);
        log::info("GPU Driver Version: {}", driverVersion);
        log::info("GPU Driver Info: {}", driverInfo);
    }
    else
    {
        const SDL_PropertiesID rendererProperties = SDL_GetRendererProperties(_renderer);
        const char* backend =
            SDL_GetStringProperty(rendererProperties, SDL_PROP_RENDERER_NAME_STRING, "Unknown");
        log::info("Using fallback renderer backend: {}", backend);
    }
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

void setTarget(const Texture* target)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not yet initialized");

    if (!target)
    {
        if (!SDL_SetRenderTarget(
                _renderer, (_primaryTarget != nullptr) ? _primaryTarget->getSDL() : nullptr
            ))
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
    // Regular present if no custom renderer size
    if (_primaryTarget == nullptr)
    {
        if (!SDL_RenderPresent(_renderer))
            throw std::runtime_error("Failed to present renderer: " + std::string(SDL_GetError()));
        return;
    }

    // Hold old cam pos and set pos to origin
    auto currCamera = camera::_getActiveCamera();
    Vec2 cameraPos;
    if (currCamera)
    {
        cameraPos = currCamera->getPos();
        currCamera->setPos({});
    }

    // Truly reset render target since SetTarget bit my butt
    if (!SDL_SetRenderTarget(_renderer, nullptr))
        throw std::runtime_error("Failed to unset render target: " + std::string(SDL_GetError()));

    // Draw custom render size, scaled up to true renderer
    draw(*_primaryTarget, Rect{_size});

    // Finally present
    if (!SDL_RenderPresent(_renderer))
        throw std::runtime_error("Failed to present renderer: " + std::string(SDL_GetError()));

    // Restore custom renderer size and cam pos
    setTarget(_primaryTarget);
    if (currCamera)
        currCamera->setPos(cameraPos);
}

void setPresentResolution(const int width, const int height)
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Resolution width and height must be positive integers");

    if (_primaryTarget != nullptr)
    {
        delete _primaryTarget;
        _primaryTarget = nullptr;
    }

    _primaryTarget = new Texture(width, height, TextureScaleMode::NEAREST);
    setTarget(_primaryTarget);
}

Vec2 getCurrentResolution()
{
    SDL_Texture* currentTarget = SDL_GetRenderTarget(_renderer);
    if (currentTarget)
    {
        if (_primaryTarget && currentTarget == _primaryTarget->getSDL())
            return _primaryTarget->getSize();

        float w, h;
        if (!SDL_GetTextureSize(currentTarget, &w, &h))
        {
            throw std::runtime_error(
                "Failed to get render target size: " + std::string(SDL_GetError())
            );
        }

        return Vec2{w, h};
    }

    if (_primaryTarget)
        return _primaryTarget->getSize();

    return _size;
}

std::unique_ptr<PixelArray> readPixels(const Rect& src)
{
    if (src.w < 0.0 || src.h < 0.0)
        throw std::invalid_argument("Source rectangle must have positive width and height");

    const auto sdlRect = static_cast<SDL_Rect>(src);
    const bool hasSize = (src.w > 0.0 && src.h > 0.0);

    SDL_Surface* surface = SDL_RenderReadPixels(_renderer, hasSize ? &sdlRect : nullptr);
    if (!surface)
        throw std::runtime_error("Failed to read pixels: " + std::string(SDL_GetError()));

    return std::make_unique<PixelArray>(surface);
}

void draw(const Texture& texture, const Transform& transform, const Vec2& anchor, const Vec2& pivot)
{
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

    // cull using the logical resolution (camera/view is already in logical/world coords)
    const Vec2 rendRes = getCurrentResolution();
    if (dstRect.getRight() < 0.0 || dstRect.x >= rendRes.x || dstRect.getBottom() < 0.0 ||
        dstRect.y >= rendRes.y)
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

void draw(const Texture& texture, const Rect& dst)
{
    if (!texture.getSDL())
        throw std::runtime_error("Invalid texture provided for drawing");
    if (texture.getAlpha() == 0.0f)
        return;

    const Rect clipArea = texture.getClipArea();
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;

    Rect dstRect = dst;
    dstRect.x -= camera::getActivePos().x;
    dstRect.y -= camera::getActivePos().y;

    const Vec2 rendRes = getCurrentResolution();
    if (dstRect.getRight() < 0.0 || dstRect.x >= rendRes.x || dstRect.getBottom() < 0.0 ||
        dstRect.y >= rendRes.y)
        return;

    const auto dstSDLRect = static_cast<SDL_FRect>(dstRect);
    const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    if (!SDL_RenderTextureRotated(
            _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, 0.0, nullptr, flipAxis
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
    const Vec2 rendRes = getCurrentResolution();

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

        if (dstRect.getRight() < 0.0 || dstRect.x >= rendRes.x || dstRect.getBottom() < 0.0 ||
            dstRect.y >= rendRes.y)
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

void drawBatchNDArray(
    const Texture& texture,
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr, const Vec2& anchor,
    const Vec2& pivot
)
{
    if (!texture.getSDL())
        throw std::runtime_error("Invalid texture provided for drawing");

    const auto n = static_cast<size_t>(arr.shape(0));
    const auto cols = static_cast<size_t>(arr.shape(1));

    if (n == 0)
        return;
    if (cols < 2 || cols > 5)
        throw std::invalid_argument(
            "Expected array with 2, 3, 4, or 5 columns [x, y, (angle), (scale or scale_x, scale_y)]"
        );

    const Rect clipArea = texture.getClipArea();
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;
    if (texture.getAlpha() == 0.0f)
        return;

    const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);
    const Vec2 clipSize = clipArea.getSize();
    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 rendRes = getCurrentResolution();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    const auto* data = arr.data();

    for (size_t i = 0; i < n; ++i)
    {
        const size_t row = i * cols;
        const Vec2 pos = Vec2{data[row], data[row + 1]} - cameraPos;
        const double angle = (cols >= 3) ? data[row + 2] : 0.0;

        Vec2 scale{1.0};
        if (cols == 4)
        {
            scale.x = data[row + 3];
            scale.y = scale.x;
        }
        else if (cols == 5)
        {
            scale.x = data[row + 3];
            scale.y = data[row + 4];
        }
        if (scale.isZero())
            continue;

        Rect dstRect{0.0, 0.0, clipSize * scale};
        dstRect.setTopLeft(pos - (dstRect.getSize() * anchor));

        if (dstRect.getRight() < 0.0 || dstRect.x >= rendRes.x || dstRect.getBottom() < 0.0 ||
            dstRect.y >= rendRes.y)
            continue;

        const auto dstSDLRect = static_cast<SDL_FRect>(dstRect);
        const auto pivotPoint = static_cast<SDL_FPoint>(dstRect.getSize() * pivot);

        if (!SDL_RenderTextureRotated(
                _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(angle),
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
Set the default TextureScaleMode for new textures. The factory default is TextureScaleMode.LINEAR.

Args:
    scale_mode (TextureScaleMode): The default scaling/filtering mode to use for new textures.
    )doc");

    subRenderer.def("get_default_scale_mode", &getDefaultScaleMode, R"doc(
Get the current default TextureScaleMode for new textures.

Returns:
    TextureScaleMode: The current default scaling/filtering mode.
    )doc");

    subRenderer.def("clear", &clear, "color"_a = Color{}, R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
        )doc");

    subRenderer.def("present", &present, nb::call_guard<nb::gil_scoped_release>(), R"doc(
Present the rendered content to the screen. This finalizes the current frame and displays it. Should be called after
all drawing operations for the frame are complete.
    )doc");

    subRenderer.def("set_present_resolution", &setPresentResolution, "width"_a, "height"_a, R"doc(
Set a custom resolution for rendering. This creates an internal render target of the specified size,
and all rendering will be done to that target, which is then scaled up to the actual screen resolution when presented.

Args:
    width (int): The width of the render target in pixels.
    height (int): The height of the render target in pixels.

Raises:
    ValueError: If width or height are not positive integers.
    )doc");

    subRenderer.def("get_current_resolution", &getCurrentResolution, R"doc(
Get the resolution of the current render target for rendering. If a custom render target is set, this will return
the size of it. Otherwise, it returns the presenting resolution of the renderer.

Returns:
    Vec2: The width and height of the current resolution.
    )doc");

    subRenderer.def("set_target", &setTarget, "target"_a = nb::none(), R"doc(
Set the current render target to the provided Texture.

Args:
    target (Texture, optional): A Texture created with TextureAccess.TARGET, or None to unset.

Raises:
    RuntimeError: If the texture is not a TARGET texture.
        )doc");

    subRenderer.def(
        "draw",
        nb::overload_cast<const Texture&, const Transform&, const Vec2&, const Vec2&>(&draw),
        "texture"_a, "transform"_a = Transform{}, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, R"doc(
Render a texture.

Args:
    texture (Texture): The texture to render.
    transform (Transform, optional): The transform (position, rotation, scale).
    anchor (Vec2, optional): The anchor point (0.0-1.0). Defaults to top left (0, 0).
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
        )doc"
    );

    subRenderer.def(
        "draw", nb::overload_cast<const Texture&, const Rect&>(&draw), "texture"_a, "dst"_a, R"doc(
Render a texture stretched into a destination rectangle.

This is a simpler alternative to the transform-based draw when you only
need to place a texture at a specific screen rectangle without rotation.
The source region is determined by the texture's clip area.

Args:
    texture (Texture): The texture to render.
    dst (Rect): Destination rectangle on screen.
        )doc"
    );

    subRenderer.def("read_pixels", &readPixels, "src"_a = Rect{}, R"doc(
Read pixel data from the renderer within the specified rectangle.

Args:
    src (Rect, optional): The rectangle area to read pixels from.
        Defaults to entire renderer if None or area has no width or height.

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

    subRenderer.def(
        "draw_batch", &drawBatchNDArray, "texture"_a, "transforms"_a, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, nb::call_guard<nb::gil_scoped_release>(), R"doc(
Render a texture multiple times using a NumPy array for maximum throughput.

Each row of the array describes one instance. The number of columns determines
the layout:

- **2 columns** ``[x, y]`` — position only (angle=0, scale=1).
- **3 columns** ``[x, y, angle]`` — position + rotation (scale=1).
- **4 columns** ``[x, y, angle, scale]`` — position + rotation + uniform scale.
- **5 columns** ``[x, y, angle, scale_x, scale_y]`` — full transform.

Args:
    texture (Texture): The texture to render.
    transforms (numpy.ndarray): float64 array with shape ``(N, 2|3|4|5)``.
    anchor (Vec2, optional): The anchor point (0.0-1.0). Defaults to top left (0, 0).
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).

Raises:
    ValueError: If the array does not have 2, 3, 4, or 5 columns.
        )doc"
    );
}
}  // namespace kn::renderer
