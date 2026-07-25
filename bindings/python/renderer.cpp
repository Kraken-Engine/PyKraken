#include "kraken/graphics/Renderer.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/vector.h>

#include <cmath>
#include <limits>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Texture.hpp"

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

        Rect clipArea = (cols == 9) ? Rect{row[5], row[6], row[7], row[8]} : textureClipArea;

        if (clipArea.w <= 0.0 || clipArea.h <= 0.0)
            continue;

        const Vec2 clipSize = clipArea.getSize();

        Rect dstRect{0.0, 0.0, clipSize * scale};
        dstRect.setTopLeft(_anchoredTopLeft(pos, dstRect.getSize(), anchor, pivot, cameraAngle));

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
            _get(), sdlTexture, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
            static_cast<int>(indices.size())
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
}  // namespace kn::renderer
