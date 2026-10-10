#include "kraken/graphics/Texture.hpp"

#include <string>

#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Renderer.hpp"

namespace kn
{
static TextureUsage _kAllowedUsages = TextureUsage::Drawable | TextureUsage::ShaderSampled;

static SDL_ScaleMode _toSDLScaleMode(const FilterMode mode)
{
    switch (mode)
    {
    case FilterMode::Nearest:
        return SDL_SCALEMODE_NEAREST;
    case FilterMode::Linear:
        return SDL_SCALEMODE_LINEAR;
    case FilterMode::PixelArt:
        return SDL_SCALEMODE_PIXELART;
    default:
        return _toSDLScaleMode(renderer::getDefaultFilterMode());
    }
}

Texture::Texture(
    const int width, const int height, const FilterMode filter, const TextureUsage usage
)
{
    if (!_isValidUsage(usage))
        throw std::invalid_argument(
            "Invalid texture usage flags specified. Only Drawable and ShaderSampled are allowed."
        );

    if (width < 1 || height < 1)
        throw std::invalid_argument("Texture size values must be at least 1");

    m_texPtr = SDL_CreateTexture(
        renderer::_get(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height
    );
    if (!m_texPtr)
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));

    if (!SDL_SetTextureScaleMode(m_texPtr, _toSDLScaleMode(filter)))
        throw std::runtime_error(
            "Failed to set texture scale mode: " + std::string(SDL_GetError())
        );

    m_width = width;
    m_height = height;
    m_usage = usage;

    if (!SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_BLEND))
        throw std::runtime_error(
            "Failed to set texture blend mode: " + std::string(SDL_GetError())
        );
}

Texture::Texture(
    const PixelArray& pixelArray, const FilterMode filter, const TextureAccess access,
    const TextureUsage usage
)
{
    if (!_isValidUsage(usage))
        throw std::invalid_argument(
            "Invalid texture usage flags specified. Only Drawable and ShaderSampled are allowed."
        );

    SDL_Surface* surface = pixelArray.getSDL();
    SDL_Surface* uploadSurface = surface;
    SDL_Surface* keyedUploadSurface = nullptr;

    uint32_t colorKey = 0;
    if (SDL_GetSurfaceColorKey(surface, &colorKey))
    {
        keyedUploadSurface = SDL_CreateSurface(surface->w, surface->h, SDL_PIXELFORMAT_RGBA32);
        if (!keyedUploadSurface)
            throw std::runtime_error(
                "Failed to create color-key upload surface: " + std::string(SDL_GetError())
            );

        const uint32_t transparent = SDL_MapSurfaceRGBA(keyedUploadSurface, 0, 0, 0, 0);
        SDL_FillSurfaceRect(keyedUploadSurface, nullptr, transparent);

        if (!SDL_BlitSurface(surface, nullptr, keyedUploadSurface, nullptr))
        {
            SDL_DestroySurface(keyedUploadSurface);
            throw std::runtime_error(
                "Failed to apply PixelArray color key before texture upload: " +
                std::string(SDL_GetError())
            );
        }

        uploadSurface = keyedUploadSurface;
    }

    m_usage = usage;
    if (hasUsage(TextureUsage::Drawable))
        _createTexture(uploadSurface, access, filter);
    if (hasUsage(TextureUsage::ShaderSampled))
        _createGPUTexture(uploadSurface);

    if (keyedUploadSurface)
        SDL_DestroySurface(keyedUploadSurface);
}

Texture::Texture(
    const std::filesystem::path& filePath, const FilterMode filter, const TextureAccess access,
    const TextureUsage usage
)
    : Texture(PixelArray(filePath), filter, access, usage)
{
}

Texture::~Texture()
{
    if (m_texPtr)
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;
    }
    if (m_gpuTexPtr)
    {
        SDL_ReleaseGPUTexture(renderer::_getGPUDevice(), m_gpuTexPtr);
        m_gpuTexPtr = nullptr;
    }
}

Texture::Texture(Texture&& other) noexcept
    : m_texPtr(other.m_texPtr),
      m_gpuTexPtr(other.m_gpuTexPtr),
      m_usage(other.m_usage),
      m_width(other.m_width),
      m_height(other.m_height)
{
    other.m_texPtr = nullptr;
    other.m_gpuTexPtr = nullptr;
    other.m_usage = static_cast<TextureUsage>(0);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        if (m_texPtr)
            SDL_DestroyTexture(m_texPtr);
        if (m_gpuTexPtr)
            SDL_ReleaseGPUTexture(renderer::_getGPUDevice(), m_gpuTexPtr);

        m_width = other.m_width;
        m_height = other.m_height;
        m_texPtr = other.m_texPtr;
        m_gpuTexPtr = other.m_gpuTexPtr;
        m_usage = other.m_usage;
        other.m_texPtr = nullptr;
        other.m_gpuTexPtr = nullptr;
        other.m_usage = static_cast<TextureUsage>(0);
    }

    return *this;
}

bool Texture::_isValidUsage(TextureUsage usage) const
{
    constexpr TextureUsage none = static_cast<TextureUsage>(0);
    return usage != none && (usage & ~_kAllowedUsages) == none;
}

