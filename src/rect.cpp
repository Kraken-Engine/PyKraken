#include "Rect.hpp"
#include "Math.hpp"

namespace rect
{
void _bind(py::module_& module)
{
    py::class_<Rect>(module, "Rect")
        .def(py::init())
        .def(py::init<double, double, double, double>())
        .def(py::init<double, double, const math::Vec2&>())
        .def(py::init<const math::Vec2&, double, double>())
        .def(py::init<const math::Vec2&, const math::Vec2&>())
        .def(py::init(
            [](const py::sequence& s) -> Rect*
            {
                if (s.size() != 4)
                    throw std::runtime_error("Rect((x, y, w, h)) expects a 4-element sequence");
                return new Rect{s[0].cast<double>(), s[1].cast<double>(), s[2].cast<double>(),
                                s[3].cast<double>()};
            }))

        .def_readwrite("x", &Rect::x)
        .def_readwrite("y", &Rect::y)
        .def_readwrite("w", &Rect::w)
        .def_readwrite("h", &Rect::h)

        .def_property("left", &Rect::getLeft, &Rect::setLeft)
        .def_property("right", &Rect::getRight, &Rect::setRight)
        .def_property("top", &Rect::getTop, &Rect::setTop)
        .def_property("bottom", &Rect::getBottom, &Rect::setBottom)
        .def_property(
            "size", [](Rect& self) -> py::tuple { return self.getSize(); }, &Rect::setSize)
        .def_property(
            "top_left", [](Rect& self) -> py::tuple { return self.getTopLeft(); },
            &Rect::setTopLeft)
        .def_property(
            "top_mid", [](Rect& self) -> py::tuple { return self.getTopMid(); }, &Rect::setTopMid)
        .def_property(
            "top_right", [](Rect& self) -> py::tuple { return self.getTopRight(); },
            &Rect::setTopRight)
        .def_property(
            "mid_left", [](Rect& self) -> py::tuple { return self.getMidLeft(); },
            &Rect::setMidLeft)
        .def_property(
            "center", [](Rect& self) -> py::tuple { return self.getCenter(); }, &Rect::setCenter)
        .def_property(
            "mid_right", [](Rect& self) -> py::tuple { return self.getMidRight(); },
            &Rect::setMidRight)
        .def_property(
            "bottom_left", [](Rect& self) -> py::tuple { return self.getBottomLeft(); },
            &Rect::setBottomLeft)
        .def_property(
            "bottom_mid", [](Rect& self) -> py::tuple { return self.getBottomMid(); },
            &Rect::setBottomMid)
        .def_property(
            "bottom_right", [](Rect& self) -> py::tuple { return self.getBottomRight(); },
            &Rect::setBottomRight)

        .def("copy", &Rect::copy)
        .def("move", &Rect::move)
        .def("inflate", &Rect::inflate)
        .def("fit", &Rect::fit)
        .def("contains", &Rect::contains)
        .def("collide_point", &Rect::collidePoint)
        .def("collide_rect", &Rect::collideRect)
        .def("clamp", py::overload_cast<const Rect&>(&Rect::clamp))
        .def("clamp", py::overload_cast<const math::Vec2&, const math::Vec2&>(&Rect::clamp),
             py::arg("min"), py::arg("max"))
        .def("scale_by", py::overload_cast<double>(&Rect::scaleBy))
        .def("scale_by", py::overload_cast<const math::Vec2&>(&Rect::scaleBy))
        .def("scale_to", &Rect::scaleTo)

        .def("__eq__", &Rect::operator==)
        .def("__ne__", &Rect::operator!=)
        .def("__bool__", [](const Rect& rect) -> bool { return rect.w > 0 && rect.h > 0; })
        .def("__str__",
             [](const Rect& rect) -> std::string
             {
                 return "[" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " +
                        std::to_string(rect.w) + ", " + std::to_string(rect.h) + "]";
             })
        .def("__repr__",
             [](const Rect& rect) -> std::string
             {
                 return "Rect(x=" + std::to_string(rect.x) + ", y=" + std::to_string(rect.y) +
                        ", w=" + std::to_string(rect.w) + ", h=" + std::to_string(rect.h) + ")";
             })
        .def(
            "__iter__", [](const Rect& rect) -> py::iterator
            { return py::make_iterator(&rect.x, &rect.x + 4); }, py::keep_alive<0, 1>())
        .def("__len__", [](const Rect& rect) -> int { return 4; })
        .def("__getitem__",
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
             });
    py::implicitly_convertible<py::sequence, Rect>();

    auto subRect = module.def_submodule("rect", "Rectangle related functions");

    subRect.def("move", &move, py::arg("rect"), py::arg("offset"));
    subRect.def("clamp",
                py::overload_cast<const Rect&, const math::Vec2&, const math::Vec2&>(&clamp),
                py::arg("rect"), py::arg("min"), py::arg("max"));
    subRect.def("clamp", py::overload_cast<const Rect&, const Rect&>(&clamp));
    subRect.def("scale_by", py::overload_cast<const Rect&, double>(&scaleBy));
    subRect.def("scale_by", py::overload_cast<const Rect&, const math::Vec2&>(&scaleBy));
    subRect.def("scale_to", &scaleTo);
}

