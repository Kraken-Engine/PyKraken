#include <SDL3_image/SDL_image.h>
// #include <gfx/SDL3_rotozoom.h>
#include <pybind11/native_enum.h>

#include "Color.hpp"
#include "Math.hpp"
#include "PixelArray.hpp"
#include "Rect.hpp"

namespace kn
{
PixelArray::PixelArray(SDL_Surface* sdlSurface)
    : m_surface(sdlSurface)
{
}

PixelArray::PixelArray(const Vec2& size)
{
    m_surface = SDL_CreateSurface(
        static_cast<int>(size.x), static_cast<int>(size.y), SDL_PIXELFORMAT_RGBA32
    );

    if (!m_surface)
        throw std::runtime_error("PixelArray failed to create: " + std::string(SDL_GetError()));
}

PixelArray::PixelArray(const std::string& filePath)
{
    m_surface = IMG_Load(filePath.c_str());
    if (!m_surface)
        throw std::runtime_error(
            "Failed to load pixel array from file '" + filePath +
            "': " + std::string(SDL_GetError())
        );
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

void PixelArray::blit(
    const PixelArray& other, const Vec2& pos, const Anchor anchor, const Rect& srcRect
) const
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
    const auto srcSDL = static_cast<SDL_Rect>(
        srcRect.w == 0.0 && srcRect.h == 0.0 ? this->getRect() : srcRect
    );

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::blit(const PixelArray& other, const Rect& dstRect, const Rect& srcRect) const
{
    const auto dstSDL = static_cast<SDL_Rect>(dstRect);
    const auto srcSDL = static_cast<SDL_Rect>(
        srcRect.w == 0.0 && srcRect.h == 0.0 ? this->getRect() : srcRect
    );

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::setColorKey(const Color& color) const
{
    SDL_SetSurfaceColorKey(
        m_surface, true, SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a)
    );
}

Color PixelArray::getColorKey() const
{
    uint32_t key;
    if (!SDL_GetSurfaceColorKey(m_surface, &key))
        throw std::runtime_error(
            "Failed to get pixel array color key: " + std::string(SDL_GetError())
        );

    Color color;
    color.r = static_cast<uint8_t>(key >> 24 & 0xFF);
    color.g = static_cast<uint8_t>(key >> 16 & 0xFF);
    color.b = static_cast<uint8_t>(key >> 8 & 0xFF);
    color.a = static_cast<uint8_t>(key & 0xFF);

    return color;
}

void PixelArray::setAlpha(const uint8_t alpha) const
{
    SDL_SetSurfaceAlphaMod(m_surface, alpha);
}

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

int PixelArray::getWidth() const
{
    return m_surface->w;
}

int PixelArray::getHeight() const
{
    return m_surface->h;
}

Vec2 PixelArray::getSize() const
{
    return {m_surface->w, m_surface->h};
}

Rect PixelArray::getRect() const
{
    return {0, 0, m_surface->w, m_surface->h};
}

std::unique_ptr<PixelArray> PixelArray::copy() const
{
    SDL_Surface* surfaceCopy = SDL_CreateSurface(m_surface->w, m_surface->h, m_surface->format);
    if (!surfaceCopy)
        throw std::runtime_error(
            "Failed to create copy pixel array: " + std::string(SDL_GetError())
        );

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

            std::
                memcpy(dstRow + dstX * bytesPerPixel, srcRow + srcX * bytesPerPixel, bytesPerPixel);
        }
    }
}

SDL_Surface* PixelArray::getSDL() const
{
    return m_surface;
}

