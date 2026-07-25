#include "kraken/graphics/Mask.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <algorithm>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/math/Math.hpp"

namespace kn
{
namespace mask
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Mask>(module, "Mask", R"doc(
A collision mask for pixel-perfect collision detection.

A Mask represents a 2D bitmap, typically used for precise collision detection based on
non-transparent pixels.
    )doc")
        .def(nb::init(), R"doc(
Create an empty mask with size (0, 0).
        )doc")
        .def(
            nb::init<const Vec2&, bool>(), "size"_a, "filled"_a = false,
            R"doc(
Create a mask with specified size.

Args:
    size (Vec2): The size of the mask as (width, height).
    filled (bool): Whether to fill the mask with solid pixels. Defaults to False.
        )doc"
        )
        .def(
            nb::init<const PixelArray&, uint8_t>(), "pixel_array"_a, "threshold"_a = 1,
            R"doc(
Create a mask from a pixel array based on alpha threshold.

Args:
    pixel_array (PixelArray): The source pixel array to create the mask from.
    threshold (int): Alpha threshold value (0-255). Pixels with alpha >= threshold are solid.

Raises:
    RuntimeError: If the pixel array is invalid.
        )doc"
        )

        .def_prop_ro("width", &Mask::getWidth, R"doc(
The width of the mask in pixels.
    )doc")
        .def_prop_ro("height", &Mask::getHeight, R"doc(
The height of the mask in pixels.
    )doc")
        .def_prop_ro("size", &Mask::getSize, R"doc(
The size of the mask as a Vec2.
    )doc")

        .def("copy", &Mask::copy, R"doc(
Create a copy of this mask.

Returns:
    Mask: A new Mask with the same dimensions and pixel data.
        )doc")
        .def("get_at", &Mask::getAt, "x"_a, "y"_a, R"doc(
Get the pixel value at a specific position.

Args:
    x (int): The x-coordinate of the position to check.
    y (int): The y-coordinate of the position to check.

Returns:
    bool: True if the pixel is solid (above threshold), False otherwise.
        )doc")
        .def("set_at", &Mask::setAt, "x"_a, "y"_a, "value"_a, R"doc(
Set the pixel value at a specific position.

Args:
    x (int): The x-coordinate of the position to set.
    y (int): The y-coordinate of the position to set.
    value (bool): The pixel value (True for solid, False for transparent).
        )doc")
        .def(
            "get_overlap_area", &Mask::getOverlapArea, "other"_a, "offset"_a = Vec2{},
            R"doc(
Get the number of overlapping pixels between this mask and another.

Args:
    other (Mask): The other mask to check overlap with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    int: The number of overlapping solid pixels.
        )doc"
        )
        .def(
            "get_overlap_mask", &Mask::getOverlapMask, "other"_a, "offset"_a = Vec2{},
            R"doc(
Get a mask representing the overlapping area between this mask and another.

Args:
    other (Mask): The other mask to check overlap with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    Mask: A new mask containing only the overlapping pixels.
        )doc"
        )
        .def("fill", &Mask::fill, R"doc(
Fill the entire mask with solid pixels.
        )doc")
        .def("clear", &Mask::clear, R"doc(
Clear the entire mask, setting all pixels to transparent.
        )doc")
        .def("invert", &Mask::invert, R"doc(
Invert all pixels in the mask.

Solid pixels become transparent and transparent pixels become solid.
        )doc")
        .def(
            "add", &Mask::add, "other"_a, "offset"_a = Vec2{},
            R"doc(
Add another mask to this mask with an offset.

Performs a bitwise OR operation between the masks.

Args:
    other (Mask): The mask to add.
    offset (Vec2): Position offset for the other mask. Defaults to (0, 0).
        )doc"
        )
        .def(
            "subtract", &Mask::subtract, "other"_a, "offset"_a = Vec2{},
            R"doc(
Subtract another mask from this mask with an offset.

Removes pixels where the other mask has solid pixels.

Args:
    other (Mask): The mask to subtract.
    offset (Vec2): Position offset for the other mask. Defaults to (0, 0).
        )doc"
        )
        .def("get_count", &Mask::getCount, R"doc(
Get the number of solid pixels in the mask.

Returns:
    int: The count of solid pixels.
        )doc")
        .def("get_center_of_mass", &Mask::getCenterOfMass, R"doc(
Calculate the center of mass of all solid pixels.

Returns:
    Vec2: The center of mass position. Returns (0, 0) if mask is empty.
        )doc")
        .def("get_outline", &Mask::getOutline, R"doc(
Get the outline points of the mask.

Returns a list of points that form the outline of all solid regions.

Returns:
    list[Vec2]: A list of outline points.
        )doc")
        .def("get_bounding_rect", &Mask::getBoundingRect, R"doc(
Get the bounding rectangle that contains all solid pixels.

Returns:
    Rect: The smallest rectangle containing all solid pixels.
          Returns empty rect if mask has no solid pixels.
        )doc")
        .def(
            "collide_mask", &Mask::collideMask, "other"_a, "offset"_a = Vec2{},
            R"doc(
Check collision between this mask and another mask with an offset.

Args:
    other (Mask): The other mask to test collision with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    bool: True if the masks collide, False otherwise.
        )doc"
        )
        .def(
            "get_collision_points", &Mask::getCollisionPoints, "other"_a, "offset"_a = Vec2{},
            R"doc(
Get all points where this mask collides with another mask.

Args:
    other (Mask): The other mask to test collision with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    list[Vec2]: A list of collision points.
        )doc"
        )
        .def("is_empty", &Mask::isEmpty, R"doc(
Check if the mask contains no solid pixels.

Returns:
    bool: True if the mask is empty, False otherwise.
        )doc")
        .def(
            "get_pixel_array", &Mask::getPixelArray, "color"_a = Color{255, 255, 255, 255},
            R"doc(
Convert the mask to a pixel array with the specified color.

Solid pixels become the specified color, transparent pixels become transparent.

Args:
    color (Color): The color to use for solid pixels. Defaults to white (255, 255, 255, 255).

Returns:
    PixelArray: A new pixel array representation of the mask.

Raises:
    RuntimeError: If pixel array creation fails.
        )doc"
        )
        .def("get_rect", &Mask::getRect, R"doc(
Get the bounding rectangle of the mask starting at (0, 0).
    )doc");
}
}  // namespace mask
}  // namespace kn
