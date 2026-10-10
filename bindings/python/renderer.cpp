#include "kraken/graphics/Renderer.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/vector.h>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Texture.hpp"

namespace kn::renderer
{
static void drawBatchList(
    const Texture& texture, const std::vector<Transform>& transforms, const Vec2& anchor,
    const Vec2& pivot, const std::optional<std::vector<Rect>>& clipRects,
    const DrawOptions& options, const std::optional<std::vector<DrawOptions>>& instanceOptions
)
{
    if (clipRects && clipRects->size() != transforms.size())
        throw std::invalid_argument("clip_rects must have one entry per transform");
    if (instanceOptions && instanceOptions->size() != transforms.size())
        throw std::invalid_argument("instance_options must have one entry per transform");

    _validateDrawOptions(texture, options);
    for (size_t i = 0; i < transforms.size(); ++i)
    {
        DrawOptions item = instanceOptions ? (*instanceOptions)[i] : options;

        if (clipRects)
            item.source = (*clipRects)[i];

        draw(texture, transforms[i], anchor, pivot, item);
    }
}

static void drawBatchNDArray(
    const Texture& texture,
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr, const Vec2& anchor,
    const Vec2& pivot, Batcher* batcher, const DrawOptions& options,
    const std::optional<std::vector<DrawOptions>>& instanceOptions,
    const std::optional<std::vector<Rect>>& clipRects
)
{
    if (instanceOptions && instanceOptions->size() != arr.shape(0))
        throw std::invalid_argument("instance_options must have one entry per transform");
    if (clipRects && clipRects->size() != arr.shape(0))
        throw std::invalid_argument("clip_rects must have one entry per transform");

    drawBatch(
        texture, std::span<const double>{arr.data(), arr.size()},
        static_cast<uint8_t>(arr.shape(1)), batcher, options, anchor, pivot,
        instanceOptions ? std::span<const DrawOptions>{*instanceOptions}
                        : std::span<const DrawOptions>{},
        clipRects ? std::span<const Rect>{*clipRects} : std::span<const Rect>{}
    );
}

static DrawOptions resolveOptions(
    const std::optional<DrawOptions>& options, const std::optional<Rect>& source,
    const std::optional<Color>& tint, const std::optional<float>& alpha,
    const std::optional<bool>& flipH, const std::optional<bool>& flipV,
    const std::optional<BlendMode>& blend
)
{
    DrawOptions result = options.value_or(DrawOptions{});

    if (source)
        result.source = source;
    if (tint)
        result.tint = *tint;
    if (alpha)
        result.alpha = *alpha;
    if (flipH)
        result.flipH = *flipH;
    if (flipV)
        result.flipV = *flipV;
    if (blend)
        result.blend = *blend;

    return result;
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

    nb::enum_<BlendMode>(module, "BlendMode")
        .value("NORMAL", BlendMode::Normal)
        .value("ADDITIVE", BlendMode::Additive)
        .value("MULTIPLY", BlendMode::Multiply);

    nb::class_<DrawOptions>(module, "DrawOptions", "Reusable local texture draw properties.")
        .def(
            "__init__",
            [](DrawOptions* self, std::optional<Rect> source, Color tint, float alpha, bool flipH,
               bool flipV, BlendMode blend)
            { new (self) DrawOptions{source, tint, alpha, flipH, flipV, blend}; },
            nb::kw_only(), "source"_a = nb::none(), "tint"_a = Color::WHITE, "alpha"_a = 1.0f,
            "flip_h"_a = false, "flip_v"_a = false, "blend"_a = BlendMode::Normal
        )
        .def_rw("source", &DrawOptions::source)
        .def_rw("tint", &DrawOptions::tint)
        .def_rw("alpha", &DrawOptions::alpha)
        .def_rw("flip_h", &DrawOptions::flipH)
        .def_rw("flip_v", &DrawOptions::flipV)
        .def_rw("blend", &DrawOptions::blend);

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
        [](const Texture& texture, const Transform& transform, const Vec2& anchor,
           const Vec2& pivot, const std::optional<DrawOptions>& options,
           const std::optional<Rect>& source, const std::optional<Color>& tint,
           const std::optional<float>& alpha, const std::optional<bool>& flipH,
           const std::optional<bool>& flipV, const std::optional<BlendMode>& blend)
        {
            draw(
                texture, transform, anchor, pivot,
                resolveOptions(options, source, tint, alpha, flipH, flipV, blend)
            );
        },
        "texture"_a, "transform"_a = Transform{}, "anchor"_a = Anchor::TOP_LEFT,
        "pivot"_a = Anchor::CENTER, nb::kw_only(), "options"_a = nb::none(),
        "source"_a = nb::none(), "tint"_a = nb::none(), "alpha"_a = nb::none(),
        "flip_h"_a = nb::none(), "flip_v"_a = nb::none(), "blend"_a = nb::none(), R"doc(
Draw a texture in world coordinates. Properties are local to this call.

Args:
    texture (Texture): The texture to draw.
    transform (Transform, optional): The position, rotation, and scale of the texture. Defaults to no transform.
    anchor (Vec2, optional): The anchor point of the texture. Defaults to Anchor.TOP_LEFT.
    pivot (Vec2, optional): The pivot point for rotation and scaling. Defaults to Anchor.CENTER.
    options (DrawOptions, optional): Additional draw options. Defaults to None.
    source (Rect, optional): The source rectangle from the texture. Defaults to None (full texture).
    tint (Color, optional): The color tint to apply. Defaults to None (no tint).
    alpha (float, optional): The alpha multiplier (0..1). Defaults to None (no change).
    flip_h (bool, optional): Whether to flip the texture horizontally. Defaults to None (no flip).
    flip_v (bool, optional): Whether to flip the texture vertically. Defaults to None (no flip).
    blend (BlendMode, optional): The blend mode to use. Defaults to None (normal blending).
        )doc"
    );

    subRenderer.def(
        "draw",
        [](const Texture& texture, Rect dst, double angle, const Vec2& pivot,
           const std::optional<DrawOptions>& options, const std::optional<Rect>& source,
           const std::optional<Color>& tint, const std::optional<float>& alpha,
           const std::optional<bool>& flipH, const std::optional<bool>& flipV,
           const std::optional<BlendMode>& blend)
        {
            draw(
                texture, dst, angle, pivot,
                resolveOptions(options, source, tint, alpha, flipH, flipV, blend)
            );
        },
        "texture"_a, "dst"_a, "angle"_a = 0.0, "pivot"_a = Anchor::CENTER, nb::kw_only(),
        "options"_a = nb::none(), "source"_a = nb::none(), "tint"_a = nb::none(),
        "alpha"_a = nb::none(), "flip_h"_a = nb::none(), "flip_v"_a = nb::none(),
        "blend"_a = nb::none(), R"doc(
Draw a texture into a screen-space rectangle, bypassing the camera.

Args:
    texture (Texture): The texture to draw.
    dst (Rect): The destination rectangle in screen coordinates.
    angle (float, optional): The rotation angle in degrees. Defaults to 0.
    pivot (Vec2, optional): The pivot point for rotation and scaling. Defaults to Anchor.CENTER.
    options (DrawOptions, optional): Additional draw options. Defaults to None.
    source (Rect, optional): The source rectangle from the texture. Defaults to None (full texture).
    tint (Color, optional): The color tint to apply. Defaults to None (no tint).
    alpha (float, optional): The alpha multiplier (0..1). Defaults to None (no change).
    flip_h (bool, optional): Whether to flip the texture horizontally. Defaults to None (no flip).
    flip_v (bool, optional): Whether to flip the texture vertically. Defaults to None (no flip).
    blend (BlendMode, optional): The blend mode to use. Defaults to None (normal blending).
        )doc"
    );

    subRenderer.def(
        "draw_9slice",
        [](const Texture& texture, const Rect& dst, const Rect& slice,
           const std::optional<DrawOptions>& options, const std::optional<Rect>& source,
           const std::optional<Color>& tint, const std::optional<float>& alpha,
           const std::optional<bool>& flipH, const std::optional<bool>& flipV,
           const std::optional<BlendMode>& blend)
        {
            draw9Slice(
                texture, dst, slice,
                resolveOptions(options, source, tint, alpha, flipH, flipV, blend)
            );
        },
        "texture"_a, "dst"_a, "slice"_a, nb::kw_only(), "options"_a = nb::none(),
        "source"_a = nb::none(), "tint"_a = nb::none(), "alpha"_a = nb::none(),
        "flip_h"_a = nb::none(), "flip_v"_a = nb::none(), "blend"_a = nb::none(), R"doc(
Draw a nine-slice in screen coordinates.

Args:
    texture (Texture): The texture to draw.
    dst (Rect): The destination rectangle in screen coordinates.
    slice (Rect): The nine-slice rectangle in texture coordinates.
    options (DrawOptions, optional): Additional draw options. Defaults to None.
    source (Rect, optional): The source rectangle from the texture. Defaults to None (full texture).
    tint (Color, optional): The color tint to apply. Defaults to None (no tint).
    alpha (float, optional): The alpha multiplier (0..1). Defaults to None (no change).
    flip_h (bool, optional): Whether to flip the texture horizontally. Defaults to None (no flip).
    flip_v (bool, optional): Whether to flip the texture vertically. Defaults to None (no flip).
    blend (BlendMode, optional): The blend mode to use. Defaults to None (normal blending).
        )doc"
    );

    subRenderer.def(
        "draw_batch",
        [](const Texture& texture,
           nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> transforms,
           const Vec2& anchor, const Vec2& pivot, Batcher* batcher,
           const std::optional<DrawOptions>& options, const std::optional<Rect>& source,
           const std::optional<Color>& tint, const std::optional<float>& alpha,
           const std::optional<bool>& flipH, const std::optional<bool>& flipV,
           const std::optional<BlendMode>& blend,
           const std::optional<std::vector<DrawOptions>>& instanceOptions,
           const std::optional<std::vector<Rect>>& clipRects)
        {
            const auto common = resolveOptions(options, source, tint, alpha, flipH, flipV, blend);
            nb::gil_scoped_release release;
            drawBatchNDArray(
                texture, transforms, anchor, pivot, batcher, common, instanceOptions, clipRects
            );
        },
        "texture"_a, "transforms"_a, "anchor"_a = Anchor::TOP_LEFT, "pivot"_a = Anchor::CENTER,
        "batcher"_a = nb::none(), nb::kw_only(), "options"_a = nb::none(), "source"_a = nb::none(),
        "tint"_a = nb::none(), "alpha"_a = nb::none(), "flip_h"_a = nb::none(),
        "flip_v"_a = nb::none(), "blend"_a = nb::none(), "instance_options"_a = nb::none(),
        "clip_rects"_a = nb::none(), R"doc(
Draw a contiguous float64 NumPy array with 2, 3, 4, 5, or 9 columns:
[x, y, (angle), (scale or scale_x, scale_y), (source_x, source_y, source_w, source_h)].
The shared-style path uses geometry and optionally reusable Batcher buffers.
Per-instance colors, flips, and sources remain on the geometry path.
Blend-mode changes split geometry submissions while preserving order.
Array source columns override option sources; clip_rects overrides both.

Args:
    texture (Texture): The texture to draw.
    transforms (ndarray): A 2D NumPy array of shape (N, M) where N is the number of instances and M is 2, 3, 4, 5, or 9.
    anchor (Vec2, optional): The anchor point of the texture. Defaults to Anchor.TOP_LEFT.
    pivot (Vec2, optional): The pivot point for rotation and scaling. Defaults to Anchor.CENTER.
    batcher (Batcher, optional): A reusable Batcher for internal memory. Defaults to None (internal allocation).
    options (DrawOptions, optional): Additional draw options. Defaults to None.
    source (Rect, optional): The source rectangle from the texture. Defaults to None (full texture).
    tint (Color, optional): The color tint to apply. Defaults to None (no tint).
    alpha (float, optional): The alpha multiplier (0..1). Defaults to None (no change).
    flip_h (bool, optional): Whether to flip the texture horizontally. Defaults to None (no flip).
    flip_v (bool, optional): Whether to flip the texture vertically. Defaults to None (no flip).
    blend (BlendMode, optional): The blend mode to use. Defaults to None (normal blending).
    instance_options (list of DrawOptions, optional): A list of DrawOptions for each instance. Must have the same length as transforms if provided.
    clip_rects (list of Rect, optional): A list of source rectangles for each instance. Must have the same length as transforms if provided.
        )doc"
    );

    subRenderer.def(
        "draw_batch",
        [](const Texture& texture, const std::vector<Transform>& transforms, const Vec2& anchor,
           const Vec2& pivot, const std::optional<std::vector<Rect>>& clipRects,
           const std::optional<DrawOptions>& options, const std::optional<Rect>& source,
           const std::optional<Color>& tint, const std::optional<float>& alpha,
           const std::optional<bool>& flipH, const std::optional<bool>& flipV,
           const std::optional<BlendMode>& blend,
           const std::optional<std::vector<DrawOptions>>& instanceOptions)
        {
            const auto common = resolveOptions(options, source, tint, alpha, flipH, flipV, blend);
            nb::gil_scoped_release release;
            drawBatchList(texture, transforms, anchor, pivot, clipRects, common, instanceOptions);
        },
        "texture"_a, "transforms"_a, "anchor"_a = Anchor::TOP_LEFT, "pivot"_a = Anchor::CENTER,
        "clip_rects"_a = nb::none(), nb::kw_only(), "options"_a = nb::none(),
        "source"_a = nb::none(), "tint"_a = nb::none(), "alpha"_a = nb::none(),
        "flip_h"_a = nb::none(), "flip_v"_a = nb::none(), "blend"_a = nb::none(),
        "instance_options"_a = nb::none(), R"doc(
Draw a sequence of transforms using local properties.
instance_options, if supplied, replaces common options per instance and must have
one entry per transform. clip_rects overrides source and must match the batch length.

Args:
    texture (Texture): The texture to draw.
    transforms (list of Transform): A list of Transform objects for each instance.
    anchor (Vec2, optional): The anchor point of the texture. Defaults to Anchor.TOP_LEFT.
    pivot (Vec2, optional): The pivot point for rotation and scaling. Defaults to Anchor.CENTER.
    clip_rects (list of Rect, optional): A list of source rectangles for each instance. Must have the same length as transforms if provided.
    options (DrawOptions, optional): Additional draw options. Defaults to None.
    source (Rect, optional): The source rectangle from the texture. Defaults to None (full texture).
    tint (Color, optional): The color tint to apply. Defaults to None (no tint).
    alpha (float, optional): The alpha multiplier (0..1). Defaults to None (no change).
    flip_h (bool, optional): Whether to flip the texture horizontally. Defaults to None (no flip).
    flip_v (bool, optional): Whether to flip the texture vertically. Defaults to None (no flip).
    blend (BlendMode, optional): The blend mode to use. Defaults to None (normal blending).
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
