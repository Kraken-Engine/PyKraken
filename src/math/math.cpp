#include "kraken/math/Math.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
const Vec2 Vec2::ZERO = {0.0, 0.0};
const Vec2 Vec2::LEFT = {-1.0, 0.0};
const Vec2 Vec2::RIGHT = {1.0, 0.0};
const Vec2 Vec2::UP = {0.0, -1.0};
const Vec2 Vec2::DOWN = {0.0, 1.0};

Vec2 PolarCoordinate::toCartesian() const
{
    return {radius * std::cos(angle), radius * std::sin(angle)};
}

bool PolarCoordinate::operator==(const PolarCoordinate& other) const
{
    return angle == other.angle && radius == other.radius;
}

bool PolarCoordinate::operator!=(const PolarCoordinate& other) const
{
    return !(*this == other);
}

Vec2 Vec2::copy() const
{
    return {x, y};
}

bool Vec2::isZero(const double tolerance) const
{
    return std::abs(x) < tolerance && std::abs(y) < tolerance;
}

double Vec2::getLength() const
{
    return std::hypot(x, y);
}

double Vec2::getLengthSquared() const
{
    return x * x + y * y;
}

double Vec2::getAngle() const
{
    return std::atan2(y, x);
}

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

Vec2 Vec2::rotated(const double rad) const
{
    Vec2 result = *this;
    result.rotate(rad);
    return result;
}

PolarCoordinate Vec2::toPolar() const
{
    return {getAngle(), getLength()};
}

void Vec2::scaleToLength(const double scalar)
{
    if (scalar == 0.0)
    {
        x = 0.0;
        y = 0.0;
        return;
    }

    if (isZero())
        return;  // cannot determine direction for a zero vector

    const double length = getLength();
    if (std::abs(length - scalar) < 1e-12)
        return;

    const double scale = scalar / length;
    x *= scale;
    y *= scale;
}

Vec2 Vec2::scaledToLength(const double scalar) const
{
    Vec2 result = *this;
    result.scaleToLength(scalar);
    return result;
}

Vec2 Vec2::project(const Vec2& other) const
{
    if (other.isZero())
        return {};

    const double lenSq = other.x * other.x + other.y * other.y;
    return other * math::dot(*this, other) / lenSq;
}

Vec2 Vec2::reject(const Vec2& other) const
{
    return *this - project(other);
}

Vec2 Vec2::reflect(const Vec2& other) const
{
    return *this - project(other) * 2.0;
}

void Vec2::normalize()
{
    if (isZero())
        return;

    const double length = getLength();
    x /= length;
    y /= length;
}

Vec2 Vec2::normalized() const
{
    Vec2 result = *this;
    result.normalize();
    return result;
}

double Vec2::distanceTo(const Vec2& other) const
{
    return (other - *this).getLength();
}

double Vec2::distanceSquaredTo(const Vec2& other) const
{
    return (other - *this).getLengthSquared();
}

void Vec2::moveToward(const Vec2& target, const double maxStep)
{
    if (maxStep <= 0.0)
        return;

    const Vec2 diff = target - *this;
    const double dist = diff.getLength();

    if (dist <= maxStep)
    {
        *this = target;
        return;
    }

    *this += diff / dist * maxStep;
}

Vec2 Vec2::movedToward(const Vec2& target, const double maxStep) const
{
    Vec2 result = *this;
    result.moveToward(target, maxStep);
    return result;
}

void Vec2::floor()
{
    x = std::floor(x);
    y = std::floor(y);
}

Vec2 Vec2::floored() const
{
    return {std::floor(x), std::floor(y)};
}

void Vec2::ceil()
{
    x = std::ceil(x);
    y = std::ceil(y);
}

Vec2 Vec2::ceiled() const
{
    return {std::ceil(x), std::ceil(y)};
}

void Vec2::round()
{
    x = std::round(x);
    y = std::round(y);
}

Vec2 Vec2::rounded() const
{
    return {std::round(x), std::round(y)};
}

void Vec2::slide(const Vec2& normal)
{
    *this -= normal * math::dot(*this, normal);
}

Vec2 Vec2::slid(const Vec2& normal) const
{
    return *this - normal * math::dot(*this, normal);
}

Vec2 Vec2::operator-() const
{
    return {-x, -y};
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return {x + other.x, y + other.y};
}
Vec2 Vec2::operator-(const Vec2& other) const
{
    return {x - other.x, y - other.y};
}
Vec2 Vec2::operator*(const double scalar) const
{
    return {x * scalar, y * scalar};
}
Vec2 Vec2::operator/(const double scalar) const
{
    return {x / scalar, y / scalar};
}

Vec2 Vec2::operator/(const Vec2& other) const
{
    return {x / other.x, y / other.y};
}

Vec2 Vec2::operator*(const Vec2& other) const
{
    return {x * other.x, y * other.y};
}

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

Vec2& Vec2::operator*=(const Vec2& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Vec2& Vec2::operator/=(const double scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vec2& Vec2::operator/=(const Vec2& other)
{
    x /= other.x;
    y /= other.y;
    return *this;
}

Vec2 operator*(const double lhs, const Vec2& rhs)
{
    return rhs * lhs;
}

bool Vec2::operator==(const Vec2& other) const
{
    return (*this - other).isZero();
}
bool Vec2::operator!=(const Vec2& other) const
{
    return !(*this == other);
}

Vec2::operator bool() const
{
    return !(x == 0.0 && y == 0.0);
}

Vec2::operator SDL_Point() const
{
    return {static_cast<int>(x), static_cast<int>(y)};
}

Vec2::operator SDL_FPoint() const
{
    return {static_cast<float>(x), static_cast<float>(y)};
}

Vec2::operator b2Vec2() const
{
    return {static_cast<float>(x), static_cast<float>(y)};
}

namespace math
{
Vec2 fromPolar(const double rad, const double radius)
{
    return {radius * std::cos(rad), radius * std::sin(rad)};
}

Vec2 clampVec(const Vec2& vec, const Vec2& min, const Vec2& max)
{
    return {std::clamp(vec.x, min.x, max.x), std::clamp(vec.y, min.y, max.y)};
}

Vec2 lerp(const Vec2& a, const Vec2& b, const double t)
{
    return {lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}

double lerp(const double a, const double b, const double t)
{
    return a + (b - a) * t;
}

double remap(
    const double in_min, const double in_max, const double out_min, const double out_max,
    const double value
)
{
    if (in_min == in_max)
        throw std::invalid_argument("in_min and in_max must not be equal");
    const double scale = (value - in_min) / (in_max - in_min);

    return out_min + scale * (out_max - out_min);
}

double toDegrees(const double angle)
{
    return angle * (180.0 / M_PI);
}

double toRadians(const double angle)
{
    return angle * (M_PI / 180.0);
}

double dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

double cross(const Vec2& a, const Vec2& b)
{
    return a.x * b.y - a.y * b.x;
}

double angleBetween(const Vec2& a, const Vec2& b)
{
    const double lengths = a.getLength() * b.getLength();
    if (lengths == 0.0)
        return 0.0;

    const double dotProduct = dot(a, b);
    const double cosTheta = dotProduct / lengths;
    return std::acos(std::clamp(cosTheta, -1.0, 1.0));
}

double moveToward(const double current, const double target, const double maxStep)
{
    if (maxStep <= 0.0)
        return current;

    const double diff = target - current;
    if (std::abs(diff) <= maxStep)
        return target;

    return current + std::copysign(maxStep, diff);
}

}  // namespace math
}  // namespace kn
