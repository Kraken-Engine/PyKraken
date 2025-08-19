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
    py::classh<PolarCoordinate>(module, "PolarCoordinate", R"doc(
Represents a polar coordinate with angle and radius components.

A polar coordinate system uses an angle (in radians) and radius to define a position
relative to a fixed origin point.
    )doc")
        .def(py::init(), R"doc(
Create a PolarCoordinate with default values (0.0, 0.0).
        )doc")
        .def(py::init<double, double>(), py::arg("angle"), py::arg("radius"), R"doc(
Create a PolarCoordinate from angle and radius.

Args:
    angle (float): The angle in radians.
    radius (float): The radius/distance from origin.
        )doc")
        .def(py::init(
                 [](const py::sequence& s) -> PolarCoordinate
                 {
                     if (s.size() != 2)
                         throw std::runtime_error("PolarCoordinate expects a 2-element sequence");
                     return {s[0].cast<double>(), s[1].cast<double>()};
                 }),
             R"doc(
Create a PolarCoordinate from a sequence of two elements.

Args:
    sequence: A sequence (list, tuple) containing [angle, radius].

Raises:
    RuntimeError: If sequence doesn't contain exactly 2 elements.
        )doc")

        .def("to_cartesian", &PolarCoordinate::toCartesian, R"doc(
Convert polar coordinates to Cartesian coordinates.

Returns:
    Vec2: The equivalent Cartesian coordinates as a Vec2.
        )doc")

        .def("__eq__", &PolarCoordinate::operator==, R"doc(
Check if two PolarCoordinates are equal.

Args:
    other (PolarCoordinate): The other PolarCoordinate to compare.

Returns:
    bool: True if both angle and radius are equal.
        )doc")
        .def("__ne__", &PolarCoordinate::operator!=, R"doc(
Check if two PolarCoordinates are not equal.

Args:
    other (PolarCoordinate): The other PolarCoordinate to compare.

Returns:
    bool: True if angle or radius are different.
        )doc")

        .def_readwrite("angle", &PolarCoordinate::angle, R"doc(
The angle component in radians.
        )doc")
        .def_readwrite("radius", &PolarCoordinate::radius, R"doc(
The radius component (distance from origin).
        )doc")
        .def(
            "__str__", [](const PolarCoordinate& p) -> std::string
            { return "(" + std::to_string(p.angle) + ", " + std::to_string(p.radius) + ")"; },
            R"doc(
Return a human-readable string representation.

Returns:
    str: String in format "(angle, radius)".
        )doc")
        .def(
            "__repr__",
            [](const PolarCoordinate& p) -> std::string
            {
                return "PolarCoordinate(" + std::to_string(p.angle) + ", " +
                       std::to_string(p.radius) + ")";
            },
            R"doc(
Return a string suitable for debugging and recreation.

Returns:
    str: String in format "PolarCoordinate(angle, radius)".
        )doc")
        .def(
            "__iter__", [](const PolarCoordinate& p) -> py::iterator
            { return py::make_iterator(&p.angle, &p.angle + 2); }, py::keep_alive<0, 1>(), R"doc(
Return an iterator over (angle, radius).

Returns:
    iterator: Iterator that yields angle first, then radius.
        )doc")
        .def(
            "__getitem__",
            [](const PolarCoordinate& p, const size_t i) -> double
            {
                if (i == 0)
                    return p.angle;
                else if (i == 1)
                    return p.radius;
                else
                    throw py::index_error("Index out of range");
            },
            py::arg("index"), R"doc(
Access polar coordinate components by index.

Args:
    index (int): Index (0=angle, 1=radius).

Returns:
    float: The component value.

Raises:
    IndexError: If index is not 0 or 1.
        )doc")
        .def(
            "__setitem__",
            [](PolarCoordinate& p, const size_t i, const double value) -> void
            {
                if (i == 0)
                    p.angle = value;
                else if (i == 1)
                    p.radius = value;
                else
                    throw py::index_error("Index out of range");
            },
            py::arg("index"), py::arg("value"), R"doc(
Set polar coordinate components by index.

Args:
    index (int): Index (0=angle, 1=radius).
    value (float): The new value to set.

Raises:
    IndexError: If index is not 0 or 1.
        )doc")
        .def(
            "__len__", [](const PolarCoordinate&) -> int { return 2; }, R"doc(
Return the number of components (always 2).

Returns:
    int: Always returns 2 (angle and radius).
        )doc")
        .def(
            "__hash__",
            [](const PolarCoordinate& p) -> size_t
            {
                size_t ha = std::hash<double>{}(p.angle);
                size_t hr = std::hash<double>{}(p.radius);
                return ha ^ (hr << 1);
            },
            R"doc(
Return a hash value for the PolarCoordinate.

Returns:
    int: Hash value based on angle and radius.
        )doc");
    py::implicitly_convertible<py::sequence, PolarCoordinate>();

    py::classh<Vec2>(module, "Vec2", R"doc(
Represents a 2D vector with x and y components.

Vec2 is used for positions, directions, velocities, and other 2D vector operations.
Supports arithmetic operations, comparisons, and various mathematical functions.
    )doc")
        .def(py::init(), R"doc(
Create a zero vector (0, 0).
        )doc")
        .def(py::init<double>(), py::arg("value"), R"doc(
Create a Vec2 with both x and y set to the same value.

Args:
    value (float): Value to set for both x and y components.
        )doc")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"), R"doc(
