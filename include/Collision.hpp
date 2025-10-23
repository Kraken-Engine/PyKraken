#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Rect;
class Circle;
class Line;
class Vec2;

namespace collision
{
// Any intersection checks
bool overlap(const Rect& a, const Rect& b);
bool overlap(const Rect& rect, const Circle& circle);
bool overlap(const Rect& rect, const Line& line);
bool overlap(const Rect& rect, const Vec2& point);
bool overlap(const Circle& a, const Circle& b);
bool overlap(const Circle& circle, const Line& line);
bool overlap(const Circle& circle, const Vec2& point);
bool overlap(const Line& a, const Line& b);

// Complete containment checks
bool contains(const Rect& outer, const Rect& inner);
bool contains(const Rect& rect, const Circle& circle);
bool contains(const Rect& rect, const Line& line);
bool contains(const Circle& outer, const Circle& inner);
bool contains(const Circle& circle, const Rect& rect);
bool contains(const Circle& circle, const Line& line);

void _bind(py::module_& module);
} // namespace collision
} // namespace kn