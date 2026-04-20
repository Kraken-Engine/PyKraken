#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>

#include "Math.hpp"
#include "Rect.hpp"
#include "_flagenum.hpp"
#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class PixelArray;
struct Color;

enum class TextureAccess
{
    Static = SDL_TEXTUREACCESS_STATIC,
    Target = SDL_TEXTUREACCESS_TARGET,
};

enum class TextureUsage : uint8_t
{
    Drawable = 1 << 0,       // Means SDL_Texture
    ShaderSampled = 1 << 1,  // Means SDL_GPUTexture
};
ENABLE_ENUM_FLAGS(TextureUsage)

class Texture
{
  public:
    struct Flip
    {
        bool h = false;
        bool v = false;
    } flip;

    Texture(
        int width, int height, FilterMode filter = FilterMode::Default,
        TextureUsage usage = TextureUsage::Drawable
    );
    Texture(
        const PixelArray& pixelArray, FilterMode filter = FilterMode::Default,
        TextureAccess access = TextureAccess::Static, TextureUsage usage = TextureUsage::Drawable
    );
    Texture(
        const std::filesystem::path& filePath, FilterMode filter = FilterMode::Default,
        TextureAccess access = TextureAccess::Static, TextureUsage usage = TextureUsage::Drawable
    );
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
    [[nodiscard]] Vec2 getSize() const;

    [[nodiscard]] Rect getRect() const;

    [[nodiscard]] Rect getClipArea() const;
    void setClipArea(const Rect& area);

    void setTint(const Color& tint) const;
    [[nodiscard]] Color getTint() const;

    void setAlpha(float alpha) const;
    [[nodiscard]] float getAlpha() const;

    void makeAdditive() const;
    void makeMultiply() const;
    void makeNormal() const;

    [[nodiscard]] TextureUsage getUsage() const;
    bool hasUsage(TextureUsage usage) const;

    [[nodiscard]] SDL_Texture* getSDL() const;
    [[nodiscard]] SDL_GPUTexture* getGPU() const;

  private:
    SDL_Texture* m_texPtr = nullptr;
    SDL_GPUTexture* m_gpuTexPtr = nullptr;

    TextureUsage m_usage;

    int m_width = 0;
    int m_height = 0;
    Rect m_clipArea{};

    void _createGPUTexture(SDL_Surface* surface);
    void _createTexture(SDL_Surface* surface, TextureAccess access, FilterMode filter);
    bool _isValidUsage(TextureUsage usage) const;
};

#ifdef KRAKEN_ENABLE_PYTHON
namespace texture
{
void _bind(const nb::module_& module);
}  // namespace texture
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn
