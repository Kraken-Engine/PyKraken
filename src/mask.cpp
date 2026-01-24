#include "Mask.hpp"

#include <pybind11/stl.h>

#include "Color.hpp"
#include "Math.hpp"
#include "PixelArray.hpp"
#include "Rect.hpp"

namespace kn
{
Mask::Mask(const Vec2& size, const bool filled)
    : m_width(static_cast<int>(size.x)),
      m_height(static_cast<int>(size.y)),
      m_maskData(m_width * m_height, filled)
{
}

Mask::Mask(const PixelArray& pixelArray, const uint8_t threshold)
    : m_width(pixelArray.getWidth()),
      m_height(pixelArray.getHeight()),
      m_maskData(m_width * m_height, false)
{
    SDL_Surface* rawSurface = pixelArray.getSDL();
    if (!rawSurface)
        throw std::runtime_error("PixelArray object internal SDL surface pointer is null");

    uint8_t alpha;
    for (int y = 0; y < m_height; y++)
        for (int x = 0; x < m_width; x++)
        {
            SDL_ReadSurfacePixel(rawSurface, x, y, nullptr, nullptr, nullptr, &alpha);
            m_maskData[y * m_width + x] = alpha >= threshold;
        }
}

Vec2 Mask::getSize() const
{
    return {m_width, m_height};
}

Rect Mask::getRect() const
{
    return {0, 0, m_width, m_height};
}

bool Mask::getAt(const Vec2& pos) const
{
    if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
        return false;

    return m_maskData[static_cast<int>(pos.y * m_width + pos.x)];
}

void Mask::setAt(const Vec2& pos, const bool value)
{
    if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
        return;

    m_maskData[static_cast<int>(pos.y * m_width + pos.x)] = value;
}

int Mask::getOverlapArea(const Mask& other, const Vec2& offset) const
{
    const auto xOffset = static_cast<int>(std::round(offset.x));
    const auto yOffset = static_cast<int>(std::round(offset.y));

    int overlapCount = 0;
    for (int y = 0; y < other.m_height; ++y)
        for (int x = 0; x < other.m_width; ++x)
        {
            const int targetX = x + xOffset;
            const int targetY = y + yOffset;
            if (targetX >= 0 && targetX < m_width && targetY >= 0 && targetY < m_height)
                if (m_maskData[targetY * m_width + targetX] &&
                    other.m_maskData[y * other.m_width + x])
                    ++overlapCount;
        }
    return overlapCount;
}

Mask Mask::getOverlapMask(const Mask& other, const Vec2& offset) const
{
    const auto xOffset = static_cast<int>(std::round(offset.x));
    const auto yOffset = static_cast<int>(std::round(offset.y));

    const int xStart = std::max(0, -xOffset);
    const int yStart = std::max(0, -yOffset);
    const int xEnd = std::min(m_width, other.m_width - xOffset);
    const int yEnd = std::min(m_height, other.m_height - yOffset);

    // No overlap
    if (xStart >= xEnd || yStart >= yEnd)
        return {};

    Mask overlapMask({xEnd - xStart, yEnd - yStart}, false);

    for (int y = yStart; y < yEnd; ++y)
        for (int x = xStart; x < xEnd; ++x)
            if (getAt({x, y}) && other.getAt({x + xOffset, y + yOffset}))
                overlapMask.setAt({x - xStart, y - yStart}, true);

    return overlapMask;
}

void Mask::fill()
{
    std::fill(m_maskData.begin(), m_maskData.end(), true);
}

void Mask::clear()
{
    std::fill(m_maskData.begin(), m_maskData.end(), false);
}

void Mask::invert()
{
    m_maskData.flip();
}

void Mask::add(const Mask& other, const Vec2& offset)
{
    const auto xOffset = static_cast<int>(std::round(offset.x));
    const auto yOffset = static_cast<int>(std::round(offset.y));

    for (int y = 0; y < other.m_height; ++y)
        for (int x = 0; x < other.m_width; ++x)
        {
            const int targetX = x + xOffset;
            const int targetY = y + yOffset;
            if (targetX >= 0 && targetX < m_width && targetY >= 0 && targetY < m_height)
                m_maskData[targetY * m_width + targetX] = m_maskData[targetY * m_width + targetX] ||
                                                          other.m_maskData[y * other.m_width + x];
        }
}

void Mask::subtract(const Mask& other, const Vec2& offset)
{
    const auto xOffset = static_cast<int>(std::round(offset.x));
    const auto yOffset = static_cast<int>(std::round(offset.y));

    for (int y = 0; y < other.m_height; ++y)
        for (int x = 0; x < other.m_width; ++x)
        {
            const int targetX = x + xOffset;
            const int targetY = y + yOffset;
            if (targetX >= 0 && targetX < m_width && targetY >= 0 && targetY < m_height)
                m_maskData[targetY * m_width + targetX] = m_maskData[targetY * m_width + targetX] &&
                                                          !other.m_maskData[y * other.m_width + x];
        }
}

int Mask::getCount() const
{
    return static_cast<int>(std::ranges::count(m_maskData, true));
}

Vec2 Mask::getCenterOfMass() const
{
    int sumX = 0, sumY = 0, count = 0;
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            if (m_maskData[y * m_width + x])
            {
                sumX += x;
                sumY += y;
                ++count;
            }

    if (count == 0)
        return {};

    return {static_cast<double>(sumX) / count, static_cast<double>(sumY) / count};
}

std::vector<Vec2> Mask::getOutline() const
{
    std::vector<Vec2> outlinePoints;
    const int directions[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                                  {1, 1},   {0, 1},  {-1, 1}, {-1, 0}};

    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
        {
            if (!m_maskData[y * m_width + x])
                continue;

            for (const auto& dir : directions)
            {
                const int nx = x + dir[0];
                const int ny = y + dir[1];
                if (nx < 0 || nx >= m_width || ny < 0 || ny >= m_height ||
                    !m_maskData[ny * m_width + nx])
                {
                    outlinePoints.emplace_back(x, y);
                    break;
                }
            }
        }

    return outlinePoints;
}

Rect Mask::getBoundingRect() const
{
    int minX = m_width, minY = m_height, maxX = -1, maxY = -1;

    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
        {
            if (!m_maskData[y * m_width + x])  // if pixel is opaque
                continue;

            if (x < minX)
                minX = x;
            if (x > maxX)
                maxX = x;
            if (y < minY)
                minY = y;
            if (y > maxY)
                maxY = y;
        }

    if (maxX == -1 || maxY == -1)
        return {};  // Empty mask

    return {
        static_cast<double>(minX), static_cast<double>(minY), static_cast<double>(maxX - minX + 1),
        static_cast<double>(maxY - minY + 1)
    };
}

bool Mask::collideMask(const Mask& other, const Vec2& offset) const
{
    const auto xOffset = static_cast<int>(std::round(offset.x));
    const auto yOffset = static_cast<int>(std::round(offset.y));

    const int xStart = std::max(0, -xOffset);
    const int yStart = std::max(0, -yOffset);
    const int xEnd = std::min(m_width, other.m_width - xOffset);
    const int yEnd = std::min(m_height, other.m_height - yOffset);

    if (xStart >= xEnd || yStart >= yEnd)
        return false;  // No overlap

    for (int y = yStart; y < yEnd; ++y)
        for (int x = xStart; x < xEnd; ++x)
            if (getAt({x, y}) && other.getAt({x + xOffset, y + yOffset}))
                return true;

    return false;
}

std::vector<Vec2> Mask::getCollisionPoints(const Mask& other, const Vec2& offset) const
{
    std::vector<Vec2> collisionPoints;
    const auto xOffset = static_cast<int>(std::round(offset.x));
    const auto yOffset = static_cast<int>(std::round(offset.y));

    const int xStart = std::max(0, -xOffset);
    const int yStart = std::max(0, -yOffset);
    const int xEnd = std::min(m_width, other.m_width - xOffset);
    const int yEnd = std::min(m_height, other.m_height - yOffset);

    if (xStart >= xEnd || yStart >= yEnd)
        return collisionPoints;  // No overlap

    for (int y = yStart; y < yEnd; ++y)
        for (int x = xStart; x < xEnd; ++x)
            if (getAt({x, y}) && other.getAt({x + xOffset, y + yOffset}))
                collisionPoints.emplace_back(x, y);

    return collisionPoints;
}

bool Mask::isEmpty() const
{
    return std::ranges::none_of(m_maskData, [](const bool v) { return v; });
}

int Mask::getWidth() const
{
    return m_width;
}

int Mask::getHeight() const
{
    return m_height;
}

std::unique_ptr<PixelArray> Mask::getPixelArray(const Color& color) const
{
    auto pixelArray = std::make_unique<PixelArray>(Vec2{m_width, m_height});
    SDL_Surface* surface = pixelArray->getSDL();
    if (!surface)
        throw std::runtime_error("Failed to create PixelArray surface");

    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
        {
            if (m_maskData[y * m_width + x])
                SDL_WriteSurfacePixel(surface, x, y, color.r, color.g, color.b, color.a);
            else
                SDL_WriteSurfacePixel(surface, x, y, 0, 0, 0, 0);
        }

    return pixelArray;
}

Mask Mask::copy() const
{
    Mask copy;
    copy.m_width = m_width;
    copy.m_height = m_height;
    copy.m_maskData = m_maskData;
    return copy;
}

namespace mask
{
void _bind(const py::module_& module)
{
    py::classh<Mask>(module, "Mask", R"doc(
A collision mask for pixel-perfect collision detection.

A Mask represents a 2D bitmap, typically used for precise collision detection based on 
non-transparent pixels.
    )doc")
        .def(py::init(), R"doc(
Create an empty mask with size (0, 0).
        )doc")
        .def(
            py::init<const Vec2&, bool>(), py::arg("size"), py::arg("filled") = false,
            R"doc(
Create a mask with specified size.

Args:
    size (Vec2): The size of the mask as (width, height).
    filled (bool): Whether to fill the mask with solid pixels. Defaults to False.
        )doc"
        )
        .def(
            py::init<const PixelArray&, uint8_t>(), py::arg("pixel_array"),
            py::arg("threshold") = 1,
            R"doc(
Create a mask from a pixel array based on alpha threshold.

Args:
    pixel_array (PixelArray): The source pixel array to create the mask from.
    threshold (int): Alpha threshold value (0-255). Pixels with alpha >= threshold are solid.

Raises:
    RuntimeError: If the pixel array is invalid.
        )doc"
        )

