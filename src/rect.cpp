#include "Rect.hpp"
#include "Math.hpp"

namespace rect
{
void _bind(py::module_& module)
{
    py::classh<Rect>(module, "Rect", R"doc(
Represents a rectangle with position and size.

A Rect is defined by its top-left corner position (x, y) and dimensions (w, h).
Supports various geometric operations, collision detection, and positioning methods.
    )doc")
        .def(py::init(), R"doc(
Create a Rect with default values (0, 0, 0, 0).
        )doc")
        .def(py::init<double, double, double, double>(), py::arg("x"), py::arg("y"), py::arg("w"),
             py::arg("h"), R"doc(
Create a Rect with specified position and dimensions.

Args:
    x (float): The x coordinate of the top-left corner.
    y (float): The y coordinate of the top-left corner.
    w (float): The width of the rectangle.
    h (float): The height of the rectangle.
        )doc")
        .def(py::init<double, double, const Vec2&>(), py::arg("x"), py::arg("y"), py::arg("size"),
             R"doc(
Create a Rect with specified position and size vector.

Args:
    x (float): The x coordinate of the top-left corner.
    y (float): The y coordinate of the top-left corner.
    size (Vec2): The size as a Vec2 (width, height).
        )doc")
        .def(py::init<const Vec2&, double, double>(), py::arg("pos"), py::arg("w"), py::arg("h"),
             R"doc(
Create a Rect with specified position vector and dimensions.

Args:
    pos (Vec2): The position as a Vec2 (x, y).
    w (float): The width of the rectangle.
    h (float): The height of the rectangle.
        )doc")
        .def(py::init<const Vec2&, const Vec2&>(), py::arg("pos"), py::arg("size"), R"doc(
Create a Rect with specified position and size vectors.

Args:
    pos (Vec2): The position as a Vec2 (x, y).
    size (Vec2): The size as a Vec2 (width, height).
        )doc")
        .def(py::init(
                 [](const py::sequence& s) -> Rect
                 {
                     if (s.size() != 4)
                         throw std::runtime_error(
                             "Rect((x, y, w, h)) expects a 4-element sequence");

                     return {s[0].cast<double>(), s[1].cast<double>(), s[2].cast<double>(),
                             s[3].cast<double>()};
                 }),
             R"doc(
Create a Rect from a sequence of four elements.

Args:
    sequence: A sequence (list, tuple) containing [x, y, w, h].

Raises:
    RuntimeError: If sequence doesn't contain exactly 4 elements.
        )doc")

        .def_readwrite("x", &Rect::x, R"doc(
The x coordinate of the top-left corner.
        )doc")
        .def_readwrite("y", &Rect::y, R"doc(
The y coordinate of the top-left corner.
        )doc")
        .def_readwrite("w", &Rect::w, R"doc(
The width of the rectangle.
        )doc")
        .def_readwrite("h", &Rect::h, R"doc(
The height of the rectangle.
        )doc")

        .def_property("left", &Rect::getLeft, &Rect::setLeft, R"doc(
The x coordinate of the left edge.
        )doc")
        .def_property("right", &Rect::getRight, &Rect::setRight, R"doc(
The x coordinate of the right edge.
        )doc")
        .def_property("top", &Rect::getTop, &Rect::setTop, R"doc(
The y coordinate of the top edge.
        )doc")
        .def_property("bottom", &Rect::getBottom, &Rect::setBottom, R"doc(
The y coordinate of the bottom edge.
        )doc")
        .def_property("size", &Rect::getSize, &Rect::setSize, R"doc(
The size of the rectangle as (width, height).
        )doc")
        .def_property("top_left", &Rect::getTopLeft, &Rect::setTopLeft, R"doc(
The position of the top-left corner as (x, y).
        )doc")
        .def_property("top_mid", &Rect::getTopMid, &Rect::setTopMid, R"doc(
The position of the top-middle point as (x, y).
        )doc")
        .def_property("top_right", &Rect::getTopRight, &Rect::setTopRight, R"doc(
The position of the top-right corner as (x, y).
        )doc")
        .def_property("mid_left", &Rect::getMidLeft, &Rect::setMidLeft, R"doc(
The position of the middle-left point as (x, y).
        )doc")
        .def_property("center", &Rect::getCenter, &Rect::setCenter,
                      R"doc(
The position of the center point as (x, y).
        )doc")
        .def_property("mid_right", &Rect::getMidRight, &Rect::setMidRight, R"doc(
The position of the middle-right point as (x, y).
        )doc")
        .def_property("bottom_left", &Rect::getBottomLeft, &Rect::setBottomLeft, R"doc(
The position of the bottom-left corner as (x, y).
        )doc")
        .def_property("bottom_mid", &Rect::getBottomMid, &Rect::setBottomMid, R"doc(
The position of the bottom-middle point as (x, y).
        )doc")
        .def_property("bottom_right", &Rect::getBottomRight, &Rect::setBottomRight, R"doc(
The position of the bottom-right corner as (x, y).
        )doc")

        .def("copy", &Rect::copy, R"doc(
Create a copy of this rectangle.

Returns:
    Rect: A new Rect with the same position and size.
        )doc")
        .def("move", &Rect::move, py::arg("offset"), R"doc(
Move the rectangle by the given offset.

Args:
    offset (Vec2): The offset to move by as (dx, dy).
        )doc")
        .def("inflate", &Rect::inflate, py::arg("offset"), R"doc(
Inflate the rectangle by the given offset.

The rectangle grows in all directions. The position is adjusted to keep the center
in the same place.

Args:
    offset (Vec2): The amount to inflate by as (dw, dh).
        )doc")
        .def("fit", &Rect::fit, py::arg("other"), R"doc(
Scale this rectangle to fit inside another rectangle while maintaining aspect ratio.

Args:
    other (Rect): The rectangle to fit inside.

Raises:
    ValueError: If other rectangle has non-positive dimensions.
        )doc")
        .def("contains", &Rect::contains, py::arg("other"), R"doc(
Check if this rectangle completely contains another rectangle.

Args:
    other (Rect): The rectangle to check.

Returns:
    bool: True if this rectangle completely contains the other.
        )doc")
        .def("collide_point", &Rect::collidePoint, py::arg("point"), R"doc(
Check if a point is inside this rectangle.

Args:
    point (Vec2): The point to check.

Returns:
    bool: True if the point is inside this rectangle.
        )doc")
        .def("collide_rect", &Rect::collideRect, py::arg("other"), R"doc(
Check if this rectangle collides with another rectangle.

Args:
    other (Rect): The rectangle to check collision with.

Returns:
    bool: True if the rectangles overlap.
        )doc")
        .def("clamp", py::overload_cast<const Rect&>(&Rect::clamp), py::arg("other"), R"doc(
Clamp this rectangle to be within another rectangle.

Args:
    other (Rect): The rectangle to clamp within.

Raises:
    ValueError: If this rectangle is larger than the clamp area.
        )doc")
        .def("clamp", py::overload_cast<const Vec2&, const Vec2&>(&Rect::clamp), py::arg("min"),
             py::arg("max"), R"doc(
Clamp this rectangle to be within the specified bounds.

Args:
    min (Vec2): The minimum bounds as (min_x, min_y).
    max (Vec2): The maximum bounds as (max_x, max_y).

Raises:
    ValueError: If min >= max or rectangle is larger than the clamp area.
        )doc")
        .def("scale_by", py::overload_cast<double>(&Rect::scaleBy), py::arg("factor"), R"doc(
Scale the rectangle by a uniform factor.

Args:
    factor (float): The scaling factor (must be > 0).

Raises:
    ValueError: If factor is <= 0.
        )doc")
        .def("scale_by", py::overload_cast<const Vec2&>(&Rect::scaleBy), py::arg("factor"), R"doc(
Scale the rectangle by different factors for width and height.

Args:
    factor (Vec2): The scaling factors as (scale_x, scale_y).

Raises:
    ValueError: If any factor is <= 0.
        )doc")
        .def("scale_to", &Rect::scaleTo, py::arg("size"), R"doc(
Scale the rectangle to the specified size.

Args:
    size (Vec2): The new size as (width, height).

Raises:
    ValueError: If width or height is <= 0.
        )doc")

        .def("__eq__", &Rect::operator==, py::arg("other"), R"doc(
Check if two rectangles are equal.

Args:
    other (Rect): The other rectangle to compare.

Returns:
    bool: True if all components (x, y, w, h) are equal.
        )doc")
        .def("__ne__", &Rect::operator!=, py::arg("other"), R"doc(
Check if two rectangles are not equal.

Args:
    other (Rect): The other rectangle to compare.

Returns:
    bool: True if any component differs.
        )doc")
        .def(
            "__bool__", [](const Rect& rect) -> bool { return rect.w > 0 && rect.h > 0; }, R"doc(
Check if the rectangle has positive area.

Returns:
    bool: True if both width and height are greater than 0.
        )doc")
        .def(
            "__str__",
            [](const Rect& rect) -> std::string
            {
                return "[" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " +
                       std::to_string(rect.w) + ", " + std::to_string(rect.h) + "]";
            },
            R"doc(
Return a human-readable string representation.

Returns:
    str: String in format "[x, y, w, h]".
        )doc")
        .def(
            "__repr__",
            [](const Rect& rect) -> std::string
            {
                return "Rect(x=" + std::to_string(rect.x) + ", y=" + std::to_string(rect.y) +
                       ", w=" + std::to_string(rect.w) + ", h=" + std::to_string(rect.h) + ")";
            },
            R"doc(
Return a string suitable for debugging and recreation.

Returns:
    str: String in format "Rect(x=..., y=..., w=..., h=...)".
        )doc")
        .def(
            "__iter__", [](const Rect& rect) -> py::iterator
            { return py::make_iterator(&rect.x, &rect.x + 4); }, py::keep_alive<0, 1>(), R"doc(
Return an iterator over (x, y, w, h).

Returns:
    iterator: Iterator that yields x, y, w, h in order.
        )doc")
        .def(
            "__len__", [](const Rect& rect) -> int { return 4; }, R"doc(
Return the number of components (always 4).

Returns:
    int: Always returns 4 (x, y, w, h).
        )doc")
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
                    throw py::index_error("Index out of range");
                }
            },
            py::arg("index"), R"doc(