namespace pixel_array
{
void _bind(py::module_& module)
{
    py::native_enum<ScrollMode>(module, "ScrollMode", "enum.IntEnum", R"doc(
Edge handling behavior for PixelArray scrolling.
    )doc")
        .value("SMEAR", ScrollMode::SMEAR, "Clamp edge pixels when scrolling")
        .value("ERASE", ScrollMode::ERASE, "Erase pixels that scroll out")
        .value("REPEAT", ScrollMode::REPEAT, "Wrap pixels when scrolling")
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
               const Anchor anchor, const py::object& srcObj) -> void
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
        )doc"
        )
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
        )doc"
        )
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
        .def(
            "scroll", &PixelArray::scroll, py::arg("dx"), py::arg("dy"), py::arg("scroll_mode"),
            R"doc(
Scroll the pixel array's contents by the specified offset.

Args:
    dx (int): Horizontal scroll offset in pixels.
    dy (int): Vertical scroll offset in pixels.
    scroll_mode (ScrollMode, optional): Behavior for pixels scrolled off the edge.
        - REPEAT: Wrap pixels around to the opposite edge.
        - ERASE: Fill scrolled areas with transparent pixels.
        - SMEAR: Extend edge pixels into scrolled areas.
        )doc"
        );

    auto subPixelArray =
        module.def_submodule("pixel_array", "Functions for manipulating PixelArray objects");

    subPixelArray.def(
        "flip", &flip, py::arg("pixel_array"), py::arg("flip_x"), py::arg("flip_y"),
        R"doc(
Flip a pixel array horizontally, vertically, or both.

Args:
    pixel_array (PixelArray): The pixel array to flip.
    flip_x (bool): Whether to flip horizontally (mirror left-right).
    flip_y (bool): Whether to flip vertically (mirror top-bottom).

Returns:
    PixelArray: A new pixel array with the flipped image.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc"
    );

    subPixelArray.def("scale_to", &scaleTo, py::arg("pixel_array"), py::arg("size"), R"doc(
Scale a pixel array to a new exact size.

Args:
    pixel_array (PixelArray): The pixel array to scale.
    size (Vec2): The target size as (width, height).

Returns:
    PixelArray: A new pixel array scaled to the specified size.

Raises:
    RuntimeError: If pixel array creation or scaling fails.
    )doc");

    subPixelArray.def(
        "scale_by", py::overload_cast<const PixelArray&, double>(&scaleBy), py::arg("pixel_array"),
        py::arg("factor"), R"doc(
Scale a pixel array by a given factor.

Args:
    pixel_array (PixelArray): The pixel array to scale.
    factor (float): The scaling factor (must be > 0). Values > 1.0 enlarge,
                   values < 1.0 shrink the pixel array.

Returns:
    PixelArray: A new pixel array scaled by the specified factor.

Raises:
    ValueError: If factor is <= 0.
    RuntimeError: If pixel array creation or scaling fails.
    )doc"
    );

    subPixelArray.def("rotate", &rotate, py::arg("pixel_array"), py::arg("angle"), R"doc(
Rotate a pixel array by a given angle.

Args:
    pixel_array (PixelArray): The pixel array to rotate.
    angle (float): The rotation angle in degrees. Positive values rotate clockwise.

Returns:
    PixelArray: A new pixel array containing the rotated image. The output pixel array may be
            larger than the input to accommodate the rotated image.

Raises:
    RuntimeError: If pixel array rotation fails.
    )doc");

    subPixelArray.def(
        "box_blur", &boxBlur, py::arg("pixel_array"), py::arg("radius"),
        py::arg("repeat_edge_pixels") = true, R"doc(
Apply a box blur effect to a pixel array.

Box blur creates a uniform blur effect by averaging pixels within a square kernel.
It's faster than Gaussian blur but produces a more uniform, less natural look.

Args:
    pixel_array (PixelArray): The pixel array to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the pixel array bounds. Defaults to True.

Returns:
    PixelArray: A new pixel array with the box blur effect applied.

Raises:
    RuntimeError: If pixel array creation fails during the blur process.
    )doc"
    );

    subPixelArray.def(
        "gaussian_blur", &gaussianBlur, py::arg("pixel_array"), py::arg("radius"),
        py::arg("repeat_edge_pixels") = true, R"doc(
Apply a Gaussian blur effect to a pixel array.

Gaussian blur creates a natural, smooth blur effect using a Gaussian distribution
for pixel weighting. It produces higher quality results than box blur but is
computationally more expensive.

Args:
    pixel_array (PixelArray): The pixel array to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the pixel array bounds. Defaults to True.

Returns:
    PixelArray: A new pixel array with the Gaussian blur effect applied.

Raises:
    RuntimeError: If pixel array creation fails during the blur process.
    )doc"
    );

    subPixelArray.def("invert", &invert, py::arg("pixel_array"), R"doc(
Invert the colors of a pixel array.

Creates a negative image effect by inverting each color channel (RGB).
The alpha channel is preserved unchanged.

Args:
    pixel_array (PixelArray): The pixel array to invert.

Returns:
    PixelArray: A new pixel array with inverted colors.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc");

    subPixelArray.def("grayscale", &grayscale, py::arg("pixel_array"), R"doc(
Convert a pixel array to grayscale.

Converts the pixel array to grayscale using the standard luminance formula:
gray = 0.299 * red + 0.587 * green + 0.114 * blue

This formula accounts for human perception of brightness across different colors.
The alpha channel is preserved unchanged.

Args:
    pixel_array (PixelArray): The pixel array to convert to grayscale.

Returns:
    PixelArray: A new pixel array converted to grayscale.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc");
}

std::unique_ptr<PixelArray> flip(const PixelArray& pixelArray, const bool flipX, const bool flipY)
{
    const SDL_Surface* sdlSurface = pixelArray.getSDL();
    SDL_Surface* flipped = SDL_CreateSurface(sdlSurface->w, sdlSurface->h, SDL_PIXELFORMAT_RGBA32);

    if (!flipped)
        throw std::runtime_error("Failed to create flipped pixel array.");

    const int bpp = SDL_GetPixelFormatDetails(sdlSurface->format)->bytes_per_pixel;

    for (int y = 0; y < sdlSurface->h; ++y)
        for (int x = 0; x < sdlSurface->w; ++x)
        {
            const int srcX = flipX ? sdlSurface->w - 1 - x : x;
            const int srcY = flipY ? sdlSurface->h - 1 - y : y;

            const uint8_t* srcPixel = static_cast<uint8_t*>(sdlSurface->pixels) +
                                      srcY * sdlSurface->pitch + srcX * bpp;
            uint8_t* dstPixel = static_cast<uint8_t*>(flipped->pixels) + y * flipped->pitch +
                                x * bpp;

            memcpy(dstPixel, srcPixel, bpp);
        }

    return std::make_unique<PixelArray>(flipped);
}

std::unique_ptr<PixelArray> scaleTo(const PixelArray& pixelArray, const Vec2& size)
{
    SDL_Surface* sdlSurface = pixelArray.getSDL();

    const auto newW = static_cast<int>(size.x);
    const auto newH = static_cast<int>(size.y);

    SDL_Surface* scaled = SDL_CreateSurface(newW, newH, SDL_PIXELFORMAT_RGBA32);
    if (!scaled)
        throw std::runtime_error("Failed to create scaled pixel array.");

    const SDL_Rect dstRect = {0, 0, newW, newH};
    if (!SDL_BlitSurfaceScaled(sdlSurface, nullptr, scaled, &dstRect, SDL_SCALEMODE_NEAREST))
    {
        SDL_DestroySurface(scaled);
        throw std::runtime_error("SDL_BlitScaled failed: " + std::string(SDL_GetError()));
    }

    return std::make_unique<PixelArray>(scaled);
}

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, const double factor)
{
    if (factor <= 0.0)
        throw std::invalid_argument("Scale factor must be a positive value.");

    return scaleTo(pixelArray, pixelArray.getSize() * factor);
}

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, const Vec2& factor)
{
    if (factor.x <= 0.0 || factor.y <= 0.0)
        throw std::invalid_argument("Scale factors must be positive values.");

    const Vec2 originalSize = pixelArray.getSize();
    return scaleTo(pixelArray, {originalSize.x * factor.x, originalSize.y * factor.y});
}

