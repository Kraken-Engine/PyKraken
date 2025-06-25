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
        .def(py::init(
            [](const py::object& aObj, double bx, double by) -> Line*
            {
                auto line = new Line();
                line->bx = bx;
                line->by = by;

                if (py::isinstance<math::Vec2>(aObj))
                {
                    const auto aVec = aObj.cast<math::Vec2>();
                    line->ax = aVec.x;
                    line->ay = aVec.y;
                }
                else if (py::isinstance<py::sequence>(aObj))
                {
                    const auto aSeq = aObj.cast<py::sequence>();
                    line->ax = aSeq[0].cast<double>();
                    line->ay = aSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("A position must be a Vec2 or 2-element sequence");

                return line;
            }))
        .def(py::init(
            [](double ax, double ay, const py::object& bObj) -> Line*
            {
                auto line = new Line();
                line->ax = ax;
                line->ax = ax;

                if (py::isinstance<math::Vec2>(bObj))
                {
                    const auto bVec = bObj.cast<math::Vec2>();
                    line->bx = bVec.x;
                    line->by = bVec.y;
                }
                else if (py::isinstance<py::sequence>(bObj))
                {
                    const auto bSeq = bObj.cast<py::sequence>();
                    line->bx = bSeq[0].cast<double>();
                    line->by = bSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("B position must be a Vec2 or 2-element sequence");

                return line;
            }))
        .def(py::init(
            [](const py::object& aObj, const py::object& bObj) -> Line*
            {
                auto line = new Line();

                if (py::isinstance<math::Vec2>(aObj))
                {
                    const auto aVec = aObj.cast<math::Vec2>();
                    line->ax = aVec.x;
                    line->ay = aVec.y;
                }
                else if (py::isinstance<py::sequence>(aObj))
                {
                    const auto aSeq = aObj.cast<py::sequence>();
                    line->ax = aSeq[0].cast<double>();
                    line->ay = aSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("A position must be a Vec2 or 2-element sequence");

                if (py::isinstance<math::Vec2>(bObj))
                {
                    const auto bVec = bObj.cast<math::Vec2>();
                    line->bx = bVec.x;
                    line->by = bVec.y;
                }
                else if (py::isinstance<py::sequence>(bObj))
                {
                    const auto bSeq = bObj.cast<py::sequence>();
                    line->bx = bSeq[0].cast<double>();
                    line->by = bSeq[1].cast<double>();
                }
                else
                    throw std::invalid_argument("B position must be a Vec2 or 2-element sequence");

                return line;
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
