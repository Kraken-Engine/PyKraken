#include "kraken/graphics/Mask.hpp"

#include <algorithm>

#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/math/Math.hpp"

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

bool Mask::getAt(const int x, const int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return false;

    return m_maskData[static_cast<int>(y * m_width + x)];
}

void Mask::setAt(const int x, const int y, const bool value)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;

    m_maskData[static_cast<int>(y * m_width + x)] = value;
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
            if (getAt(x, y) && other.getAt(x + xOffset, y + yOffset))
                overlapMask.setAt(x - xStart, y - yStart, true);

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
            if (getAt(x, y) && other.getAt(x + xOffset, y + yOffset))
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
            if (getAt(x, y) && other.getAt(x + xOffset, y + yOffset))
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

PixelArray Mask::getPixelArray(const Color& color) const
{
    PixelArray pixelArray(m_width, m_height);

    SDL_Surface* surface = pixelArray.getSDL();
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

}  // namespace kn
