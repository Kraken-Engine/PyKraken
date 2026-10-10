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

Rect _validateDrawOptions(const Texture& texture, const DrawOptions& options)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");
    if (!std::isfinite(options.alpha) || options.alpha < 0 || options.alpha > 1)
        throw std::invalid_argument("Draw alpha must be finite and between 0 and 1");
    if (options.blend != BlendMode::Normal && options.blend != BlendMode::Additive &&
        options.blend != BlendMode::Multiply)
        throw std::invalid_argument("Invalid draw blend mode");

    const Rect source = options.source.value_or(texture.getRect());
    if (!std::isfinite(source.x) || !std::isfinite(source.y) || !std::isfinite(source.w) ||
        !std::isfinite(source.h) || source.x < 0 || source.y < 0 || source.w < 0 || source.h < 0 ||
        source.getRight() > texture.getWidth() || source.getBottom() > texture.getHeight())
        throw std::invalid_argument("Draw source rectangle must fit within the texture");

    return source;
}

void _applyDrawOptions(const Texture& texture, const DrawOptions& options)
{
    auto* tex = texture.getSDL();
    if (!SDL_SetTextureColorMod(tex, options.tint.r, options.tint.g, options.tint.b) ||
        !SDL_SetTextureAlphaModFloat(tex, options.alpha * options.tint.a / 255.0f) ||
        !SDL_SetTextureBlendMode(tex, static_cast<SDL_BlendMode>(options.blend)))
        throw std::runtime_error("Failed to apply draw options: " + std::string(SDL_GetError()));
}

void draw(
    const Texture& texture, const Transform& transform, const Vec2& anchor, const Vec2& pivot,
    const DrawOptions& options
)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    Rect clipArea = _validateDrawOptions(texture, options);
    if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
        return;
    if (transform.scale.isZero() || options.alpha == 0.0f)
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
        return;

    const auto dstSDLRect = static_cast<SDL_FRect>(dstRect);
    const auto srcSDLRect = static_cast<SDL_FRect>(clipArea);

    // Pivot is normalized 0..1 relative to dstRect, for rotation center
    const auto pivotPoint = static_cast<SDL_FPoint>(dstRect.getSize() * pivot);

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (options.flipH)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (options.flipV)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    _applyDrawOptions(texture, options);
    if (!SDL_RenderTextureRotated(
            _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(renderAngle),
            &pivotPoint, flipAxis
        ))
    {
        throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
    }
}

