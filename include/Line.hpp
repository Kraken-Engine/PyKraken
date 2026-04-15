#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Vec2;

class Line
{
  public:
    double ax, ay, bx, by;

    Line();
    Line(double ax, double ay, double bx, double by);
    Line(double ax, double ay, const Vec2& b);
    Line(const Vec2& a, double bx, double by);
    Line(const Vec2& a, const Vec2& b);
    ~Line() = default;

    [[nodiscard]] double getLength() const;

    [[nodiscard]] Vec2 getA() const;
    void setA(const Vec2& pos);

    [[nodiscard]] Vec2 getB() const;
    void setB(const Vec2& pos);

    void move(const Vec2& offset);
    [[nodiscard]] Line moved(const Vec2& offset) const;

    Vec2 getMidpoint() const;

    Line getPerpendicular() const;

    double getAngle() const;

    Vec2 getClosestPoint(const Vec2& point) const;

    [[nodiscard]] Line copy() const;

    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
};

#ifdef KRAKEN_ENABLE_PYTHON
namespace line
{
void _bind(nb::module_& module);
}  // namespace line
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn
