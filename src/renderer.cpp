#include "Renderer.hpp"

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
    _gpuDevice = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL, true,
        nullptr
    );
    if (_gpuDevice == nullptr)
        throw std::runtime_error("GPU device failed to create: " + std::string(SDL_GetError()));

    _renderer = SDL_CreateGPURenderer(_gpuDevice, window);
    if (_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    SDL_SetRenderLogicalPresentation(_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    const SDL_PropertiesID gpuProperties = SDL_GetGPUDeviceProperties(_gpuDevice);
    const char* gpuName =
        SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown Device");
    log::info("GPU Device: {}", gpuName);
}

void _quit()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_gpuDevice)
    {
        SDL_DestroyGPUDevice(_gpuDevice);
        _gpuDevice = nullptr;
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

Vec2 getTargetResolution()
{
    if (SDL_Texture* target = SDL_GetRenderTarget(_renderer); target)
    {
        float w, h;
        SDL_GetTextureSize(target, &w, &h);
        return {w, h};
    }

    int w, h;
    SDL_GetRenderLogicalPresentation(_renderer, &w, &h, nullptr);
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
    const auto access =
        SDL_GetNumberProperty(textureProperties, SDL_PROP_TEXTURE_ACCESS_NUMBER, -1);

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
    SDL_RenderPresent(_renderer);
}

std::unique_ptr<PixelArray> readPixels(const Rect& src)
{
    const auto sdlRect = static_cast<SDL_Rect>(src);
    SDL_Surface* surface = SDL_RenderReadPixels(_renderer, &sdlRect);
    if (!surface)
        throw std::runtime_error("Failed to read pixels: " + std::string(SDL_GetError()));

    return std::make_unique<PixelArray>(surface);
}

void draw(const std::shared_ptr<Texture>& texture, Transform transform, const Rect& srcRect)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not yet initialized");

    const Vec2 cameraPos = camera::getActivePos();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture->flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture->flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    Vec2 baseSize = transform.size;
    if (baseSize.isZero())
    {
        const Vec2 srcSize = srcRect.getSize();
        baseSize = !srcSize.isZero() ? srcSize : texture->getSize();
    }

    Rect dstRect{};
    dstRect.setSize({baseSize.x * transform.scale.x, baseSize.y * transform.scale.y});

    // Position based on anchor
    Vec2 pos = transform.pos - cameraPos;
    switch (transform.anchor)
    {
    case Anchor::TopLeft:
        dstRect.setTopLeft(pos);
        break;
    case Anchor::TopMid:
        dstRect.setTopMid(pos);
        break;
    case Anchor::TopRight:
        dstRect.setTopRight(pos);
        break;
    case Anchor::MidLeft:
        dstRect.setMidLeft(pos);
        break;
    case Anchor::Center:
        dstRect.setCenter(pos);
        break;
    case Anchor::MidRight:
        dstRect.setMidRight(pos);
        break;
    case Anchor::BottomLeft:
        dstRect.setBottomLeft(pos);
        break;
    case Anchor::BottomMid:
        dstRect.setBottomMid(pos);
        break;
    case Anchor::BottomRight:
        dstRect.setBottomRight(pos);
        break;
    }
    const SDL_FRect dstSDLRect = static_cast<SDL_FRect>(dstRect);

    SDL_FRect srcSDLRect{};
    if (srcRect.w == 0.0 && srcRect.h == 0.0)
    {
        const Vec2 textureSize = texture->getSize();
        srcSDLRect.w = static_cast<float>(textureSize.x);
        srcSDLRect.h = static_cast<float>(textureSize.y);
    }
    else
    {
        srcSDLRect = static_cast<SDL_FRect>(srcRect);
    }

    SDL_FPoint pivot =
        {dstSDLRect.w * static_cast<float>(transform.pivot.x),
         dstSDLRect.h * static_cast<float>(transform.pivot.y)};

    if (!SDL_RenderTextureRotated(
            _renderer, texture->getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(transform.angle),
            &pivot, flipAxis
        ))
        throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
}

