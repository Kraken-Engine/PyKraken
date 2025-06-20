#include "Renderer.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#include <SDL3/SDL.h>

namespace renderer
{
void _bind(pybind11::module_& module)
{
    py::class_<Renderer>(module, "Renderer")
        .def(py::init(
                 [](const py::object& resObj)
                 {
                     math::Vec2 res;
                     if (py::isinstance<math::Vec2>(resObj))
                         res = resObj.cast<math::Vec2>();
                     else if (py::isinstance<py::sequence>(resObj))
                     {
                         const auto resSeq = resObj.cast<py::sequence>();
                         if (resSeq.size() != 2)
                             throw std::invalid_argument("Resolution sequence must be of length 2");
                         res = math::Vec2(resSeq[0].cast<double>(), resSeq[1].cast<double>());
                     }
                     else
                         throw std::invalid_argument(
                             "Invalid resolution type, expected Vec2 or sequence");

                     return Renderer(res);
                 }),
             py::arg("resolution"), "Create a Renderer with the specified resolution")
        .def(
            "clear",
            [](Renderer& self, const py::object& colorObj)
            {
                if (py::isinstance<Color>(colorObj))
                    self.clear(colorObj.cast<Color>());
                else if (py::isinstance<py::sequence>(colorObj))
                {
                    const auto colorSeq = colorObj.cast<py::sequence>();
                    if (colorSeq.size() != 3 || colorSeq.size() != 4)
                        throw std::invalid_argument("Color sequence must be of length 3 or 4");
                    self.clear(color::_fromSeq(colorSeq));
                }
                else
                    throw std::invalid_argument("Invalid color type, expected Color or Vec2");
            },
            py::arg("color") = py::cast<Color>({0, 0, 0, 255}),
            "Clear the renderer with the specified color")
        .def("present", &Renderer::present, "Present the rendered content")
        .def("draw", &Renderer::draw, py::arg("texture"), "Draw a texture to the renderer");
}
} // namespace renderer

Renderer::Renderer(const math::Vec2& resolution)
{
    if (resolution.x <= 0 || resolution.y <= 0)
        throw std::invalid_argument("Resolution must be greater than zero");

    m_renderer = SDL_CreateRenderer(window::getWindow(), nullptr);
    if (!m_renderer)
        throw std::runtime_error(SDL_GetError());

    if (!SDL_SetRenderLogicalPresentation(m_renderer, resolution.x, resolution.y,
                                          SDL_LOGICAL_PRESENTATION_LETTERBOX))
        throw std::runtime_error(SDL_GetError());

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

// Renderer::Renderer(const Renderer& other) { m_renderer = other.getSDL(); }

void Renderer::clear(const Color& color)
{
    if (!m_renderer)
        throw std::runtime_error("Renderer not initialized");

    if (!color._isValid())
        throw std::invalid_argument("Color values must be between 0 and 255");

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);
}

void Renderer::draw(const Texture& texture)
{
    if (!m_renderer)
        throw std::runtime_error("Renderer not initialized");

    SDL_Texture* sdlTexture = texture.getSDL();
    if (SDL_GetRendererFromTexture(sdlTexture) != m_renderer)
        throw std::runtime_error("Texture does not belong to this renderer");

    SDL_FRect sdlRect = texture.getRect();

    SDL_RenderTexture(m_renderer, sdlTexture, nullptr, &sdlRect);
}

void Renderer::present()
{
    if (!m_renderer)
        throw std::runtime_error("Renderer not initialized");

    SDL_RenderPresent(m_renderer);
}

SDL_Renderer* Renderer::getSDL() const
{
    if (!m_renderer)
        throw std::runtime_error("Renderer not initialized");

    return m_renderer;
}
