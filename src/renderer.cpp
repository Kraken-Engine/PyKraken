#include "Renderer.hpp"
#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "_globals.hpp"

#include <gfx/SDL3_gfxPrimitives.h>
#include <iostream>
#include <pybind11/stl.h>
#include <set>

static SDL_Renderer* _renderer = nullptr;
static SDL_Texture* _target = nullptr;

static void _circleThin(SDL_Renderer* renderer, Vec2 center, int radius, const Color& color);
static void _circle(SDL_Renderer* renderer, Vec2 center, int radius, const Color& color,
                    int thickness);
static void _lineThin(SDL_Renderer* renderer, Vec2 start, Vec2 end, const Color& color);
static void _line(SDL_Renderer* renderer, Vec2 start, Vec2 end, const Color& color, int thickness);
static Uint64 packPoint(int x, int y);

namespace renderer
{
void _bind(pybind11::module_& module)
{
    auto subRenderer = module.def_submodule("renderer", "Function for rendering graphics");

    subRenderer.def("clear", &clear, py::arg("color") = Color{0, 0, 0, 255}, R"doc(
Clear the renderer with the specified color.

Args:
    color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).

Raises:
    ValueError: If color values are not between 0 and 255.
    )doc");

    subRenderer.def("present", &present, R"doc(
Present the rendered content to the screen.

This finalizes the current frame and displays it. Should be called after
all drawing operations for the frame are complete.
    )doc");

    subRenderer.def("get_resolution", &getResolution, R"doc(
Get the resolution of the renderer.

Returns:
    Vec2: The current rendering resolution as (width, height).
    )doc");

    subRenderer.def("draw_point", &drawPoint, py::arg("point"), py::arg("color"), R"doc(
Draw a single point to the renderer.

Args:
    point (Vec2): The position of the point.
    color (Color): The color of the point.

Raises:
    RuntimeError: If point rendering fails.
    )doc");

    subRenderer.def("draw_points", &drawPoints, py::arg("points"), py::arg("color"), R"doc(
Batch draw an array of points to the renderer.

Args:
    points (Sequence[Vec2]): The points to batch draw.
    color (Color): The color of the points.

Raises:
    RuntimeError: If point rendering fails.
    )doc");

    subRenderer.def("draw_texture",
                    py::overload_cast<const Texture&, Rect, const Rect&>(&drawTexture),
                    py::arg("texture"), py::arg("dst_rect"), py::arg("src_rect") = Rect(), R"doc(
Draw a texture with specified destination and source rectangles.

Args:
    texture (Texture): The texture to draw.
    dst_rect (Rect): The destination rectangle on the renderer.
    src_rect (Rect, optional): The source rectangle from the texture. 
                              Defaults to entire texture if not specified.
    )doc");

    subRenderer.def("draw_texture", py::overload_cast<const Texture&, Vec2, Anchor>(&drawTexture),
                    py::arg("texture"), py::arg("pos") = Vec2(), py::arg("anchor") = Anchor::CENTER,
                    R"doc(
Draw a texture at the specified position with anchor alignment.

Args:
    texture (Texture): The texture to draw.
    pos (Vec2, optional): The position to draw at. Defaults to (0, 0).
    anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    )doc");

    subRenderer.def("draw_circle", &drawCircle, py::arg("circle"), py::arg("color"),
                    py::arg("thickness") = 0,
                    R"doc(
Draw a circle to the renderer.

Args:
    circle (Circle): The circle to draw.
    color (Color): The color of the circle.
    thickness (int, optional): The line thickness. If 0 or >= radius, draws filled circle.
                              Defaults to 0 (filled).
    )doc");

    subRenderer.def("draw_line", &drawLine, py::arg("line"), py::arg("color"),
                    py::arg("thickness") = 1, R"doc(
Draw a line to the renderer.

Args:
    line (Line): The line to draw.
    color (Color): The color of the line.
    thickness (int, optional): The line thickness in pixels. Defaults to 1.
    )doc");

    subRenderer.def("draw_rect", &drawRect, py::arg("rect"), py::arg("color"),
                    py::arg("thickness") = 0,
                    R"doc(
Draw a rectangle to the renderer.

Args:
    rect (Rect): The rectangle to draw.
    color (Color): The color of the rectangle.
    thickness (int, optional): The border thickness. If 0 or >= half width/height, draws filled rectangle. Defaults to 0 (filled).
    )doc");

    subRenderer.def("draw_rects", &drawRects, py::arg("rects"), py::arg("color"), py::arg("thickness") = 0, R"doc(
Batch draw an array of rectangles to the renderer.

Args:
    rects (Sequence[Rect]): The rectangles to batch draw.
    color (Color): The color of the rectangles.
    thickness (int, optional): The border thickness of the rectangles. If 0 or >= half width/height, draws filled rectangles. Defaults to 0 (filled).
    )doc");

    subRenderer.def("draw_polygon", &drawPolygon, py::arg("polygon"), py::arg("color"),
                    py::arg("filled") = false,
                    R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline.
                             Defaults to True (filled). Works with both convex and concave polygons.
    )doc");
}

void init(SDL_Window* window, const Vec2& resolution)
{
    _renderer = SDL_CreateRenderer(window, nullptr);
    if (_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    SDL_SetRenderLogicalPresentation(_renderer, resolution.x, resolution.y,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    _target = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
                                resolution.x, resolution.y);
    SDL_SetTextureScaleMode(_target, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderTarget(_renderer, _target);
}

void quit()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
}

void clear(const Color& color)
{
    if (!color._isValid())
        throw std::invalid_argument("Color values must be between 0 and 255");

    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(_renderer);
}

Vec2 getResolution()
{
    int w, h;
    SDL_GetRenderLogicalPresentation(_renderer, &w, &h, nullptr);
    return {w, h};
}

void drawPoint(const Vec2& point, const Color& color)
{
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);

    SDL_FPoint sdlPoint = point - camera::getActivePos();
    if (!SDL_RenderPoint(_renderer, sdlPoint.x, sdlPoint.y))
        throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));
}

void drawPoints(const std::vector<Vec2>& points, const Color& color)
{
    const size_t size = points.size();
    if (size == 0)
        return;

    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(size);

    for (const Vec2& point : points)
        sdlPoints.emplace_back(point - cameraPos);

    if (!SDL_RenderPoints(_renderer, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

void drawTexture(const Texture& texture, Rect dstRect, const Rect& srcRect)
{
    SDL_Texture* sdlTexture = texture.getSDL();

    Vec2 cameraPos = camera::getActivePos();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    dstRect.x -= cameraPos.x;
    dstRect.y -= cameraPos.y;
    SDL_FRect dstSDLRect = dstRect;
    SDL_FRect srcSDLRect = (srcRect.getSize() == Vec2()) ? texture.getRect() : srcRect;

    SDL_RenderTextureRotated(_renderer, sdlTexture, &srcSDLRect, &dstSDLRect, texture.angle,
                             nullptr, flipAxis);
}

void drawTexture(const Texture& texture, Vec2 pos, const Anchor anchor)
{
    SDL_Texture* sdlTexture = texture.getSDL();

    Vec2 cameraPos = camera::getActivePos();

    SDL_FlipMode flipAxis = SDL_FLIP_NONE;
    if (texture.flip.h)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_HORIZONTAL);
    if (texture.flip.v)
        flipAxis = static_cast<SDL_FlipMode>(flipAxis | SDL_FLIP_VERTICAL);

    pos -= cameraPos;
    Rect rect = texture.getRect();
    switch (anchor)
    {
    case Anchor::TOP_LEFT:
        rect.setTopLeft(pos);
        break;
    case Anchor::TOP_MID:
        rect.setTopMid(pos);
        break;
    case Anchor::TOP_RIGHT:
        rect.setTopRight(pos);
        break;
    case Anchor::MID_LEFT:
        rect.setMidLeft(pos);
        break;
    case Anchor::CENTER:
        rect.setCenter(pos);
        break;
    case Anchor::MID_RIGHT:
        rect.setMidRight(pos);
        break;
    case Anchor::BOTTOM_LEFT:
        rect.setBottomLeft(pos);
        break;
    case Anchor::BOTTOM_MID:
        rect.setBottomMid(pos);
        break;
    case Anchor::BOTTOM_RIGHT:
        rect.setBottomRight(pos);
        break;
    }

    SDL_FRect dstSDLRect = rect;
    SDL_RenderTextureRotated(_renderer, sdlTexture, nullptr, &dstSDLRect, texture.angle, nullptr,
                             flipAxis);
}

void drawCircle(const Circle& circle, const Color& color, const int thickness)
{
    if (circle.radius < 1)
        return;

    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);

    if (thickness == 1)
        _circleThin(_renderer, circle.pos - camera::getActivePos(), circle.radius, color);
    else
        _circle(_renderer, circle.pos - camera::getActivePos(), circle.radius, color, thickness);
}

void drawLine(const Line& line, const Color& color, const int thickness)
{
    const Vec2 cameraPos = camera::getActivePos();
    const auto x1 = static_cast<Sint16>(line.ax - cameraPos.x);
    const auto y1 = static_cast<Sint16>(line.ay - cameraPos.y);
    const auto x2 = static_cast<Sint16>(line.bx - cameraPos.x);
    const auto y2 = static_cast<Sint16>(line.by - cameraPos.y);

    if (thickness <= 1)
        lineRGBA(_renderer, x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    else
        thickLineRGBA(_renderer, x1, y1, x2, y2, thickness, color.r, color.g, color.b, color.a);
}

void drawRect(Rect rect, const Color& color, const int thickness)
{
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();
    rect.x -= cameraPos.x;
    rect.y -= cameraPos.y;
    SDL_FRect sdlRect = rect;

    const auto halfWidth = static_cast<int>(rect.w / 2.0);
    const auto halfHeight = static_cast<int>(rect.h / 2.0);
    if (thickness <= 0 || thickness > halfWidth || thickness > halfHeight)
    {
        SDL_RenderFillRect(_renderer, &sdlRect);
        return;
    }

    SDL_RenderRect(_renderer, &sdlRect);
    for (int i = 1; i < thickness; i++)
    {
        rect.inflate({-2, -2});
        sdlRect = rect;
        SDL_RenderRect(_renderer, &sdlRect);
    }
}

void drawRects(const std::vector<Rect>& rects, const Color& color, const int thickness)
{
    if (rects.empty())
        return;

    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();

    // Convert to SDL_FRect array with camera offset
    std::vector<SDL_FRect> sdlRects;
    sdlRects.reserve(rects.size());

    for (const Rect& rect : rects)
    {
        SDL_FRect sdlRect;
        sdlRect.x = rect.x - cameraPos.x;
        sdlRect.y = rect.y - cameraPos.y;
        sdlRect.w = rect.w;
        sdlRect.h = rect.h;
        sdlRects.push_back(sdlRect);
    }

    // For filled rectangles or thick outlines, use batch fill
    if (thickness <= 0)
    {
        SDL_RenderFillRects(_renderer, sdlRects.data(), static_cast<int>(sdlRects.size()));
    }
    else
    {
        // For outlined rectangles, use batch outline
        SDL_RenderRects(_renderer, sdlRects.data(), static_cast<int>(sdlRects.size()));

        // For thick outlines, draw additional nested rectangles
        if (thickness > 1)
        {
            for (int i = 1; i < thickness; i++)
            {
                std::vector<SDL_FRect> innerRects;
                innerRects.reserve(rects.size());

                for (const Rect& rect : rects)
                {
                    const auto halfWidth = static_cast<int>(rect.w / 2.0);
                    const auto halfHeight = static_cast<int>(rect.h / 2.0);

                    // Skip if thickness would exceed rectangle dimensions
                    if (i >= halfWidth || i >= halfHeight)
                        continue;

                    SDL_FRect innerRect;
                    innerRect.x = rect.x - cameraPos.x + i;
                    innerRect.y = rect.y - cameraPos.y + i;
                    innerRect.w = rect.w - (2 * i);
                    innerRect.h = rect.h - (2 * i);
                    innerRects.push_back(innerRect);
                }

                if (!innerRects.empty())
                    SDL_RenderRects(_renderer, innerRects.data(),
                                    static_cast<int>(innerRects.size()));
            }
        }
    }
}

void drawPolygon(const Polygon& polygon, const Color& color, const bool filled)
{
    const size_t size = polygon.points.size();
    if (size == 0)
        return;
    if (size == 1)
    {
        drawPoint(polygon.points.at(0), color);
        return;
    }
    if (size == 2)
    {
        drawLine({polygon.points.at(0), polygon.points.at(1)}, color);
        return;
    }

    const Vec2 cameraPos = camera::getActivePos();

    std::vector<Sint16> vx(size);
    std::vector<Sint16> vy(size);
    for (size_t i = 0; i < size; ++i)
    {
        vx[i] = static_cast<Sint16>(polygon.points.at(i).x - cameraPos.x);
        vy[i] = static_cast<Sint16>(polygon.points.at(i).y - cameraPos.y);
    }

    if (filled)
        filledPolygonRGBA(_renderer, vx.data(), vy.data(), static_cast<int>(size), color.r, color.g,
                          color.b, color.a);
    else
        polygonRGBA(_renderer, vx.data(), vy.data(), static_cast<int>(size), color.r, color.g,
                    color.b, color.a);
}

void present()
{
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_RenderTexture(_renderer, _target, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
    SDL_SetRenderTarget(_renderer, _target);
}

SDL_Renderer* get() { return _renderer; }

} // namespace renderer

void _circleThin(SDL_Renderer* renderer, Vec2 center, const int radius, const Color& color)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    std::set<Uint64> pointSet;

    center -= camera::getActivePos();
    auto emit = [&](int dx, int dy) { pointSet.insert(packPoint(center.x + dx, center.y + dy)); };

    while (x <= y)
    {
        emit(x, y);
        emit(-x, y);
        emit(x, -y);
        emit(-x, -y);
        emit(y, x);
        emit(-y, x);
        emit(y, -x);
        emit(-y, -x);

        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
    }

    // Convert to SDL_FPoint vector
    std::vector<SDL_FPoint> points;
    points.reserve(pointSet.size());
    for (auto packed : pointSet)
    {
        int x = int(int32_t(packed >> 32)) - 32768;
        int y = int(int32_t(packed & 0xFFFFFFFF)) - 32768;
        points.push_back(SDL_FPoint{float(x), float(y)});
    }

    SDL_RenderPoints(renderer, points.data(), static_cast<int>(points.size()));
}

void _circle(SDL_Renderer* renderer, Vec2 center, const int radius, const Color& color,
             const int thickness)
{
    const int innerRadius = (thickness <= 0 || thickness >= radius) ? -1 : radius - thickness;
    center -= camera::getActivePos();

    auto hline = [&](int x1, int y, int x2)
    {
        SDL_RenderLine(renderer, static_cast<float>(x1), static_cast<float>(y),
                       static_cast<float>(x2), static_cast<float>(y));
    };

    auto drawCircleSpan = [&](int r, std::unordered_map<int, std::pair<int, int>>& bounds)
    {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;

        while (x <= y)
        {
            auto update = [&](int y_offset, int xval)
            {
                int yPos = static_cast<int>(center.y) + y_offset;
                int minX = static_cast<int>(center.x) - xval;
                int maxX = static_cast<int>(center.x) + xval;
                bounds[yPos].first = std::min(bounds[yPos].first, minX);
                bounds[yPos].second = std::max(bounds[yPos].second, maxX);
            };

            for (int sign : {-1, 1})
            {
                update(sign * y, x);
                update(sign * x, y);
            }

            if (d < 0)
                d += 4 * x + 6;
            else
            {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    };

    std::unordered_map<int, std::pair<int, int>> outerBounds;
    std::unordered_map<int, std::pair<int, int>> innerBounds;

    // Initialize bounds with max/mins
    for (int i = -radius; i <= radius; ++i)
    {
        outerBounds[static_cast<int>(center.y) + i] = {INT_MAX, INT_MIN};
        innerBounds[static_cast<int>(center.y) + i] = {INT_MAX, INT_MIN};
    }

    drawCircleSpan(radius, outerBounds);
    drawCircleSpan(innerRadius, innerBounds);

    for (const auto& [y, outer] : outerBounds)
    {
        const auto& inner = innerBounds[y];

        if (inner.first == INT_MAX || inner.second == INT_MIN)
            // No inner circle on this line → full span
            hline(outer.first, y, outer.second);
        else
        {
            // Left ring
            if (outer.first < inner.first)
                hline(outer.first, y, inner.first - 1);
            // Right ring
            if (outer.second > inner.second)
                hline(inner.second + 1, y, outer.second);
        }
    }
}

void _lineThin(SDL_Renderer* renderer, Vec2 start, Vec2 end, const Color& color)
{
    start -= camera::getActivePos();
    end -= camera::getActivePos();
    auto x1 = static_cast<int>(start.x);
    auto y1 = static_cast<int>(start.y);
    const auto x2 = static_cast<int>(end.x);
    const auto y2 = static_cast<int>(end.y);

    if (y1 == y2 || x1 == x2)
    {
        SDL_RenderLine(renderer, x1, y1, x2, y2);
        return;
    }

    const int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    const int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (true)
    {
        SDL_RenderPoint(renderer, x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void _line(SDL_Renderer* renderer, Vec2 start, Vec2 end, const Color& color, const int thickness)
{
    start -= camera::getActivePos();
    end -= camera::getActivePos();
    auto x1 = static_cast<int>(start.x);
    auto y1 = static_cast<int>(start.y);
    const auto x2 = static_cast<int>(end.x);
    const auto y2 = static_cast<int>(end.y);

    const int extraWidth = 1 - (thickness % 2);
    const int halfWidth = thickness / 2;

    if (y1 == y2)
    {
        for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
            SDL_RenderLine(renderer, x1, y1 + i, x2, y2 + i);
        return;
    }

    if (x1 == x2)
    {
        for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
            SDL_RenderLine(renderer, x1 + i, y1, x2 + i, y2);
        return;
    }

    const int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    const int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    const bool xinc = (dx <= dy); // Direction of thickness

    while (true)
    {
        if (xinc)
            // Thickness is horizontal, draw horizontal segment
            for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
                SDL_RenderPoint(renderer, x1 + i, y1);
        else
            // Thickness is vertical, draw vertical segment
            for (int i = -halfWidth + extraWidth; i <= halfWidth; i++)
                SDL_RenderPoint(renderer, x1, y1 + i);

        if (x1 == x2 && y1 == y2)
            break;

        const int e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y1 += sy;
        }
    }
}

Uint64 packPoint(const int x, const int y)
{
    return (uint64_t(uint32_t(x + 32768)) << 32) | uint32_t(y + 32768);
}