std::unique_ptr<PixelArray> rotate(const PixelArray& pixelArray, const double angle)
{
    SDL_Surface* sdlSurface = pixelArray.getSDL();
    SDL_Surface* rotated =
        rotozoomSurface(sdlSurface, angle, 1.0, SMOOTHING_OFF);  // rotate, don't scale
    if (!rotated)
        throw std::runtime_error("Failed to rotate pixel array.");

    return std::make_unique<PixelArray>(rotated);
}

std::unique_ptr<PixelArray> boxBlur(
    const PixelArray& pixelArray, const int radius, const bool repeatEdgePixels
)
{
    const SDL_Surface* src = pixelArray.getSDL();
    const int width = src->w;
    const int height = src->h;

    SDL_Surface* temp = SDL_CreateSurface(width, height, src->format);
    SDL_Surface* result = SDL_CreateSurface(width, height, src->format);
    if (!temp || !result)
        throw std::runtime_error("Failed to create surfaces for box blur.");

    auto clamp = [](const int v, const int low, const int high) -> int
    { return std::max(low, std::min(v, high)); };

    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* tmpPx = static_cast<uint32_t*>(temp->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const int diameter = radius * 2 + 1;
    const auto srcDetails = SDL_GetPixelFormatDetails(src->format);
    const auto tmpDetails = SDL_GetPixelFormatDetails(temp->format);
    const auto resDetails = SDL_GetPixelFormatDetails(result->format);

    // Horizontal
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            uint8_t r = 0, g = 0, b = 0, a = 0;
            for (int dx = -radius; dx <= radius; ++dx)
            {
                const int sx = repeatEdgePixels ? clamp(x + dx, 0, width - 1) : x + dx;
                if (sx < 0 || sx >= width)
                    continue;

                uint8_t pr, pg, pb, pa;
                SDL_GetRGBA(srcPx[y * width + sx], srcDetails, nullptr, &pr, &pg, &pb, &pa);
                r += pr;
                g += pg;
                b += pb;
                a += pa;
            }
            r /= diameter;
            g /= diameter;
            b /= diameter;
            a /= diameter;
            tmpPx[y * width + x] = SDL_MapRGBA(tmpDetails, nullptr, r, g, b, a);
        }

    // Vertical
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            uint8_t r = 0, g = 0, b = 0, a = 0;
            for (int dy = -radius; dy <= radius; ++dy)
            {
                const int sy = repeatEdgePixels ? clamp(y + dy, 0, height - 1) : y + dy;
                if (sy < 0 || sy >= height)
                    continue;
                uint8_t pr, pg, pb, pa;
                SDL_GetRGBA(tmpPx[sy * width + x], tmpDetails, nullptr, &pr, &pg, &pb, &pa);
                r += pr;
                g += pg;
                b += pb;
                a += pa;
            }
            r /= diameter;
            g /= diameter;
            b /= diameter;
            a /= diameter;
            dstPx[y * width + x] = SDL_MapRGBA(resDetails, nullptr, r, g, b, a);
        }

    SDL_DestroySurface(temp);

    return std::make_unique<PixelArray>(result);
}

