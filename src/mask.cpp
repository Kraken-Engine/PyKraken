#include "Mask.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Surface.hpp"

namespace mask
{
void _bind(py::module_& module)
{
    py::class_<Mask>(module, "Mask")
        .def(py::init<const Surface&, uint8_t>(), py::arg("surface"), py::arg("threshold") = 0)

        .def("collide_mask",
             py::overload_cast<const Mask&, const Vec2&>(&Mask::collideMask, py::const_))
        .def("collide_mask", py::overload_cast<const Mask&, const Rect&, const Rect&>(
                                 &Mask::collideMask, py::const_))
        .def("get_pixel", &Mask::getPixel);
}
} // namespace mask

Mask::Mask(const Surface& surface, const uint8_t threshold)
    : m_width(surface.getWidth()), m_height(surface.getHeight()),
      m_maskData(m_width * m_height, false)
{
    SDL_Surface* rawSurface = surface.getSDL();
    if (rawSurface == nullptr)
        throw std::runtime_error("Surface object internal pointer is null");

    const auto* pixels = static_cast<uint8_t*>(rawSurface->pixels);
    const int pitch = rawSurface->pitch;

    for (int y = 0; y < m_height; y++)
        for (int x = 0; x < m_height; x++)
        {
            const uint8_t* pixel = pixels + y * pitch + x * 4;
            m_maskData[y * m_width + x] = pixel[3] >= threshold;
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
            if (getPixel({x, y}) && other.getPixel({x + xOffset, y + yOffset}))
                return true;

    return false;
}

bool Mask::collideMask(const Mask& other, const Rect& rectA, const Rect& rectB) const
{
    return collideMask(other, rectA.getTopLeft() - rectB.getTopLeft());
}

bool Mask::getPixel(const Vec2& pos) const
{
    if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
        return false;

    return m_maskData[pos.y * m_width + pos.x];
}
