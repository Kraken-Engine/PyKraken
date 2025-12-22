#include "Texture.hpp"

#include <SDL3_image/SDL_image.h>
#include <pybind11/native_enum.h>

#include "Camera.hpp"
#include "Color.hpp"
#include "PixelArray.hpp"
#include "Renderer.hpp"

namespace kn
{
Texture::Texture(const Vec2& size, const TextureScaleMode scaleMode)
{
    const int width = static_cast<int>(size.x);
    const int height = static_cast<int>(size.y);

    if (width < 1 || height < 1)
        throw std::invalid_argument("Texture size values must be at least 1");

    m_texPtr = SDL_CreateTexture(
        renderer::_get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height
    );
    if (!m_texPtr)
    {
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
    }

    SDL_SetTextureScaleMode(m_texPtr, static_cast<SDL_ScaleMode>(scaleMode));

    m_width = size.x;
    m_height = size.y;
}

Texture::Texture(
    const PixelArray& pixelArray, const TextureScaleMode scaleMode, const TextureAccess access
)
{
    SDL_Surface* surface = pixelArray.getSDL();

    if (access == TextureAccess::STATIC)
    {
        m_texPtr = SDL_CreateTextureFromSurface(renderer::_get(), surface);
    }
    else if (access == TextureAccess::TARGET)
    {
        m_texPtr = SDL_CreateTexture(
            renderer::_get(), surface->format, SDL_TEXTUREACCESS_TARGET, surface->w, surface->h
        );

        if (!SDL_UpdateTexture(m_texPtr, nullptr, surface->pixels, surface->pitch))
        {
            SDL_DestroyTexture(m_texPtr);
            m_texPtr = nullptr;
            throw std::runtime_error(
                "Failed to copy PixelArray to texture: " + std::string(SDL_GetError())
            );
        }
    }

    if (!m_texPtr)
    {
        throw std::runtime_error(
            "Failed to create texture from PixelArray: " + std::string(SDL_GetError())
        );
    }

    SDL_SetTextureScaleMode(m_texPtr, static_cast<SDL_ScaleMode>(scaleMode));

    float w, h;
    SDL_GetTextureSize(m_texPtr, &w, &h);
    m_width = static_cast<double>(w);
    m_height = static_cast<double>(h);
}

Texture::Texture(
    const std::string& filePath, const TextureScaleMode scaleMode, const TextureAccess access
)
{
    if (filePath.empty())
        throw std::invalid_argument("File path cannot be empty");

    if (access == TextureAccess::STATIC)
    {
        m_texPtr = IMG_LoadTexture(renderer::_get(), filePath.c_str());
        if (!m_texPtr)
            throw std::runtime_error("Failed to load texture: " + std::string(SDL_GetError()));
    }
    else if (access == TextureAccess::TARGET)
    {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (!surface)
            throw std::runtime_error(
                "Failed to load image from file: " + std::string(SDL_GetError())
            );

        m_texPtr = SDL_CreateTexture(
            renderer::_get(), surface->format, SDL_TEXTUREACCESS_TARGET, surface->w, surface->h
        );
        if (!m_texPtr)
        {
            SDL_DestroySurface(surface);
            throw std::runtime_error(
                "Failed to create target texture: " + std::string(SDL_GetError())
            );
        }

        if (!SDL_UpdateTexture(m_texPtr, nullptr, surface->pixels, surface->pitch))
        {
            SDL_DestroyTexture(m_texPtr);
            SDL_DestroySurface(surface);
            m_texPtr = nullptr;
            throw std::runtime_error(
                "Failed to copy image to texture: " + std::string(SDL_GetError())
            );
        }

        SDL_DestroySurface(surface);
    }

    SDL_SetTextureScaleMode(m_texPtr, static_cast<SDL_ScaleMode>(scaleMode));

    float w, h;
    SDL_GetTextureSize(m_texPtr, &w, &h);
    m_width = static_cast<double>(w);
    m_height = static_cast<double>(h);
}

Texture::~Texture()
{
    if (m_texPtr)
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;
    }
}

double Texture::getWidth() const
{
    return m_width;
}

double Texture::getHeight() const
{
    return m_height;
}

Vec2 Texture::getSize() const
{
    return {m_width, m_height};
}