Access rectangle components by index.

Args:
    index (int): Index (0=x, 1=y, 2=w, 3=h).

Returns:
    float: The component value.

Raises:
    IndexError: If index is not 0, 1, 2, or 3.
        )doc");
    py::implicitly_convertible<py::sequence, Rect>();

    auto subRect = module.def_submodule("rect", "Rectangle related functions");

    subRect.def("move", &move, py::arg("rect"), py::arg("offset"), R"doc(
Move a rectangle by the given offset.

Args:
    rect (Rect): The rectangle to move.
    offset (Vec2): The offset to move by as (dx, dy).

Returns:
    Rect: A new rectangle moved by the offset.
    )doc");
    subRect.def("clamp", py::overload_cast<const Rect&, const Vec2&, const Vec2&>(&clamp),
                py::arg("rect"), py::arg("min"), py::arg("max"), R"doc(
Clamp a rectangle to be within the specified bounds.

Args:
    rect (Rect): The rectangle to clamp.
    min (Vec2): The minimum bounds as (min_x, min_y).
    max (Vec2): The maximum bounds as (max_x, max_y).

Returns:
    Rect: A new rectangle clamped within the bounds.

Raises:
    ValueError: If min >= max or rectangle is larger than the clamp area.
    )doc");
    subRect.def("clamp", py::overload_cast<const Rect&, const Rect&>(&clamp), py::arg("rect"),
                py::arg("other"), R"doc(
Clamp a rectangle to be within another rectangle.

Args:
    rect (Rect): The rectangle to clamp.
    other (Rect): The rectangle to clamp within.

Returns:
    Rect: A new rectangle clamped within the other rectangle.

Raises:
    ValueError: If rect is larger than the clamp area.
    )doc");
    subRect.def("scale_by", py::overload_cast<const Rect&, double>(&scaleBy), py::arg("rect"),
                py::arg("factor"), R"doc(
Scale a rectangle by a uniform factor.

Args:
    rect (Rect): The rectangle to scale.
    factor (float): The scaling factor (must be > 0).

Returns:
    Rect: A new rectangle scaled by the factor.

Raises:
    ValueError: If factor is <= 0.
    )doc");
    subRect.def("scale_by", py::overload_cast<const Rect&, const Vec2&>(&scaleBy), py::arg("rect"),
                py::arg("factor"), R"doc(
Scale a rectangle by different factors for width and height.

Args:
    rect (Rect): The rectangle to scale.
    factor (Vec2): The scaling factors as (scale_x, scale_y).

Returns:
    Rect: A new rectangle scaled by the factors.

Raises:
    ValueError: If any factor is <= 0.
    )doc");
    subRect.def("scale_to", &scaleTo, py::arg("rect"), py::arg("size"), R"doc(
Scale a rectangle to the specified size.

Args:
    rect (Rect): The rectangle to scale.
    size (Vec2): The new size as (width, height).

Returns:
    Rect: A new rectangle scaled to the specified size.

Raises:
    ValueError: If width or height is <= 0.
    )doc");
}

