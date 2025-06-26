#include "Renderer.hpp"
#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>
#include <iostream>
#include <set>

static void _circleThin(SDL_Renderer* renderer, math::Vec2 center, int radius, const Color& color);
static void _circle(SDL_Renderer* renderer, math::Vec2 center, int radius, const Color& color,
                    int thickness);
static void _lineThin(SDL_Renderer* renderer, math::Vec2 start, math::Vec2 end, const Color& color);
static void _line(SDL_Renderer* renderer, math::Vec2 start, math::Vec2 end, const Color& color,
                  int thickness);
static Uint64 packPoint(int x, int y);

namespace renderer
{

void _bind(pybind11::module_& module)
{
    py::class_<Renderer>(module, "Renderer")
        .def(py::init<const math::Vec2&>(), py::arg("resolution"),
             "Create a Renderer with the specified resolution")
        .def("to_view", &Renderer::toView, py::arg("coordinate"))
        .def("clear", &Renderer::clear, py::arg("color") = py::cast<Color>({0, 0, 0, 255}),
             "Clear the renderer with the specified color")
        .def("present", &Renderer::present, "Present the rendered content")

        .def("draw", py::overload_cast<const math::Vec2&, const Color&>(&Renderer::draw),
             py::arg("point"), py::arg("color"), "Draw a point to the renderer")
        .def("draw", py::overload_cast<const Texture&>(&Renderer::draw), py::arg("texture"),
             "Draw a texture to the renderer")
        .def("draw", py::overload_cast<const Circle&, const Color&, int>(&Renderer::draw),
             py::arg("circle"), py::arg("color"), py::arg("thickness") = 0,
             "Draw a circle to the renderer")
        .def("draw", py::overload_cast<const Line&, const Color&, int>(&Renderer::draw),
             py::arg("line"), py::arg("color"), py::arg("thickness") = 1,
             "Draw a line to the renderer")
        .def("draw", py::overload_cast<const Rect&, const Color&, int>(&Renderer::draw),
             py::arg("rect"), py::arg("color"), py::arg("thickness") = 0,
             "Draw a rect to the renderer");
}
} // namespace renderer

Renderer::Renderer(const math::Vec2& resolution)
{
    if (resolution.x <= 0 || resolution.y <= 0)
        throw std::invalid_argument("Resolution must be greater than zero");

    m_renderer = SDL_CreateRenderer(window::getWindow(), nullptr);
    if (m_renderer == nullptr)
        throw std::runtime_error("Renderer failed to create: " + std::string(SDL_GetError()));

    SDL_SetRenderLogicalPresentation(m_renderer, resolution.x, resolution.y,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

void Renderer::clear(const Color& color)
{
    if (!color._isValid())
        throw std::invalid_argument("Color values must be between 0 and 255");

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

math::Vec2 Renderer::toView(const math::Vec2& windowCoord) const
{
    float rCoordX, rCoordY;
    SDL_RenderCoordinatesFromWindow(m_renderer, windowCoord.x, windowCoord.y, &rCoordX, &rCoordY);

    return {rCoordX, rCoordY};
}

void Renderer::draw(const math::Vec2& point, const Color& color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    SDL_FPoint sdlPoint = point - camera::getActivePos();
    if (!SDL_RenderPoint(m_renderer, sdlPoint.x, sdlPoint.y))
        throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));
}

void Renderer::draw(const Texture& texture)
{
    SDL_Texture* sdlTexture = texture.getSDL();
    if (SDL_GetRendererFromTexture(sdlTexture) != m_renderer)
        throw std::runtime_error("Texture does not belong to this renderer");

    SDL_FRect sdlRect = texture.getRect();

    sdlRect.x -= camera::getActivePos().x;
    sdlRect.y -= camera::getActivePos().y;

    SDL_RenderTexture(m_renderer, sdlTexture, nullptr, &sdlRect);
}

void Renderer::draw(const Circle& circle, const Color& color, const int thickness)
{
    if (circle.radius < 1)
        return;

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    if (thickness == 1)
        _circleThin(m_renderer, circle.pos, circle.radius, color);
    else
        _circle(m_renderer, circle.pos, circle.radius, color, thickness);
}

void Renderer::draw(const Line& line, const Color& color, const int thickness)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    if (thickness <= 1)
        _lineThin(m_renderer, line.getA(), line.getB(), color);
    else
        _line(m_renderer, line.getA(), line.getB(), color, thickness);
}

void Renderer::draw(const Rect& rect, const Color& color, const int thickness)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    Rect screenRect = rect;
    screenRect.setTopLeft(screenRect.getTopLeft() - camera::getActivePos());
    SDL_FRect sdlRect = screenRect;

    const auto halfWidth = static_cast<int>(rect.w / 2.0);
    const auto halfHeight = static_cast<int>(rect.h / 2.0);
    if (thickness <= 0 || thickness > halfWidth || thickness > halfHeight)
    {
        SDL_RenderFillRect(m_renderer, &sdlRect);
        return;
    }

    SDL_RenderRect(m_renderer, &sdlRect);
    for (int i = 1; i < thickness; i++)
    {
        screenRect.inflate({-2, -2});
        sdlRect = screenRect;
        SDL_RenderRect(m_renderer, &sdlRect);
    }
}

void Renderer::present() { SDL_RenderPresent(m_renderer); }

SDL_Renderer* Renderer::getSDL() const { return m_renderer; }

void _circleThin(SDL_Renderer* renderer, math::Vec2 center, const int radius, const Color& color)
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

void _circle(SDL_Renderer* renderer, math::Vec2 center, const int radius, const Color& color,
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

void _lineThin(SDL_Renderer* renderer, math::Vec2 start, math::Vec2 end, const Color& color)
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

void _line(SDL_Renderer* renderer, math::Vec2 start, math::Vec2 end, const Color& color,
           const int thickness)
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
