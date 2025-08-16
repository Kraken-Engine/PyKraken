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

    py::class_<Surface>(module, "Surface", R"doc(
Represents a 2D pixel buffer for image manipulation and blitting operations.

A Surface is a collection of pixels that can be manipulated, drawn on, and used as a source
for texture creation or blitting to other surfaces. Supports pixel-level operations,
color key transparency, and alpha blending.
    )doc")
        .def(py::init<const Vec2&>(), py::arg("size"), R"doc(
Create a new Surface with the specified dimensions.

Args:
    size (Vec2): The size of the surface as (width, height).

Raises:
    RuntimeError: If surface creation fails.
        )doc")
        .def(py::init<const std::string&>(), py::arg("file_path"), R"doc(
Create a Surface by loading an image from a file.

Args:
    file_path (str): Path to the image file to load.

Raises:
    RuntimeError: If the file cannot be loaded or doesn't exist.
        )doc")

        .def("fill", &Surface::fill, py::arg("color"), R"doc(
Fill the entire surface with a solid color.

Args:
    color (Color): The color to fill the surface with.
        )doc")
        .def("blit",
             py::overload_cast<const Surface&, const Vec2&, Anchor, py::object>(&Surface::blit,
                                                                                py::const_),
             py::arg("surface"), py::arg("pos"), py::arg("anchor") = Anchor::CENTER,
             py::arg("src") = py::none(), R"doc(
Blit (copy) another surface onto this surface at the specified position with anchor alignment.

Args:
    surface (Surface): The source surface to blit from.
    pos (Vec2): The position to blit to.
    anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source surface.

Raises:
    RuntimeError: If the blit operation fails.
        )doc")
        .def("blit",
             py::overload_cast<const Surface&, const Rect&, py::object>(&Surface::blit, py::const_),
             py::arg("surface"), py::arg("dst"), py::arg("src") = py::none(), R"doc(
Blit (copy) another surface onto this surface with specified destination and source rectangles.

Args:
    surface (Surface): The source surface to blit from.
    dst (Rect): The destination rectangle on this surface.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source surface.

Raises:
    RuntimeError: If the blit operation fails.
        )doc")
        .def("get_at", &Surface::getAt, py::arg("coord"), R"doc(
Get the color of a pixel at the specified coordinates.

Args:
    coord (Vec2): The coordinates of the pixel as (x, y).

Returns:
    Color: The color of the pixel at the specified coordinates.

Raises:
    IndexError: If coordinates are outside the surface bounds.
        )doc")
        .def("set_at", &Surface::setAt, py::arg("coord"), py::arg("color"), R"doc(
Set the color of a pixel at the specified coordinates.

Args:
    coord (Vec2): The coordinates of the pixel as (x, y).
    color (Color): The color to set the pixel to.

Raises:
    IndexError: If coordinates are outside the surface bounds.
        )doc")
        .def("copy", &Surface::copy, R"doc(
Create a copy of this surface.

Returns:
    Surface: A new Surface that is an exact copy of this one.

Raises:
    RuntimeError: If surface copying fails.
        )doc")

        .def_property("color_key", &Surface::getColorKey, &Surface::setColorKey, R"doc(
The color key for transparency.

When set, pixels of this color will be treated as transparent during blitting operations.
Used for simple transparency effects.

Returns:
    Color: The current color key.

Raises:
    RuntimeError: If getting the color key fails.
        )doc")
        .def_property("alpha_mod", &Surface::getAlpha, &Surface::setAlpha, R"doc(
The alpha modulation value for the surface.

Controls the overall transparency of the surface. Values range from 0 (fully transparent)
to 255 (fully opaque).

Returns:
    int: The current alpha modulation value [0-255].

Raises:
    RuntimeError: If getting the alpha value fails.
        )doc")

        .def_property_readonly("width", &Surface::getWidth, R"doc(
The width of the surface in pixels.

Returns:
    int: The surface width.
        )doc")
        .def_property_readonly("height", &Surface::getHeight, R"doc(
The height of the surface in pixels.

Returns:
    int: The surface height.
        )doc")
        .def_property_readonly("size", &Surface::getSize, R"doc(
The size of the surface as a Vec2.

Returns:
    Vec2: The surface size as (width, height).
        )doc")
        .def_property_readonly("rect", &Surface::getRect, R"doc(
A rectangle representing the surface bounds.

Returns:
    Rect: A rectangle with position (0, 0) and the surface's dimensions.
        )doc");
}
} // namespace surface

Surface::Surface(SDL_Surface* sdlSurface) : m_surface(sdlSurface) {}

Surface::Surface(const Vec2& size)
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

void Surface::blit(const Surface& other, const Vec2& pos, const Anchor anchor,
                   py::object srcRect) const
{
    SDL_Rect srcSDL;
    if (!srcRect.is_none())
    {
        try
        {
            srcSDL = srcRect.cast<Rect>();
        }
        catch (const py::cast_error&)
        {
            throw std::runtime_error("srcRect must be a Rect");
        }
    }
    else
    {
        srcSDL = other.getRect();
    }

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

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit surface: " + std::string(SDL_GetError()));
}

void Surface::blit(const Surface& other, const Rect& dstRect, py::object srcRect) const
{
    SDL_Rect dstSDL = dstRect;
    SDL_Rect srcSDL;

    if (!srcRect.is_none())
    {
        try
        {
            srcSDL = srcRect.cast<Rect>();
        }
        catch (const py::cast_error&)
        {
            throw std::runtime_error("srcRect must be a Rect");
        }
    }
    else
    {
        srcSDL = other.getRect();
    }

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

    Color color;
    color.r = static_cast<uint8_t>((key >> 24) & 0xFF);
    color.g = static_cast<uint8_t>((key >> 16) & 0xFF);
    color.b = static_cast<uint8_t>((key >> 8) & 0xFF);
    color.a = static_cast<uint8_t>(key & 0xFF);

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

Color Surface::getAt(const Vec2& coord) const
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

void Surface::setAt(const Vec2& coord, const Color& color) const
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

Vec2 Surface::getSize() const { return {m_surface->w, m_surface->h}; }

Rect Surface::getRect() const { return Rect(0, 0, m_surface->w, m_surface->h); }

std::unique_ptr<Surface> Surface::copy() const
{
    SDL_Surface* surfaceCopy = SDL_CreateSurface(m_surface->w, m_surface->h, m_surface->format);
    if (!surfaceCopy)
        throw std::runtime_error("Failed to create copy surface: " + std::string(SDL_GetError()));

    if (!SDL_BlitSurface(m_surface, nullptr, surfaceCopy, nullptr))
        throw std::runtime_error("Failed to blit surface copy: " + std::string(SDL_GetError()));

    auto copy = std::make_unique<Surface>();
    copy->m_surface = surfaceCopy;
    return copy;
}

SDL_Surface* Surface::getSDL() const { return m_surface; }
