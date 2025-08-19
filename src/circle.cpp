#include "Circle.hpp"
#include "Line.hpp"
#include "Rect.hpp"

#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace circle
{
void _bind(py::module_& module)
{
    py::classh<Circle>(module, "Circle", R"doc(
Represents a circle shape with position and radius.

Supports collision detection with points, rectangles, other circles, and lines.
    )doc")

        .def(py::init<const Vec2&, double>(), py::arg("pos"), py::arg("radius"), R"doc(
Create a circle at a given position and radius.

Args:
    pos (Vec2): Center position of the circle.
    radius (float): Radius of the circle.
        )doc")

        .def(py::init(
                 [](const py::sequence& prSeq) -> Circle
                 {
                     if (prSeq.size() != 2)
                         throw std::invalid_argument("Circle expects a 2-element sequence");

                     if (!py::isinstance<py::sequence>(prSeq[0]))
                         throw std::invalid_argument("Position must be a sequence");
                     if (!py::isinstance<double>(prSeq[1]))
                         throw std::invalid_argument("Radius must be an int or float");

                     py::sequence posSeq = prSeq[0].cast<py::sequence>();
                     double radius = prSeq[1].cast<double>();

                     if (posSeq.size() != 2)
                         throw std::invalid_argument("Position must be a 2-element sequence");

                     return {{posSeq[0].cast<double>(), posSeq[1].cast<double>()}, radius};
                 }),
             R"doc(
Create a circle from a nested sequence: ([x, y], radius).
        )doc")

        .def(
            "__iter__",
            [](const Circle& circle) -> py::iterator
            {
                static double data[3];
                data[0] = circle.pos.x;
                data[1] = circle.pos.y;
                data[2] = circle.radius;
                return py::make_iterator(data, data + 3);
            },
            py::keep_alive<0, 1>(), R"doc(
Return an iterator over (x, y, radius).
        )doc")

        .def(
            "__getitem__",
            [](const Circle& circle, size_t i) -> double
            {
                switch (i)
                {
                case 0:
                    return circle.pos.x;
                case 1:
                    return circle.pos.y;
                case 2:
                    return circle.radius;
                default:
                    throw py::index_error("Index out of range");
                }
            },
            py::arg("index"), R"doc(
Get component by index: 0 = x, 1 = y, 2 = radius.
        )doc")

        .def(
            "__len__", [](const Circle&) -> int { return 3; }, R"doc(
Always returns 3 for (x, y, radius).
        )doc")

        .def("__eq__", &Circle::operator==, py::arg("other"), R"doc(
Check if two circles are equal.
        )doc")

        .def("__ne__", &Circle::operator!=, py::arg("other"), R"doc(
Check if two circles are not equal.
        )doc")

        .def_readwrite("pos", &Circle::pos, R"doc(
The center position of the circle as a Vec2.
        )doc")

        .def_readwrite("radius", &Circle::radius, R"doc(
The radius of the circle.
        )doc")

        .def_property_readonly("area", &Circle::getArea, R"doc(
Return the area of the circle.
        )doc")

        .def_property_readonly("circumference", &Circle::getCircumference, R"doc(
Return the circumference of the circle.
        )doc")

        .def("collide_point", &Circle::collidePoint, py::arg("point"), R"doc(
Check if a point lies inside the circle.

Args:
    point (Vec2): The point to test.
        )doc")

        .def("collide_circle", &Circle::collideCircle, py::arg("circle"), R"doc(
Check collision with another circle.

Args:
    circle (Circle): The circle to test.
        )doc")

        .def("collide_rect", &Circle::collideRect, py::arg("rect"), R"doc(
Check collision with a rectangle.

Args:
    rect (Rect): The rectangle to test.
        )doc")

        .def("collide_line", &Circle::collideLine, py::arg("line"), R"doc(
Check collision with a line.

Args:
    line (Line): The line to test.
        )doc")

        .def(
            "contains",
            [](const Circle& self, const py::object& shapeObject) -> bool
            {
                if (py::isinstance<Vec2>(shapeObject))
                    return self.collidePoint(shapeObject.cast<Vec2>());
                else if (py::isinstance<Circle>(shapeObject))
                    return self.contains(shapeObject.cast<Circle>());
                else if (py::isinstance<Rect>(shapeObject))
                    return self.contains(shapeObject.cast<Rect>());
                else
                    throw std::invalid_argument("Shape must be a Vec2, Circle, or Rect");
            },
            py::arg("shape"), R"doc(
Check if the circle fully contains the given shape.

Args:
    shape (Vec2, Circle, or Rect): The shape to test.
        )doc")

        .def("as_rect", &Circle::asRect, R"doc(
Return the smallest rectangle that fully contains the circle.
        )doc")

        .def("copy", &Circle::copy, R"doc(
Return a copy of the circle.
        )doc");

    py::implicitly_convertible<py::sequence, Circle>();
}

} // namespace circle

Circle::Circle(const Vec2& center, const double radius) : pos(center), radius(radius) {}

double Circle::getArea() const { return M_PI * radius * radius; }

double Circle::getCircumference() const { return 2 * M_PI * radius; }

bool Circle::collidePoint(const Vec2& point) const
{
    const double diff = (point - pos).getLength();
    return diff <= radius;
}

bool Circle::collideCircle(const Circle& circle) const
{
    const double diff = (pos - circle.pos).getLength();
    return diff <= (radius + circle.radius);
}

bool Circle::collideRect(const Rect& rect) const
{
    auto closestPos = math::clampVec(pos, rect.getTopLeft(), rect.getBottomRight());
    return (pos - closestPos).getLength() <= radius;
}

bool Circle::collideLine(const Line& line) const
{
    const Vec2 a = line.getA();
    const Vec2 ab = line.getB() - a;
    const Vec2 ac = pos - a;

    const double abLengthSquared = ab.getLengthSquared();
    if (abLengthSquared == 0.0)
        return ac.getLength() <= radius;

    const double t = std::clamp(math::dot(ac, ab) / abLengthSquared, 0.0, 1.0);
    const Vec2 closestPoint = a + ab * t;
    const double distSquared = (closestPoint - pos).getLengthSquared();

    return distSquared <= (radius * radius);
}

bool Circle::contains(const Circle& circle) const
{
    const double centerDist = (pos - circle.pos).getLength();
    return (centerDist + circle.radius) <= radius;
}

bool Circle::contains(const Rect& rect) const
{
    const Vec2 corners[] = {
        rect.getTopLeft(),
        rect.getTopRight(),
        rect.getBottomLeft(),
        rect.getBottomRight(),
    };

    for (const auto& corner : corners)
    {
        const double dist = (corner - pos).getLength();
        if (dist > radius)
            return false;
    }

    return true;
}

bool Circle::contains(const Line& line) const
{
    const double radiusSquared = radius * radius;

    const double distA = (line.getA() - pos).getLengthSquared();
    const double distB = (line.getB() - pos).getLengthSquared();

    return (distA <= radiusSquared) && (distB <= radiusSquared);
}

Rect Circle::asRect() const
{
    Rect rect;
    rect.setSize({radius * 2});
    rect.setCenter(pos);
    return rect;
}

Circle Circle::copy() const { return {pos, radius}; }

bool Circle::operator==(const Circle& other) const
{
    return pos == other.pos && radius == other.radius;
}

bool Circle::operator!=(const Circle& other) const { return !(*this == other); }
