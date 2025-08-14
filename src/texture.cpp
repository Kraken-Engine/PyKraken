#include "Texture.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "Surface.hpp"
#include "Window.hpp"

#include <SDL3_image/SDL_image.h>

namespace texture
{
void _bind(py::module_& module)
{
    py::class_<Texture> texture(module, "Texture", R"doc(
Represents a hardware-accelerated image that can be efficiently rendered.

Textures are optimized for fast rendering operations and support various effects
like rotation, flipping, tinting, alpha blending, and different blend modes.
They are created from image files or surfaces and must be associated with a renderer.
    )doc");

    py::class_<Texture::Flip>(texture, "Flip", R"doc(
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
        .def(py::init<const Surface&>(), py::arg("surface"), R"doc(
Create a Texture from an existing Surface.

Args:
    surface (Surface): The surface to convert to a texture.

Raises:
    RuntimeError: If texture creation from surface fails.
        )doc")

        .def_readwrite("angle", &Texture::angle, R"doc(
The rotation angle in degrees for rendering.

When the texture is drawn, it will be rotated by this angle around its center.
        )doc")
        .def_readwrite("flip", &Texture::flip, R"doc(
The flip settings for horizontal and vertical mirroring.

Controls whether the texture is flipped horizontally and/or vertically during rendering.
        )doc")

        .def(
            "get_size", [](Texture& self) -> py::tuple { return self.getSize(); }, R"doc(
Get the size of the texture.

Returns:
    tuple[float, float]: The texture size as (width, height).
        )doc")
        .def("get_rect", &Texture::getRect, R"doc(
Get a rectangle representing the texture bounds.

Returns:
    Rect: A rectangle with position (0, 0) and the texture's dimensions.
        )doc")
        .def("set_tint", &Texture::setTint, py::arg("color"), R"doc(
Set the color tint applied to the texture during rendering.

The tint color is multiplied with the texture's pixels, allowing for
color effects and lighting.

Args:
    color (Color): The tint color to apply.
        )doc")
        .def("get_tint", &Texture::getTint, R"doc(
Get the current color tint applied to the texture.

Returns:
    Color: The current tint color.
        )doc")
        .def("set_alpha", &Texture::setAlpha, py::arg("alpha"), R"doc(
Set the alpha (transparency) modulation for the texture.

Args:
    alpha (float): The alpha value, typically in range [0.0, 1.0] where
                  0.0 is fully transparent and 1.0 is fully opaque.
        )doc")
        .def("get_alpha", &Texture::getAlpha, R"doc(
Get the current alpha modulation value.

Returns:
    float: The current alpha value.
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

Texture::Texture(const Surface& surface)
{
    m_texPtr = SDL_CreateTextureFromSurface(renderer::get(), surface.getSDL());

    if (!m_texPtr)
    {
        throw std::runtime_error("Failed to create texture from surface: " +
                                 std::string(SDL_GetError()));
    }

    SDL_SetTextureScaleMode(m_texPtr, SDL_SCALEMODE_NEAREST);
}

Texture::Texture(const std::string& filePath)
{
    if (filePath.empty())
        throw std::invalid_argument("File path cannot be empty");

    if (m_texPtr)
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;
    }

    m_texPtr = IMG_LoadTexture(renderer::get(), filePath.c_str());
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
