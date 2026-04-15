#pragma once

#include <SDL3/SDL.h>
#include <box2d/box2d.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <ostream>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Vec2;

class PolarCoordinate
{
  public:
    double angle = 0.0;
    double radius = 0.0;

    PolarCoordinate() = default;

    template <typename T>
    PolarCoordinate(T angle, T radius)
        : angle(static_cast<double>(angle)),
          radius(static_cast<double>(radius))
    {
    }

    [[nodiscard]] Vec2 toCartesian() const;

    bool operator==(const PolarCoordinate& other) const;

    bool operator!=(const PolarCoordinate& other) const;
};

class Vec2
{
  public:
    double x = 0.0;
    double y = 0.0;

    const static Vec2 ZERO;
    const static Vec2 LEFT;
    const static Vec2 RIGHT;
    const static Vec2 UP;
    const static Vec2 DOWN;

    Vec2() = default;
    ~Vec2() = default;

    template <typename T>
    explicit Vec2(T value)
        : x(static_cast<double>(value)),
          y(static_cast<double>(value))
    {
    }

    template <typename T>
    Vec2(T x, T y)
        : x(static_cast<double>(x)),
          y(static_cast<double>(y))
    {
    }

    [[nodiscard]] Vec2 copy() const;

    [[nodiscard]] bool isZero(double tolerance = 1e-8) const;

    [[nodiscard]] double getLength() const;

    [[nodiscard]] double getLengthSquared() const;

    [[nodiscard]] double getAngle() const;

    void rotate(double rad);
    Vec2 rotated(double rad) const;

    [[nodiscard]] PolarCoordinate toPolar() const;

    void scaleToLength(double scalar);
    Vec2 scaledToLength(double scalar) const;

    [[nodiscard]] Vec2 project(const Vec2& other) const;

    [[nodiscard]] Vec2 reject(const Vec2& other) const;

    [[nodiscard]] Vec2 reflect(const Vec2& other) const;

    void normalize();
    Vec2 normalized() const;

    [[nodiscard]] double distanceTo(const Vec2& other) const;
    [[nodiscard]] double distanceSquaredTo(const Vec2& other) const;

    void moveToward(const Vec2& target, double maxStep);
    Vec2 movedToward(const Vec2& target, double maxStep) const;

    void floor();
    [[nodiscard]] Vec2 floored() const;

    void ceil();
    [[nodiscard]] Vec2 ceiled() const;

    void round();
    [[nodiscard]] Vec2 rounded() const;

    void slide(const Vec2& normal);
    Vec2 slid(const Vec2& normal) const;

    friend std::ostream& operator<<(std::ostream& os, const Vec2& v)
    {
        return os << "<" << v.x << ", " << v.y << ">";
    }

    Vec2 operator-() const;  // Unary negation

    Vec2 operator+(const Vec2& other) const;

    Vec2 operator-(const Vec2& other) const;

    Vec2 operator*(const Vec2& other) const;

    Vec2 operator*(double scalar) const;

    Vec2 operator/(const Vec2& other) const;

    Vec2 operator/(double scalar) const;

    Vec2& operator+=(const Vec2& other);

    Vec2& operator-=(const Vec2& other);

    Vec2& operator*=(const Vec2& other);

    Vec2& operator*=(double scalar);

    Vec2& operator/=(const Vec2& other);

    Vec2& operator/=(double scalar);

    bool operator==(const Vec2& other) const;

    bool operator!=(const Vec2& other) const;

    explicit operator bool() const;

    explicit operator SDL_Point() const;

    explicit operator SDL_FPoint() const;

    explicit operator b2Vec2() const;
};
}  // namespace kn

namespace mapbox
{
namespace util
{
template <size_t I, typename T>
struct nth;

template <>
struct nth<0, kn::Vec2>
{
    inline static float get(const kn::Vec2& v)
    {
        return static_cast<float>(v.x);
    }
};
template <>
struct nth<1, kn::Vec2>
{
    inline static float get(const kn::Vec2& v)
    {
        return static_cast<float>(v.y);
    }
};
}  // namespace util
}  // namespace mapbox

namespace kn
{
Vec2 operator*(double lhs, const Vec2& rhs);

namespace math
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

Vec2 fromPolar(double rad, double radius);

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max);

Vec2 lerp(const Vec2& a, const Vec2& b, double t);

double lerp(double a, double b, double t);

double remap(double in_min, double in_max, double out_min, double out_max, double value);

double toDegrees(double angle);

double toRadians(double angle);

double dot(const Vec2& a, const Vec2& b);

double cross(const Vec2& a, const Vec2& b);

double angleBetween(const Vec2& a, const Vec2& b);

double moveToward(double current, double target, double maxStep);
}  // namespace math
}  // namespace kn