Rect Texture::getRect() const
{
    return {0.0, 0.0, m_width, m_height};
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

void Texture::setAlpha(const float alpha) const
{
    if (!SDL_SetTextureAlphaModFloat(m_texPtr, alpha))
    {
        throw std::runtime_error("Failed to set texture alpha mod: " + std::string(SDL_GetError()));
    }
}

float Texture::getAlpha() const
{
    float alphaMod;
    if (!SDL_GetTextureAlphaModFloat(m_texPtr, &alphaMod))
    {
        throw std::runtime_error("Failed to get texture alpha mod: " + std::string(SDL_GetError()));
    }
    return alphaMod;
}

void Texture::makeAdditive() const
{
    SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_ADD);
}

void Texture::makeMultiply() const
{
    SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_MUL);
}

void Texture::makeNormal() const
{
    SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_BLEND);
}

SDL_Texture* Texture::getSDL() const
{
    return m_texPtr;
}

namespace texture
{
void _bind(const py::module_& module)
{
    py::native_enum<TextureAccess>(module, "TextureAccess", "enum.IntEnum")
        .value("STATIC", TextureAccess::STATIC)
        .value("TARGET", TextureAccess::TARGET)
        .finalize();

    py::native_enum<TextureScaleMode>(module, "TextureScaleMode", "enum.IntEnum")
        .value("NEAREST", TextureScaleMode::NEAREST)
        .value("LINEAR", TextureScaleMode::LINEAR)
        .value("PIXEL_ART", TextureScaleMode::PIXELART)
        .finalize();

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
        .def(
            py::init(
                [](const std::string& filePath, const py::object& scaleModeObj,
                   const TextureAccess access) -> std::shared_ptr<Texture>
                {
                    TextureScaleMode mode = renderer::getDefaultScaleMode();
                    try
                    {
                        if (!scaleModeObj.is_none())
                            mode = scaleModeObj.cast<TextureScaleMode>();
                    }
                    catch (const py::cast_error&)
                    {
                        throw std::invalid_argument("Invalid scale mode provided");
                    }

                    return std::make_shared<Texture>(filePath, mode, access);
                }
            ),
            py::arg("file_path"), py::arg("scale_mode") = py::none(),
            py::arg("access") = TextureAccess::STATIC, R"doc(
Create a Texture by loading an image from a file.
If no scale mode is provided, the default renderer scale mode is used.

Args:
    file_path (str): Path to the image file to load.
    scale_mode (TextureScaleMode, optional): Scaling/filtering mode for the texture.
    access (TextureAccess, optional): Texture access type (STATIC or TARGET).

Raises:
    ValueError: If file_path is empty.
    RuntimeError: If the file cannot be loaded or texture creation fails.
        )doc"
        )
        .def(
            py::init(
                [](const PixelArray& pixelArray, const py::object& scaleModeObj,
                   const TextureAccess access) -> std::shared_ptr<Texture>
                {
                    TextureScaleMode mode = renderer::getDefaultScaleMode();
                    try
                    {
                        if (!scaleModeObj.is_none())
                            mode = scaleModeObj.cast<TextureScaleMode>();
                    }
                    catch (const py::cast_error&)
                    {
                        throw std::invalid_argument("Invalid scale mode provided");
                    }
                    return std::make_shared<Texture>(pixelArray, mode, access);
                }
            ),
            py::arg("pixel_array"), py::arg("scale_mode") = py::none(),
            py::arg("access") = TextureAccess::STATIC, R"doc(
Create a Texture from an existing PixelArray.
If no scale mode is provided, the default renderer scale mode is used.

Args:
    pixel_array (PixelArray): The pixel array to convert to a texture.
    scale_mode (TextureScaleMode, optional): Scaling/filtering mode for the texture.
    access (TextureAccess, optional): Texture access type (STATIC or TARGET).

Raises:
    RuntimeError: If texture creation from pixel array fails.
        )doc"
        )
        .def(
            py::init(
                [](const Vec2& size, const py::object& scaleModeObj) -> std::shared_ptr<Texture>
                {
                    TextureScaleMode mode = renderer::getDefaultScaleMode();
                    try
                    {
                        if (!scaleModeObj.is_none())
                            mode = scaleModeObj.cast<TextureScaleMode>();
                    }
                    catch (const py::cast_error&)
                    {
                        throw std::invalid_argument("Invalid scale mode provided");
                    }
                    return std::make_shared<Texture>(size, mode);
                }
            ),
            py::arg("size"), py::arg("scale_mode") = py::none(), R"doc(
Create a (render target) Texture with the specified size.
If no scale mode is provided, the default renderer scale mode is used.

Args:
    size (Vec2): The width and height of the texture.
    scale_mode (TextureScaleMode, optional): Scaling/filtering mode for the texture.

Raises:
    RuntimeError: If texture creation fails.
        )doc"
        )

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
}  // namespace texture
}  // namespace kn
