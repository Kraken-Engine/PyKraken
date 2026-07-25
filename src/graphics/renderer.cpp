#include "kraken/graphics/Renderer.hpp"

#include <cmath>
#include <limits>

#include "kraken/core/Log.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Texture.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

constexpr double CONVERSION = 180.0 / M_PI;
#define TO_DEGREES(x) (x * CONVERSION)

namespace kn::renderer
{
static Rect _rotatedBounds(const Rect& dstRect, const double angle, const Vec2& pivot)
{
    if (angle == 0.0)
        return dstRect;

    const double c = std::cos(angle);
    const double s = std::sin(angle);

    const Vec2 corners[4] = {
        {0.0, 0.0},
        {dstRect.w, 0.0},
        {dstRect.w, dstRect.h},
        {0.0, dstRect.h},
    };

    double minX = std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();

    const Vec2 pivotPoint = dstRect.getSize() * pivot;
    for (const auto& corner : corners)
    {
        const auto [localX, localY] = corner - pivotPoint;
        const double rotatedX = localX * c - localY * s + pivotPoint.x;
        const double rotatedY = localX * s + localY * c + pivotPoint.y;

        minX = std::min(minX, rotatedX);
        minY = std::min(minY, rotatedY);
        maxX = std::max(maxX, rotatedX);
        maxY = std::max(maxY, rotatedY);
    }

    return {dstRect.x + minX, dstRect.y + minY, maxX - minX, maxY - minY};
}

static Vec2 _anchoredTopLeft(
    const Vec2& screenAnchor, const Vec2& size, const Vec2& anchor, const Vec2& pivot,
    const double cameraAngle
)
{
    const Vec2 anchorPoint = size * anchor;
    const Vec2 pivotPoint = size * pivot;
    Vec2 pivotToAnchor = anchorPoint - pivotPoint;
    if (cameraAngle != 0.0)
        pivotToAnchor.rotate(cameraAngle);

    return screenAnchor - pivotPoint - pivotToAnchor;
}

static SDL_Renderer* _renderer = nullptr;
static SDL_GPUDevice* _gpuDevice = nullptr;
static FilterMode _defaultFilterMode = FilterMode::Linear;
static Texture* _primaryTarget = nullptr;
static RenderBackend _forcedBackend = RenderBackend::Auto;

static Vec2 _size{};

void _init(SDL_Window* window, const int width, const int height)
{
    _size = {width, height};

    if (_forcedBackend != RenderBackend::Legacy)
    {
        SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL |
                                     SDL_GPU_SHADERFORMAT_DXIL;
        const char* driverName = nullptr;

        switch (_forcedBackend)
        {
        case RenderBackend::Vulkan:
            format = SDL_GPU_SHADERFORMAT_SPIRV;
            driverName = "vulkan";
            break;
        case RenderBackend::Metal:
            format = SDL_GPU_SHADERFORMAT_MSL;
            driverName = "metal";
            break;
        case RenderBackend::Direct3d12:
            format = SDL_GPU_SHADERFORMAT_DXIL;
            driverName = "direct3d12";
            break;
        default:
            break;
        }

        log::info("Attempting to initialize {} GPU backend...", driverName ? driverName : "AUTO");
        _gpuDevice = SDL_CreateGPUDevice(format, true, driverName);

        if (_gpuDevice)
            _renderer = SDL_CreateGPURenderer(_gpuDevice, window);

        if (!_renderer && _forcedBackend != RenderBackend::Auto)
        {
            log::warn("Preferred GPU backend failed: {}. Falling back to AUTO.", SDL_GetError());

            if (_gpuDevice)
            {
                SDL_DestroyGPUDevice(_gpuDevice);
                _gpuDevice = nullptr;
            }

            _renderer = SDL_CreateGPURenderer(nullptr, window);
        }
    }

    // Fallback to legacy renderer
    if (!_renderer)
    {
        if (_forcedBackend != RenderBackend::Legacy)
            log::warn("GPU backend failed: {}. Falling back to LEGACY renderer.", SDL_GetError());
        else
            log::info("Using LEGACY renderer backend.");

        _renderer = SDL_CreateRenderer(window, nullptr);

        if (!_renderer)
            throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));
    }

    SDL_SetRenderLogicalPresentation(_renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    if (!_gpuDevice)
        _gpuDevice = SDL_GetGPURendererDevice(_renderer);

    if (_gpuDevice)
    {
        const SDL_PropertiesID gpuProperties = SDL_GetGPUDeviceProperties(_gpuDevice);
        const char* driverName = SDL_GetGPUDeviceDriver(_gpuDevice);

        log::info(
            "GPU Device: {}",
            SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown")
        );
        log::info("GPU Driver: {}", (driverName ? driverName : "Unknown"));
        log::info(
            "GPU Driver Version: {}",
            SDL_GetStringProperty(
                gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING, "Unknown"
            )
        );
        log::info(
            "GPU Driver Info: {}",
            SDL_GetStringProperty(gpuProperties, SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, "None")
        );
    }
    else
    {
        log::info("Initialized renderer: {}", SDL_GetRendererName(_renderer));
    }
}

