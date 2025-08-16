#pragma once

#include <pybind11/pybind11.h>
#include <SDL3/SDL.h>

class PolarCoordinate;
class Vec2;

namespace py = pybind11;

namespace math
{
void _bind(py::module_& module);

Vec2 scaleToLength(const Vec2& vec, double scalar);

Vec2 fromPolar(double rad, double radius);

Vec2 fromPolar(const PolarCoordinate& polar);

Vec2 normalize(Vec2 vec);

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max);

Vec2 lerp(const Vec2& a, const Vec2& b, double t);

double lerp(double a, double b, double t);

double remap(double in_min, double in_max, double out_min, double out_max, double value);

double toDegrees(double angle);

double toRadians(double angle);

double dot(const Vec2& a, const Vec2& b);

double cross(const Vec2& a, const Vec2& b);

double angleBetween(const Vec2& a, const Vec2& b);
} // namespace math

struct PolarCoordinate
{
    double angle = 0.0;
    double radius = 0.0;

    PolarCoordinate() = default;
    template <typename T>
    PolarCoordinate(T angle, T radius)
        : angle(static_cast<double>(angle)), radius(static_cast<double>(radius))
    {
    }

    Vec2 toCartesian() const;

    bool operator==(const PolarCoordinate& other) const;
    bool operator!=(const PolarCoordinate& other) const;
};

class Vec2
{
  public:
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;
    ~Vec2() = default;

    template <typename T>
    Vec2(T value) : x(static_cast<double>(value)), y(static_cast<double>(value))
    {
    }

    template <typename T> Vec2(T x, T y) : x(static_cast<double>(x)), y(static_cast<double>(y)) {}

    Vec2 copy() const;

    bool isZero(double tolerance = 1e-8) const;

    double getLength() const;

    double getLengthSquared() const;

    double getAngle() const;

    void rotate(double rad);

    PolarCoordinate toPolar() const;

    void scaleToLength(double scalar);

    Vec2 project(const Vec2& other) const;

    Vec2 reject(const Vec2& other) const;

    Vec2 reflect(const Vec2& other) const;

    void normalize();

    double distanceTo(const Vec2& other) const;

    Vec2 operator-() const; // Unary negation

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(double scalar) const;
    Vec2 operator/(double scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(double scalar);
    Vec2& operator/=(double scalar);

    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;
    bool operator<(const Vec2& other) const;
    bool operator>(const Vec2& other) const;
    bool operator<=(const Vec2& other) const;
    bool operator>=(const Vec2& other) const;

    operator SDL_Point() const;
    operator SDL_FPoint() const;

    operator py::tuple() const;
};

Vec2 operator*(double lhs, const Vec2& rhs);