Rect move(const Rect& rect, const Vec2& offset)
{
    Rect newRect = rect;
    newRect.x += offset.x;
    newRect.y += offset.y;
    return newRect;
}

Rect clamp(const Rect& rect, const Vec2& min, const Vec2& max)
{
    Rect result = rect;
    result.clamp(min, max);
    return result;
}

Rect clamp(const Rect& rect, const Rect& other)
{
    Rect result = rect;
    result.clamp(other);
    return result;
}

Rect scaleBy(const Rect& rect, const double factor)
{
    Rect result = rect;
    result.scaleBy(factor);
    return result;
}

Rect scaleBy(const Rect& rect, const Vec2& factor)
{
    Rect result = rect;
    result.scaleBy(factor);
    return result;
}

Rect scaleTo(const Rect& rect, const Vec2& size)
{
    Rect result = rect;
    result.scaleTo(size);
    return result;
}
} // namespace rect

Rect::Rect(const Vec2& pos, const Vec2& size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {}

Rect Rect::copy() const { return {x, y, w, h}; }

void Rect::move(const Vec2& offset)
{
    x += offset.x;
    y += offset.y;
}

void Rect::inflate(const Vec2& offset)
{
    x -= offset.x / 2.0;
    y -= offset.y / 2.0;
    w = std::max(0.0, w + offset.x);
    h = std::max(0.0, h + offset.y);
}

void Rect::fit(const Rect& other)
{
    if (other.w <= 0 || other.h <= 0)
    {
        throw std::invalid_argument("Other rect must have positive width and height");
        return;
    }

    const double scaleX = other.w / w;
    const double scaleY = other.h / h;
    const double scale = std::min(scaleX, scaleY);
    w *= scale;
    h *= scale;
    x = other.x + (other.w - w) / 2.0;
    y = other.y + (other.h - h) / 2.0;
}

bool Rect::contains(const Rect& other) const
{
    return x <= other.x && y <= other.y && x + w >= other.x + other.w && y + h >= other.y + other.h;
}

bool Rect::collidePoint(const Vec2& point) const
{
    const auto px = point.x;
    const auto py = point.y;

    return px >= x && px <= x + w && py >= y && py <= y + h;
}

bool Rect::collideRect(const Rect& other) const
{
    return x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y;
}

void Rect::clamp(const Vec2& min, const Vec2& max)
{
    const auto minX = min.x;
    const auto minY = min.y;
    const auto maxX = max.x;
    const auto maxY = max.y;

    if (minX > maxX || minY > maxY)
        throw std::invalid_argument("Invalid min/max values: min must be less than max");

    if (w > maxX - minX || h > maxY - minY)
        throw std::invalid_argument("Rect size exceeds the given area");

    x = std::max(minX, std::min(x, maxX));
    y = std::max(minY, std::min(y, maxY));
    w = std::max(0.0, std::min(w, maxX - x));
    h = std::max(0.0, std::min(h, maxY - y));
}

void Rect::clamp(const Rect& other) { clamp(other.getTopLeft(), other.getBottomRight()); }

void Rect::scaleBy(const double factor)
{
    if (factor <= 0)
        throw std::invalid_argument("Factor must be greater than 0");

    w *= factor;
    h *= factor;
}

void Rect::scaleBy(const Vec2& factor)
{
    const auto scaleX = factor.x;
    const auto scaleY = factor.y;

    if (scaleX <= 0 || scaleY <= 0)
        throw std::invalid_argument("Factor must be greater than 0");

    w *= scaleX;
    h *= scaleY;
}

void Rect::scaleTo(const Vec2& size)
{
    const auto width = size.x;
    const auto height = size.y;

    if (width <= 0.0)
        throw std::invalid_argument("Width must be greater than 0");
    if (height <= 0.0)
        throw std::invalid_argument("Height must be greater than 0");

    w = width;
    h = height;
}

bool Rect::operator==(const Rect& other) const
{
    return x == other.x && y == other.y && w == other.w && h == other.h;
}

bool Rect::operator!=(const Rect& other) const { return !(*this == other); }

Rect::operator SDL_Rect() const
{
    return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h)};
}

