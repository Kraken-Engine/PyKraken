#include "Math.hpp"

#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace math
{
void _bind(py::module_& module)
{
    py::class_<PolarCoordinate>(module, "PolarCoordinate")
        .def(py::init())
        .def(py::init(
                 [](const py::sequence& s) -> PolarCoordinate*
                 {
                     if (s.size() != 2)
                         throw std::runtime_error("PolarCoordinate expects a 2-element sequence");
                     return new PolarCoordinate(s[0].cast<double>(), s[1].cast<double>());
                 }),
             "Construct from sequence (angle, radius)")
        .def(py::init<double, double>(), py::arg("angle"), py::arg("radius"),
             "Construct from angle and radius")

        .def("to_cartesian", &PolarCoordinate::toCartesian,
             "Convert to Cartesian coordinates (Vec2)")

        .def("__eq__", &PolarCoordinate::operator==)
        .def("__ne__", &PolarCoordinate::operator!=)

        .def_readwrite("angle", &PolarCoordinate::angle)
        .def_readwrite("radius", &PolarCoordinate::radius)
        .def("__str__", [](const PolarCoordinate& p) -> std::string
             { return "(" + std::to_string(p.angle) + ", " + std::to_string(p.radius) + ")"; })
        .def("__repr__",
             [](const PolarCoordinate& p) -> std::string
             {
                 return "PolarCoordinate(" + std::to_string(p.angle) + ", " +
                        std::to_string(p.radius) + ")";
             })
        .def(
            "__iter__", [](const PolarCoordinate& p) -> py::iterator
            { return py::make_iterator(&p.angle, &p.angle + 2); }, py::keep_alive<0, 1>())
        .def("__getitem__",
             [](const PolarCoordinate& p, const size_t i) -> double
             {
                 if (i == 0)
                     return p.angle;
                 else if (i == 1)
                     return p.radius;
                 else
                     throw py::index_error("Index out of range");
             })
        .def("__setitem__",
             [](PolarCoordinate& p, const size_t i, const double value) -> void
             {
                 if (i == 0)
                     p.angle = value;
                 else if (i == 1)
                     p.radius = value;
                 else
                     throw py::index_error("Index out of range");
             })
        .def("__len__", [](const PolarCoordinate&) -> int { return 2; })
        .def("__hash__",
             [](const PolarCoordinate& p) -> size_t
             {
                 std::size_t ha = std::hash<double>{}(p.angle);
                 std::size_t hr = std::hash<double>{}(p.radius);
                 return ha ^ (hr << 1);
             });
    py::implicitly_convertible<py::sequence, PolarCoordinate>();

    py::class_<Vec2>(module, "Vec2")
        .def(py::init(), "Create a zero vector")
        .def(py::init<double>(), py::arg("value"),
             "Create a Vec2 with both x and y set to the same value")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"),
             "Create a Vec2 with given x and y values")
        .def(py::init(
                 [](py::sequence s) -> Vec2*
                 {
                     if (s.size() != 2)
                         throw std::runtime_error("Vec2 requires a 2-element sequence");
                     return new Vec2(s[0].cast<double>(), s[1].cast<double>());
                 }),
             "Create a Vec2 from a sequence of two elements")

        // Fields
        .def_readwrite("x", &Vec2::x)
        .def_readwrite("y", &Vec2::y)

        // Magic methods
        .def("__str__", [](const Vec2& v) -> std::string
             { return "<" + std::to_string(v.x) + ", " + std::to_string(v.y) + ">"; })
        .def("__repr__", [](const Vec2& v) -> std::string
             { return "Vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"; })
        .def(
            "__iter__", [](const Vec2& v) -> py::iterator
            { return py::make_iterator(&v.x, &v.x + 2); }, py::keep_alive<0, 1>())
        .def("__getitem__",
             [](const Vec2& v, const size_t i) -> double
             {
                 if (i == 0)
                     return v.x;
                 else if (i == 1)
                     return v.y;
                 else
                     throw py::index_error("Index out of range");
             })
        .def("__setitem__",
             [](Vec2& v, const size_t i, const double value) -> void
             {
                 if (i == 0)
                     v.x = value;
                 else if (i == 1)
                     v.y = value;
                 else
                     throw py::index_error("Index out of range");
             })
        .def("__len__", [](const Vec2&) -> int { return 2; })

        // Arithmetic operators
        .def("__add__",
             [](const Vec2& a, const py::object& b) -> Vec2*
             {
                 if (py::isinstance<Vec2>(b))
                     return new Vec2(a + b.cast<Vec2>());
                 if (py::isinstance<py::sequence>(b))
                 {
                     py::sequence seq = b.cast<py::sequence>();
                     if (seq.size() == 2)
                         return new Vec2(a + Vec2(seq[0].cast<double>(), seq[1].cast<double>()));
                 }

                 throw py::type_error(
                     "Vec2 can only be added to another Vec2 or a sequence of 2 numbers");
             })
        .def("__radd__",
             [](const Vec2& b, const py::object& a) -> Vec2*
             {
                 if (py::isinstance<Vec2>(a))
                     return new Vec2(a.cast<Vec2>() + b);
                 if (py::isinstance<py::sequence>(a))
                 {
                     py::sequence seq = a.cast<py::sequence>();
                     if (seq.size() == 2)
                         return new Vec2(Vec2(seq[0].cast<double>(), seq[1].cast<double>()) + b);
                 }

                 throw py::type_error(
                     "Vec2 can only be added to another Vec2 or a sequence of 2 numbers");
             })
        .def("__iadd__",
             [](Vec2& a, const py::object& b) -> Vec2&
             {
                 if (py::isinstance<Vec2>(b))
                 {
                     a += b.cast<Vec2>();
                     return a;
                 }
                 if (py::isinstance<py::sequence>(b))
                 {
                     py::sequence seq = b.cast<py::sequence>();
                     if (seq.size() == 2)
                     {
                         a += Vec2(seq[0].cast<double>(), seq[1].cast<double>());
                         return a;
                     }
                 }

                 throw py::type_error(
                     "Vec2 can only be added to another Vec2 or a sequence of 2 numbers");
             })

        .def("__sub__",
             [](const Vec2& a, const py::object& b) -> Vec2*
             {
                 if (py::isinstance<Vec2>(b))
                     return new Vec2(a - b.cast<Vec2>());
                 if (py::isinstance<py::sequence>(b))
                 {
                     py::sequence seq = b.cast<py::sequence>();
                     if (seq.size() == 2)
                         return new Vec2(a - Vec2(seq[0].cast<double>(), seq[1].cast<double>()));
                 }

                 throw py::type_error(
                     "Vec2 can only be subtracted by another Vec2 or a sequence of 2 numbers");
             })
        .def("__rsub__",
             [](const Vec2& b, const py::object& a) -> Vec2*
             {
                 if (py::isinstance<Vec2>(a))
                     return new Vec2(a.cast<Vec2>() - b);
                 if (py::isinstance<py::sequence>(a))
                 {
                     py::sequence seq = a.cast<py::sequence>();
                     if (seq.size() == 2)
                         return new Vec2(Vec2(seq[0].cast<double>(), seq[1].cast<double>()) - b);
                 }

                 throw py::type_error(
                     "Vec2 can only be subtracted by another Vec2 or a sequence of 2 numbers");
             })
        .def("__isub__",
             [](Vec2& a, const py::object& b) -> Vec2&
             {
                 if (py::isinstance<Vec2>(b))
                 {
                     a -= b.cast<Vec2>();
                     return a;
                 }
                 if (py::isinstance<py::sequence>(b))
                 {
                     py::sequence seq = b.cast<py::sequence>();
                     if (seq.size() == 2)
                     {
                         a -= Vec2(seq[0].cast<double>(), seq[1].cast<double>());
                         return a;
                     }
                 }

                 throw py::type_error(
                     "Vec2 can only be subtracted by another Vec2 or a sequence of 2 numbers");
             })

        .def("__neg__", [](const Vec2& v) -> Vec2* { return new Vec2(-v); })
        .def(
            "__bool__", [](const Vec2& v) -> bool { return !v.isZero(); },
            "Check if the vector is not zero")

        .def("__truediv__", &Vec2::operator/)
        .def("__itruediv__", &Vec2::operator/=)

        .def("__mul__", &Vec2::operator*)
        .def("__rmul__", &operator*)
        .def("__imul__", &Vec2::operator*=)

        .def("__hash__",
             [](const Vec2& v)
             {
                 std::size_t hx = std::hash<double>{}(v.x);
                 std::size_t hy = std::hash<double>{}(v.y);
                 return hx ^ (hy << 1);
             })

        // Comparison operators
        .def("__eq__", &Vec2::operator==)
        .def("__ne__", &Vec2::operator!=)
        .def("__lt__", &Vec2::operator<)
        .def("__le__", &Vec2::operator<=)
        .def("__gt__", &Vec2::operator>)
        .def("__ge__", &Vec2::operator>=)

        // Utility methods
        .def_property_readonly("length", &Vec2::getLength, "Get the length of the vector")
        .def_property_readonly("angle", &Vec2::getAngle, "Get the angle of the vector in radians")
        .def("rotate", &Vec2::rotate, py::arg("radians"))
        .def("normalize", &Vec2::normalize)
        .def("scale_to_length", &Vec2::scaleToLength, py::arg("length"))
        .def("distance_to", &Vec2::distanceTo, py::arg("other"))
        .def("to_polar", &Vec2::toPolar, "Return a polar coordinate pair (angle, length)");
    py::implicitly_convertible<py::sequence, Vec2>();

    auto subMath = module.def_submodule("math", "Math related functions");

    subMath.def("scale_to_length", &scaleToLength, "Scale a vector to a given length");
    subMath.def("from_polar", static_cast<Vec2 (*)(double, double)>(&fromPolar), py::arg("angle"),
                py::arg("radius"), "Convert radians and a radius to a Cartesian vector");
    subMath.def("from_polar", static_cast<Vec2 (*)(const PolarCoordinate&)>(&fromPolar),
                py::arg("polar"), "Convert a PolarCoordinate object to a Cartesian vector");
    subMath.def("normalize", &normalize, "Normalize a vector");
    subMath.def("clamp", &clampVec, "Clamp a vector between two vectors");
    subMath.def(
        "clamp", [](double value, double min_val, double max_val) -> double
        { return std::clamp(value, min_val, max_val); }, "Clamp a value between two values");
    subMath.def("lerp", static_cast<Vec2 (*)(const Vec2&, const Vec2&, double)>(&lerp),
                "Linearly interpolate between two Vec2s");
    subMath.def("lerp", static_cast<double (*)(double, double, double)>(&lerp),
                "Linearly interpolate between two doubles");
    subMath.def("remap", &remap, "Remap a value from one range to another");
    subMath.def("to_degrees", &toDegrees, "Convert radians to degrees");
    subMath.def("to_radians", &toRadians, "Convert degrees to radians");
    subMath.def("dot", &dot, "Calculate the dot product of two vectors");
    subMath.def("cross", &cross, "Calculate the cross product of two vectors");
    subMath.def("angle_between", &angleBetween, "Calculate the angle between two vectors");
}

