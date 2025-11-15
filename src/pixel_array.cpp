#include "Color.hpp"
#include "Math.hpp"
#include "PixelArray.hpp"
#include "Rect.hpp"

#include <SDL3_image/SDL_image.h>
#include <pybind11/native_enum.h>

namespace kn
{
PixelArray::PixelArray(SDL_Surface* sdlSurface) : m_surface(sdlSurface) {}

PixelArray::PixelArray(const Vec2& size)
{
    m_surface = SDL_CreateSurface(static_cast<int>(size.x), static_cast<int>(size.y),
                                  SDL_PIXELFORMAT_RGBA32);

    if (!m_surface)
        throw std::runtime_error("PixelArray failed to create: " + std::string(SDL_GetError()));
}

PixelArray::PixelArray(const std::string& filePath)
{
    m_surface = IMG_Load(filePath.c_str());
    if (!m_surface)
        throw std::runtime_error("Failed to load pixel array from file '" + filePath +
                                 "': " + std::string(SDL_GetError()));
}

PixelArray::~PixelArray()
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }
}

void PixelArray::fill(const Color& color) const
{
    const auto colorMap = SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a);
    SDL_FillSurfaceRect(m_surface, nullptr, colorMap);
}

void PixelArray::blit(const PixelArray& other, const Vec2& pos, const Anchor anchor,
                      const Rect& srcRect) const
{
    Rect dstRect = other.getRect();
    switch (anchor)
    {
    case Anchor::TopLeft:
        dstRect.setTopLeft(pos);
        break;
    case Anchor::TopMid:
        dstRect.setTopMid(pos);
        break;
    case Anchor::TopRight:
        dstRect.setTopRight(pos);
        break;
    case Anchor::MidLeft:
        dstRect.setMidLeft(pos);
        break;
    case Anchor::Center:
        dstRect.setCenter(pos);
        break;
    case Anchor::MidRight:
        dstRect.setMidRight(pos);
        break;
    case Anchor::BottomLeft:
        dstRect.setBottomLeft(pos);
        break;
    case Anchor::BottomMid:
        dstRect.setBottomMid(pos);
        break;
    case Anchor::BottomRight:
        dstRect.setBottomRight(pos);
        break;
    }

    const auto dstSDL = static_cast<SDL_Rect>(dstRect);
    const auto srcSDL =
        static_cast<SDL_Rect>(srcRect.w == 0.0 && srcRect.h == 0.0 ? this->getRect() : srcRect);

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::blit(const PixelArray& other, const Rect& dstRect, const Rect& srcRect) const
{
    const auto dstSDL = static_cast<SDL_Rect>(dstRect);
    const auto srcSDL =
        static_cast<SDL_Rect>(srcRect.w == 0.0 && srcRect.h == 0.0 ? this->getRect() : srcRect);

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::setColorKey(const Color& color) const
{
    SDL_SetSurfaceColorKey(m_surface, true,
                           SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a));
}

Color PixelArray::getColorKey() const
{
    uint32_t key;
    if (!SDL_GetSurfaceColorKey(m_surface, &key))
        throw std::runtime_error("Failed to get pixel array color key: " +
                                 std::string(SDL_GetError()));

    Color color;
    color.r = static_cast<uint8_t>(key >> 24 & 0xFF);
    color.g = static_cast<uint8_t>(key >> 16 & 0xFF);
    color.b = static_cast<uint8_t>(key >> 8 & 0xFF);
    color.a = static_cast<uint8_t>(key & 0xFF);

    return color;
}

void PixelArray::setAlpha(const uint8_t alpha) const { SDL_SetSurfaceAlphaMod(m_surface, alpha); }

int PixelArray::getAlpha() const
{
    uint8_t alpha;
    if (!SDL_GetSurfaceAlphaMod(m_surface, &alpha))
        throw std::runtime_error("Failed to get pixel array alpha: " + std::string(SDL_GetError()));
    return alpha;
}

Color PixelArray::getAt(const Vec2& coord) const
{
    if (coord.x < 0 || coord.x >= m_surface->w || coord.y < 0 || coord.y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for pixel array");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    const int pitch = m_surface->pitch;
    const auto x = static_cast<int>(coord.x);
    const auto y = static_cast<int>(coord.y);

    const uint32_t pixel = *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t));

    Color color;
    const auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    SDL_GetRGBA(pixel, formatDetails, nullptr, &color.r, &color.g, &color.b, &color.a);

    return color;
}

void PixelArray::setAt(const Vec2& coord, const Color& color) const
{
    if (coord.x < 0 || coord.x >= m_surface->w || coord.y < 0 || coord.y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for pixel array");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    const int pitch = m_surface->pitch;
    const auto x = static_cast<int>(coord.x);
    const auto y = static_cast<int>(coord.y);

    const auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    const uint32_t pixel = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, color.a);
    *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t)) = pixel;
}

