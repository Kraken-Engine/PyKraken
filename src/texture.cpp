#include "Texture.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "Window.hpp"

#include <SDL3_image/SDL_image.h>

namespace texture
{
void _bind(py::module_& module)
{
    py::class_<Texture>(module, "Texture")
        .def(py::init<const Renderer&, const std::string&>())
        .def("get_size", &Texture::getSize)
        .def("get_rect", &Texture::getRect)
        .def("set_tint",
             [](Texture& self, const py::object& tintObj)
             {
                 Color tint;

                 if (py::isinstance<Color>(tintObj))
                     tint = tintObj.cast<Color>();
                 else if (py::isinstance<py::sequence>(tintObj))
                 {
                     auto tintSeq = tintObj.cast<py::tuple>();
                     if (tintSeq.size() != 3 && tintSeq.size() != 4)
                         throw std::invalid_argument("Tint expected a 3 or 4-element sequence");

                     tint = {tintSeq[0].cast<uint8_t>(), tintSeq[1].cast<uint8_t>(),
                             tintSeq[2].cast<uint8_t>(),
                             tintSeq.size() == 4 ? tintSeq[3].cast<uint8_t>()
                                                 : static_cast<uint8_t>(255)};
                 }
                 else
                     throw std::runtime_error("Invalid tint type, expected Color or sequence.");

                 self.setTint(tint);
             })
        .def("get_tint", &Texture::getTint)
        .def("set_alpha", &Texture::setAlpha)
        .def("get_alpha", &Texture::getAlpha)
        .def("make_additive", &Texture::makeAdditive)
        .def("make_multiply", &Texture::makeMultiply)
        .def("make_normal", &Texture::makeNormal);
}
} // namespace texture

Texture::Texture(const Renderer& renderer, const std::string& filePath)
{
    SDL_Renderer* sdlRenderer = renderer.getSDL();

    if (filePath.empty())
        throw std::invalid_argument("File path cannot be empty");

    if (m_texPtr)
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;
    }

    m_texPtr = IMG_LoadTexture(sdlRenderer, filePath.c_str());
    if (!m_texPtr)
        throw std::runtime_error("Failed to load texture: " + std::string(SDL_GetError()));
}

Texture::Texture(SDL_Texture* sdlTexture) { this->loadFromSDL(sdlTexture); }

Texture::~Texture()
{
    if (m_texPtr)
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;
    }
}

void Texture::loadFromSDL(SDL_Texture* sdlTexture)
{
    if (m_texPtr)
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;
    }

    m_texPtr = sdlTexture;
}

py::tuple Texture::getSize() const
{
    float w, h;
    SDL_GetTextureSize(m_texPtr, &w, &h);
    return py::make_tuple(w, h);
}

Rect Texture::getRect() const
{
    float w, h;
    SDL_GetTextureSize(m_texPtr, &w, &h);
    return {0.f, 0.f, w, h};
}

void Texture::setTint(const Color& tint) const
{
    SDL_SetTextureColorMod(m_texPtr, tint.r, tint.g, tint.b);
}

Color Texture::getTint() const
{
    Color colorMod;
    SDL_GetTextureColorMod(m_texPtr, &colorMod.r, &colorMod.g, &colorMod.b);
    return colorMod;
}

void Texture::setAlpha(float alpha) const { SDL_SetTextureAlphaModFloat(m_texPtr, alpha); }

float Texture::getAlpha() const
{
    float alphaMod;
    SDL_GetTextureAlphaModFloat(m_texPtr, &alphaMod);
    return alphaMod;
}

void Texture::makeAdditive() const { SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_ADD); }

void Texture::makeMultiply() const { SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_MUL); }

void Texture::makeNormal() const { SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_BLEND); }

SDL_Texture* Texture::getSDL() const { return m_texPtr; }