Vec2 scaleToLength(const Vec2& vec, double scalar)
{
    double length = vec.getLength();
    if (length == 0.0)
        return vec;

    double scale = scalar / length;
    return Vec2(vec.x * scale, vec.y * scale);
}

Vec2 fromPolar(double rad, double radius)
{
    return Vec2(radius * std::cos(rad), radius * std::sin(rad));
}

Vec2 fromPolar(const PolarCoordinate& polar) { return fromPolar(polar.angle, polar.radius); }

Vec2 normalize(const Vec2& vec)
{
    auto newVec = vec;
    newVec.normalize();
    return newVec;
}

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max)
{
    return Vec2(std::clamp(vec.x, min.x, max.x), std::clamp(vec.y, min.y, max.y));
}

Vec2 lerp(const Vec2& a, const Vec2& b, double t)
{
    return Vec2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
}

double lerp(double a, double b, double t) { return a + (b - a) * t; }

double remap(double in_min, double in_max, double out_min, double out_max, double value)
{
    if (in_min == in_max)
        throw std::invalid_argument("in_min and in_max must not be equal");

    double scale = (value - in_min) / (in_max - in_min);
    return out_min + scale * (out_max - out_min);
}

double toDegrees(double angle) { return angle * (180.0 / M_PI); }

double toRadians(double angle) { return angle * (M_PI / 180.0); }

double dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }

double cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }

double angleBetween(const Vec2& a, const Vec2& b)
{
    double lengths = a.getLength() * b.getLength();
    if (lengths == 0.0)
        return 0.0;

    double dotProduct = dot(a, b);
    double cosTheta = dotProduct / lengths;
    return std::acos(std::clamp(cosTheta, -1.0, 1.0));
}

Vec2 PolarCoordinate::toCartesian() const
{
    return {radius * std::cos(angle), radius * std::sin(angle)};
}

bool PolarCoordinate::operator==(const PolarCoordinate& other) const
{
    return angle == other.angle && radius == other.radius;
}

bool PolarCoordinate::operator!=(const PolarCoordinate& other) const { return !(*this == other); }

Vec2 Vec2::copy() const { return {x, y}; }

bool Vec2::isZero(double tolerance) const
{
    return std::abs(x) < tolerance && std::abs(y) < tolerance;
}

double Vec2::getLength() const { return std::hypot(x, y); }

double Vec2::getLengthSquared() const { return x * x + y * y; }

double Vec2::getAngle() const { return std::atan2(y, x); }

void Vec2::rotate(const double rad)
{
    if (isZero())
        return;

    double cosTheta = std::cos(rad);
    double sinTheta = std::sin(rad);
    double newX = x * cosTheta - y * sinTheta;
    double newY = x * sinTheta + y * cosTheta;
    x = newX;
    y = newY;
}

