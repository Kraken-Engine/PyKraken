#include "kraken/geometry/Rect.hpp"

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>

#include <string>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Line.hpp"

namespace kn
{
namespace rect
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Rect>(module, "Rect", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY), R"doc(
Represents a rectangle with position and size.

A Rect is defined by its top-left corner position (x, y) and dimensions (w, h).
Supports various geometric operations, collision detection, and positioning methods.
        )doc")
        .def(nb::init(), R"doc(Create a Rect with default values (0, 0, 0, 0).)doc")
        .def(nb::init<const Vec2&>(), "size"_a, R"doc(
Create a Rect at position (0, 0) with the given size.

Args:
    size (Vec2): The size as a Vec2 (width, height).
        )doc")
        .def(nb::init<double, double, double, double>(), "x"_a, "y"_a, "w"_a, "h"_a, R"doc(
Create a Rect with specified position and dimensions.

Args:
    x (float): The x coordinate of the top-left corner.
    y (float): The y coordinate of the top-left corner.
    w (float): The width of the rectangle.
    h (float): The height of the rectangle.
        )doc")
        .def(nb::init<double, double, const Vec2&>(), "x"_a, "y"_a, "size"_a, R"doc(
Create a Rect with specified position and size vector.

Args:
    x (float): The x coordinate of the top-left corner.
    y (float): The y coordinate of the top-left corner.
    size (Vec2): The size as a Vec2 (width, height).
        )doc")
        .def(nb::init<const Vec2&, double, double>(), "pos"_a, "w"_a, "h"_a, R"doc(
Create a Rect with specified position vector and dimensions.

Args:
    pos (Vec2): The position as a Vec2 (x, y).
    w (float): The width of the rectangle.
    h (float): The height of the rectangle.
        )doc")
        .def(nb::init<const Vec2&, const Vec2&>(), "pos"_a, "size"_a, R"doc(
Create a Rect with specified position and size vectors.

Args:
    pos (Vec2): The position as a Vec2 (x, y).
    size (Vec2): The size as a Vec2 (width, height).
        )doc")

        .def_rw("x", &Rect::x, R"doc(
The x coordinate of the top-left corner.
        )doc")
        .def_rw("y", &Rect::y, R"doc(
The y coordinate of the top-left corner.
        )doc")
        .def_rw("w", &Rect::w, R"doc(
The width of the rectangle.
        )doc")
        .def_rw("h", &Rect::h, R"doc(
The height of the rectangle.
        )doc")

        .def_prop_rw("left", &Rect::getLeft, &Rect::setLeft, R"doc(
The x coordinate of the left edge.
        )doc")
        .def_prop_rw("right", &Rect::getRight, &Rect::setRight, R"doc(
The x coordinate of the right edge.
        )doc")
        .def_prop_rw("top", &Rect::getTop, &Rect::setTop, R"doc(
The y coordinate of the top edge.
        )doc")
        .def_prop_rw("bottom", &Rect::getBottom, &Rect::setBottom, R"doc(
The y coordinate of the bottom edge.
        )doc")
        .def_prop_rw("pos", &Rect::getPos, &Rect::setPos, R"doc(
The position of the top-left corner as (x, y).
        )doc")
        .def_prop_rw("size", &Rect::getSize, &Rect::setSize, R"doc(
The size of the rectangle as (width, height).
        )doc")
        .def_prop_rw("top_left", &Rect::getTopLeft, &Rect::setTopLeft, R"doc(
The position of the top-left corner as (x, y).
        )doc")
        .def_prop_rw("top_mid", &Rect::getTopMid, &Rect::setTopMid, R"doc(
The position of the top-middle point as (x, y).
        )doc")
        .def_prop_rw("top_right", &Rect::getTopRight, &Rect::setTopRight, R"doc(
The position of the top-right corner as (x, y).
        )doc")
        .def_prop_rw("mid_left", &Rect::getMidLeft, &Rect::setMidLeft, R"doc(
The position of the middle-left point as (x, y).
        )doc")
        .def_prop_rw("center", &Rect::getCenter, &Rect::setCenter, R"doc(
The position of the center point as (x, y).
        )doc")
        .def_prop_rw("mid_right", &Rect::getMidRight, &Rect::setMidRight, R"doc(
The position of the middle-right point as (x, y).
        )doc")
        .def_prop_rw("bottom_left", &Rect::getBottomLeft, &Rect::setBottomLeft, R"doc(
The position of the bottom-left corner as (x, y).
        )doc")
        .def_prop_rw("bottom_mid", &Rect::getBottomMid, &Rect::setBottomMid, R"doc(
The position of the bottom-middle point as (x, y).
        )doc")
        .def_prop_rw("bottom_right", &Rect::getBottomRight, &Rect::setBottomRight, R"doc(
The position of the bottom-right corner as (x, y).
        )doc")

        .def("copy", &Rect::copy, R"doc(
Create a copy of this rectangle.

Returns:
    Rect: A new Rect with the same position and size.
        )doc")
        .def("move", &Rect::move, "offset"_a, R"doc(
Move the rectangle by the given offset.

Args:
    offset (Vec2): The offset to move by as (dx, dy).
        )doc")
        .def("moved", &Rect::moved, "offset"_a, R"doc(
Return a new Rect moved by the given offset.

Args:
    offset (Vec2): The offset to move by.

Returns:
    Rect: A new rectangle moved by the offset.
        )doc")
        .def("inflate", &Rect::inflate, "offset"_a, R"doc(
Inflate the rectangle by the given offset.

The rectangle grows in all directions. The position is adjusted to keep the center
in the same place.

Args:
    offset (Vec2): The amount to inflate by as (dw, dh).
        )doc")
        .def("fit", &Rect::fit, "other"_a, R"doc(
Scale this rectangle to fit inside another rectangle while maintaining aspect ratio.

Args:
    other (Rect): The rectangle to fit inside.

Raises:
    ValueError: If other rectangle has non-positive dimensions.
        )doc")
        .def("clamp", nb::overload_cast<const Rect&>(&Rect::clamp), "other"_a, R"doc(
Clamp this rectangle to be within another rectangle.

Args:
    other (Rect): The rectangle to clamp within.

Raises:
    ValueError: If this rectangle is larger than the clamp area.
        )doc")
        .def(
            "clamped", nb::overload_cast<const Rect&>(&Rect::clamped, nb::const_), "other"_a,
            R"doc(
Return a new Rect clamped within another rectangle.

Args:
    other (Rect): The rectangle to clamp within.

Returns:
    Rect: A new clamped rectangle.
        )doc"
        )
        .def(
            "clamp", nb::overload_cast<const Vec2&, const Vec2&>(&Rect::clamp), "min"_a, "max"_a,
            R"doc(
Clamp this rectangle to be within the specified bounds.

Args:
    min (Vec2): The minimum bounds as (min_x, min_y).
    max (Vec2): The maximum bounds as (max_x, max_y).

Raises:
    ValueError: If min >= max or rectangle is larger than the clamp area.
        )doc"
        )
        .def(
            "clamped", nb::overload_cast<const Vec2&, const Vec2&>(&Rect::clamped, nb::const_),
            "min"_a, "max"_a, R"doc(
Return a new Rect clamped within the specified bounds.

Args:
    min (Vec2): The minimum bounds as (min_x, min_y).
    max (Vec2): The maximum bounds as (max_x, max_y).

Returns:
    Rect: A new clamped rectangle.
        )doc"
        )
        .def("scale_by", nb::overload_cast<double>(&Rect::scaleBy), "factor"_a, R"doc(
Scale the rectangle by a uniform factor.

Args:
    factor (float): The scaling factor (must be > 0).

Raises:
    ValueError: If factor is <= 0.
        )doc")
        .def("scaled_by", nb::overload_cast<double>(&Rect::scaledBy, nb::const_), "factor"_a, R"doc(
Return a new Rect scaled by a uniform factor.

Args:
    factor (float): The scaling factor (must be > 0).

Returns:
    Rect: A new scaled rectangle.
        )doc")
        .def("scale_by", nb::overload_cast<const Vec2&>(&Rect::scaleBy), "factor"_a, R"doc(
Scale the rectangle by different factors for width and height.

Args:
    factor (Vec2): The scaling factors as (scale_x, scale_y).

Raises:
    ValueError: If any factor is <= 0.
        )doc")
        .def(
            "scaled_by", nb::overload_cast<const Vec2&>(&Rect::scaledBy, nb::const_), "factor"_a,
            R"doc(
Return a new Rect scaled by different factors for width and height.

Args:
    factor (Vec2): The scaling factors as (scale_x, scale_y).

Returns:
    Rect: A new scaled rectangle.
        )doc"
        )
        .def("scale_to", &Rect::scaleTo, "size"_a, R"doc(
Scale the rectangle to the specified size.

Args:
    size (Vec2): The new size as (width, height).

Raises:
    ValueError: If width or height is <= 0.
        )doc")
        .def("scaled_to", &Rect::scaledTo, "size"_a, R"doc(
Return a new Rect scaled to the specified size.

Args:
    size (Vec2): The new size as (width, height).

Returns:
    Rect: A new scaled rectangle.
        )doc")
        .def("get_corners", &Rect::getCorners, R"doc(
Get the corners of the rectangle.

Returns:
    List[Vec2]: A list of the four corners in the order: top-left, top-right, bottom-right, bottom-left.
        )doc")
        .def("get_edges", &Rect::getEdges, R"doc(
Get the edges of the rectangle as Line segments.

Returns:
    List[Line]: A list of the four edges as Line objects in the order: top, right, bottom, left.
        )doc")

        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def("__bool__", [](const Rect& rect) -> bool { return rect.w > 0 && rect.h > 0; })
        .def(
            "__str__",
            [](const Rect& rect) -> std::string
            {
                return "[" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " +
                       std::to_string(rect.w) + ", " + std::to_string(rect.h) + "]";
            }
        )
        .def(
            "__repr__",
            [](const Rect& rect) -> std::string
            {
                return "Rect(x=" + std::to_string(rect.x) + ", y=" + std::to_string(rect.y) +
                       ", w=" + std::to_string(rect.w) + ", h=" + std::to_string(rect.h) + ")";
            }
        )
        .def(
            "__iter__", [](const Rect& rect) -> nb::iterator
            { return nb::make_iterator(nb::type<Rect>(), "iterator", &rect.x, &rect.x + 4); },
            nb::keep_alive<0, 1>()
        )
        .def("__len__", [](const Rect&) -> int { return 4; })
        .def(
            "__getitem__",
            [](const Rect& rect, const size_t i) -> double
            {
                switch (i)
                {
                case 0:
                    return rect.x;
                case 1:
                    return rect.y;
                case 2:
                    return rect.w;
                case 3:
                    return rect.h;
                default:
                    throw nb::index_error("Index out of range");
                }
            },
            "index"_a
        );
}
}  // namespace rect
}  // namespace kn
