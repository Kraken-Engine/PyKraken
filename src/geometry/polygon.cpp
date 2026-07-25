#include "kraken/geometry/Polygon.hpp"

#include <cmath>
#include <limits>

#include "kraken/core/_globals.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/math/Math.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
Polygon::Polygon(const std::vector<Vec2>& points)
    : points(points)
{
}

Polygon::Polygon(const uint32_t n, const double radius, const Vec2& centroid)
{
    if (n == 0)
        return;

    points.reserve(n);
    const double angleStep = 2.0 * M_PI / n;
    for (uint32_t i = 0; i < n; ++i)
    {
        const double angle = i * angleStep;
        points.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }

    setCentroid(centroid);
}

Polygon Polygon::copy() const
{
    return Polygon{points};
}

double Polygon::getPerimeter() const
{
    if (points.size() < 2)
        return 0.0;

    double distance = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        distance += current.distanceTo(next);
    }

    return distance;
}

double Polygon::getArea() const
{
    if (points.size() < 3)
        return 0.0;

    double sum = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        sum += math::cross(current, next);
    }

    return std::abs(sum) * 0.5;
}

Vec2 Polygon::getCentroid() const
{
    if (points.empty())
        return {};
    const size_t n = points.size();

    if (n == 1)
        return points[0];
    if (n == 2)
        return {(points[0].x + points[1].x) * 0.5, (points[0].y + points[1].y) * 0.5};

    Vec2 centroid{0.0, 0.0};
    double signedArea = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % n];
        const double cross = math::cross(current, next);
        signedArea += cross;
        centroid += (current + next) * cross;
    }

    signedArea *= 0.5;
    if (std::abs(signedArea) < 1e-10)
    {
        Vec2 sum{0.0, 0.0};
        for (const auto& point : points)
            sum += point;
        return sum / n;
    }

    centroid /= (6.0 * signedArea);
    return centroid;
}

void Polygon::setCentroid(const Vec2& centroid)
{
    const Vec2 currentCentroid = getCentroid();
    const Vec2 offset = centroid - currentCentroid;
    move(offset);
}

bool Polygon::isConvex() const
{
    if (points.size() < 3)
        return false;

    bool initialized = false;
    bool positive = false;
    const size_t n = points.size();

    for (size_t i = 0; i < n; ++i)
    {
        const Vec2& p1 = points[i];
        const Vec2& p2 = points[(i + 1) % n];
        const Vec2& p3 = points[(i + 2) % n];

        double cp = math::cross(p2 - p1, p3 - p2);
        if (std::abs(cp) > 1e-10)  // Ignore very small cross products
        {
            if (!initialized)
            {
                positive = cp > 0;
                initialized = true;
            }
            else if (positive != (cp > 0))
            {
                return false;
            }
        }
    }

    return true;
}

bool Polygon::isConcave() const
{
    return !isConvex();
}

Rect Polygon::getRect() const
{
    if (points.empty())
        return {};

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& point : points)
    {
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }

    return {minX, minY, maxX - minX, maxY - minY};
}

void Polygon::rotate(double angle)
{
    const Vec2 absPivot = getCentroid();
    for (auto& point : points)
        point = absPivot + (point - absPivot).rotated(angle);
}

Polygon Polygon::rotated(double angle) const
{
    Polygon p = *this;
    p.rotate(angle);
    return p;
}

void Polygon::move(const Vec2& offset)
{
    for (auto& point : points)
        point += offset;
}

Polygon Polygon::moved(const Vec2& offset) const
{
    Polygon p = *this;
    p.move(offset);
    return p;
}

void Polygon::scaleBy(double factor)
{
    const Vec2 absPivot = getCentroid();
    for (auto& point : points)
        point = absPivot + (point - absPivot) * factor;
}

void Polygon::scaleBy(const Vec2& factor)
{
    const Vec2 absPivot = getCentroid();
    for (auto& point : points)
        point = absPivot + (point - absPivot) * factor;
}

Polygon Polygon::scaledBy(double factor) const
{
    Polygon p = *this;
    p.scaleBy(factor);
    return p;
}

Polygon Polygon::scaledBy(const Vec2& factor) const
{
    Polygon p = *this;
    p.scaleBy(factor);
    return p;
}

}  // namespace kn
