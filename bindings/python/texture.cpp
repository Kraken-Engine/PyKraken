#include "kraken/graphics/Texture.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>

#include <string>

#include "bindings/python/bindings.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Renderer.hpp"

namespace kn
{
namespace texture
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::enum_<TextureAccess>(module, "TextureAccess", R"doc(
Texture access mode for GPU textures.
    )doc")
        .value("STATIC", TextureAccess::Static, "Static texture")
        .value("TARGET", TextureAccess::Target, "Render target texture");

    nb::enum_<TextureUsage>(module, "TextureUsage", nb::is_flag(), R"doc(
Texture usage flags describing how a texture can be used.

These values can be combined to create textures that are both drawable and
shader-sampled.
    )doc")
        .value("DRAWABLE", TextureUsage::Drawable, "Renderer texture storage")
        .value("SHADER_SAMPLED", TextureUsage::ShaderSampled, "GPU shader-sampled texture");

    nb::class_<Texture> texture(module, "Texture", R"doc(
Represents a hardware-accelerated image that can be efficiently rendered.

Textures are optimized for fast rendering operations and support various effects
like rotation, flipping, tinting, alpha blending, and different blend modes.
They can be created from image files or pixel arrays.
    )doc");

    nb::class_<Texture::Flip>(texture, "Flip", R"doc(
Controls horizontal and vertical flipping of a texture during rendering.

Used to mirror textures along the horizontal and/or vertical axes without
creating additional texture data.
    )doc")
        .def_rw("h", &Texture::Flip::h, R"doc(
Enable or disable horizontal flipping.

When True, the texture is mirrored horizontally (left-right flip).
        )doc")
        .def_rw("v", &Texture::Flip::v, R"doc(
Enable or disable vertical flipping.

When True, the texture is mirrored vertically (top-bottom flip).
        )doc");

    texture
        .def(
            nb::init<const std::filesystem::path&, FilterMode, TextureAccess, TextureUsage>(),
            "file_path"_a, "filter"_a = FilterMode::Default, "access"_a = TextureAccess::Static,
            "usage"_a = TextureUsage::Drawable, R"doc(
Create a Texture by loading an image from a file.
If no scale mode is provided, the default renderer scale mode is used.

Args:
    file_path (str | os.PathLike[str]): Path to the image file to load.
    filter (FilterMode, optional): Scaling/filtering mode for the texture.
    access (TextureAccess, optional): Texture access type (STATIC or TARGET).
    usage (TextureUsage, optional): Texture usage flags controlling renderer and GPU access.

Raises:
    ValueError: If file_path is empty.
    RuntimeError: If the file cannot be loaded or texture creation fails.
        )doc"
        )
        .def(
            nb::init<const PixelArray&, FilterMode, TextureAccess, TextureUsage>(), "pixel_array"_a,
            "filter"_a = FilterMode::Default, "access"_a = TextureAccess::Static,
            "usage"_a = TextureUsage::Drawable, R"doc(
Create a Texture from an existing PixelArray.
If no scale mode is provided, the default renderer scale mode is used.

Args:
    pixel_array (PixelArray): The pixel array to convert to a texture.
    filter (FilterMode, optional): Scaling/filtering mode for the texture.
    access (TextureAccess, optional): Texture access type (STATIC or TARGET).
    usage (TextureUsage, optional): Texture usage flags controlling renderer and GPU access.

Raises:
    RuntimeError: If texture creation from pixel array fails.
        )doc"
        )
        .def(
            nb::init<int, int, FilterMode, TextureUsage>(), "width"_a, "height"_a,
            "filter"_a = FilterMode::Default, "usage"_a = TextureUsage::Drawable,
            R"doc(
Create a (render target) Texture with the specified size.
If no scale mode is provided, the default renderer scale mode is used.

Args:
    width (int): The width of the texture in pixels (must be > 0).
    height (int): The height of the texture in pixels (must be > 0).
    filter (FilterMode, optional): Scaling/filtering mode for the texture.
    usage (TextureUsage, optional): Texture usage flags controlling renderer and GPU access.

Raises:
    RuntimeError: If texture creation fails.
        )doc"
        )

        .def_prop_ro("usage", &Texture::getUsage, R"doc(
The usage flags describing how the texture can be used.

Returns:
    TextureUsage: The texture usage bitmask.
        )doc")

        .def("has_usage", &Texture::hasUsage, "usage"_a, R"doc(
Check whether the texture was created with a specific usage flag.

Args:
    usage (TextureUsage): Usage flag to test.

Returns:
    bool: True if the usage flag is present.
        )doc")

        .def_rw("flip", &Texture::flip, R"doc(
The flip settings for horizontal and vertical mirroring.

Controls whether the texture is flipped horizontally and/or vertically during rendering.
        )doc")

        .def_prop_rw("alpha", &Texture::getAlpha, &Texture::setAlpha, R"doc(
Get or set the alpha modulation of the texture as a float between `0.0` and `1.0`.
        )doc")
        .def_prop_rw("clip_area", &Texture::getClipArea, &Texture::setClipArea, R"doc(
Get or set the clip area (atlas region) of the texture.
        )doc")
        .def_prop_rw("tint", &Texture::getTint, &Texture::setTint, R"doc(
Get or set the color tint applied to the texture.
        )doc")
        .def_prop_ro("width", &Texture::getWidth, R"doc(
The width of the texture in pixels.
        )doc")
        .def_prop_ro("height", &Texture::getHeight, R"doc(
The height of the texture in pixels.
        )doc")
        .def_prop_ro("size", &Texture::getSize, R"doc(
The dimensions of the texture as a `Vec2`.
        )doc")

        .def("get_rect", &Texture::getRect, R"doc(
Return a Rect with position (0, 0) and the texture's dimensions.

Returns:
    Rect: A rectangle representing the texture's bounds.
        )doc")

        .def("make_additive", &Texture::makeAdditive, R"doc(
Set the texture to use additive blending mode.

In additive mode, the texture's colors are added to the destination,
creating bright, glowing effects.
        )doc")
        .def("make_multiply", &Texture::makeMultiply, R"doc(
Set the texture to use multiply blending mode.

In multiply mode, the texture's colors are multiplied with the destination,
creating darkening and shadow effects.
        )doc")
        .def("make_normal", &Texture::makeNormal, R"doc(
Set the texture to use normal (alpha) blending mode.

This is the default blending mode for standard transparency effects.
        )doc");
}
}  // namespace texture
}  // namespace kn
