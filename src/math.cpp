#include "Math.hpp"

#include <SDL3/SDL.h>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
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

bool Vec2::isZero(const double tolerance) const
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

double Vec2::distanceSquaredTo(const Vec2& other) const
{
    return (other - *this).getLengthSquared();
}

Vec2 Vec2::operator-() const { return {-x, -y}; }

Vec2 Vec2::operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
Vec2 Vec2::operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
Vec2 Vec2::operator*(const double scalar) const { return {x * scalar, y * scalar}; }
Vec2 Vec2::operator/(const double scalar) const { return {x / scalar, y / scalar}; }

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

Vec2& Vec2::operator*=(const double scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2& Vec2::operator/=(const double scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vec2 operator*(const double lhs, const Vec2& rhs) { return rhs * lhs; }

bool Vec2::operator==(const Vec2& other) const { return (*this - other).isZero(); }
bool Vec2::operator!=(const Vec2& other) const { return !(*this == other); }
bool Vec2::operator<(const Vec2& other) const { return x < other.x && y < other.y; }
bool Vec2::operator>(const Vec2& other) const { return x > other.x && y > other.y; }
bool Vec2::operator<=(const Vec2& other) const { return !(*this > other); }
bool Vec2::operator>=(const Vec2& other) const { return !(*this < other); }

Vec2::operator SDL_Point() const { return {static_cast<int>(x), static_cast<int>(y)}; }
Vec2::operator SDL_FPoint() const { return {static_cast<float>(x), static_cast<float>(y)}; }

namespace math
{
Vec2 scaleToLength(const Vec2& vec, const double scalar)
{
    if (vec.x == 0.0 && vec.y == 0.0 || scalar == 1.0)
        return vec;

    if (scalar == 0.0)
        return {};

    const double scale = scalar / vec.getLength();
    return {vec.x * scale, vec.y * scale};
}

Vec2 fromPolar(const double rad, const double radius)
{
    return {radius * std::cos(rad), radius * std::sin(rad)};
}

Vec2 fromPolar(const PolarCoordinate& polar) { return fromPolar(polar.angle, polar.radius); }

Vec2 normalize(Vec2 vec)
{
    vec.normalize();
    return vec;
}

Vec2 rotate(const Vec2& vec, const double rad)
{
    Vec2 r = vec;
    r.rotate(rad);
    return r;
}

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max)
{
    return {std::clamp(vec.x, min.x, max.x), std::clamp(vec.y, min.y, max.y)};
}

Vec2 lerp(const Vec2& a, const Vec2& b, const double t)
{
    return {lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

double lerp(const double a, const double b, const double t) { return a + (b - a) * t; }

double remap(const double in_min, const double in_max, const double out_min, const double out_max,
             const double value)
{
    if (in_min == in_max)
        throw std::invalid_argument("in_min and in_max must not be equal");
    const double scale = (value - in_min) / (in_max - in_min);

    return out_min + scale * (out_max - out_min);
}

double toDegrees(const double angle) { return angle * (180.0 / M_PI); }

double toRadians(const double angle) { return angle * (M_PI / 180.0); }

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

void _bind(py::module_& module)
{
    auto vec2PyClass = py::classh<Vec2>(module, "Vec2", R"doc(
Vec2 represents a 2D vector.

Attributes:
    x (float): Horizontal component.
    y (float): Vertical component.

Methods:
    copy: Return a duplicated Vec2.
    is_zero: Test whether components are near zero.
    rotate: Rotate the vector in place.
    to_polar: Convert the vector to a PolarCoordinate.
    scale_to_length: Scale the vector to a specific length.
    project: Project onto another Vec2.
    reject: Remove the projection onto another Vec2.
    reflect: Reflect across another Vec2.
    normalize: Normalize the vector in place.
    distance_to: Measure distance to another Vec2.
    distance_squared_to: Measure squared distance to another Vec2.
        )doc");

    // -------------- PolarCoordinate ----------------
    py::classh<PolarCoordinate>(module, "PolarCoordinate", R"doc(
PolarCoordinate models a polar coordinate pair.

Attributes:
    angle (float): Angle in radians.
    radius (float): Distance from origin.

Methods:
    to_cartesian: Convert the coordinate to a Vec2.
        )doc")
        .def(py::init(), R"doc(
Initialize a PolarCoordinate with zero angle and radius.
        )doc")
        .def(py::init<double, double>(), py::arg("angle"), py::arg("radius"), R"doc(
Initialize a PolarCoordinate from explicit values.

Args:
    angle (float): Angle in radians.
    radius (float): Distance from the origin.
        )doc")
        .def(py::init(
                 [](const py::sequence& s) -> PolarCoordinate
                 {
                     if (s.size() != 2)
                         throw std::runtime_error("PolarCoordinate expects a 2-element sequence");
                     return {s[0].cast<double>(), s[1].cast<double>()};
                 }),
             R"doc(
Initialize a PolarCoordinate from a two-item sequence.

Args:
    sequence (Sequence[float]): Iterable containing [angle, radius].

Raises:
    RuntimeError: If the sequence does not contain exactly two elements.
        )doc")

        // Properties
        .def_readwrite("angle", &PolarCoordinate::angle, R"doc(
The angle component in radians.
        )doc")
        .def_readwrite("radius", &PolarCoordinate::radius, R"doc(
The radius component (distance from origin).
        )doc")

        // Methods
        .def("to_cartesian", &PolarCoordinate::toCartesian, R"doc(
Convert this PolarCoordinate to a Vec2.

Returns:
    Vec2: Cartesian representation of this coordinate.
        )doc")

        // Dunder methods
        .def("__eq__", &PolarCoordinate::operator==)
        .def("__ne__", &PolarCoordinate::operator!=)
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
        .def(
            "__getitem__",
            [](const PolarCoordinate& p, const size_t i) -> double
            {
                if (i == 0)
                    return p.angle;
                if (i == 1)
                    return p.radius;
                throw py::index_error("Index out of range");
            },
            py::arg("index"))
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
            py::arg("index"), py::arg("value"))
        .def("__len__", [](const PolarCoordinate&) -> int { return 2; })
        .def("__hash__",
             [](const PolarCoordinate& p) -> size_t
             {
                 const size_t ha = std::hash<double>{}(p.angle);
                 const size_t hr = std::hash<double>{}(p.radius);
                 return ha ^ hr << 1;
             });
    py::implicitly_convertible<py::sequence, PolarCoordinate>();

    // -------------- Vec2 ----------------

    vec2PyClass
        .def(py::init(), R"doc(
Initialize a Vec2 with zeroed components.
        )doc")
        .def(py::init<double>(), py::arg("value"), R"doc(
Initialize a Vec2 with identical x and y values.

Args:
    value (float): Value assigned to both components.
        )doc")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"), R"doc(
Initialize a Vec2 with explicit component values.

Args:
    x (float): Horizontal component.
    y (float): Vertical component.
        )doc")
        .def(py::init(
                 [](const py::sequence& s) -> Vec2
                 {
                     if (s.size() != 2)
                         throw std::runtime_error("Vec2 requires a 2-element sequence");
                     return {s[0].cast<double>(), s[1].cast<double>()};
                 }),
             R"doc(
Initialize a Vec2 from a two-item sequence.

Args:
    sequence (Sequence[float]): Iterable containing [x, y].

Raises:
    RuntimeError: If the sequence does not contain exactly two elements.
        )doc")

        // Properties
        .def_readwrite("x", &Vec2::x, R"doc(
The x component of the vector.
        )doc")
        .def_readwrite("y", &Vec2::y, R"doc(
The y component of the vector.
        )doc")
        .def_property_readonly("length", &Vec2::getLength, R"doc(
Return the magnitude of this Vec2.

Returns:
    float: Euclidean length of the vector.
        )doc")
        .def_property_readonly("length_squared", &Vec2::getLengthSquared, R"doc(
Return the squared magnitude of this Vec2.

Returns:
    float: Squared Euclidean length.
        )doc")
        .def_property_readonly("angle", &Vec2::getAngle, R"doc(
Return the vector angle in radians.

Returns:
    float: Angle measured from the positive x-axis.
        )doc")
        .def_property_readonly(
            "xx", [](const Vec2& self) -> Vec2 { return {self.x, self.x}; },
            R"doc(
Return a Vec2 with both components set to x.

Returns:
    Vec2: Vector composed of (x, x).
        )doc")
        .def_property(
            "xy", [](const Vec2& self) -> Vec2 { return {self.x, self.y}; },
            [](Vec2& self, const double lhs, const double rhs)
            {
                self.x = lhs;
                self.y = rhs;
            },
            R"doc(
Access or assign the (x, y) components as a Vec2.

Returns:
    Vec2: Current (x, y) components.
        )doc")
        .def_property(
            "yx", [](const Vec2& self) -> Vec2 { return {self.y, self.x}; },
            [](Vec2& self, const double lhs, const double rhs)
            {
                self.x = lhs;
                self.y = rhs;
            },
            R"doc(
Access or assign the (y, x) components as a Vec2.

Returns:
    Vec2: Current (y, x) components.
        )doc")
        .def_property_readonly(
            "yy", [](const Vec2& self) -> Vec2 { return {self.y, self.y}; },
            R"doc(
Return a Vec2 with both components set to y.

Returns:
    Vec2: Vector composed of (y, y).
        )doc")

        // Methods
        .def("copy", &Vec2::copy, R"doc(
Return a copy of this Vec2.

Returns:
    Vec2: A duplicated vector with the same components.
        )doc")
        .def("is_zero", &Vec2::isZero, py::arg("tolerance") = 1e-8, R"doc(
Determine whether this Vec2 is effectively zero.

Args:
    tolerance (float): Largest allowed absolute component magnitude.

Returns:
    bool: True if both components are within the tolerance.
        )doc")
        .def("project", &Vec2::project, py::arg("other"), R"doc(
Project this Vec2 onto another Vec2.

Args:
    other (Vec2): The vector to project onto.

Returns:
    Vec2: Projection of this vector onto the other vector.
        )doc")
        .def("reject", &Vec2::reject, py::arg("other"), R"doc(
Compute the rejection of this Vec2 from another Vec2.

Args:
    other (Vec2): The vector defining the projection axis.

Returns:
    Vec2: Component of this vector orthogonal to the other vector.
        )doc")
        .def("reflect", &Vec2::reflect, py::arg("other"), R"doc(
Reflect this Vec2 across another Vec2.

Args:
    other (Vec2): The vector used as the reflection normal.

Returns:
    Vec2: Reflected vector.
        )doc")
        .def("rotate", &Vec2::rotate, py::arg("radians"), R"doc(
Rotate this Vec2 in place.

Args:
    radians (float): Rotation angle in radians.
        )doc")
        .def("normalize", &Vec2::normalize, R"doc(
Normalize this Vec2 in place.
        )doc")
        .def("scale_to_length", &Vec2::scaleToLength, py::arg("length"), R"doc(
Scale this Vec2 to a specific magnitude.

Args:
    length (float): Target vector length.
        )doc")
        .def("distance_to", &Vec2::distanceTo, py::arg("other"), R"doc(
Compute the Euclidean distance to another Vec2.

Args:
    other (Vec2): Comparison vector.

Returns:
    float: Distance between the vectors.
        )doc")
        .def("distance_squared_to", &Vec2::distanceSquaredTo, py::arg("other"), R"doc(
Compute the squared distance to another Vec2.

Args:
    other (Vec2): Comparison vector.

Returns:
    float: Squared distance between the vectors.
        )doc")
        .def("to_polar", &Vec2::toPolar, R"doc(
Convert this Vec2 to polar coordinates.

Returns:
    PolarCoordinate: Polar representation with angle and length.
        )doc")

        // Dunder methods
        .def("__str__", [](const Vec2& v) -> std::string
             { return "<" + std::to_string(v.x) + ", " + std::to_string(v.y) + ">"; })
        .def("__repr__", [](const Vec2& v) -> std::string
             { return "Vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")"; })
        .def(
            "__iter__", [](const Vec2& v) -> py::iterator
            { return py::make_iterator(&v.x, &v.x + 2); }, py::keep_alive<0, 1>())
        .def(
            "__getitem__",
            [](const Vec2& v, const size_t i) -> double
            {
                if (i == 0)
                    return v.x;
                if (i == 1)
                    return v.y;

                throw py::index_error("Index out of range");
            },
            py::arg("index"))
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
            py::arg("index"), py::arg("value"))
        .def("__len__", [](const Vec2&) -> int { return 2; })

        // Arithmetic dunder methods
        .def("__add__", &Vec2::operator+, py::arg("other"))
        .def("__radd__", &Vec2::operator+, py::arg("other"))
        .def("__iadd__", &Vec2::operator+=, py::arg("other"))
        .def("__sub__", py::overload_cast<const Vec2&>(&Vec2::operator-, py::const_),
             py::arg("other"))
        .def(
            "__rsub__", [](const Vec2& self, const Vec2& other) -> Vec2 { return other - self; },
            py::arg("other"))
        .def("__isub__", &Vec2::operator-=, py::arg("other"))
        .def("__neg__", py::overload_cast<>(&Vec2::operator-, py::const_))
        .def("__bool__", [](const Vec2& v) -> bool { return !v.isZero(); })
        .def("__truediv__", &Vec2::operator/, py::arg("scalar"))
        .def("__itruediv__", &Vec2::operator/=, py::arg("scalar"))
        .def("__mul__", &Vec2::operator*, py::arg("scalar"))
        .def(
            "__rmul__", [](const Vec2& self, const double s) { return self * s; },
            py::arg("scalar"))
        .def("__imul__", &Vec2::operator*=, py::arg("scalar"))

        // Hash and comparison dunder methods
        .def("__hash__",
             [](const Vec2& v) -> size_t
             {
                 const std::size_t hx = std::hash<double>{}(v.x);
                 const std::size_t hy = std::hash<double>{}(v.y);
                 return hx ^ hy << 1;
             })
        .def("__eq__", &Vec2::operator==, py::arg("other"))
        .def("__ne__", &Vec2::operator!=, py::arg("other"))
        .def("__lt__", &Vec2::operator<, py::arg("other"))
        .def("__le__", &Vec2::operator<=, py::arg("other"))
        .def("__gt__", &Vec2::operator>, py::arg("other"))
        .def("__ge__", &Vec2::operator>=, py::arg("other"));
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
    vec (Vec2): The input vector.

Returns:
    Vec2: A new normalized vector.
        )doc");

    subMath.def("clamp", &clampVec, py::arg("vec"), py::arg("min_vec"), py::arg("max_vec"),
                R"doc(
Clamp a vector between two boundary vectors.

Args:
    vec (Vec2): The vector to clamp.
    min_vec (Vec2): The minimum boundary vector.
    max_vec (Vec2): The maximum boundary vector.

Returns:
    Vec2: A new vector with components clamped between min and max.
        )doc");

    subMath.def(
        "clamp", [](const double value, const double min_val, const double max_val) -> double
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

    subMath.def("rotate", &rotate, py::arg("vec"), py::arg("angle"), R"doc(
Rotate a vector by an angle.

Args:
    vec (Vec2): The vector to rotate.
    angle (float): Rotation angle in radians.

Returns:
    Vec2: A new rotated vector.
        )doc");
}
} // namespace math
} // namespace kn