SDL_Renderer* _get()
{
    return _renderer;
}

SDL_GPUDevice* _getGPUDevice()
{
    return _gpuDevice;
}

void _bind(py::module_& module)
{
    auto subRenderer = module.def_submodule("renderer", "Functions for rendering graphics");

    subRenderer.def("set_default_scale_mode", &setDefaultScaleMode, py::arg("scale_mode"), R"doc(
Set the default TextureScaleMode for new textures.

Args:
    scale_mode (TextureScaleMode): The default scaling/filtering mode to use for new textures.
    )doc");

    subRenderer.def("get_default_scale_mode", &getDefaultScaleMode, R"doc(
Get the current default TextureScaleMode for new textures.

Returns:
    TextureScaleMode: The current default scaling/filtering mode.
    )doc");

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
                throw py::type_error("Invalid type for 'color', expected Color, sequence, or None");
            }
        },
        py::arg("color") = py::none(), R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
    )doc"
    );

    subRenderer.def(
        "clear", py::overload_cast<uint8_t, uint8_t, uint8_t, uint8_t>(&clear), py::arg("r"),
        py::arg("g"), py::arg("b"), py::arg("a") = 255, R"doc(
Clear the renderer with the specified color.

Args:
    r (int): Red component (0-255).
    g (int): Green component (0-255).
    b (int): Blue component (0-255).
    a (int, optional): Alpha component (0-255). Defaults to 255.
    )doc"
    );

    subRenderer.def("present", &present, R"doc(
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

    subRenderer.def("set_target", &setTarget, py::arg("target"), R"doc(
Set the current render target to the provided Texture, or unset if None.

Args:
    target (Texture, optional): Texture created with TextureAccess.TARGET, or None to unset.

Raises:
    RuntimeError: If the renderer is not initialized or the texture is not a TARGET texture.
        )doc");

    subRenderer.def(
        "draw",
        [](const std::shared_ptr<Texture>& texture, const py::object& transformObj,
           const py::object& srcObj)
        {
            try
            {
                const auto transform = transformObj.is_none() ? Transform()
                                                              : transformObj.cast<Transform>();
                const auto srcRect = srcObj.is_none() ? Rect() : srcObj.cast<Rect>();
                draw(texture, transform, srcRect);
            }
            catch (const py::cast_error&)
            {
                throw py::type_error(
                    "Invalid type for arguments, expected (Texture, Transform, Rect)"
                );
            }
        },
        py::arg("texture"), py::arg("transform") = py::none(), py::arg("src") = py::none(), R"doc(
Render a texture with the specified transform and source rectangle.

Args:
    texture (Texture): The texture to render.
    transform (Transform, optional): The transform (position, rotation, scale, anchor, pivot). Defaults to identity transform.
    src (Rect, optional): The source rectangle from the texture. Defaults to entire texture if not specified.

Raises:
    TypeError: If arguments are not of expected types.
    RuntimeError: If renderer is not initialized.
        )doc"
    );

    subRenderer.def(
        "read_pixels",
        [](const py::object& rectObj) -> std::unique_ptr<PixelArray>
        {
            try
            {
                return readPixels(rectObj.is_none() ? Rect() : rectObj.cast<Rect>());
            }
            catch (const py::cast_error&)
            {
                throw py::type_error("Invalid type for 'src', expected Rect or None");
            }
        },
        py::arg("src") = py::none(), R"doc(
Read pixel data from the renderer within the specified rectangle.

Args:
    src (Rect, optional): The rectangle area to read pixels from. Defaults to entire renderer if None.
Returns:
    PixelArray: An array containing the pixel data.
Raises:
    RuntimeError: If reading pixels fails.
        )doc"
    );
}
}  // namespace kn::renderer
