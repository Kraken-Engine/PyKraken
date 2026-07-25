#include "kraken/graphics/Viewport.hpp"

#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Renderer.hpp"

namespace kn
{
namespace viewport
{
std::vector<Rect> layout(const uint8_t count, const ViewportMode mode)
{
    if (count > 4 || count < 2)
        throw std::runtime_error("'count' must be between 2 and 4");

    const Vec2 rendRes = renderer::getCurrentResolution();
    std::vector<Rect> viewports;
    viewports.reserve(count);

    switch (count)
    {
    case 4:
    {
        const Vec2 vpSize = rendRes * 0.5;
        viewports.push_back({0.0, 0.0, vpSize});
        viewports.push_back({vpSize.x, 0.0, vpSize});
        viewports.push_back({0.0, vpSize.y, vpSize});
        viewports.push_back({vpSize, vpSize});
        break;
    }
    case 3:
    {
        const Vec2 vpSize = rendRes * 0.5;
        viewports.push_back({0.0, 0.0, vpSize});
        viewports.push_back({vpSize.x, 0.0, vpSize});
        viewports.push_back({0.0, vpSize.y, {rendRes.x, vpSize.y}});
        break;
    }
    case 2:
    {
        switch (mode)
        {
        case ViewportMode::HORIZONTAL:
        {
            const Vec2 vpSize{rendRes.x, rendRes.y * 0.5};
            viewports.push_back({0.0, 0.0, vpSize});
            viewports.push_back({0.0, vpSize.y, vpSize});
            break;
        }
        case ViewportMode::VERTICAL:
        {
            const Vec2 vpSize{rendRes.x * 0.5, rendRes.y};
            viewports.push_back({0.0, 0.0, vpSize});
            viewports.push_back({vpSize.x, 0.0, vpSize});
            break;
        }
        }
        break;
    }
    }

    return viewports;
}

void set(const Rect& rect)
{
    if (rect.w == 0 || rect.h == 0)
        throw std::runtime_error("Viewport width and height must be greater than zero");

    const SDL_Rect sdlRect = static_cast<SDL_Rect>(rect);
    if (!SDL_SetRenderViewport(renderer::_get(), &sdlRect))
        throw std::runtime_error(std::string("viewport::set failed: ") + SDL_GetError());
}

void unset()
{
    if (!SDL_SetRenderViewport(renderer::_get(), nullptr))
        throw std::runtime_error(std::string("viewport::unset failed: ") + SDL_GetError());
}

}  // namespace viewport
}  // namespace kn
