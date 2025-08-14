#pragma once

#include <pybind11/pybind11.h>
#include <SDL3/SDL.h>

namespace py = pybind11;

class Vec2;

enum class Anchor;
struct SDL_Renderer;
struct SDL_Texture;
class Color;
class Texture;
class Circle;
class Line;
class Rect;
class Polygon;

namespace renderer
{
void _bind(py::module_& module);

void init(SDL_Window* window, const Vec2& resolution);
void quit();

void clear(const Color& color);
void present();

Vec2 getResolution();

void drawTexture(const Texture& texture, Rect dstRect, const Rect& srcRect);
void drawTexture(const Texture& texture, Vec2 pos, Anchor anchor);

void drawCircle(const Circle& circle, const Color& color, int thickness = 0);

void drawPoint(const Vec2& point, const Color& color);
void drawPoints(const std::vector<Vec2>& points, const Color& color);

void drawLine(const Line& line, const Color& color, int thickness = 1);
// void drawLines(const std::vector<Line>& lines, const Color& color, int thickness = 1);

void drawRect(Rect rect, const Color& color, int thickness = 0);
void drawRects(const std::vector<Rect>& rects, const Color& color, int thickness = 0);

void drawPolygon(const Polygon& polygon, const Color& color, bool filled = false);
// void drawPolygons(const std::vector<Polygon>& polygons, const Color& color, bool filled = false);

SDL_Renderer* get();
} // namespace renderer
