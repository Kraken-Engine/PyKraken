#include "kraken/geometry/Collision.hpp"

#include <nanobind/nanobind.h>

#include <algorithm>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Circle.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/math/Math.hpp"

namespace kn::collision
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subCollision = module.def_submodule("collision", "Collision detection functions");

    // overlap functions
    subCollision
        .def("overlap", nb::overload_cast<const Rect&, const Rect&>(&overlap), "a"_a, "b"_a, R"doc(
Check whether two rectangles overlap.

Args:
    a (Rect): The first rectangle.
    b (Rect): The second rectangle.

Returns:
    bool: True if the rectangles overlap.
                     )doc");
    subCollision.def(
        "overlap", nb::overload_cast<const Rect&, const Circle&>(&overlap), "rect"_a, "circle"_a,
        R"doc(
Check whether a rectangle and a circle overlap.

Args:
    rect (Rect): The rectangle.
    circle (Circle): The circle.

Returns:
    bool: True if the rectangle and circle overlap.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Rect&, const Line&>(&overlap), "rect"_a, "line"_a,
        R"doc(
Check whether a rectangle and a line overlap.

Args:
    rect (Rect): The rectangle.
    line (Line): The line.

Returns:
    bool: True if the rectangle and line overlap.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Rect&, const Vec2&>(&overlap), "rect"_a, "point"_a,
        R"doc(
Check whether a rectangle contains a point.

Args:
    rect (Rect): The rectangle.
    point (Vec2): The point.

Returns:
    bool: True if the rectangle contains the point.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Circle&, const Circle&>(&overlap), "a"_a, "b"_a, R"doc(
Check whether two circles overlap.

Args:
    a (Circle): The first circle.
    b (Circle): The second circle.

Returns:
    bool: True if the circles overlap.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Circle&, const Rect&>(&overlap), "circle"_a, "rect"_a,
        R"doc(
Check whether a circle and a rectangle overlap.

Args:
    circle (Circle): The circle.
    rect (Rect): The rectangle.

Returns:
    bool: True if the circle and rectangle overlap.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Circle&, const Line&>(&overlap), "circle"_a, "line"_a,
        R"doc(
Check whether a circle and a line overlap.

Args:
    circle (Circle): The circle.
    line (Line): The line.

Returns:
    bool: True if the circle and line overlap.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Circle&, const Vec2&>(&overlap), "circle"_a, "point"_a,
        R"doc(
Check whether a circle contains a point.

Args:
    circle (Circle): The circle.
    point (Vec2): The point.

Returns:
    bool: True if the circle contains the point.
                     )doc"
    );
    subCollision
        .def("overlap", nb::overload_cast<const Line&, const Line&>(&overlap), "a"_a, "b"_a, R"doc(
Check whether two lines overlap (intersect).

Args:
    a (Line): The first line.
    b (Line): The second line.

Returns:
    bool: True if the lines intersect.
                     )doc");
    subCollision.def(
        "overlap", nb::overload_cast<const Line&, const Rect&>(&overlap), "line"_a, "rect"_a,
        R"doc(
Check whether a line and a rectangle overlap.

Args:
    line (Line): The line.
    rect (Rect): The rectangle.

Returns:
    bool: True if the line and rectangle overlap.
                    )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Line&, const Circle&>(&overlap), "line"_a, "circle"_a,
        R"doc(
Check whether a line and a circle overlap.

Args:
    line (Line): The line.
    circle (Circle): The circle.

Returns:
    bool: True if the line and circle overlap.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Vec2&, const Rect&>(&overlap), "point"_a, "rect"_a,
        R"doc(
Check whether a point is inside a rectangle.

Args:
    point (Vec2): The point.
    rect (Rect): The rectangle.

Returns:
    bool: True if the point is inside the rectangle.
                     )doc"
    );
    subCollision.def(
        "overlap", nb::overload_cast<const Vec2&, const Circle&>(&overlap), "point"_a, "circle"_a,
        R"doc(
Check whether a point is inside a circle.

Args:
    point (Vec2): The point.
    circle (Circle): The circle.

Returns:
    bool: True if the point is inside the circle.
                     )doc"
    );

    // contains functions
    subCollision.def(
        "contains", nb::overload_cast<const Rect&, const Rect&>(&contains), "outer"_a, "inner"_a,
        R"doc(
Check whether one rectangle completely contains another rectangle.

Args:
    outer (Rect): The outer rectangle.
    inner (Rect): The inner rectangle.

Returns:
    bool: True if the outer rectangle completely contains the inner rectangle.
                     )doc"
    );
    subCollision.def(
        "contains", nb::overload_cast<const Rect&, const Circle&>(&contains), "rect"_a, "circle"_a,
        R"doc(
Check whether a rectangle completely contains a circle.

Args:
    rect (Rect): The rectangle.
    circle (Circle): The circle.

Returns:
    bool: True if the rectangle completely contains the circle.
                     )doc"
    );
    subCollision.def(
        "contains", nb::overload_cast<const Rect&, const Line&>(&contains), "rect"_a, "line"_a,
        R"doc(
Check whether a rectangle completely contains a line.

Args:
    rect (Rect): The rectangle.
    line (Line): The line.

Returns:
    bool: True if the rectangle completely contains the line.
                     )doc"
    );
    subCollision.def(
        "contains", nb::overload_cast<const Circle&, const Circle&>(&contains), "outer"_a,
        "inner"_a, R"doc(
Check whether one circle completely contains another circle.

Args:
    outer (Circle): The outer circle.
    inner (Circle): The inner circle.

Returns:
    bool: True if the outer circle completely contains the inner circle.
                     )doc"
    );
    subCollision.def(
        "contains", nb::overload_cast<const Circle&, const Rect&>(&contains), "circle"_a, "rect"_a,
        R"doc(
Check whether a circle completely contains a rectangle.

Args:
    circle (Circle): The circle.
    rect (Rect): The rectangle.

Returns:
    bool: True if the circle completely contains the rectangle.
                     )doc"
    );
    subCollision.def(
        "contains", nb::overload_cast<const Circle&, const Line&>(&contains), "circle"_a, "line"_a,
        R"doc(
Check whether a circle completely contains a line.

Args:
    circle (Circle): The circle.
    line (Line): The line.

Returns:
    bool: True if the circle completely contains the line.
                     )doc"
    );

    subCollision.def(
        "overlap", nb::overload_cast<const Polygon&, const Vec2&>(&overlap), "polygon"_a, "point"_a,
        R"doc(
Check whether a polygon contains a point.

Args:
    polygon (Polygon): The polygon.
    point (Vec2): The point.

Returns:
    bool: True if the polygon contains the point.
                     )doc"
    );

    subCollision.def(
        "overlap", nb::overload_cast<const Vec2&, const Polygon&>(&overlap), "point"_a, "polygon"_a,
        R"doc(
Check whether a point is inside a polygon.

Args:
    point (Vec2): The point.
    polygon (Polygon): The polygon.

Returns:
    bool: True if the point is inside the polygon.
                     )doc"
    );

    subCollision.def(
        "overlap", nb::overload_cast<const Polygon&, const Rect&>(&overlap), "polygon"_a, "rect"_a,
        R"doc(
Check whether a polygon and a rectangle overlap.

Args:
    polygon (Polygon): The polygon.
    rect (Rect): The rectangle.

Returns:
    bool: True if the polygon and rectangle overlap.
                     )doc"
    );

    subCollision.def(
        "overlap", nb::overload_cast<const Rect&, const Polygon&>(&overlap), "rect"_a, "polygon"_a,
        R"doc(
Check whether a rectangle and a polygon overlap.

Args:
    rect (Rect): The rectangle.
    polygon (Polygon): The polygon.

Returns:
    bool: True if the rectangle and polygon overlap.
                     )doc"
    );
}
}  // namespace kn::collision
