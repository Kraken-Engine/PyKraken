#include "Line.hpp"
#include "Math.hpp"

#include <cmath>

namespace line
{
void _bind(py::module_& module)
{
    py::class_<Line>(module, "Line")
        .def(py::init())
        .def(py::init<double, double, double, double>())
        .def(py::init<double, double, const math::Vec2&>())
        .def(py::init<const math::Vec2&, double, double>())
        .def(py::init<const math::Vec2&, const math::Vec2&>())
        .def(py::init(
            [](const py::sequence& abSeq) -> Line*
            {
                if (abSeq.size() != 2)
                    throw std::invalid_argument("Line expects two 2D points");

                if (!py::isinstance<py::sequence>(abSeq[0]))
                    throw std::invalid_argument("A point must be a sequence");
                if (!py::isinstance<py::sequence>(abSeq[1]))
                    throw std::invalid_argument("B point must be a sequence");

                py::sequence aSeq = abSeq[0].cast<py::sequence>();
                py::sequence bSeq = abSeq[1].cast<py::sequence>();

                if (aSeq.size() != 2)
                    throw std::invalid_argument("A point must be a 2-element sequence");
                if (bSeq.size() != 2)
                    throw std::invalid_argument("B point must be a 2-element sequence");

                return new Line(aSeq[0].cast<double>(), aSeq[1].cast<double>(),
                                bSeq[0].cast<double>(), bSeq[1].cast<double>());
            }))

        .def(
            "__iter__", [](const Line& self) -> py::iterator
            { return py::make_iterator(&self.ax, &self.ax + 4); }, py::keep_alive<0, 1>())
        .def("__getitem__",
             [](const Line& self, size_t i) -> double
             {
                 switch (i)
                 {
                 case 0:
                     return self.ax;
                 case 1:
                     return self.ay;
                 case 2:
                     return self.bx;
                 case 3:
                     return self.by;
                 default:
                     throw py::index_error("Index out of range");
                 }
             })
        .def("__len__", [](const Line&) -> int { return 4; })
        .def("__eq__", &Line::operator==)
        .def("__ne__", &Line::operator!=)

        .def_property(
            "a", [](const Line& self) -> py::tuple { return self.getA(); },
            [](Line& self, const py::object& pos) -> void
            {
                if (py::isinstance<math::Vec2>(pos))
                {
                    const auto aVec = pos.cast<math::Vec2>();
                    self.ax = aVec.x;
                    self.ay = aVec.y;
                }
                else if (py::isinstance<py::sequence>(pos))
                {
                    const auto aSeq = pos.cast<py::sequence>();
                    self.ax = aSeq[0].cast<double>();
                    self.ay = aSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("Position must be a Vec2 or 2-element sequence");
            })
        .def_property(
            "b", [](const Line& self) -> py::tuple { return self.getB(); },
            [](Line& self, const py::object& pos) -> void
            {
                if (py::isinstance<math::Vec2>(pos))
                {
                    const auto bVec = pos.cast<math::Vec2>();
                    self.bx = bVec.x;
                    self.by = bVec.y;
                }
                else if (py::isinstance<py::sequence>(pos))
                {
                    const auto bSeq = pos.cast<py::sequence>();
                    self.bx = bSeq[0].cast<double>();
                    self.by = bSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("Position must be a Vec2 or 2-element sequence");
            })
        .def_readwrite("ax", &Line::ax)
        .def_readwrite("ay", &Line::ax)
        .def_readwrite("bx", &Line::ax)
        .def_readwrite("by", &Line::ax)
        .def_property_readonly("length", &Line::getLength)
        .def("copy", &Line::copy)
        .def("move",
             [](Line& self, const py::object& offsetObj) -> void
             {
                 math::Vec2 offsetVec;
                 if (py::isinstance<math::Vec2>(offsetObj))
                     offsetVec = offsetObj.cast<math::Vec2>();
                 else if (py::isinstance<py::sequence>(offsetObj))
                 {
                     const auto offsetSeq = offsetObj.cast<py::sequence>();
                     if (offsetSeq.size() != 2)
                         throw std::invalid_argument("Expected 2-element sequence");
                     offsetVec.x = offsetSeq[0].cast<double>();
                     offsetVec.y = offsetSeq[1].cast<double>();
                 }
                 else
                     throw std::invalid_argument("Expected a Vec2 or 2-element sequence");

                 self.move(offsetVec);
             });
    py::implicitly_convertible<py::sequence, Line>();

    auto subLine = module.def_submodule("line");

    subLine.def("move",
                [](const Line& line, const py::object& offsetObj) -> void
                {
                    math::Vec2 offsetVec;
                    if (py::isinstance<math::Vec2>(offsetObj))
                        offsetVec = offsetObj.cast<math::Vec2>();
                    else if (py::isinstance<py::sequence>(offsetObj))
                    {
                        const auto offsetSeq = offsetObj.cast<py::sequence>();
                        if (offsetSeq.size() != 2)
                            throw std::invalid_argument("Expected 2-element sequence");
                        offsetVec.x = offsetSeq[0].cast<double>();
                        offsetVec.y = offsetSeq[1].cast<double>();
                    }
                    else
                        throw std::invalid_argument("Expected a Vec2 or 2-element sequence");

                    move(line, offsetVec);
                });
}

Line move(const Line& line, const math::Vec2& offset)
{
    return {line.ax + offset.x, line.ay + offset.y, line.bx + offset.x, line.by + offset.y};
}
} // namespace line

Line::Line() : ax(0.0), ay(0.0), bx(0.0), by(0.0) {}

Line::Line(double ax, double ay, double bx, double by) : ax(ax), ay(ay), bx(bx), by(by) {}

Line::Line(double ax, double ay, const math::Vec2& b) : ax(ax), ay(ay), bx(b.x), by(b.y) {}

Line::Line(const math::Vec2& a, double bx, double by) : ax(a.x), ay(a.y), bx(bx), by(by) {}

Line::Line(const math::Vec2& a, const math::Vec2& b) : ax(a.x), ay(a.y), bx(b.x), by(b.y) {}

double Line::getLength() const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    return sqrt(dx * dx + dy * dy);
}

math::Vec2 Line::getA() const { return {ax, ay}; }

math::Vec2 Line::getB() const { return {bx, by}; }

void Line::move(const math::Vec2& offset)
{
    ax += offset.x;
    ay += offset.y;
    bx += offset.x;
    by += offset.y;
}

Line Line::copy() const { return {ax, ay, bx, by}; }

bool Line::operator==(const Line& other) const
{
    return ax == other.ax && ay == other.ay && bx == other.bx && by == other.by;
}

bool Line::operator!=(const Line& other) const { return !(*this == other); }
