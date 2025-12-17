#include "Texture.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "PixelArray.hpp"
#include "Renderer.hpp"

#include <SDL3_image/SDL_image.h>

namespace kn
{
Texture::Texture(const PixelArray& pixelArray)
{
    m_texPtr = SDL_CreateTextureFromSurface(renderer::_get(), pixelArray.getSDL());

    if (!m_texPtr)
    {
        throw std::runtime_error("Failed to create texture from PixelArray: " +
                                 std::string(SDL_GetError()));
    }

    SDL_SetTextureScaleMode(m_texPtr, SDL_SCALEMODE_NEAREST);
}

Texture::Texture(const std::string& filePath)
{
    if (filePath.empty())
        throw std::invalid_argument("File path cannot be empty");

    m_texPtr = IMG_LoadTexture(renderer::_get(), filePath.c_str());
    if (!m_texPtr)
        throw std::runtime_error("Failed to load texture: " + std::string(SDL_GetError()));

    SDL_SetTextureScaleMode(m_texPtr, SDL_SCALEMODE_NEAREST);
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

Vec2 Texture::getSize() const
{
    float w, h;
    SDL_GetTextureSize(m_texPtr, &w, &h);
    return {w, h};
}

Rect Texture::getRect() const { return {0.0, 0.0, getSize()}; }

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

void Texture::setAlpha(const float alpha) const { SDL_SetTextureAlphaModFloat(m_texPtr, alpha); }

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

namespace texture
{
void _bind(const py::module_& module)
{
    py::classh<Texture> texture(module, "Texture", R"doc(
Represents a hardware-accelerated image that can be efficiently rendered.

Textures are optimized for fast rendering operations and support various effects
like rotation, flipping, tinting, alpha blending, and different blend modes.
They can be created from image files or pixel arrays.
    )doc");

    py::classh<Texture::Flip>(texture, "Flip", R"doc(
Controls horizontal and vertical flipping of a texture during rendering.

Used to mirror textures along the horizontal and/or vertical axes without
creating additional texture data.
    )doc")
        .def_readwrite("h", &Texture::Flip::h, R"doc(
Enable or disable horizontal flipping.

When True, the texture is mirrored horizontally (left-right flip).
        )doc")
        .def_readwrite("v", &Texture::Flip::v, R"doc(
Enable or disable vertical flipping.

When True, the texture is mirrored vertically (top-bottom flip).
        )doc");

    texture
        .def(py::init<const std::string&>(), py::arg("file_path"), R"doc(
Create a Texture by loading an image from a file.

Args:
    file_path (str): Path to the image file to load.

Raises:
    ValueError: If file_path is empty.
    RuntimeError: If the file cannot be loaded or texture creation fails.
        )doc")
        .def(py::init<const PixelArray&>(), py::arg("pixel_array"), R"doc(
Create a Texture from an existing PixelArray.

Args:
    pixel_array (PixelArray): The pixel array to convert to a texture.

Raises:
    RuntimeError: If texture creation from pixel array fails.
        )doc")

        .def_readwrite("flip", &Texture::flip, R"doc(
The flip settings for horizontal and vertical mirroring.

Controls whether the texture is flipped horizontally and/or vertically during rendering.
        )doc")

        .def_property("alpha", &Texture::getAlpha, &Texture::setAlpha, R"doc(
Get or set the alpha modulation of the texture as a float between `0.0` and `1.0`.
        )doc")
        .def_property("tint", &Texture::getTint, &Texture::setTint, R"doc(
Get or set the color tint applied to the texture during rendering.
        )doc")
        .def_property_readonly("size", &Texture::getSize, R"doc(
Get the dimensions of the texture as a Vec2 (width, height).
        )doc")

        .def("get_rect", &Texture::getRect, R"doc(
Get a rectangle representing the texture bounds.

Returns:
    Rect: A rectangle with position (0, 0) and the texture's dimensions.
        )doc")
        .def("make_additive", &Texture::makeAdditive, R"doc(
Set the texture to use additive blending mode.

In additive mode, the texture's colors are added to the destination,
creating bright, glowing effects.
        )doc")
        .def("make_multiply", &Texture::makeMultiply, R"doc(
Set the texture to use multiply blending mode.

In multiply mode, the texture's colors are multiplied with the destination,
creating darkening and shadow effects.
        )doc")
        .def("make_normal", &Texture::makeNormal, R"doc(
Set the texture to use normal (alpha) blending mode.

This is the default blending mode for standard transparency effects.
        )doc");
}
} // namespace texture
} // namespace kn