std::unique_ptr<PixelArray> gaussianBlur(
    const PixelArray& pixelArray, const int radius, const bool repeatEdgePixels
)
{
    const SDL_Surface* src = pixelArray.getSDL();

    const int w = src->w, h = src->h;
    const int diameter = radius * 2 + 1;

    // Build Gaussian kernel (σ = radius/2)
    const float sigma = radius > 0 ? static_cast<float>(radius) / 2.f : 1.f;
    const float twoSigmaSq = 2.f * sigma * sigma;
    const auto invSigmaRoot = static_cast<float>(1.0 / (std::sqrt(2 * M_PI) * sigma));
    std::vector<float> kernel(diameter);
    for (int i = 0; i < diameter; ++i)
    {
        const int x = i - radius;
        kernel[i] = invSigmaRoot * std::exp(-static_cast<float>(x * x) / twoSigmaSq);
    }

    // Normalize
    float sum = 0;
    for (const float v : kernel)
        sum += v;
    for (float& v : kernel)
        v /= sum;

    // Create intermediate and output surfaces
    SDL_Surface* temp = SDL_CreateSurface(w, h, src->format);
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!temp)
        throw std::runtime_error("Failed to create temporary surface for gaussian blur.");
    if (!result)
        throw std::runtime_error("Failed to create result surface for gaussian blur.");

    auto clamp = [](const int v, const int low, const int high) -> int
    { return std::max(low, std::min(v, high)); };
    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* tmpPx = static_cast<uint32_t*>(temp->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const auto srcDetails = SDL_GetPixelFormatDetails(src->format);
    const auto tmpDetails = SDL_GetPixelFormatDetails(temp->format);
    const auto resDetails = SDL_GetPixelFormatDetails(result->format);

    // Horizontal pass
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            float fr = 0, fg = 0, fb = 0, fa = 0;
            for (int k = 0; k < diameter; ++k)
            {
                int sx = x + (k - radius);
                if (repeatEdgePixels)
                    sx = clamp(sx, 0, w - 1);
                if (sx < 0 || sx >= w)
                    continue;

                Uint8 pr, pg, pb, pa;
                SDL_GetRGBA(srcPx[y * w + sx], srcDetails, nullptr, &pr, &pg, &pb, &pa);
                fr += static_cast<float>(pr) * kernel[k];
                fg += static_cast<float>(pg) * kernel[k];
                fb += static_cast<float>(pb) * kernel[k];
                fa += static_cast<float>(pa) * kernel[k];
            }
            tmpPx[y * w + x] = SDL_MapRGBA(
                tmpDetails, nullptr, static_cast<Uint8>(fr), static_cast<Uint8>(fg),
                static_cast<Uint8>(fb), static_cast<Uint8>(fa)
            );
        }

    // Vertical pass
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            float fr = 0.f, fg = 0.f, fb = 0.f, fa = 0.f;
            for (int k = 0; k < diameter; ++k)
            {
                int sy = y + (k - radius);
                if (repeatEdgePixels)
                    sy = clamp(sy, 0, h - 1);
                if (sy < 0 || sy >= h)
                    continue;
                uint8_t pr, pg, pb, pa;
                SDL_GetRGBA(tmpPx[sy * w + x], tmpDetails, nullptr, &pr, &pg, &pb, &pa);
                fr += static_cast<float>(pr) * kernel[k];
                fg += static_cast<float>(pg) * kernel[k];
                fb += static_cast<float>(pb) * kernel[k];
                fa += static_cast<float>(pa) * kernel[k];
            }
            dstPx[y * w + x] = SDL_MapRGBA(
                resDetails, nullptr, static_cast<uint8_t>(fr), static_cast<uint8_t>(fg),
                static_cast<uint8_t>(fb), static_cast<uint8_t>(fa)
            );
        }

    SDL_DestroySurface(temp);

    return std::make_unique<PixelArray>(result);
}

