#include "Surface.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"

#include <SDL3_image/SDL_image.h>

namespace surface
{
void _bind(py::module_& module)
{
    // py::enum_<ScrollType>(module, "ScrollType", py::arithmetic())
    //     .value("SCROLL_SMEAR", ScrollType::SCROLL_SMEAR)
    //     .value("SCROLL_ERASE", ScrollType::SCROLL_ERASE)
    //     .value("SCROLL_REPEAT", ScrollType::SCROLL_REPEAT)
    //     .export_values();

    py::class_<Surface>(module, "Surface")
        .def(py::init<const math::Vec2&>())
        .def(py::init<const std::string&>())

        .def("fill", &Surface::fill)
        .def("blit", py::overload_cast<const Surface&, const math::Vec2&, Anchor, const Rect&>(
                         &Surface::blit, py::const_))
        .def("blit", py::overload_cast<const Surface&, const Rect&, const Rect&>(&Surface::blit,
                                                                                 py::const_))
        .def("get_at", &Surface::getAt)
        .def("set_at", &Surface::setAt)
        .def("copy", &Surface::copy)

        .def_property("color_key", &Surface::getColorKey, &Surface::setColorKey)
        .def_property("alpha_mod", &Surface::getAlpha, &Surface::setAlpha)

        .def_property_readonly("width", &Surface::getWidth)
        .def_property_readonly("height", &Surface::getHeight)
        .def_property_readonly("size", &Surface::getSize)
        .def_property_readonly("rect", &Surface::getRect);
}
} // namespace surface

Surface::Surface(SDL_Surface* sdlSurface) : m_surface(sdlSurface) {}

Surface::Surface(const math::Vec2& size)
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }

    m_surface = SDL_CreateSurface(static_cast<int>(size.x), static_cast<int>(size.y),
                                  SDL_PIXELFORMAT_RGBA32);

    if (!m_surface)
        throw std::runtime_error("Surface failed to create: " + std::string(SDL_GetError()));
}

Surface::Surface(const std::string& filePath)
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }

    m_surface = IMG_Load(filePath.c_str());
    if (!m_surface)
        throw std::runtime_error("Failed to load surface from file '" + filePath +
                                 "': " + std::string(SDL_GetError()));
}

Surface::~Surface()
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }
}

void Surface::fill(const Color& color) const
{
    auto colorMap = SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a);
    SDL_FillSurfaceRect(m_surface, nullptr, colorMap);
}

void Surface::blit(const Surface& other, const math::Vec2& pos, const Anchor anchor,
                   const Rect& srcRect) const
{
    Rect dstRect = other.getRect();
    switch (anchor)
    {
    case Anchor::TOP_LEFT:
        dstRect.setTopLeft(pos);
        break;
    case Anchor::TOP_MID:
        dstRect.setTopMid(pos);
        break;
    case Anchor::TOP_RIGHT:
        dstRect.setTopRight(pos);
        break;
    case Anchor::MID_LEFT:
        dstRect.setMidLeft(pos);
        break;
    case Anchor::CENTER:
        dstRect.setCenter(pos);
        break;
    case Anchor::MID_RIGHT:
        dstRect.setMidRight(pos);
        break;
    case Anchor::BOTTOM_LEFT:
        dstRect.setBottomLeft(pos);
        break;
    case Anchor::BOTTOM_MID:
        dstRect.setBottomMid(pos);
        break;
    case Anchor::BOTTOM_RIGHT:
        dstRect.setBottomRight(pos);
        break;
    }

    SDL_Rect dstSDL = dstRect;
    SDL_Rect srcSDL = (srcRect.getSize() == math::Vec2()) ? other.getRect() : srcRect;

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
}

void Surface::blit(const Surface& other, const Rect& dstRect, const Rect& srcRect) const
{
    SDL_Rect dstSDL = dstRect;
    SDL_Rect srcSDL = (srcRect.getSize() == math::Vec2()) ? other.getRect() : srcRect;

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
}

void Surface::setColorKey(const Color& color) const
{
    SDL_SetSurfaceColorKey(m_surface, true,
                           SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a));
}

Color Surface::getColorKey() const
{
    uint32_t key;
    if (!SDL_GetSurfaceColorKey(m_surface, &key))
        throw std::runtime_error("Failed to get surface color key: " + std::string(SDL_GetError()));

    Color color = {(key >> 24) & 0xFF, // Extract red component
                   (key >> 16) & 0xFF, // Extract green component
                   (key >> 8) & 0xFF,  // Extract blue component
                   key & 0xFF};        // Extract alpha component

    return color;
}

void Surface::setAlpha(const uint8_t alpha) const { SDL_SetSurfaceAlphaMod(m_surface, alpha); }

int Surface::getAlpha() const
{
    uint8_t alpha;
    if (!SDL_GetSurfaceAlphaMod(m_surface, &alpha))
        throw std::runtime_error("Failed to get surface alpha: " + std::string(SDL_GetError()));
    return alpha;
}

Color Surface::getAt(const math::Vec2& coord) const
{
    if (coord.x < 0 || coord.x >= m_surface->w || coord.y < 0 || coord.y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for surface");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    int pitch = m_surface->pitch;
    auto x = static_cast<int>(coord.x);
    auto y = static_cast<int>(coord.y);

    uint32_t pixel = *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t));

    Color color;
    auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    SDL_GetRGBA(pixel, formatDetails, nullptr, &color.r, &color.g, &color.b, &color.a);

    return color;
}

void Surface::setAt(const math::Vec2& coord, const Color& color) const
{
    if (coord.x < 0 || coord.x >= m_surface->w || coord.y < 0 || coord.y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for surface");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    int pitch = m_surface->pitch;
    auto x = static_cast<int>(coord.x);
    auto y = static_cast<int>(coord.y);

    auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    uint32_t pixel = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, color.a);
    *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t)) = pixel;
}

int Surface::getWidth() const { return m_surface->w; }

int Surface::getHeight() const { return m_surface->h; }

math::Vec2 Surface::getSize() const { return {m_surface->w, m_surface->h}; }

Rect Surface::getRect() const { return Rect(0, 0, m_surface->w, m_surface->h); }

Surface Surface::copy() const
{
    SDL_Surface* surfaceCopy = SDL_CreateSurface(m_surface->w, m_surface->h, m_surface->format);
    if (!surfaceCopy)
        throw std::runtime_error("Failed to create copy surface: " + std::string(SDL_GetError()));

    if (!SDL_BlitSurface(m_surface, nullptr, surfaceCopy, nullptr))
        throw std::runtime_error("Failed to blit surface copy: " + std::string(SDL_GetError()));

    Surface copy;
    copy.setSDL(surfaceCopy);
    return copy;
}

SDL_Surface* Surface::getSDL() const { return m_surface; }

void Surface::setSDL(SDL_Surface* surface)
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }

    m_surface = surface;
}