void Texture::_createTexture(
    SDL_Surface* surface, const TextureAccess access, const FilterMode filter
)
{
    const auto textureAccess = static_cast<SDL_TextureAccess>(access);

    m_texPtr = SDL_CreateTexture(
        renderer::_get(), SDL_PIXELFORMAT_RGBA32, textureAccess, surface->w, surface->h
    );
    if (!m_texPtr)
    {
        throw std::runtime_error(
            "Failed to create texture from PixelArray: " + std::string(SDL_GetError())
        );
    }

    if (!SDL_UpdateTexture(m_texPtr, nullptr, surface->pixels, surface->pitch))
    {
        SDL_DestroyTexture(m_texPtr);
        m_texPtr = nullptr;

        throw std::runtime_error(
            "Failed to copy PixelArray to texture: " + std::string(SDL_GetError())
        );
    }

    if (!SDL_SetTextureScaleMode(m_texPtr, _toSDLScaleMode(filter)))
    {
        throw std::runtime_error(
            "Failed to set texture scale mode: " + std::string(SDL_GetError())
        );
    }

    m_width = surface->w;
    m_height = surface->h;

    if (!SDL_SetTextureBlendMode(m_texPtr, SDL_BLENDMODE_BLEND))
    {
        throw std::runtime_error(
            "Failed to set texture blend mode: " + std::string(SDL_GetError())
        );
    }
}

void Texture::_createGPUTexture(SDL_Surface* surface)
{
    SDL_GPUDevice* device = renderer::_getGPUDevice();
    if (!device)
        throw std::runtime_error("No GPU device available");

    SDL_GPUTextureCreateInfo texInfo{
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GetGPUTextureFormatFromPixelFormat(surface->format),
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = static_cast<Uint32>(surface->w),
        .height = static_cast<Uint32>(surface->h),
        .layer_count_or_depth = 1,
        .num_levels = 1,
        .sample_count = SDL_GPU_SAMPLECOUNT_1,
        .props = 0,
    };
    SDL_GPUTexture* gpuTex = SDL_CreateGPUTexture(device, &texInfo);
    if (!gpuTex)
        throw std::runtime_error("Failed to create GPU texture: " + std::string(SDL_GetError()));

    SDL_GPUTransferBufferCreateInfo transBufInfo{
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = static_cast<Uint32>(surface->pitch * surface->h),
        .props = 0,
    };
    SDL_GPUTransferBuffer* transBuf = SDL_CreateGPUTransferBuffer(device, &transBufInfo);
    if (!transBuf)
    {
        SDL_ReleaseGPUTexture(device, gpuTex);
        throw std::runtime_error(
            "Failed to create GPU transfer buffer: " + std::string(SDL_GetError())
        );
    }

    void* mapped = SDL_MapGPUTransferBuffer(device, transBuf, false);
    if (!mapped)
    {
        SDL_ReleaseGPUTexture(device, gpuTex);
        SDL_ReleaseGPUTransferBuffer(device, transBuf);
        throw std::runtime_error(
            "Failed to map GPU transfer buffer: " + std::string(SDL_GetError())
        );
    }
    SDL_memcpy(mapped, surface->pixels, transBufInfo.size);
    SDL_UnmapGPUTransferBuffer(device, transBuf);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    if (!cmd)
    {
        SDL_ReleaseGPUTexture(device, gpuTex);
        SDL_ReleaseGPUTransferBuffer(device, transBuf);
        throw std::runtime_error(
            "Failed to acquire GPU command buffer: " + std::string(SDL_GetError())
        );
    }

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);
    if (!copyPass)
    {
        SDL_ReleaseGPUTexture(device, gpuTex);
        SDL_ReleaseGPUTransferBuffer(device, transBuf);
        throw std::runtime_error("Failed to begin GPU copy pass: " + std::string(SDL_GetError()));
    }

    const auto pixelsPerRow = static_cast<Uint32>(
        surface->pitch / SDL_BYTESPERPIXEL(surface->format)
    );
    SDL_GPUTextureTransferInfo src{
        .transfer_buffer = transBuf,
        .offset = 0,
        .pixels_per_row = pixelsPerRow,
        .rows_per_layer = static_cast<Uint32>(surface->h),
    };

    SDL_GPUTextureRegion dst{
        .texture = gpuTex,
        .mip_level = 0,
        .layer = 0,
        .x = 0,
        .y = 0,
        .z = 0,
        .w = static_cast<Uint32>(surface->w),
        .h = static_cast<Uint32>(surface->h),
        .d = 1,
    };

    SDL_UploadToGPUTexture(copyPass, &src, &dst, false);
    SDL_EndGPUCopyPass(copyPass);

    if (!SDL_SubmitGPUCommandBuffer(cmd))
    {
        SDL_ReleaseGPUTexture(device, gpuTex);
        SDL_ReleaseGPUTransferBuffer(device, transBuf);
        throw std::runtime_error(
            "Failed to submit GPU command buffer: " + std::string(SDL_GetError())
        );
    }

    SDL_ReleaseGPUTransferBuffer(device, transBuf);

    m_gpuTexPtr = gpuTex;
}

bool Texture::hasUsage(const TextureUsage usage) const
{
    return static_cast<uint8_t>(m_usage & usage) != 0;
}

int Texture::getWidth() const
{
    return m_width;
}

int Texture::getHeight() const
{
    return m_height;
}

Vec2 Texture::getSize() const
{
    return {m_width, m_height};
}

Rect Texture::getRect() const
{
    return {0, 0, m_width, m_height};
}

TextureUsage Texture::getUsage() const
{
    return m_usage;
}

SDL_Texture* Texture::getSDL() const
{
    return m_texPtr;
}

SDL_GPUTexture* Texture::getGPU() const
{
    return m_gpuTexPtr;
}

}  // namespace kn
