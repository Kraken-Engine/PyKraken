#include "Renderer.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/ndarray.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/vector.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <cmath>
#include <limits>

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
    if (!SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set render draw color: " + std::string(SDL_GetError()));

    if (!SDL_RenderClear(_renderer))
        throw std::runtime_error("Failed to clear renderer: " + std::string(SDL_GetError()));
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
    Vec2 cameraPos;
    double cameraAngle = 0.0;
    if (currCamera)
    {
        cameraPos = currCamera->getWorldPos();
        cameraAngle = currCamera->getAngle();
        currCamera->setWorldPos({0.0, 0.0});
        currCamera->setAngle(0.0);
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
    {
        currCamera->setWorldPos(cameraPos);
        currCamera->setAngle(cameraAngle);
    }
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

    Rect dstRect{0.0, 0.0, clipArea.getSize() * transform.scale};

    // Position based on anchor
    const Vec2 pos = camera::worldToScreen(transform.pos);
    dstRect.setTopLeft(pos - (dstRect.getSize() * anchor));

    const double renderAngle = transform.angle + camera::getActiveAngle();

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

    const Vec2 cameraPos = camera::getActivePos();
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

        Rect dstRect{0.0, 0.0, clipSize * transform.scale};
        const Vec2 pos = camera::worldToScreen(transform.pos);
        dstRect.setTopLeft(pos - (dstRect.getSize() * anchor));

        const double renderAngle = transform.angle + cameraAngle;

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

#ifdef KRAKEN_ENABLE_PYTHON
class Batcher;

static void drawBatchNDArray(
    const Texture& texture,
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr, const Vec2& anchor,
    const Vec2& pivot, Batcher* batcher
);

class Batcher
{
  public:
    Batcher() = default;
    ~Batcher() = default;

    void preallocate(const size_t nSprites)
    {
        vertices.reserve(nSprites * 4);
        indices.reserve(nSprites * 6);
    }

    void free()
    {
        vertices.clear();
        vertices.shrink_to_fit();
        indices.clear();
        indices.shrink_to_fit();
    }

  private:
    friend void drawBatchNDArray(
        const Texture& texture,
        nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr,
        const Vec2& anchor, const Vec2& pivot, Batcher* batcher
    );

    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;
};

void drawBatchNDArray(
    const Texture& texture,
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr, const Vec2& anchor,
    const Vec2& pivot, Batcher* batcher
)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    const size_t n = arr.shape(0);
    const size_t cols = arr.shape(1);

    if (n == 0)
        return;

    if (cols < 2 || cols > 5 && cols != 9)
        throw std::invalid_argument(
            "Expected array with 2-5 or 9 columns: [x, y, (angle), (scale or scale_x, scale_y), "
            "(clip_left, clip_top, clip_width, clip_height)]"
        );

    const Rect textureClipArea = texture.getClipArea();
    if (textureClipArea.w <= 0.0 || textureClipArea.h <= 0.0)
        return;

    const float alpha = texture.getAlpha();
    if (alpha == 0.0f)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    const double cameraAngle = camera::getActiveAngle();

    const Vec2 rendRes = getCurrentResolution();
    SDL_Texture* sdlTexture = texture.getSDL();

    const Color tint = texture.getTint();
    const auto vertexColor = static_cast<SDL_FColor>(tint);

    const double texW = texture.getWidth();
    const double texH = texture.getHeight();

    const auto t_u1 = static_cast<float>(textureClipArea.x / texW);
    const auto t_v1 = static_cast<float>(textureClipArea.y / texH);
    const auto t_u2 = static_cast<float>(textureClipArea.getRight() / texW);
    const auto t_v2 = static_cast<float>(textureClipArea.getBottom() / texH);

    std::vector<SDL_Vertex>* pVertices;
    std::vector<int>* pIndices;
    std::vector<SDL_Vertex> localVertices;
    std::vector<int> localIndices;

    if (batcher)
    {
        batcher->vertices.clear();
        batcher->indices.clear();
        pVertices = &batcher->vertices;
        pIndices = &batcher->indices;
    }
    else
    {
        localVertices.reserve(n * 4);
        localIndices.reserve(n * 6);
        pVertices = &localVertices;
        pIndices = &localIndices;
    }

    const double* data = arr.data();

    std::vector<SDL_Vertex>& vertices = *pVertices;
    std::vector<int>& indices = *pIndices;

    for (size_t i = 0; i < n; ++i)
    {
        const double* row = data + i * cols;
        const Vec2 pos = Vec2{row[0], row[1]} - cameraPos;
        const double angle = ((cols >= 3) ? row[2] : 0.0) + cameraAngle;

        Vec2 scale{1.0};
        if (cols >= 4)
        {
            scale.x = row[3];
            scale.y = (cols == 4) ? scale.x : row[4];
        }

        if (scale.isZero())
            continue;

        Rect clipArea = (cols == 9) ? Rect{row[5], row[6], row[7], row[8]} : textureClipArea;

        if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
            continue;

        const Vec2 clipSize = clipArea.getSize();

        Rect dstRect{0.0, 0.0, clipSize * scale};
        dstRect.setTopLeft(pos - (dstRect.getSize() * anchor));

        const Rect cullRect = _rotatedBounds(dstRect, angle, pivot);
        if (cullRect.getRight() < 0.0 || cullRect.x >= rendRes.x || cullRect.getBottom() < 0.0 ||
            cullRect.y >= rendRes.y)
            continue;

        float u1, v1, u2, v2;
        if (cols == 9)
        {
            u1 = static_cast<float>(clipArea.x / texW);
            v1 = static_cast<float>(clipArea.y / texH);
            u2 = static_cast<float>(clipArea.getRight() / texW);
            v2 = static_cast<float>(clipArea.getBottom() / texH);
        }
        else
        {
            u1 = t_u1;
            v1 = t_v1;
            u2 = t_u2;
            v2 = t_v2;
        }

        if (texture.flip.h)
            std::swap(u1, u2);
        if (texture.flip.v)
            std::swap(v1, v2);

        const double pivotX = dstRect.w * pivot.x;
        const double pivotY = dstRect.h * pivot.y;

        const auto dx = static_cast<float>(dstRect.x + pivotX);
        const auto dy = static_cast<float>(dstRect.y + pivotY);

        const auto x1 = static_cast<float>(-pivotX);
        const auto y1 = static_cast<float>(-pivotY);
        const auto x2 = static_cast<float>(dstRect.w - pivotX);
        const auto y2 = static_cast<float>(dstRect.h - pivotY);

        const int base = static_cast<int>(vertices.size());

        if (angle == 0.0)
        {
            vertices.push_back({{dx + x1, dy + y1}, vertexColor, {u1, v1}});
            vertices.push_back({{dx + x2, dy + y1}, vertexColor, {u2, v1}});
            vertices.push_back({{dx + x2, dy + y2}, vertexColor, {u2, v2}});
            vertices.push_back({{dx + x1, dy + y2}, vertexColor, {u1, v2}});
        }
        else
        {
            const auto c = static_cast<float>(std::cos(angle));
            const auto s = static_cast<float>(std::sin(angle));

            vertices.push_back(
                {{dx + x1 * c - y1 * s, dy + x1 * s + y1 * c}, vertexColor, {u1, v1}}
            );
            vertices.push_back(
                {{dx + x2 * c - y1 * s, dy + x2 * s + y1 * c}, vertexColor, {u2, v1}}
            );
            vertices.push_back(
                {{dx + x2 * c - y2 * s, dy + x2 * s + y2 * c}, vertexColor, {u2, v2}}
            );
            vertices.push_back(
                {{dx + x1 * c - y2 * s, dy + x1 * s + y2 * c}, vertexColor, {u1, v2}}
            );
        }

        indices.push_back(base);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }

    if (vertices.empty())
        return;

    if (!SDL_RenderGeometry(
            _renderer, sdlTexture, vertices.data(), static_cast<int>(vertices.size()),
            indices.data(), static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error("Failed to render geometry: " + std::string(SDL_GetError()));
    }
}

void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::enum_<RenderBackend>(module, "RenderBackend")
        .value("AUTO", RenderBackend::Auto, "Auto select the best available GPU backend.")
        .value("LEGACY", RenderBackend::Legacy, "Use the legacy OpenGL backend.")
        .value("VULKAN", RenderBackend::Vulkan, "Use the Vulkan backend.")
        .value("METAL", RenderBackend::Metal, "Use the Metal backend.")
        .value("DIRECT3D12", RenderBackend::Direct3d12, "Use the Direct3D 12 backend.");

    auto subRenderer = module.def_submodule("renderer", "Functions for rendering graphics");

    nb::class_<Batcher>(subRenderer, "Batcher", R"doc(
A reusable memory buffer for batched rendering, designed for maximum throughput.
        )doc")
        .def(nb::init<>())
        .def("preallocate", &Batcher::preallocate, "n_sprites"_a, R"doc(
Preallocate internal buffers for a specific number of sprites.
This prevents runtime allocations when drawing large batches.

Args:
    n_sprites (int): The number of sprites to preallocate capacity for.
        )doc")
        .def("free", &Batcher::free, R"doc(
Free the allocated internal memory.
        )doc");

    subRenderer.def("set_default_filter_mode", &setDefaultFilterMode, "filter"_a, R"doc(
Set the default FilterMode for new textures. The factory default is FilterMode::Default.

Args:
    filter (FilterMode): The default scaling/filtering mode to use for new textures.
    )doc");

    subRenderer.def("get_default_filter_mode", &getDefaultFilterMode, R"doc(
Get the current default FilterMode for new textures.

Returns:
    FilterMode: The current default scaling/filtering mode.
    )doc");

    subRenderer.def("clear", &clear, "color"_a = Color{}, R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
        )doc");

    subRenderer.def("present", &present, nb::call_guard<nb::gil_scoped_release>(), R"doc(
Present the rendered content to the screen.
This finalizes the current frame and displays it.
Should be called after all drawing operations for the frame are complete.
    )doc");

    subRenderer.def("set_virtual_resolution", &setVirtualResolution, "width"_a, "height"_a, R"doc(
Set a virtual resolution for rendering. This creates an internal render target of the specified size,
and all rendering will be done to that target, which is then scaled up to the actual screen resolution when presented.

Args:
    width (int): The width of the render target in pixels.
    height (int): The height of the render target in pixels.

Raises:
    ValueError: If width or height are not positive integers.
    )doc");

    subRenderer.def("unset_virtual_resolution", &unsetVirtualResolution, R"doc(
Unset any previously configured virtual resolution and restore rendering directly to the output.
    )doc");

    subRenderer.def("set_render_backend", &setRenderBackend, "backend"_a, R"doc(
Set the renderer backend to use for future initialization.
This must be called before creating the window/renderer, otherwise it will have no effect.

Args:
    backend (RenderBackend): One of the RenderBackend enum values.
    )doc");

    subRenderer.def("get_virtual_resolution", &getVirtualResolution, R"doc(
Get the currently configured virtual resolution (the internal render target size).
If no virtual resolution is set, this returns the current output resolution.

Returns:
    Vec2: The width and height of the virtual resolution in pixels.
    )doc");

    subRenderer.def("get_output_resolution", &getOutputResolution, R"doc(
Get the renderer's output/presenting resolution (the actual window or output size).

Returns:
    Vec2: The output width and height in pixels.
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
        "draw", nb::overload_cast<const Texture&, Rect, double, const Vec2&>(&draw), "texture"_a,
        "dst"_a, "angle"_a = 0.0, "pivot"_a = Anchor::CENTER, R"doc(
Render a texture stretched into a destination rectangle without a camera's transform applied.

This is a simpler alternative to the transform-based draw when you only
need to place a texture at a specific screen rectangle.
The source region is determined by the texture's clip area.

Args:
    texture (Texture): The texture to render.
    dst (Rect): Destination rectangle on screen.
    angle (float, optional): The rotation angle in degrees. Defaults to 0.0.
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
        )doc"
    );

    subRenderer.def(
        "draw_9slice", &draw9Slice, "texture"_a, "dst"_a, "slice"_a, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, R"doc(
Render a texture using 9-slice scaling (9-grid). The camera's transform is not applied to this draw.

This divides the texture into 9 regions: 4 corners (unscaled), 4 edges (scaled in one axis),
and 1 center (scaled in both axes).

Args:
    texture (Texture): The source texture.
    dst (Rect): The destination rectangle on screen.
    slice (Rect): A rectangle defining the slice widths/heights (left_width, top_height, right_width, bottom_height).
    anchor (Vec2, optional): The anchor point. Defaults to top left.
    pivot (Vec2, optional): The rotation pivot. Defaults to center.
        )doc"
    );

    subRenderer.def(
        "draw_batch", &drawBatch, "texture"_a, "transforms"_a, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, "clip_rects"_a = nb::none(),
        nb::call_guard<nb::gil_scoped_release>(), R"doc(
Render a texture multiple times with different transforms in a single batch call.

This is much faster than calling draw() in a loop because it avoids
per-call Python/C++ dispatch overhead.

Args:
    texture (Texture): The texture to render.
    transforms (Sequence[Transform]): A list of transforms (position, rotation, scale).
    clip_rects (Sequence[Rect], optional): Per-instance clip rectangles. If provided, these override
        the texture's clip area for each instance. If None, all instances use the texture's clip area.
    anchor (Vec2, optional): The anchor point (0.0-1.0). Defaults to top left (0, 0).
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
        )doc"
    );

    subRenderer.def(
        "draw_batch", &drawBatchNDArray, "texture"_a, "transforms"_a, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, "batcher"_a = nb::none(),
        nb::call_guard<nb::gil_scoped_release>(), R"doc(
Render a texture multiple times using a NumPy array for maximum throughput. This is *the* fastest way to render large batches of sprites,
being significantly faster than the list-based draw_batch() due to no-copy viewing of contiguous array data.

Each row of the array describes one instance. The number of columns determines
the layout:

- **2 columns** ``[x, y]`` — position only (angle=0, scale=1).
- **3 columns** ``[x, y, angle]`` — position + rotation (scale=1).
- **4 columns** ``[x, y, angle, scale]`` — position + rotation + uniform scale.
- **5 columns** ``[x, y, angle, scale_x, scale_y]`` — full transform.
- **9 columns** ``[x, y, angle, scale_x, scale_y, clip_left, clip_top, clip_width, clip_height]`` — full transform + per-instance clip rect.

Args:
    texture (Texture): The texture to render.
    transforms (numpy.ndarray): float64 array with shape ``(N, 2|3|4|5|9)``.
    anchor (Vec2, optional): The anchor point (0.0-1.0). Defaults to top left (0, 0).
    pivot (Vec2, optional): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
    batcher (Batcher, optional): Pre-allocated rendering buffer for higher performance.

Raises:
    ValueError: If the array does not have 2, 3, 4, 5, or 9 columns.
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
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn::renderer