Create a Vec2 with given x and y values.

Args:
    x (float): The x component.
    y (float): The y component.
        )doc")
        .def(py::init(
                 [](py::sequence s) -> Vec2
                 {
                     if (s.size() != 2)
                         throw std::runtime_error("Vec2 requires a 2-element sequence");
                     return {s[0].cast<double>(), s[1].cast<double>()};
                 }),
             R"doc(
Create a Vec2 from a sequence of two elements.

Args:
    sequence: A sequence (list, tuple) containing [x, y].

Raises:
    RuntimeError: If sequence doesn't contain exactly 2 elements.
        )doc")

        // Fields
        .def_readwrite("x", &Vec2::x, R"doc(
The x component of the vector.
        )doc")
        .def_readwrite("y", &Vec2::y, R"doc(
The y component of the vector.
        )doc")

        // Magic methods
        .def(
            "__str__", [](const Vec2& v) -> std::string
            { return "<" + std::to_string(v.x) + ", " + std::to_string(v.y) + ">"; }, R"doc(
Return a human-readable string representation.

Returns:
    str: String in format "<x, y>".
        )doc")
        .def(
            "__repr__", [](const Vec2& v) -> std::string
            { return "Vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"; }, R"doc(
Return a string suitable for debugging and recreation.

Returns:
    str: String in format "Vec2(x, y)".
        )doc")
        .def(
            "__iter__", [](const Vec2& v) -> py::iterator
            { return py::make_iterator(&v.x, &v.x + 2); }, py::keep_alive<0, 1>(), R"doc(
Return an iterator over (x, y).

Returns:
    iterator: Iterator that yields x first, then y.
        )doc")
        .def(
            "__getitem__",
            [](const Vec2& v, const size_t i) -> double
            {
                if (i == 0)
                    return v.x;
                else if (i == 1)
                    return v.y;
                else
                    throw py::index_error("Index out of range");
            },
            py::arg("index"), R"doc(
Access vector components by index.

Args:
    index (int): Index (0=x, 1=y).

Returns:
    float: The component value.

Raises:
    IndexError: If index is not 0 or 1.
        )doc")
        .def(
            "__setitem__",
            [](Vec2& v, const size_t i, const double value) -> void
            {
                if (i == 0)
                    v.x = value;
                else if (i == 1)
                    v.y = value;
                else
                    throw py::index_error("Index out of range");
            },
            py::arg("index"), py::arg("value"), R"doc(
Set vector components by index.

Args:
    index (int): Index (0=x, 1=y).
    value (float): The new value to set.

Raises:
    IndexError: If index is not 0 or 1.
        )doc")
        .def(
            "__len__", [](const Vec2&) -> int { return 2; }, R"doc(
Return the number of components (always 2).

Returns:
    int: Always returns 2 (x and y).
        )doc")

        // Arithmetic operators
        .def("__add__", &Vec2::operator+, py::arg("other"), R"doc(
Add another Vec2 to this Vec2.

Args:
    other (Vec2): The Vec2 to add.

Returns:
    Vec2: A new Vec2 with the result of the addition.
        )doc")
        .def("__radd__", &Vec2::operator+, py::arg("other"), R"doc(
Right-hand addition (other + self).

Args:
    other (Vec2): The Vec2 to add.

Returns:
    Vec2: A new Vec2 with the result of the addition.
        )doc")
        .def("__iadd__", &Vec2::operator+=, py::arg("other"), R"doc(
In-place addition (self += other).

Args:
    other (Vec2): The Vec2 to add.

Returns:
    Vec2: Reference to self after modification.
        )doc")

        .def("__sub__", py::overload_cast<const Vec2&>(&Vec2::operator-, py::const_),
             py::arg("other"), R"doc(
Subtract another Vec2 from this Vec2.

Args:
    other (Vec2): The Vec2 to subtract.

Returns:
    Vec2: A new Vec2 with the result of the subtraction.
        )doc")
        .def(
            "__rsub__", [](const Vec2& self, const Vec2& other) -> Vec2 { return other - self; },
            py::arg("other"), R"doc(
Right-hand subtraction (other - self).

Args:
    other (Vec2): The Vec2 to subtract from.

Returns:
    Vec2: A new Vec2 with the result of the subtraction.
        )doc")
        .def("__isub__", &Vec2::operator-=, py::arg("other"), R"doc(
In-place subtraction (self -= other).

Args:
    other (Vec2): The Vec2 to subtract.

Returns:
    Vec2: Reference to self after modification.
        )doc")

        .def("__neg__", py::overload_cast<>(&Vec2::operator-, py::const_), R"doc(
Return the negation of this vector (-self).

Returns:
    Vec2: A new Vec2 with negated x and y components.
        )doc")
        .def(
            "__bool__", [](const Vec2& v) -> bool { return !v.isZero(); }, R"doc(
Check if the vector is not zero.

Returns:
    bool: True if the vector is not zero, False if it is zero.
        )doc")

        .def("__truediv__", &Vec2::operator/, py::arg("scalar"), R"doc(
Divide the vector by a scalar value.

Args:
    scalar (float): The scalar to divide by.

Returns:
    Vec2: A new Vec2 with divided components.
        )doc")
        .def("__itruediv__", &Vec2::operator/=, py::arg("scalar"), R"doc(
In-place division by a scalar value (self /= scalar).

Args:
    scalar (float): The scalar to divide by.

Returns:
    Vec2: Reference to self after modification.
        )doc")

        .def("__mul__", &Vec2::operator*, py::arg("scalar"), R"doc(
Multiply the vector by a scalar value.

Args:
    scalar (float): The scalar to multiply by.

Returns:
    Vec2: A new Vec2 with multiplied components.
        )doc")
        .def("__rmul__", &operator*, py::arg("scalar"), R"doc(
Right-hand multiplication (scalar * self).

Args:
    scalar (float): The scalar to multiply by.

Returns:
    Vec2: A new Vec2 with multiplied components.
        )doc")
        .def("__imul__", &Vec2::operator*=, py::arg("scalar"), R"doc(
In-place multiplication by a scalar value (self *= scalar).

Args:
    scalar (float): The scalar to multiply by.

Returns:
    Vec2: Reference to self after modification.
        )doc")

        .def(
            "__hash__",
            [](const Vec2& v) -> size_t
            {
                std::size_t hx = std::hash<double>{}(v.x);
                std::size_t hy = std::hash<double>{}(v.y);
                return hx ^ (hy << 1);
            },
            R"doc(
Return a hash value for the Vec2.

Returns:
    int: Hash value based on x and y components.
        )doc")

        // Comparison operators
        .def("__eq__", &Vec2::operator==, py::arg("other"), R"doc(
Check if two Vec2s are equal (within tolerance).

Args:
    other (Vec2): The other Vec2 to compare.

Returns:
    bool: True if vectors are equal within tolerance.
        )doc")
        .def("__ne__", &Vec2::operator!=, py::arg("other"), R"doc(
Check if two Vec2s are not equal.

Args:
    other (Vec2): The other Vec2 to compare.

Returns:
    bool: True if vectors are not equal.
        )doc")
        .def("__lt__", &Vec2::operator<, py::arg("other"), R"doc(
Check if this Vec2 is component-wise less than another.

Args:
    other (Vec2): The other Vec2 to compare.

Returns:
    bool: True if both x and y are less than other's x and y.
        )doc")
        .def("__le__", &Vec2::operator<=, py::arg("other"), R"doc(
Check if this Vec2 is component-wise less than or equal to another.

Args:
    other (Vec2): The other Vec2 to compare.

Returns:
    bool: True if not component-wise greater than other.
        )doc")
        .def("__gt__", &Vec2::operator>, py::arg("other"), R"doc(
Check if this Vec2 is component-wise greater than another.

Args:
    other (Vec2): The other Vec2 to compare.

Returns:
    bool: True if both x and y are greater than other's x and y.
        )doc")
        .def("__ge__", &Vec2::operator>=, py::arg("other"), R"doc(
Check if this Vec2 is component-wise greater than or equal to another.

Args:
    other (Vec2): The other Vec2 to compare.

Returns:
    bool: True if not component-wise less than other.
        )doc")

        // Utility methods
        .def_property_readonly("length", &Vec2::getLength, R"doc(
Get the length (magnitude) of the vector.

Returns:
    float: The Euclidean length of the vector.
        )doc")
        .def_property_readonly("angle", &Vec2::getAngle, R"doc(
Get the angle of the vector in radians.

Returns:
    float: The angle from the positive x-axis to this vector.
        )doc")
        .def("rotate", &Vec2::rotate, py::arg("radians"), R"doc(
Rotate the vector by the given angle in radians.

Args:
    radians (float): The angle to rotate by in radians.
        )doc")
        .def("normalize", &Vec2::normalize, R"doc(
Normalize the vector to unit length in-place.

If the vector is zero, it remains unchanged.
        )doc")
        .def("scale_to_length", &Vec2::scaleToLength, py::arg("length"), R"doc(
Scale the vector to the specified length in-place.

Args:
    length (float): The target length.
        )doc")
        .def("distance_to", &Vec2::distanceTo, py::arg("other"), R"doc(
Calculate the distance to another vector.

Args:
    other (Vec2): The other vector.

Returns:
    float: The Euclidean distance between the vectors.
        )doc")
        .def("to_polar", &Vec2::toPolar, R"doc(
Convert to polar coordinates.

Returns:
    PolarCoordinate: A polar coordinate representation (angle, length).
        )doc");
    py::implicitly_convertible<py::sequence, Vec2>();

    auto subMath = module.def_submodule("math", "Math related functions");

    subMath.def("scale_to_length", &scaleToLength, py::arg("vector"), py::arg("length"), R"doc(
Scale a vector to a given length.

Args:
    vector (Vec2): The input vector.
    length (float): The target length.

Returns:
    Vec2: A new vector scaled to the specified length.
        )doc");
    subMath.def("from_polar", py::overload_cast<double, double>(&fromPolar), py::arg("angle"),
                py::arg("radius"), R"doc(
Convert polar coordinates to a Cartesian vector.

Args:
    angle (float): The angle in radians.
    radius (float): The radius/distance from origin.

Returns:
    Vec2: The equivalent Cartesian vector.
        )doc");
    subMath.def("from_polar", py::overload_cast<const PolarCoordinate&>(&fromPolar),
                py::arg("polar"), R"doc(
Convert a PolarCoordinate object to a Cartesian vector.

Args:
    polar (PolarCoordinate): The polar coordinate to convert.

Returns:
    Vec2: The equivalent Cartesian vector.
        )doc");
    subMath.def("normalize", &normalize, py::arg("vec"), R"doc(
Normalize a vector to unit length.

Args:
    vector (Vec2): The input vector.

Returns:
    Vec2: A new normalized vector.
        )doc");
    subMath.def("clamp", &clampVec, py::arg("vec"), py::arg("min_vec"), py::arg("max_vec"),
                R"doc(
Clamp a vector between two boundary vectors.

Args:
    vector (Vec2): The vector to clamp.
    min_vec (Vec2): The minimum boundary vector.
    max_vec (Vec2): The maximum boundary vector.

Returns:
    Vec2: A new vector with components clamped between min and max.
        )doc");
    subMath.def(
        "clamp", [](double value, double min_val, double max_val) -> double
        { return std::clamp(value, min_val, max_val); }, py::arg("value"), py::arg("min_val"),
        py::arg("max_val"), R"doc(
Clamp a value between two boundaries.

Args:
    value (float): The value to clamp.
    min_val (float): The minimum boundary.
    max_val (float): The maximum boundary.

Returns:
    float: The clamped value.
        )doc");
    subMath.def("lerp", py::overload_cast<const Vec2&, const Vec2&, double>(&lerp), py::arg("a"),
                py::arg("b"), py::arg("t"), R"doc(
Linearly interpolate between two Vec2s.

Args:
    a (Vec2): The start vector.
    b (Vec2): The end vector.
    t (float): The interpolation factor [0.0, 1.0].

Returns:
    Vec2: The interpolated vector.
        )doc");
    subMath.def("lerp", py::overload_cast<double, double, double>(&lerp), py::arg("a"),
                py::arg("b"), py::arg("t"), R"doc(
Linearly interpolate between two values.

Args:
    a (float): The start value.
    b (float): The end value.
    t (float): The interpolation factor [0.0, 1.0].

Returns:
    float: The interpolated value.
        )doc");
    subMath.def("remap", &remap, py::arg("in_min"), py::arg("in_max"), py::arg("out_min"),
                py::arg("out_max"), py::arg("value"), R"doc(
Remap a value from one range to another.

Args:
    in_min (float): Input range minimum.
    in_max (float): Input range maximum.
    out_min (float): Output range minimum.
    out_max (float): Output range maximum.
    value (float): The value to remap.

Returns:
    float: The remapped value in the output range.

Raises:
    ValueError: If in_min equals in_max.
        )doc");
    subMath.def("to_deg", &toDegrees, py::arg("radians"), R"doc(
Convert radians to degrees.

Args:
    radians (float): The angle in radians.

Returns:
    float: The angle in degrees.
        )doc");
    subMath.def("to_rad", &toRadians, py::arg("degrees"), R"doc(
Convert degrees to radians.

Args:
    degrees (float): The angle in degrees.

Returns:
    float: The angle in radians.
        )doc");
    subMath.def("dot", &dot, py::arg("a"), py::arg("b"), R"doc(
Calculate the dot product of two vectors.

Args:
    a (Vec2): The first vector.
    b (Vec2): The second vector.

Returns:
    float: The dot product (a.x * b.x + a.y * b.y).
        )doc");
    subMath.def("cross", &cross, py::arg("a"), py::arg("b"), R"doc(
Calculate the 2D cross product of two vectors.

Args:
    a (Vec2): The first vector.
    b (Vec2): The second vector.

Returns:
    float: The 2D cross product (a.x * b.y - a.y * b.x).
        )doc");
    subMath.def("angle_between", &angleBetween, py::arg("a"), py::arg("b"), R"doc(
Calculate the angle between two vectors.

Args:
    a (Vec2): The first vector.
    b (Vec2): The second vector.

Returns:
    float: The angle between the vectors in radians [0, π].
        )doc");
}

