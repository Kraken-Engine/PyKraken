#include <SDL3_image/SDL_image.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>

#include <cstring>
#include <vector>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/math/Math.hpp"

namespace kn::pixel_array
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::enum_<ScrollMode>(module, "ScrollMode", R"doc(
Edge handling behavior for PixelArray scrolling.
    )doc")
        .value("SMEAR", ScrollMode::SMEAR, "Clamp edge pixels when scrolling")
        .value("ERASE", ScrollMode::ERASE, "Erase pixels that scroll out")
        .value("REPEAT", ScrollMode::REPEAT, "Wrap pixels when scrolling");

    nb::class_<PixelArray>(module, "PixelArray", R"doc(
Represents a 2D pixel buffer for image manipulation and blitting operations.

A PixelArray is a 2D array of pixels that can be manipulated, drawn on, and used as a source
for texture creation or blitting to other PixelArrays. Supports pixel-level operations,
color key transparency, and alpha blending.
    )doc")
        .def(nb::init<int, int>(), "width"_a, "height"_a, R"doc(
Create a new PixelArray with the specified dimensions.

Args:
    width (int): The width of the pixel array.
    height (int): The height of the pixel array.

Raises:
    RuntimeError: If pixel array creation fails.
        )doc")
        .def(nb::init<const std::filesystem::path&>(), "file_path"_a, R"doc(
Create a PixelArray by loading an image from a file.

Args:
    file_path (str | os.PathLike[str]): Path to the image file to load.

Raises:
    RuntimeError: If the file cannot be loaded or doesn't exist.
        )doc")

        .def_prop_rw("color_key", &PixelArray::getColorKey, &PixelArray::setColorKey, R"doc(
The color key for transparency.

When set, pixels of this color will be treated as transparent during blitting operations.
Used for simple transparency effects.

Returns:
    Color: The current color key.

Raises:
    RuntimeError: If getting the color key fails.
        )doc")
        .def_prop_rw("alpha_mod", &PixelArray::getAlpha, &PixelArray::setAlpha, R"doc(
The alpha modulation value for the pixel array.

Controls the overall transparency of the pixel array. Values range from 0 (fully transparent)
to 255 (fully opaque).

Returns:
    int: The current alpha modulation value [0-255].

Raises:
    RuntimeError: If getting the alpha value fails.
        )doc")

        .def_prop_ro("width", &PixelArray::getWidth, R"doc(
The width of the pixel array.

Returns:
    int: The pixel array width.
        )doc")
        .def_prop_ro("height", &PixelArray::getHeight, R"doc(
The height of the pixel array.

Returns:
    int: The pixel array height.
        )doc")
        .def_prop_ro("size", &PixelArray::getSize, R"doc(
The size of the pixel array as a Vec2.

Returns:
    Vec2: The pixel array size as (width, height).
        )doc")

        .def("fill", &PixelArray::fill, nb::call_guard<nb::gil_scoped_release>(), "color"_a, R"doc(
Fill the entire pixel array with a solid color.

Args:
    color (Color): The color to fill the pixel array with.
        )doc")
        .def(
            "blit",
            nb::overload_cast<
                const PixelArray&, const Vec2&, const Vec2&,
                const Rect&>(&PixelArray::blit, nb::const_),
            nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "pos"_a,
            "anchor"_a = Anchor::TOP_LEFT, "src"_a = Rect{}, R"doc(
Blit (copy) another pixel array onto this pixel array at the specified position with anchor alignment.

Args:
    pixel_array (PixelArray): The source pixel array to blit from.
    pos (Vec2): The position to blit to.
    anchor (Vec2, optional): The anchor point for positioning. Defaults to (0,0) TopLeft.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.

Raises:
    RuntimeError: If the blit operation fails.
        )doc"
        )
        .def(
            "blit",
            nb::overload_cast<
                const PixelArray&, const Rect&, const Rect&>(&PixelArray::blit, nb::const_),
            nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "dst"_a, "src"_a = Rect{},
            R"doc(
Blit (copy) another pixel array onto this pixel array with specified destination and source rectangles.

Args:
    pixel_array (PixelArray): The source pixel array to blit from.
    dst (Rect): The destination rectangle on this pixel array.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.

Raises:
    RuntimeError: If the blit operation fails.
        )doc"
        )
        .def("get_at", &PixelArray::getAt, "x"_a, "y"_a, R"doc(
Get the color of a pixel at the specified coordinates.

Args:
    x (int): The x-coordinate of the pixel.
    y (int): The y-coordinate of the pixel.

Returns:
    Color: The color of the pixel at the specified coordinates.

Raises:
    IndexError: If coordinates are outside the pixel array bounds.
        )doc")
        .def("set_at", &PixelArray::setAt, "x"_a, "y"_a, "color"_a, R"doc(
Set the color of a pixel at the specified coordinates.

Args:
    x (int): The x-coordinate of the pixel.
    y (int): The y-coordinate of the pixel.
    color (Color): The color to set the pixel to.

Raises:
    IndexError: If coordinates are outside the pixel array bounds.
        )doc")
        .def("copy", &PixelArray::copy, nb::call_guard<nb::gil_scoped_release>(), R"doc(
Create a copy of this pixel array.

Returns:
    PixelArray: A new PixelArray that is an exact copy of this one.

Raises:
    RuntimeError: If pixel array copying fails.
        )doc")
        .def("get_rect", &PixelArray::getRect, R"doc(
Get a rectangle representing the pixel array bounds.

Returns:
    Rect: A rectangle with position (0, 0) and the pixel array's dimensions.
        )doc")
        .def(
            "scroll", &PixelArray::scroll, nb::call_guard<nb::gil_scoped_release>(), "dx"_a, "dy"_a,
            "scroll_mode"_a,
            R"doc(
Scroll the pixel array's contents by the specified offset.

Args:
    dx (int): Horizontal scroll offset in pixels.
    dy (int): Vertical scroll offset in pixels.
    scroll_mode (ScrollMode, optional): Behavior for pixels scrolled off the edge.
        - REPEAT: Wrap pixels around to the opposite edge.
        - ERASE: Fill scrolled areas with transparent pixels.
        - SMEAR: Extend edge pixels into scrolled areas.
        )doc"
        );

    auto subPixelArray =
        module.def_submodule("pixel_array", "Functions for manipulating PixelArray objects");

    subPixelArray.def(
        "flip", &flip, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "flip_x"_a,
        "flip_y"_a,
        R"doc(
Flip a pixel array horizontally, vertically, or both.

Args:
    pixel_array (PixelArray): The pixel array to flip.
    flip_x (bool): Whether to flip horizontally (mirror left-right).
    flip_y (bool): Whether to flip vertically (mirror top-bottom).

Returns:
    PixelArray: A new pixel array with the flipped image.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc"
    );

    subPixelArray.def(
        "scale_to", &scaleTo, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "size"_a,
        R"doc(
Scale a pixel array to a new exact size.

Args:
    pixel_array (PixelArray): The pixel array to scale.
    size (Vec2): The target size as (width, height).

Returns:
    PixelArray: A new pixel array scaled to the specified size.

Raises:
    RuntimeError: If pixel array creation or scaling fails.
    )doc"
    );

    subPixelArray.def(
        "scale_by", nb::overload_cast<const PixelArray&, double>(&scaleBy),
        nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "factor"_a, R"doc(
Scale a pixel array by a given factor.

Args:
    pixel_array (PixelArray): The pixel array to scale.
    factor (float): The scaling factor (must be > 0). Values > 1.0 enlarge,
                   values < 1.0 shrink the pixel array.

Returns:
    PixelArray: A new pixel array scaled by the specified factor.

Raises:
    ValueError: If factor is <= 0.
    RuntimeError: If pixel array creation or scaling fails.
    )doc"
    );

    subPixelArray.def(
        "rotate", &rotate, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "angle"_a,
        R"doc(
Rotate a pixel array by a given angle.

Args:
    pixel_array (PixelArray): The pixel array to rotate.
    angle (float): The rotation angle in degrees. Positive values rotate clockwise.

Returns:
    PixelArray: A new pixel array containing the rotated image. The output pixel array may be
            larger than the input to accommodate the rotated image.

Raises:
    RuntimeError: If pixel array rotation fails.
    )doc"
    );

    subPixelArray.def(
        "box_blur", &boxBlur, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, "radius"_a,
        "repeat_edge_pixels"_a = true, R"doc(
Apply a box blur effect to a pixel array.

Box blur creates a uniform blur effect by averaging pixels within a square kernel.
It's faster than Gaussian blur but produces a more uniform, less natural look.

Args:
    pixel_array (PixelArray): The pixel array to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the pixel array bounds. Defaults to True.

Returns:
    PixelArray: A new pixel array with the box blur effect applied.

Raises:
    RuntimeError: If pixel array creation fails during the blur process.
    )doc"
    );

    subPixelArray.def(
        "gaussian_blur", &gaussianBlur, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a,
        "radius"_a, "repeat_edge_pixels"_a = true,
        R"doc(
Apply a Gaussian blur effect to a pixel array.

Gaussian blur creates a natural, smooth blur effect using a Gaussian distribution
for pixel weighting. It produces higher quality results than box blur but is
computationally more expensive.

Args:
    pixel_array (PixelArray): The pixel array to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the pixel array bounds. Defaults to True.

Returns:
    PixelArray: A new pixel array with the Gaussian blur effect applied.

Raises:
    RuntimeError: If pixel array creation fails during the blur process.
    )doc"
    );

    subPixelArray
        .def("invert", &invert, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, R"doc(
Invert the colors of a pixel array.

Creates a negative image effect by inverting each color channel (RGB).
The alpha channel is preserved unchanged.

Args:
    pixel_array (PixelArray): The pixel array to invert.

Returns:
    PixelArray: A new pixel array with inverted colors.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc");

    subPixelArray.def(
        "grayscale", &grayscale, nb::call_guard<nb::gil_scoped_release>(), "pixel_array"_a, R"doc(
Convert a pixel array to grayscale.

Converts the pixel array to grayscale using the standard luminance formula:
gray = 0.299 * red + 0.587 * green + 0.114 * blue

This formula accounts for human perception of brightness across different colors.
The alpha channel is preserved unchanged.

Args:
    pixel_array (PixelArray): The pixel array to convert to grayscale.

Returns:
    PixelArray: A new pixel array converted to grayscale.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc"
    );
}
}  // namespace kn::pixel_array