void _quit()
{
    if (_primaryTarget)
    {
        delete _primaryTarget;
        _primaryTarget = nullptr;
    }

    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
}

void setRenderBackend(const RenderBackend backend)
{
    if (_renderer)
    {
        log::warn("Renderer backend cannot be changed after window creation.");
        return;
    }

    _forcedBackend = backend;
}

void clear(const Color& color)
{
    SDL_Texture* currentTarget = SDL_GetRenderTarget(_renderer);

    auto clearCurrentTarget = [&]()
    {
        if (!SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a))
            throw std::runtime_error(
                "Failed to set render draw color: " + std::string(SDL_GetError())
            );

        if (!SDL_RenderClear(_renderer))
            throw std::runtime_error("Failed to clear renderer: " + std::string(SDL_GetError()));
    };

    if (_primaryTarget && currentTarget == _primaryTarget->getSDL())
    {
        if (!SDL_SetRenderTarget(_renderer, nullptr))
            throw std::runtime_error(
                "Failed to unset render target: " + std::string(SDL_GetError())
            );

        clearCurrentTarget();

        if (!SDL_SetRenderTarget(_renderer, currentTarget))
            throw std::runtime_error(
                "Failed to restore render target: " + std::string(SDL_GetError())
            );
    }

    clearCurrentTarget();
}

void setTarget(const Texture* target)
{
    if (target)
    {
        if (!target->hasUsage(TextureUsage::Drawable))
            throw std::runtime_error("Texture is not drawable, cannot set as render target");
    }
    else
    {
        if (!SDL_SetRenderTarget(_renderer, (_primaryTarget) ? _primaryTarget->getSDL() : nullptr))
            throw std::runtime_error(
                "Failed to unset render target: " + std::string(SDL_GetError())
            );
        return;
    }

    SDL_Texture* targetSDL = target->getSDL();
    if (!SDL_SetRenderTarget(_renderer, targetSDL))
        throw std::runtime_error("Failed to set render target: " + std::string(SDL_GetError()));
}

void setDefaultFilterMode(const FilterMode filter)
{
    _defaultFilterMode = filter == FilterMode::Default ? FilterMode::Linear : filter;
}

FilterMode getDefaultFilterMode()
{
    return _defaultFilterMode;
}

void present()
{
    // Regular present if no custom renderer size
    if (!_primaryTarget)
    {
        if (!SDL_RenderPresent(_renderer))
            throw std::runtime_error("Failed to present renderer: " + std::string(SDL_GetError()));
        return;
    }

    // Hold old cam pos and set pos to origin
    kn::Camera* currCamera = camera::_getActiveCamera();
    Transform cameraXf;
    if (currCamera)
    {
        cameraXf = currCamera->transform;
        currCamera->transform.pos = {0.0, 0.0};
        currCamera->transform.angle = 0.0;
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
        currCamera->transform = cameraXf;
}

void setVirtualResolution(const int width, const int height)
{
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Resolution width and height must be positive integers");

    if (_primaryTarget)
    {
        delete _primaryTarget;
        _primaryTarget = nullptr;
    }

    _primaryTarget = new Texture(width, height);
    setTarget(_primaryTarget);
}

void unsetVirtualResolution()
{
    if (_primaryTarget)
    {
        delete _primaryTarget;
        _primaryTarget = nullptr;
    }
    setTarget(nullptr);
}

Vec2 getVirtualScale()
{
    if (!_primaryTarget)
        return {1.0, 1.0};
    return _size / _primaryTarget->getSize();
}

Vec2 getVirtualResolution()
{
    if (_primaryTarget)
        return _primaryTarget->getSize();
    return _size;
}

Vec2 getCurrentResolution()
{
    SDL_Texture* currentTarget = SDL_GetRenderTarget(_renderer);

    // No primary target nor user target set
    if (!currentTarget)
        return _size;

    // Primary target active
    if (_primaryTarget && currentTarget == _primaryTarget->getSDL())
        return _primaryTarget->getSize();

    // User target active
    float w, h;
    if (!SDL_GetTextureSize(currentTarget, &w, &h))
        throw std::runtime_error(
            "Failed to get render target size: " + std::string(SDL_GetError())
        );

    return {w, h};
}

Vec2 getOutputResolution()
{
    return _size;
}

PixelArray readPixels(const Rect& src)
{
    if (src.w < 0.0 || src.h < 0.0)
        throw std::invalid_argument("Source rectangle must have positive width and height");

    const auto sdlRect = static_cast<SDL_Rect>(src);
    const bool hasSize = (src.w > 0.0 && src.h > 0.0);

    SDL_Surface* surface = SDL_RenderReadPixels(_renderer, hasSize ? &sdlRect : nullptr);
    if (!surface)
        throw std::runtime_error("Failed to read pixels: " + std::string(SDL_GetError()));

    return PixelArray(surface);
}

void draw(const Texture& texture, const Transform& transform, const Vec2& anchor, const Vec2& pivot)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    Rect clipArea = texture.getClipArea();
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;
    if (transform.scale.isZero() || texture.getAlpha() == 0.0f)
        return;

    const Vec2 pos = camera::worldToScreen(transform.pos);
    const double cameraAngle = camera::getActiveAngle();
    const double renderAngle = transform.angle + cameraAngle;

    Rect dstRect{0.0, 0.0, clipArea.getSize() * transform.scale};
    dstRect.setTopLeft(_anchoredTopLeft(pos, dstRect.getSize(), anchor, pivot, cameraAngle));

    // cull using the rotated bounds so rotated quads don't disappear early near the edge
    const Rect cullRect = _rotatedBounds(dstRect, renderAngle, pivot);
    const Vec2 rendRes = getCurrentResolution();
    if (cullRect.getRight() < 0.0 || cullRect.x >= rendRes.x || cullRect.getBottom() < 0.0 ||
        cullRect.y >= rendRes.y)
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
            _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(renderAngle),
            &pivotPoint, flipAxis
        ))
    {
        throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
    }
}

