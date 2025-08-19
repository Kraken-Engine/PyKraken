#include "Mask.hpp"
#include "Math.hpp"
#include "PixelArray.hpp"
#include "Rect.hpp"

#include <pybind11/stl.h>

namespace mask
{
void _bind(py::module_& module)
{
    py::classh<Mask>(module, "Mask", R"doc(
A collision mask for pixel-perfect collision detection.

A Mask represents a 2D bitmap, typically used for precise collision detection based on 
non-transparent pixels.
    )doc")
        .def(py::init<const PixelArray&, uint8_t>(), py::arg("pixel_array"),
             py::arg("threshold") = 0,
             R"doc(
Create a mask from a pixel array based on alpha threshold.

Args:
    pixel_array (PixelArray): The source pixel array to create the mask from.
    threshold (int): Alpha threshold value (0-255). Pixels with alpha >= threshold are solid.

Raises:
    RuntimeError: If the pixel array is invalid.
        )doc")

        .def("collide_mask",
             py::overload_cast<const Mask&, const Vec2&>(&Mask::collideMask, py::const_),
             py::arg("other"), py::arg("offset"), R"doc(
Check collision between this mask and another mask with an offset.

Args:
    other (Mask): The other mask to test collision with.
    offset (Vec2): Position offset between the masks.

Returns:
    bool: True if the masks collide, False otherwise.
        )doc")

        .def("get_at", &Mask::getAt, py::arg("pos"), R"doc(
Get the pixel value at a specific position.

Args:
    pos (Vec2): The position to check.

Returns:
    bool: True if the pixel is solid (above threshold), False otherwise.
        )doc")

        .def_property_readonly("width", &Mask::getWidth, R"doc(
The width of the mask in pixels.
        )doc")
        .def_property_readonly("height", &Mask::getHeight, R"doc(
The height of the mask in pixels.
        )doc")
        .def_property_readonly(
            "size", [](const Mask& mask) -> py::tuple
            { return py::make_tuple(mask.getWidth(), mask.getHeight()); }, R"doc(
The size of the mask as (width, height).
        )doc")

        .def(
            "__bool__",
            [](const Mask& mask) -> bool
            {
                return true; // A mask is always "truthy" if it exists
            },
            R"doc(
Check if the mask is valid (always True for existing masks).
        )doc");
}
} // namespace mask

Mask::Mask(const Vec2& size, bool filled)
    : m_width(static_cast<int>(size.x)), m_height(static_cast<int>(size.y)),
      m_maskData(m_width * m_height, filled)
{
}

Mask::Mask(const PixelArray& pixelArray, const uint8_t threshold)
    : m_width(pixelArray.getWidth()), m_height(pixelArray.getHeight()),
      m_maskData(m_width * m_height, false)
{
    SDL_Surface* rawSurface = pixelArray.getSDL();
    if (!rawSurface)
        throw std::runtime_error("PixelArray object internal SDL surface pointer is null");

    const auto* pixels = static_cast<uint8_t*>(rawSurface->pixels);
    const int pitch = rawSurface->pitch;

    for (int y = 0; y < m_height; y++)
        for (int x = 0; x < m_height; x++)
        {
            const uint8_t* pixel = pixels + y * pitch + x * 4;
            m_maskData[y * m_width + x] = (pixel[3] >= threshold);
        }
}

bool Mask::collideMask(const Mask& other, const Vec2& offset) const
{
    const auto xOffset = static_cast<int>(offset.x);
    const auto yOffset = static_cast<int>(offset.y);

    const int xStart = std::max(0, -xOffset);
    const int yStart = std::max(0, -yOffset);
    const int xEnd = std::min(m_width, other.m_width - xOffset);
    const int yEnd = std::min(m_height, other.m_height - yOffset);

    if (xStart >= xEnd || yStart >= yEnd)
        return false; // No overlap

    for (int y = yStart; y < yEnd; ++y)
        for (int x = xStart; x < xEnd; ++x)
            if (getAt({x, y}) && other.getAt({x + xOffset, y + yOffset}))
                return true;

    return false;
}

bool Mask::getAt(const Vec2& pos) const
{
    if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
        return false;

    return m_maskData[pos.y * m_width + pos.x];
}

void Mask::setAt(const Vec2& pos, bool value)
{
    if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
        return;

    m_maskData[pos.y * m_width + pos.x] = value;
}

Rect Mask::getRect() const { return {0, 0, m_width, m_height}; }

int Mask::getWidth() const { return m_width; }

int Mask::getHeight() const { return m_height; }

Vec2 Mask::getSize() const { return {m_width, m_height}; }

Mask Mask::copy() const
{
    Mask copy;
    copy.m_width = m_width;
    copy.m_height = m_height;
    copy.m_maskData = m_maskData;
    return copy;
}