void draw(
    const Texture& texture, Rect dst, const double angle, const Vec2& pivot,
    const DrawOptions& options
)
{
    if (!texture.hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    if (options.alpha == 0.0f)
        return;

    const Rect clipArea = _validateDrawOptions(texture, options);
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
    if (options.flipH)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (options.flipV)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    _applyDrawOptions(texture, options);
    if (!SDL_RenderTextureRotated(
            _renderer, texture.getSDL(), &srcSDLRect, &dstSDLRect, TO_DEGREES(angle), &pivotPoint,
            flipAxis
        ))
    {
        throw std::runtime_error("Failed to render texture: " + std::string(SDL_GetError()));
    }
}

void draw9Slice(
    const Texture& texture, const Rect& dst, const Rect& slice, const DrawOptions& options
)
{
    const Rect source = _validateDrawOptions(texture, options);

    const double left = slice.x, top = slice.y, right = slice.w, bottom = slice.h;
    if (!std::isfinite(left) || !std::isfinite(top) || !std::isfinite(right) ||
        !std::isfinite(bottom) || left < 0 || top < 0 || right < 0 || bottom < 0 ||
        left + right > source.w || top + bottom > source.h)
        throw std::invalid_argument("9-slice borders must fit within the source rectangle");

    if (dst.w <= 0 || dst.h <= 0 || source.w <= 0 || source.h <= 0 || options.alpha == 0)
        return;

    // Preserve SDL's efficient nine-grid path when no mirroring is needed.
    if (!options.flipH && !options.flipV)
    {
        const auto srcRect = static_cast<SDL_FRect>(source);
        const auto dstRect = static_cast<SDL_FRect>(dst);
        _applyDrawOptions(texture, options);
        if (!SDL_RenderTexture9Grid(
                _renderer, texture.getSDL(), &srcRect, static_cast<float>(left),
                static_cast<float>(right), static_cast<float>(top), static_cast<float>(bottom),
                1.0f, &dstRect
            ))
            throw std::runtime_error(
                "Failed to render 9-slice texture: " + std::string(SDL_GetError())
            );
        return;
    }

    const double scaleX = left + right > dst.w ? dst.w / (left + right) : 1.0;
    const double scaleY = top + bottom > dst.h ? dst.h / (top + bottom) : 1.0;

    const double sx[] = {
        source.x,
        source.x + left,
        source.getRight() - right,
        source.getRight(),
    };
    const double sy[] = {
        source.y,
        source.y + top,
        source.getBottom() - bottom,
        source.getBottom(),
    };
    const double dx[] = {
        dst.x,
        dst.x + left * scaleX,
        dst.getRight() - right * scaleX,
        dst.getRight(),
    };
    const double dy[] = {
        dst.y,
        dst.y + top * scaleY,
        dst.getBottom() - bottom * scaleY,
        dst.getBottom(),
    };

    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
        {
            DrawOptions piece = options;

            piece.source = Rect{sx[x], sy[y], sx[x + 1] - sx[x], sy[y + 1] - sy[y]};

            Rect target{dx[x], dy[y], dx[x + 1] - dx[x], dy[y + 1] - dy[y]};
            if (target.w <= 0 || target.h <= 0)
                continue;

            if (options.flipH)
                target.x = dst.x + dst.getRight() - target.getRight();
            if (options.flipV)
                target.y = dst.y + dst.getBottom() - target.getBottom();

            draw(texture, target, 0.0, Anchor::CENTER, piece);
        }
}

void Batcher::preallocate(size_t nSprites)
{
    if (nSprites > static_cast<size_t>(std::numeric_limits<int>::max()) / 6)
        throw std::length_error("Batch exceeds SDL geometry limits");
    m_vertices.reserve(nSprites * 4);
    m_indices.reserve(nSprites * 6);
    m_runs.reserve(nSprites);
}

void Batcher::free()
{
    std::vector<SDL_Vertex>().swap(m_vertices);
    std::vector<int>().swap(m_indices);
    std::vector<Run>().swap(m_runs);
}

void drawBatch(
    const Texture& texture, std::span<const double> data, const uint8_t cols, Batcher* batcher,
    const DrawOptions& options, const Vec2& anchor, const Vec2& pivot,
    std::span<const DrawOptions> instanceOptions, std::span<const Rect> clipRects
)
{
    if (cols < 2 || (cols > 5 && cols != 9))
        throw std::invalid_argument("Expected packed rows with 2, 3, 4, 5, or 9 columns");
    if (data.size() % cols != 0)
        throw std::invalid_argument("Batch data must contain complete rows");

    const size_t n = data.size() / cols;
    if (!instanceOptions.empty() && instanceOptions.size() != n)
        throw std::invalid_argument("instance_options must have one entry per transform");
    if (!clipRects.empty() && clipRects.size() != n)
        throw std::invalid_argument("clip_rects must have one entry per transform");

    _validateDrawOptions(texture, options);
    Batcher localBuffers;
    Batcher& buffers = batcher ? *batcher : localBuffers;
    buffers.preallocate(n);
    buffers.m_vertices.clear();
    buffers.m_indices.clear();
    buffers.m_runs.clear();
    auto& vertices = buffers.m_vertices;
    auto& indices = buffers.m_indices;
    const double cameraAngle = camera::getActiveAngle();
    const Vec2 rendRes = getCurrentResolution();
    const double texW = texture.getWidth();
    const double texH = texture.getHeight();

    for (size_t i = 0; i < n; ++i)
    {
        const double* row = data.data() + i * cols;
        DrawOptions item = instanceOptions.empty() ? options : instanceOptions[i];
        if (cols == 9)
            item.source = Rect{row[5], row[6], row[7], row[8]};
        if (!clipRects.empty())
            item.source = clipRects[i];
        const Rect clipArea = _validateDrawOptions(texture, item);
        if (item.alpha == 0.0f || item.tint.a == 0)
            continue;
        auto vertexColor = static_cast<SDL_FColor>(item.tint);
        vertexColor.a *= item.alpha;
        const Vec2 pos = camera::worldToScreen(Vec2{row[0], row[1]});
        const double angle = ((cols >= 3) ? row[2] : 0.0) + cameraAngle;

        Vec2 scale{1.0};
        if (cols >= 4)
        {
            scale.x = row[3];
            scale.y = (cols == 4) ? scale.x : row[4];
        }

        if (scale.isZero())
            continue;

        if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
            continue;

        const Vec2 clipSize = clipArea.getSize();

        Rect dstRect{0.0, 0.0, clipSize * scale};
        dstRect.setTopLeft(_anchoredTopLeft(pos, dstRect.getSize(), anchor, pivot, cameraAngle));

        const Rect cullRect = _rotatedBounds(dstRect, angle, pivot);
        if (cullRect.getRight() < 0.0 || cullRect.x >= rendRes.x || cullRect.getBottom() < 0.0 ||
            cullRect.y >= rendRes.y)
            continue;

        float u1 = static_cast<float>(clipArea.x / texW);
        float v1 = static_cast<float>(clipArea.y / texH);
        float u2 = static_cast<float>(clipArea.getRight() / texW);
        float v2 = static_cast<float>(clipArea.getBottom() / texH);

        if (item.flipH)
            std::swap(u1, u2);
        if (item.flipV)
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

        const int firstIndex = static_cast<int>(indices.size());
        if (buffers.m_runs.empty() || buffers.m_runs.back().blend != item.blend)
            buffers.m_runs.push_back({item.blend, firstIndex, 6});
        else
            buffers.m_runs.back().indexCount += 6;

        indices.push_back(base);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }

    // Keep per-instance colors in vertices. Preserve draw order when blending differs.
    for (const auto& run : buffers.m_runs)
    {
        DrawOptions geometryOptions;
        geometryOptions.blend = run.blend;
        _applyDrawOptions(texture, geometryOptions);
        if (!SDL_RenderGeometry(
                _renderer, texture.getSDL(), vertices.data(), static_cast<int>(vertices.size()),
                indices.data() + run.firstIndex, run.indexCount
            ))
            throw std::runtime_error("Failed to render geometry: " + std::string(SDL_GetError()));
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