int PixelArray::getWidth() const { return m_surface->w; }

int PixelArray::getHeight() const { return m_surface->h; }

Vec2 PixelArray::getSize() const { return {m_surface->w, m_surface->h}; }

Rect PixelArray::getRect() const { return {0, 0, m_surface->w, m_surface->h}; }

std::unique_ptr<PixelArray> PixelArray::copy() const
{
    SDL_Surface* surfaceCopy = SDL_CreateSurface(m_surface->w, m_surface->h, m_surface->format);
    if (!surfaceCopy)
        throw std::runtime_error("Failed to create copy pixel array: " +
                                 std::string(SDL_GetError()));

    if (!SDL_BlitSurface(m_surface, nullptr, surfaceCopy, nullptr))
        throw std::runtime_error("Failed to blit pixel array copy: " + std::string(SDL_GetError()));

    return std::make_unique<PixelArray>(surfaceCopy);
}

void PixelArray::scroll(const int dx, const int dy, const ScrollMode scrollMode) const
{
    if (!m_surface || (dx == 0 && dy == 0))
        return;

    const int width = m_surface->w;
    const int height = m_surface->h;
    const int pitch = m_surface->pitch;
    const auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    const int bytesPerPixel = formatDetails->bytes_per_pixel;

    // For REPEAT mode, optimize with modulo; for others, keep full offset
    int scrollX = dx;
    int scrollY = dy;

    if (scrollMode == ScrollMode::REPEAT)
    {
        scrollX = dx % width;
        scrollY = dy % height;

        if (scrollX == 0 && scrollY == 0)
            return;
    }

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    std::vector<uint8_t> tempBuffer(pitch * height);
    std::memcpy(tempBuffer.data(), pixels, pitch * height);

    // Process each destination row
    for (int dstY = 0; dstY < height; ++dstY)
    {
        int srcY = dstY - scrollY;

        // Handle Y boundary based on scroll mode
        switch (scrollMode)
        {
        case ScrollMode::REPEAT:
            srcY = (srcY % height + height) % height;
            break;

        case ScrollMode::ERASE:
            if (srcY < 0 || srcY >= height)
            {
                // Erase entire row
                std::memset(pixels + dstY * pitch, 0, width * bytesPerPixel);
                continue;
            }
            break;

        case ScrollMode::SMEAR:
            srcY = std::max(0, std::min(height - 1, srcY));
            break;
        }

        // Process row with optimized X handling
        uint8_t* dstRow = pixels + dstY * pitch;
        const uint8_t* srcRow = tempBuffer.data() + srcY * pitch;

        for (int dstX = 0; dstX < width; ++dstX)
        {
            int srcX = dstX - scrollX;

            // Handle X boundary based on scroll mode
            switch (scrollMode)
            {
            case ScrollMode::REPEAT:
                srcX = (srcX % width + width) % width;
                break;

            case ScrollMode::ERASE:
                if (srcX < 0 || srcX >= width)
                {
                    std::memset(dstRow + dstX * bytesPerPixel, 0, bytesPerPixel);
                    continue;
                }
                break;

            case ScrollMode::SMEAR:
                srcX = std::max(0, std::min(width - 1, srcX));
                break;
            }

            std::memcpy(dstRow + dstX * bytesPerPixel, srcRow + srcX * bytesPerPixel,
                        bytesPerPixel);
        }
    }
}

SDL_Surface* PixelArray::getSDL() const { return m_surface; }