std::unique_ptr<PixelArray> invert(const PixelArray& pixelArray)
{
    const SDL_Surface* src = pixelArray.getSDL();

    const int w = src->w;
    const int h = src->h;

    // Create an output surface matching the source format
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!result)
        throw std::runtime_error("Failed to create result surface for invert.");

    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const SDL_PixelFormatDetails* srcDetails = SDL_GetPixelFormatDetails(src->format);
    const SDL_PixelFormatDetails* resDetails = SDL_GetPixelFormatDetails(result->format);

    uint8_t r, g, b, a;
    for (int i = 0; i < w * h; ++i)
    {
        SDL_GetRGBA(srcPx[i], srcDetails, nullptr, &r, &g, &b, &a);
        dstPx[i] = SDL_MapRGBA(resDetails, nullptr, 255 - r, 255 - g, 255 - b, a);
    }

    return std::make_unique<PixelArray>(result);
}

std::unique_ptr<PixelArray> grayscale(const PixelArray& pixelArray)
{
    const SDL_Surface* src = pixelArray.getSDL();

    const int w = src->w;
    const int h = src->h;

    // Create an output surface with its own memory
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!result)
        throw std::runtime_error("Failed to create surface for grayscale.");

    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const SDL_PixelFormatDetails* srcDetails = SDL_GetPixelFormatDetails(src->format);
    const SDL_PixelFormatDetails* resDetails = SDL_GetPixelFormatDetails(result->format);

    uint8_t r, g, b, a;
    for (int i = 0; i < w * h; ++i)
    {
        SDL_GetRGBA(srcPx[i], srcDetails, nullptr, &r, &g, &b, &a);
        const auto gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
        dstPx[i] = SDL_MapRGBA(resDetails, nullptr, gray, gray, gray, a);
    }

    return std::make_unique<PixelArray>(result);
}
}  // namespace pixel_array
}  // namespace kn