Rect move(const Rect& rect, const math::Vec2& offset)
{
    Rect newRect = rect;
    newRect.x += offset.x;
    newRect.y += offset.y;
    return newRect;
}

Rect clamp(const Rect& rect, const math::Vec2& min, const math::Vec2& max)
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

Rect scaleBy(const Rect& rect, const math::Vec2& factor)
{
    Rect result = rect;
    result.scaleBy(factor);
    return result;
}

Rect scaleTo(const Rect& rect, const math::Vec2& size)
{
    Rect result = rect;
    result.scaleTo(size);
    return result;
}
} // namespace rect

Rect::Rect(const math::Vec2& pos, const math::Vec2& size) : x(pos.x), y(pos.y), w(size.x), h(size.y)
{
}

Rect::Rect(const double x, const double y, const double w, const double h) : x(x), y(y), w(w), h(h)
{
}

Rect::Rect(const math::Vec2& pos, const double w, const double h) : x(pos.x), y(pos.y), w(w), h(h)
{
}

Rect::Rect(const double x, const double y, const math::Vec2& size)
    : x(x), y(y), w(size.x), h(size.y)
{
}

Rect Rect::copy() const { return {x, y, w, h}; }

void Rect::move(const math::Vec2& offset)
{
    x += offset.x;
    y += offset.y;
}

void Rect::inflate(const math::Vec2& offset)
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

bool Rect::collidePoint(const math::Vec2& point) const
{
    const auto px = point.x;
    const auto py = point.y;

    return px >= x && px <= x + w && py >= y && py <= y + h;
}

bool Rect::collideRect(const Rect& other) const
{
    return x < other.x + other.w && x + w > other.x && y < other.y + other.h && y + h > other.y;
}

void Rect::clamp(const math::Vec2& min, const math::Vec2& max)
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

void Rect::scaleBy(const math::Vec2& factor)
{
    const auto scaleX = factor.x;
    const auto scaleY = factor.y;

    if (scaleX <= 0 || scaleY <= 0)
        throw std::invalid_argument("Factor must be greater than 0");

    w *= scaleX;
    h *= scaleY;
}

void Rect::scaleTo(const math::Vec2& size)
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

void Rect::setSize(const math::Vec2& size)
{
    w = size.x;
    h = size.y;
}
void Rect::setLeft(const double x) { this->x = x; }
void Rect::setRight(const double x) { this->x = x - w; }
void Rect::setTop(const double y) { this->y = y; }
void Rect::setBottom(const double y) { this->y = y - h; }
void Rect::setTopLeft(const math::Vec2& pos)
{
    x = pos.x;
    y = pos.y;
}
void Rect::setTopMid(const math::Vec2& pos)
{
    x = pos.x - w / 2.0;
    y = pos.y;
}
void Rect::setTopRight(const math::Vec2& pos)
{
    x = pos.x - w;
    y = pos.y;
}
void Rect::setMidLeft(const math::Vec2& pos)
{
    x = pos.x;
    y = pos.y - h / 2.0;
}
void Rect::setCenter(const math::Vec2& pos)
{
    x = pos.x - w / 2.0;
    y = pos.y - h / 2.0;
}
void Rect::setMidRight(const math::Vec2& pos)
{
    x = pos.x - w;
    y = pos.y - h / 2.0;
}
void Rect::setBottomLeft(const math::Vec2& pos)
{
    x = pos.x;
    y = pos.y - h;
}
void Rect::setBottomMid(const math::Vec2& pos)
{
    x = pos.x - w / 2.0;
    y = pos.y - h;
}
void Rect::setBottomRight(const math::Vec2& pos)
{
    x = pos.x - w;
    y = pos.y - h;
}

math::Vec2 Rect::getSize() const { return {w, h}; }
double Rect::getLeft() const { return x; }
double Rect::getRight() const { return x + w; }
double Rect::getTop() const { return y; }
double Rect::getBottom() const { return y + h; }
math::Vec2 Rect::getTopLeft() const { return {x, y}; }
math::Vec2 Rect::getTopMid() const { return {x + w / 2.0, y}; }
math::Vec2 Rect::getTopRight() const { return {x + w, y}; }
math::Vec2 Rect::getMidLeft() const { return {x, y + h / 2.0}; }
math::Vec2 Rect::getCenter() const { return {x + w / 2.0, y + h / 2.0}; }
math::Vec2 Rect::getMidRight() const { return {x + w, y + h / 2.0}; }
math::Vec2 Rect::getBottomLeft() const { return {x, y + h}; }
math::Vec2 Rect::getBottomMid() const { return {x + w / 2.0, y + h}; }
math::Vec2 Rect::getBottomRight() const { return {x + w, y + h}; }