void draw(const Texture& texture, Rect dst, const double angle, const Vec2& pivot)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    if (texture.getAlpha() == 0.0f)
        return;

    const Rect clipArea = texture.getClipArea();
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;

    const Vec2 worldCenter = dst.getCenter();

    const Vec2 rendRes = getCurrentResolution();
    const Rect cullRect = _rotatedBounds(dst, angle, pivot);
    if (cullRect.getRight() < 0.0 || cullRect.x >= rendRes.x || cullRect.getBottom() < 0.0 ||
        cullRect.y >= rendRes.y)
        return;

    const auto dstSDLRect = static_cast<SDL_FRect>(dst);
    const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);
    const auto pivotPoint = static_cast<SDL_FPoint>(dst.getSize() * pivot);

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    if (!SDL_RenderTextureRotated(
            _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(angle), &pivotPoint,
            flipAxis
        ))
    {
        throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
    }
}

void draw9Slice(
    const Texture& texture, const Rect& dst, const Rect& slice, const Vec2& anchor,
    const Vec2& pivot
)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    if (texture.getAlpha() == 0.0f)
        return;

    const Rect textureClipArea = texture.getClipArea();
    if (textureClipArea.w <= 0.0 || textureClipArea.h <= 0.0)
        return;

    const auto [leftWidth, topHeight, rightWidth, bottomHeight] = static_cast<SDL_FRect>(slice);

    const auto dstSDLRect = static_cast<SDL_FRect>(dst);
    const auto srcSDLRect = static_cast<SDL_FRect>(textureClipArea);

    if (!SDL_RenderTexture9Grid(
            _renderer, texture.getSDL(), &srcSDLRect, leftWidth, rightWidth, topHeight,
            bottomHeight, 1.0f, &dstSDLRect
        ))
    {
        throw std::runtime_error(
            "Failed to render 9-slice texture: " + std::string(SDL_GetError())
        );
    }
}

void drawBatch(
    const Texture& texture, const std::vector<Transform>& transforms, const Vec2& anchor,
    const Vec2& pivot, const std::optional<std::vector<Rect>>& clipRects
)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    if (transforms.empty() || texture.getAlpha() == 0.0f)
        return;

    const Rect textureClipArea = texture.getClipArea();
    if (textureClipArea.w <= 0.0 || textureClipArea.h <= 0.0)
        return;

    const double cameraAngle = camera::getActiveAngle();

    const Vec2 rendRes = getCurrentResolution();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    for (size_t i = 0; i < transforms.size(); ++i)
    {
        const auto& transform = transforms[i];
        if (transform.scale.isZero())
            continue;

        // Use per-instance clip rect if provided, otherwise use texture's clip area
        const Rect& clipArea = (clipRects && i < clipRects->size()) ? (*clipRects)[i]
                                                                    : textureClipArea;
        if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
            continue;

        const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);
        const Vec2 clipSize = clipArea.getSize();

        const Vec2 pos = camera::worldToScreen(transform.pos);
        const double renderAngle = transform.angle + cameraAngle;

        Rect dstRect{0.0, 0.0, clipSize * transform.scale};
        dstRect.setTopLeft(_anchoredTopLeft(pos, dstRect.getSize(), anchor, pivot, cameraAngle));

        const Rect cullRect = _rotatedBounds(dstRect, renderAngle, pivot);
        if (cullRect.getRight() < 0.0 || cullRect.x >= rendRes.x || cullRect.getBottom() < 0.0 ||
            cullRect.y >= rendRes.y)
            continue;

        const auto dstSDLRect = static_cast<SDL_FRect>(dstRect);
        const auto pivotPoint = static_cast<SDL_FPoint>(dstRect.getSize() * pivot);

        if (!SDL_RenderTextureRotated(
                _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(renderAngle),
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

bool _primaryActive()
{
    if (!_primaryTarget)
        return false;

    return SDL_GetRenderTarget(_renderer) == _primaryTarget->getSDL();
}

}  // namespace kn::renderer
