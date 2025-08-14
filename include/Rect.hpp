#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Rect;

class Vec2;

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

class Rect
{
  public:
    double x = 0;
    double y = 0;
    double w = 0;
    double h = 0;

    Rect() = default;
    Rect(const Vec2& pos, const Vec2& size);
    Rect(double x, double y, double w, double h);
    Rect(const Vec2& pos, double w, double h);
    Rect(double x, double y, const Vec2& size);

    Rect* copy() const;

    void move(const Vec2& offset);

    void inflate(const Vec2& offset);

    void fit(const Rect& other);

    bool contains(const Rect& other) const;

    bool collidePoint(const Vec2& point) const;

    bool collideRect(const Rect& other) const;

    void clamp(const Vec2& min, const Vec2& max);

    void clamp(const Rect& other);

    void scaleBy(double factor);

    void scaleBy(const Vec2& factor);

    void scaleTo(const Vec2& size);

    bool operator==(const Rect& other) const;
    bool operator!=(const Rect& other) const;

    operator SDL_Rect() const;
    operator SDL_FRect() const;

    void setSize(const Vec2& size);
    void setLeft(double left);
    void setRight(double right);
    void setTop(double top);
    void setBottom(double bottom);
    void setTopLeft(const Vec2& topLeft);
    void setTopMid(const Vec2& topMid);
    void setTopRight(const Vec2& topRight);
    void setMidLeft(const Vec2& midLeft);
    void setCenter(const Vec2& mid);
    void setMidRight(const Vec2& midRight);
    void setBottomLeft(const Vec2& bottomLeft);
    void setBottomMid(const Vec2& bottomMid);
    void setBottomRight(const Vec2& bottomRight);

    Vec2 getSize() const;
    double getLeft() const;
    double getRight() const;
    double getTop() const;
    double getBottom() const;
    Vec2 getTopLeft() const;
    Vec2 getTopMid() const;
    Vec2 getTopRight() const;
    Vec2 getMidLeft() const;
    Vec2 getCenter() const;
    Vec2 getMidRight() const;
    Vec2 getBottomLeft() const;
    Vec2 getBottomMid() const;
    Vec2 getBottomRight() const;
};