PolarCoordinate Vec2::toPolar() const { return {getAngle(), getLength()}; }

void Vec2::scaleToLength(const double scalar)
{
    const double length = getLength();
    if (length == 0.0)
        return;

    const double scale = scalar / length;
    x *= scale;
    y *= scale;
}

Vec2 Vec2::project(const Vec2& other) const
{
    double lenSq = other.x * other.x + other.y * other.y;
    if (lenSq == 0.0)
        return {};
    return other * (dot(*this, other) / lenSq);
}

Vec2 Vec2::reject(const Vec2& other) const
{
    Vec2 projection = project(other);
    return *this - projection;
}

Vec2 Vec2::reflect(const Vec2& other) const
{
    Vec2 projection = project(other) * 2.0;
    return *this - projection;
}

void Vec2::normalize()
{
    const double length = getLength();
    if (length == 0.0)
        return;

    x /= length;
    y /= length;
}

double Vec2::distanceTo(const Vec2& other) const { return (other - *this).getLength(); }

Vec2 Vec2::operator-() const { return {-x, -y}; }

Vec2 Vec2::operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
Vec2 Vec2::operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
Vec2 Vec2::operator*(double scalar) const { return {x * scalar, y * scalar}; }
Vec2 Vec2::operator/(double scalar) const { return {x / scalar, y / scalar}; }

Vec2& Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}
Vec2& Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}
Vec2& Vec2::operator*=(double scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
Vec2& Vec2::operator/=(double scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vec2 operator*(double lhs, const Vec2& rhs) { return rhs * lhs; }

bool Vec2::operator==(const Vec2& other) const { return (*this - other).isZero(); }
bool Vec2::operator!=(const Vec2& other) const { return !(*this == other); }
bool Vec2::operator<(const Vec2& other) const { return (x < other.x && y < other.y); }
bool Vec2::operator>(const Vec2& other) const { return (x > other.x && y > other.y); }
bool Vec2::operator<=(const Vec2& other) const { return !(*this > other); }
bool Vec2::operator>=(const Vec2& other) const { return !(*this < other); }

Vec2::operator SDL_Point() const { return {static_cast<int>(x), static_cast<int>(y)}; }
Vec2::operator SDL_FPoint() const { return {static_cast<float>(x), static_cast<float>(y)}; }

Vec2::operator py::tuple() const { return py::make_tuple(x, y); }
} // namespace math