Vec2 scaleToLength(const Vec2& vec, double scalar)
{
    if (vec.x == 0.0 && vec.y == 0.0 || scalar == 1.0)
        return vec;

    if (scalar == 0.0)
        return {};

    const double scale = scalar / vec.getLength();
    return {vec.x * scale, vec.y * scale};
}

Vec2 fromPolar(double rad, double radius)
{
    return {radius * std::cos(rad), radius * std::sin(rad)};
}

Vec2 fromPolar(const PolarCoordinate& polar) { return fromPolar(polar.angle, polar.radius); }

Vec2 normalize(Vec2 vec)
{
    vec.normalize();
    return vec;
}

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max)
{
    return {std::clamp(vec.x, min.x, max.x), std::clamp(vec.y, min.y, max.y)};
}

Vec2 lerp(const Vec2& a, const Vec2& b, double t) { return {lerp(a.x, b.x, t), lerp(a.y, b.y, t)}; }

double lerp(double a, double b, double t) { return a + (b - a) * t; }

double remap(double in_min, double in_max, double out_min, double out_max, double value)
{
    if (in_min == in_max)
        throw std::invalid_argument("in_min and in_max must not be equal");
    const double scale = (value - in_min) / (in_max - in_min);

    return out_min + scale * (out_max - out_min);
}