namespace pixel_array
{
void _bind(const py::module_& module)
{
    py::native_enum<ScrollMode>(module, "ScrollMode", "enum.IntEnum")
        .value("SMEAR", ScrollMode::SMEAR)
        .value("ERASE", ScrollMode::ERASE)
        .value("REPEAT", ScrollMode::REPEAT)
        .finalize();

    py::classh<PixelArray>(module, "PixelArray", R"doc(
Represents a 2D pixel buffer for image manipulation and blitting operations.

A PixelArray is a 2D array of pixels that can be manipulated, drawn on, and used as a source
for texture creation or blitting to other PixelArrays. Supports pixel-level operations,
color key transparency, and alpha blending.
    )doc")
        .def(py::init<const Vec2&>(), py::arg("size"), R"doc(
Create a new PixelArray with the specified dimensions.

Args:
    size (Vec2): The size of the pixel array as (width, height).

Raises:
    RuntimeError: If pixel array creation fails.
        )doc")
        .def(py::init<const std::string&>(), py::arg("file_path"), R"doc(
Create a PixelArray by loading an image from a file.

Args:
    file_path (str): Path to the image file to load.

Raises:
    RuntimeError: If the file cannot be loaded or doesn't exist.
        )doc")

        .def_property("color_key", &PixelArray::getColorKey, &PixelArray::setColorKey, R"doc(
The color key for transparency.

When set, pixels of this color will be treated as transparent during blitting operations.
Used for simple transparency effects.

Returns:
    Color: The current color key.

Raises:
    RuntimeError: If getting the color key fails.
        )doc")
        .def_property("alpha_mod", &PixelArray::getAlpha, &PixelArray::setAlpha, R"doc(
The alpha modulation value for the pixel array.

Controls the overall transparency of the pixel array. Values range from 0 (fully transparent)
to 255 (fully opaque).

Returns:
    int: The current alpha modulation value [0-255].

Raises:
    RuntimeError: If getting the alpha value fails.
        )doc")

        .def_property_readonly("width", &PixelArray::getWidth, R"doc(
The width of the pixel array.

Returns:
    int: The pixel array width.
        )doc")
        .def_property_readonly("height", &PixelArray::getHeight, R"doc(
The height of the pixel array.

Returns:
    int: The pixel array height.
        )doc")
        .def_property_readonly("size", &PixelArray::getSize, R"doc(
The size of the pixel array as a Vec2.

Returns:
    Vec2: The pixel array size as (width, height).
        )doc")

        .def("fill", &PixelArray::fill, py::arg("color"), R"doc(
Fill the entire pixel array with a solid color.

Args:
    color (Color): The color to fill the pixel array with.
        )doc")
        .def(
            "blit",
            [](const PixelArray& self, const PixelArray& other, const Vec2& pos,
               const Anchor anchor, const py::object& srcObj)
            {
                try
                {
                    srcObj.is_none() ? self.blit(other, pos, anchor)
                                     : self.blit(other, pos, anchor, srcObj.cast<Rect>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'src', expected Rect");
                }
            },
            py::arg("pixel_array"), py::arg("pos"), py::arg("anchor") = Anchor::Center,
            py::arg("src") = py::none(), R"doc(
Blit (copy) another pixel array onto this pixel array at the specified position with anchor alignment.

Args:
    pixel_array (PixelArray): The source pixel array to blit from.
    pos (Vec2): The position to blit to.
    anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.

Raises:
    RuntimeError: If the blit operation fails.
        )doc")
        .def(
            "blit",
            [](const PixelArray& self, const PixelArray& other, const Rect& dst,
               const py::object& src)
            {
                try
                {
                    src.is_none() ? self.blit(other, dst) : self.blit(other, dst, src.cast<Rect>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'src', expected Rect");
                }
            },
            py::arg("pixel_array"), py::arg("dst"), py::arg("src") = py::none(), R"doc(
Blit (copy) another pixel array onto this pixel array with specified destination and source rectangles.

Args:
    pixel_array (PixelArray): The source pixel array to blit from.
    dst (Rect): The destination rectangle on this pixel array.
    src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.

Raises:
    RuntimeError: If the blit operation fails.
        )doc")
        .def("get_at", &PixelArray::getAt, py::arg("coord"), R"doc(
Get the color of a pixel at the specified coordinates.

Args:
    coord (Vec2): The coordinates of the pixel as (x, y).

Returns:
    Color: The color of the pixel at the specified coordinates.

Raises:
    IndexError: If coordinates are outside the pixel array bounds.
        )doc")
        .def("set_at", &PixelArray::setAt, py::arg("coord"), py::arg("color"), R"doc(
Set the color of a pixel at the specified coordinates.

Args:
    coord (Vec2): The coordinates of the pixel as (x, y).
    color (Color): The color to set the pixel to.

Raises:
    IndexError: If coordinates are outside the pixel array bounds.
        )doc")
        .def("copy", &PixelArray::copy, R"doc(
Create a copy of this pixel array.

Returns:
    PixelArray: A new PixelArray that is an exact copy of this one.

Raises:
    RuntimeError: If pixel array copying fails.
        )doc")
        .def("get_rect", &PixelArray::getRect, R"doc(
Get a rectangle representing the pixel array bounds.

Returns:
    Rect: A rectangle with position (0, 0) and the pixel array's dimensions.
        )doc")
        .def("scroll", &PixelArray::scroll, py::arg("dx"), py::arg("dy"), py::arg("scroll_mode"),
             R"doc(
Scroll the pixel array's contents by the specified offset.

Args:
    dx (int): Horizontal scroll offset in pixels.
    dy (int): Vertical scroll offset in pixels.
    scroll_mode (ScrollMode, optional): Behavior for pixels scrolled off the edge.
        - REPEAT: Wrap pixels around to the opposite edge.
        - ERASE: Fill scrolled areas with transparent pixels.
        - SMEAR: Extend edge pixels into scrolled areas.
        )doc");
}
} // namespace pixel_array
} // namespace kn
