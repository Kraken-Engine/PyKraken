#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include "Math.hpp"

namespace py = pybind11;

namespace kn
{
class Rect
{
  public:
    double x = 0;
    double y = 0;
    double w = 0;
    double h = 0;

    Rect() = default;
    Rect(const Vec2& pos, const Vec2& size);
    template <typename T>
    Rect(T x, T y, T w, T h) : x(x), y(y), w(static_cast<double>(w)), h(static_cast<double>(h))
    {
    }
    template <typename T>
    Rect(const Vec2& pos, T w, T h)
        : x(pos.x), y(pos.y), w(static_cast<double>(w)), h(static_cast<double>(h))
    {
    }
    template <typename T>
    Rect(T x, T y, const Vec2& size)
        : x(x), y(y), w(static_cast<double>(size.x)), h(static_cast<double>(size.y))
    {
    }
    ~Rect() = default;

    [[nodiscard]] Rect copy() const;

    void move(const Vec2& offset);

    void inflate(const Vec2& offset);

    void fit(const Rect& other);

    void clamp(const Vec2& min, const Vec2& max);

    void clamp(const Rect& other);

    void scaleBy(double factor);

    void scaleBy(const Vec2& factor);

    void scaleTo(const Vec2& size);

    bool operator==(const Rect& other) const;
    bool operator!=(const Rect& other) const;

    explicit operator SDL_Rect() const;
    explicit operator SDL_FRect() const;

    void setSize(const Vec2& size);
    void setLeft(double left);
    void setRight(double right);
    void setTop(double top);
    void setBottom(double bottom);
    void setTopLeft(const Vec2& topLeft);
    void setTopMid(const Vec2& topMid);
    void setTopRight(const Vec2& topRight);
    void setMidLeft(const Vec2& midLeft);
    void setCenter(const Vec2& center);
    void setMidRight(const Vec2& midRight);
    void setBottomLeft(const Vec2& bottomLeft);
    void setBottomMid(const Vec2& bottomMid);
    void setBottomRight(const Vec2& bottomRight);

    [[nodiscard]] Vec2 getSize() const;
    [[nodiscard]] double getLeft() const;
    [[nodiscard]] double getRight() const;
    [[nodiscard]] double getTop() const;
    [[nodiscard]] double getBottom() const;
    [[nodiscard]] Vec2 getTopLeft() const;
    [[nodiscard]] Vec2 getTopMid() const;
    [[nodiscard]] Vec2 getTopRight() const;
    [[nodiscard]] Vec2 getMidLeft() const;
    [[nodiscard]] Vec2 getCenter() const;
    [[nodiscard]] Vec2 getMidRight() const;
    [[nodiscard]] Vec2 getBottomLeft() const;
    [[nodiscard]] Vec2 getBottomMid() const;
    [[nodiscard]] Vec2 getBottomRight() const;
};

namespace rect
{
void _bind(py::module_& module);

Rect move(const Rect& rect, const Vec2& offset);
Rect clamp(const Rect& rect, const Vec2& min, const Vec2& max);
Rect clamp(const Rect& rect, const Rect& other);
Rect scaleBy(const Rect& rect, double factor);
Rect scaleBy(const Rect& rect, const Vec2& factor);
Rect scaleTo(const Rect& rect, const Vec2& size);
} // namespace rect
} // namespace kn