double toDegrees(double angle) { return angle * (180.0 / M_PI); }

double toRadians(double angle) { return angle * (M_PI / 180.0); }

double dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }

double cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }

double angleBetween(const Vec2& a, const Vec2& b)
{
    const double lengths = a.getLength() * b.getLength();
    if (lengths == 0.0)
        return 0.0;

    const double dotProduct = dot(a, b);
    const double cosTheta = dotProduct / lengths;
    return std::acos(std::clamp(cosTheta, -1.0, 1.0));
}
} // namespace math

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

    const double cosTheta = std::cos(rad);
    const double sinTheta = std::sin(rad);
    const double newX = x * cosTheta - y * sinTheta;
    const double newY = x * sinTheta + y * cosTheta;
    x = newX;
    y = newY;
}

PolarCoordinate Vec2::toPolar() const { return {getAngle(), getLength()}; }

void Vec2::scaleToLength(const double scalar)
{
    if (x == 0.0 && y == 0.0 || scalar == 1.0)
        return;

    if (scalar == 0.0)
    {
        x = 0.0;
        y = 0.0;
        return;
    }

    const double scale = scalar / getLength();
    x *= scale;
    y *= scale;
}

Vec2 Vec2::project(const Vec2& other) const
{
    if (x == 0.0 && y == 0.0)
        return {};

    const double lenSq = other.x * other.x + other.y * other.y;
    return other * math::dot(*this, other) / lenSq;
}

Vec2 Vec2::reject(const Vec2& other) const { return *this - project(other); }

Vec2 Vec2::reflect(const Vec2& other) const { return *this - project(other) * 2.0; }

void Vec2::normalize()
{
    if (x == 0.0 && y == 0.0)
        return;

    const double length = getLength();
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