Rect::operator SDL_FRect() const
{
    return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(w),
            static_cast<float>(h)};
}

void Rect::setSize(const Vec2& size)
{
    w = size.x;
    h = size.y;
}
void Rect::setLeft(const double x) { this->x = x; }
void Rect::setRight(const double x) { this->x = x - w; }
void Rect::setTop(const double y) { this->y = y; }
void Rect::setBottom(const double y) { this->y = y - h; }
void Rect::setTopLeft(const Vec2& pos)
{
    x = pos.x;
    y = pos.y;
}
void Rect::setTopMid(const Vec2& pos)
{
    x = pos.x - w / 2.0;
    y = pos.y;
}
void Rect::setTopRight(const Vec2& pos)
{
    x = pos.x - w;
    y = pos.y;
}
void Rect::setMidLeft(const Vec2& pos)
{
    x = pos.x;
    y = pos.y - h / 2.0;
}
void Rect::setCenter(const Vec2& pos)
{
    x = pos.x - w / 2.0;
    y = pos.y - h / 2.0;
}
void Rect::setMidRight(const Vec2& pos)
{
    x = pos.x - w;
    y = pos.y - h / 2.0;
}
void Rect::setBottomLeft(const Vec2& pos)
{
    x = pos.x;
    y = pos.y - h;
}
void Rect::setBottomMid(const Vec2& pos)
{
    x = pos.x - w / 2.0;
    y = pos.y - h;
}
void Rect::setBottomRight(const Vec2& pos)
{
    x = pos.x - w;
    y = pos.y - h;
}

Vec2 Rect::getSize() const { return {w, h}; }
double Rect::getLeft() const { return x; }
double Rect::getRight() const { return x + w; }
double Rect::getTop() const { return y; }
double Rect::getBottom() const { return y + h; }
Vec2 Rect::getTopLeft() const { return {x, y}; }
Vec2 Rect::getTopMid() const { return {x + w / 2.0, y}; }
Vec2 Rect::getTopRight() const { return {x + w, y}; }
Vec2 Rect::getMidLeft() const { return {x, y + h / 2.0}; }
Vec2 Rect::getCenter() const { return {x + w / 2.0, y + h / 2.0}; }
Vec2 Rect::getMidRight() const { return {x + w, y + h / 2.0}; }
Vec2 Rect::getBottomLeft() const { return {x, y + h}; }
Vec2 Rect::getBottomMid() const { return {x + w / 2.0, y + h}; }
Vec2 Rect::getBottomRight() const { return {x + w, y + h}; }