        .def_property_readonly("width", &Mask::getWidth, R"doc(
The width of the mask in pixels.
    )doc")
        .def_property_readonly("height", &Mask::getHeight, R"doc(
The height of the mask in pixels.
    )doc")
        .def_property_readonly("size", &Mask::getSize, R"doc(
The size of the mask as a Vec2.
    )doc")

        .def("copy", &Mask::copy, R"doc(
Create a copy of this mask.

Returns:
    Mask: A new Mask with the same dimensions and pixel data.
        )doc")
        .def("get_at", &Mask::getAt, py::arg("pos"), R"doc(
Get the pixel value at a specific position.

Args:
    pos (Vec2): The position to check.

Returns:
    bool: True if the pixel is solid (above threshold), False otherwise.
        )doc")
        .def("set_at", &Mask::setAt, py::arg("pos"), py::arg("value"), R"doc(
Set the pixel value at a specific position.

Args:
    pos (Vec2): The position to set.
    value (bool): The pixel value (True for solid, False for transparent).
        )doc")
        .def(
            "get_overlap_area",
            [](const Mask& self, const Mask& other, const py::object& offsetObj) -> int
            {
                if (offsetObj.is_none())
                    return self.getOverlapArea(other);

                try
                {
                    return self.getOverlapArea(other, offsetObj.cast<Vec2>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'offset', expected Vec2");
                }
            },
            py::arg("other"), py::arg("offset") = py::none(),
            R"doc(
Get the number of overlapping pixels between this mask and another.

Args:
    other (Mask): The other mask to check overlap with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    int: The number of overlapping solid pixels.
        )doc"
        )
        .def(
            "get_overlap_mask",
            [](const Mask& self, const Mask& other, const py::object& offsetObj) -> Mask
            {
                if (offsetObj.is_none())
                    return self.getOverlapMask(other, {});

                try
                {
                    return self.getOverlapMask(other, offsetObj.cast<Vec2>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'offset', expected Vec2");
                }
            },
            py::arg("other"), py::arg("offset") = py::none(),
            R"doc(
Get a mask representing the overlapping area between this mask and another.

Args:
    other (Mask): The other mask to check overlap with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    Mask: A new mask containing only the overlapping pixels.
        )doc"
        )
        .def("fill", &Mask::fill, R"doc(
Fill the entire mask with solid pixels.
        )doc")
        .def("clear", &Mask::clear, R"doc(
Clear the entire mask, setting all pixels to transparent.
        )doc")
        .def("invert", &Mask::invert, R"doc(
Invert all pixels in the mask.

Solid pixels become transparent and transparent pixels become solid.
        )doc")
        .def(
            "add",
            [](Mask& self, const Mask& other, const py::object& offsetObj) -> void
            {
                if (offsetObj.is_none())
                {
                    self.add(other, {});
                    return;
                }

                try
                {
                    self.add(other, offsetObj.cast<Vec2>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'offset', expected Vec2");
                }
            },
            py::arg("other"), py::arg("offset") = py::none(),
            R"doc(
Add another mask to this mask with an offset.

Performs a bitwise OR operation between the masks.

Args:
    other (Mask): The mask to add.
    offset (Vec2): Position offset for the other mask. Defaults to (0, 0).
        )doc"
        )
        .def(
            "subtract",
            [](Mask& self, const Mask& other, const py::object& offsetObj) -> void
            {
                if (offsetObj.is_none())
                {
                    self.subtract(other, {});
                    return;
                }

                try
                {
                    self.subtract(other, offsetObj.cast<Vec2>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'offset', expected Vec2");
                }
            },
            py::arg("other"), py::arg("offset") = py::none(),
            R"doc(
Subtract another mask from this mask with an offset.

Removes pixels where the other mask has solid pixels.

Args:
    other (Mask): The mask to subtract.
    offset (Vec2): Position offset for the other mask. Defaults to (0, 0).
        )doc"
        )
        .def("get_count", &Mask::getCount, R"doc(
Get the number of solid pixels in the mask.

Returns:
    int: The count of solid pixels.
        )doc")
        .def("get_center_of_mass", &Mask::getCenterOfMass, R"doc(
Calculate the center of mass of all solid pixels.

Returns:
    Vec2: The center of mass position. Returns (0, 0) if mask is empty.
        )doc")
        .def("get_outline", &Mask::getOutline, R"doc(
Get the outline points of the mask.

Returns a list of points that form the outline of all solid regions.

Returns:
    Vec2List: A list of outline points.
        )doc")
        .def("get_bounding_rect", &Mask::getBoundingRect, R"doc(
Get the bounding rectangle that contains all solid pixels.

Returns:
    Rect: The smallest rectangle containing all solid pixels. 
          Returns empty rect if mask has no solid pixels.
        )doc")
        .def(
            "collide_mask",
            [](const Mask& self, const Mask& other, const py::object& offsetObj) -> bool
            {
                if (offsetObj.is_none())
                    return self.collideMask(other, {});

                try
                {
                    return self.collideMask(other, offsetObj.cast<Vec2>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'offset', expected Vec2");
                }
            },
            py::arg("other"), py::arg("offset") = py::none(),
            R"doc(
Check collision between this mask and another mask with an offset.

Args:
    other (Mask): The other mask to test collision with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    bool: True if the masks collide, False otherwise.
        )doc"
        )
        .def(
            "get_collision_points",
            [](const Mask& self, const Mask& other,
               const py::object& offsetObj) -> std::vector<Vec2>
            {
                if (offsetObj.is_none())
                    return self.getCollisionPoints(other, {});

                try
                {
                    return self.getCollisionPoints(other, offsetObj.cast<Vec2>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'offset', expected Vec2");
                }
            },
            py::arg("other"), py::arg("offset") = py::none(),
            R"doc(
Get all points where this mask collides with another mask.

Args:
    other (Mask): The other mask to test collision with.
    offset (Vec2): Position offset between the masks. Defaults to (0, 0).

Returns:
    Vec2List: A list of collision points.
        )doc"
        )
        .def("is_empty", &Mask::isEmpty, R"doc(
Check if the mask contains no solid pixels.

Returns:
    bool: True if the mask is empty, False otherwise.
        )doc")
        .def(
            "get_pixel_array",
            [](const Mask& self, const py::object& colorObj) -> std::unique_ptr<PixelArray>
            {
                if (colorObj.is_none())
                    return self.getPixelArray();

                try
                {
                    return self.getPixelArray(colorObj.cast<Color>());
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("Invalid type for 'color', expected Color");
                }
            },
            py::arg("color") = py::none(),
            R"doc(
Convert the mask to a pixel array with the specified color.

Solid pixels become the specified color, transparent pixels become transparent.

Args:
    color (Color): The color to use for solid pixels. Defaults to white (255, 255, 255, 255).

Returns:
    PixelArray: A new pixel array representation of the mask.

Raises:
    RuntimeError: If pixel array creation fails.
        )doc"
        )
        .def("get_rect", &Mask::getRect, R"doc(
Get the bounding rectangle of the mask starting at (0, 0).
    )doc");
}
}  // namespace mask
}  // namespace kn
