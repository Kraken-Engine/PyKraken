#include "kraken/geometry/Rect.hpp"

#include <string>

#include "kraken/geometry/Line.hpp"

namespace kn
{

Rect::Rect(const Vec2& size)
    : x(0),
      y(0),
      w(size.x),
      h(size.y)
{
}

Rect::Rect(const Vec2& pos, const Vec2& size)
    : x(pos.x),
      y(pos.y),
      w(size.x),
      h(size.y)
{
}

Rect Rect::copy() const
{
    return {x, y, w, h};
}

void Rect::move(const Vec2& offset)
{
    x += offset.x;
    y += offset.y;
}

void Rect::inflate(const Vec2& offset)
{
    x -= offset.x / 2.0;
    y -= offset.y / 2.0;
    w = std::max(0.0, w + offset.x);
    h = std::max(0.0, h + offset.y);
}

void Rect::fit(const Rect& other)
{
    if (other.w <= 0 || other.h <= 0)
        throw std::invalid_argument("Other rect must have positive width and height");

    const double scaleX = other.w / w;
    const double scaleY = other.h / h;
    const double scale = std::min(scaleX, scaleY);
    w *= scale;
    h *= scale;
    x = other.x + (other.w - w) / 2.0;
    y = other.y + (other.h - h) / 2.0;
}

void Rect::clamp(const Vec2& min, const Vec2& max)
{
    const auto minX = min.x;
    const auto minY = min.y;
    const auto maxX = max.x;
    const auto maxY = max.y;

    if (minX > maxX || minY > maxY)
        throw std::invalid_argument("Invalid min/max values: min must be less than max");

    if (w > maxX - minX || h > maxY - minY)
        throw std::invalid_argument("Rect size exceeds the given area");

    const double maxXPos = maxX - w;
    const double maxYPos = maxY - h;

    x = std::max(minX, std::min(x, maxXPos));
    y = std::max(minY, std::min(y, maxYPos));
}

void Rect::clamp(const Rect& other)
{
    clamp(other.getTopLeft(), other.getBottomRight());
}

void Rect::scaleBy(const double factor)
{
    if (factor <= 0)
        throw std::invalid_argument("Factor must be greater than 0");

    w *= factor;
    h *= factor;
}

void Rect::scaleBy(const Vec2& factor)
{
    const auto scaleX = factor.x;
    const auto scaleY = factor.y;

    if (scaleX <= 0 || scaleY <= 0)
        throw std::invalid_argument("Factor must be greater than 0");

    w *= scaleX;
    h *= scaleY;
}

void Rect::scaleTo(const Vec2& size)
{
    const auto width = size.x;
    const auto height = size.y;

    if (width <= 0.0)
        throw std::invalid_argument("Width must be greater than 0");
    if (height <= 0.0)
        throw std::invalid_argument("Height must be greater than 0");

    w = width;
    h = height;
}

bool Rect::operator==(const Rect& other) const
{
    return x == other.x && y == other.y && w == other.w && h == other.h;
}

bool Rect::operator!=(const Rect& other) const
{
    return !(*this == other);
}

Rect::operator SDL_Rect() const
{
    return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h)};
}

Rect::operator SDL_FRect() const
{
    return {
        static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)
    };
}

void Rect::setPos(const Vec2& pos)
{
    x = pos.x;
    y = pos.y;
}

void Rect::setSize(const Vec2& size)
{
    w = size.x;
    h = size.y;
}

void Rect::setLeft(const double left)
{
    this->x = left;
}

void Rect::setRight(const double right)
{
    this->x = right - w;
}

void Rect::setTop(const double top)
{
    this->y = top;
}

void Rect::setBottom(const double bottom)
{
    this->y = bottom - h;
}

void Rect::setTopLeft(const Vec2& topLeft)
{
    x = topLeft.x;
    y = topLeft.y;
}

void Rect::setTopMid(const Vec2& topMid)
{
    x = topMid.x - w / 2.0;
    y = topMid.y;
}

void Rect::setTopRight(const Vec2& topRight)
{
    x = topRight.x - w;
    y = topRight.y;
}

void Rect::setMidLeft(const Vec2& midLeft)
{
    x = midLeft.x;
    y = midLeft.y - h / 2.0;
}

void Rect::setCenter(const Vec2& center)
{
    x = center.x - w / 2.0;
    y = center.y - h / 2.0;
}

void Rect::setMidRight(const Vec2& midRight)
{
    x = midRight.x - w;
    y = midRight.y - h / 2.0;
}

void Rect::setBottomLeft(const Vec2& bottomLeft)
{
    x = bottomLeft.x;
    y = bottomLeft.y - h;
}

void Rect::setBottomMid(const Vec2& bottomMid)
{
    x = bottomMid.x - w / 2.0;
    y = bottomMid.y - h;
}

void Rect::setBottomRight(const Vec2& bottomRight)
{
    x = bottomRight.x - w;
    y = bottomRight.y - h;
}

Vec2 Rect::getPos() const
{
    return {x, y};
}

Vec2 Rect::getSize() const
{
    return {w, h};
}

double Rect::getLeft() const
{
    return x;
}

double Rect::getRight() const
{
    return x + w;
}

double Rect::getTop() const
{
    return y;
}

double Rect::getBottom() const
{
    return y + h;
}

Vec2 Rect::getTopLeft() const
{
    return {x, y};
}

Vec2 Rect::getTopMid() const
{
    return {x + w / 2.0, y};
}

Vec2 Rect::getTopRight() const
{
    return {x + w, y};
}

Vec2 Rect::getMidLeft() const
{
    return {x, y + h / 2.0};
}

Vec2 Rect::getCenter() const
{
    return {x + w / 2.0, y + h / 2.0};
}

Vec2 Rect::getMidRight() const
{
    return {x + w, y + h / 2.0};
}

Vec2 Rect::getBottomLeft() const
{
    return {x, y + h};
}

Vec2 Rect::getBottomMid() const
{
    return {x + w / 2.0, y + h};
}

Vec2 Rect::getBottomRight() const
{
    return {x + w, y + h};
}

Rect Rect::moved(const Vec2& offset) const
{
    Rect r = *this;
    r.move(offset);
    return r;
}

Rect Rect::clamped(const Vec2& min, const Vec2& max) const
{
    Rect r = *this;
    r.clamp(min, max);
    return r;
}

Rect Rect::clamped(const Rect& other) const
{
    Rect r = *this;
    r.clamp(other);
    return r;
}

Rect Rect::scaledBy(const double factor) const
{
    Rect r = *this;
    r.scaleBy(factor);
    return r;
}

Rect Rect::scaledBy(const Vec2& factor) const
{
    Rect r = *this;
    r.scaleBy(factor);
    return r;
}

Rect Rect::scaledTo(const Vec2& size) const
{
    Rect r = *this;
    r.scaleTo(size);
    return r;
}

std::array<Vec2, 4> Rect::getCorners() const
{
    return {getTopLeft(), getTopRight(), getBottomRight(), getBottomLeft()};
}

std::array<Line, 4> Rect::getEdges() const
{
    auto corners = getCorners();
    return {
        Line(corners[0], corners[1]), Line(corners[1], corners[2]), Line(corners[2], corners[3]),
        Line(corners[3], corners[0])
    };
}

}  // namespace kn
