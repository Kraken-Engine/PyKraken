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
    py::class_<Circle>(module, "Circle")
        .def(py::init())
        .def(py::init(
            [](const py::object& centerObj, double radius) -> Circle*
            {
                math::Vec2 center;

                if (py::isinstance<math::Vec2>(centerObj))
                    center = centerObj.cast<math::Vec2>();
                else if (py::isinstance<py::sequence>(centerObj))
                {
                    const auto centerSeq = centerObj.cast<py::sequence>();
                    if (centerSeq.size() != 2)
                        throw std::invalid_argument("2-element sequence expected");
                    center.x = centerSeq[0].cast<double>();
                    center.y = centerSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("Vec2 or 2-element sequence expected");

                return new Circle(center, radius);
            }))

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
            py::keep_alive<0, 1>())
        .def("__getitem__",
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
             })
        .def("__len__", [](const Circle&) -> int { return 3; })
        .def("__eq__", &Circle::operator==)
        .def("__ne__", &Circle::operator!=)

        .def_readwrite("pos", &Circle::pos)
        .def_readwrite("radius", &Circle::radius)

        .def_property_readonly("area", &Circle::getArea)
        .def_property_readonly("circumference", &Circle::getCircumference)

        .def("collide_point",
             [](const Circle& self, const py::object& pointObj) -> bool
             {
                 math::Vec2 point;

                 if (py::isinstance<math::Vec2>(pointObj))
                     point = pointObj.cast<math::Vec2>();
                 else if (py::isinstance<py::sequence>(pointObj))
                 {
                     const auto pointSeq = pointObj.cast<py::sequence>();
                     if (pointSeq.size() != 2)
                         throw std::invalid_argument("2-element sequence expected");
                     point.x = pointSeq[0].cast<double>();
                     point.y = pointSeq[1].cast<double>();
                 }
                 else
                     throw std::invalid_argument("Vec2 of 2-element sequence expected");

                 return self.collidePoint(point);
             })
        .def("collide_circle", &Circle::collideCircle)
        .def("collide_rect", &Circle::collideRect)
        .def("collide_line", &Circle::collideLine)
        .def("contains",
             [](const Circle& self, const py::object& shapeObject) -> bool
             {
                 if (py::isinstance<math::Vec2>(shapeObject))
                     return self.collidePoint(shapeObject.cast<math::Vec2>());
                 else if (py::isinstance<Circle>(shapeObject))
                     return self.contains(shapeObject.cast<Circle>());
                 else if (py::isinstance<Rect>(shapeObject))
                     return self.contains(shapeObject.cast<Rect>());
                 else
                     throw std::invalid_argument("Shape must be a Vec2, Circle, Rect, or Line");
             })
        .def("as_rect", &Circle::asRect)
        .def("copy", &Circle::copy);
}
} // namespace circle

Circle::Circle(const math::Vec2& center, const double radius) : pos(center), radius(radius) {}

double Circle::getArea() const { return M_PI * radius * radius; }

double Circle::getCircumference() const { return 2 * M_PI * radius; }

bool Circle::collidePoint(const math::Vec2& point) const
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
    const math::Vec2 closestPos = math::clampVec(pos, rect.getTopLeft(), rect.getBottomRight());
    return (pos - closestPos).getLength() <= radius;
}

bool Circle::collideLine(const Line& line) const
{
    const math::Vec2 a = line.getA();
    const math::Vec2 b = line.getA();

    const math::Vec2 ab = b - a;
    const math::Vec2 ac = pos - a;

    const double abLengthSquared = ab.getLengthSquared();
    if (abLengthSquared == 0.0)
        return ac.getLength() <= radius;

    const double t = std::clamp(math::dot(ac, ab) / abLengthSquared, 0.0, 1.0);
    const math::Vec2 closestPoint = a + ab * t;
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
    const math::Vec2 corners[] = {
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

Circle Circle::copy() const { return Circle(pos, radius); }

bool Circle::operator==(const Circle& other) const
{
    return pos == other.pos && radius == other.radius;
}

bool Circle::operator!=(const Circle& other) const { return !(*this == other); }
