#pragma once

#include <SDL3/SDL.h>

#include <filesystem>
#include <type_traits>
#include <vector>

#include "kraken/core/_globals.hpp"

namespace kn
{
class Texture;

enum class WrapMode : uint8_t
{
    Clamp,
    Mirror,
    Repeat,
};

namespace shaders
{
class Shader;
class Sampler;

void _quit();

class Shader
{
  public:
    Shader() = delete;
    Shader(
        const std::filesystem::path& fragmentBasePath, uint32_t uniformBufferCount = 0,
        uint32_t samplerCount = 1, const std::vector<uint32_t>& storageBufferSizes = {}
    );
    ~Shader();

    // Move-Only
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void bind() const;
    void unbind() const;

    void setTextureSampler(const uint32_t binding, const Texture& texture, const Sampler& sampler);
    void setStorageBufferData(uint32_t binding, const void* data, uint32_t length);
    void setUniformData(uint32_t binding, const void* data, uint32_t length) const;

    template <typename StorageBufferType>
    void setStorageBufferData(const uint32_t binding, const StorageBufferType& data)
    {
        static_assert(
            std::is_trivially_copyable_v<StorageBufferType>,
            "Storage buffer data must be trivially copyable."
        );

        setStorageBufferData(binding, &data, static_cast<uint32_t>(sizeof(StorageBufferType)));
    }

    template <typename UniformType>
    void setUniform(const uint32_t binding, const UniformType& data) const
    {
        static_assert(
            std::is_trivially_copyable_v<UniformType>, "Uniform data must be trivially copyable."
        );

        setUniformData(binding, &data, static_cast<uint32_t>(sizeof(UniformType)));
    }

  private:
    SDL_GPUShader* m_fragShader = nullptr;
    SDL_GPURenderState* m_renderState = nullptr;

    uint32_t m_samplerCount = 0;
    std::vector<SDL_GPUTextureSamplerBinding> m_samplerBindings;

    uint32_t m_storageBufferCount = 0;
    std::vector<SDL_GPUBuffer*> m_storageBuffers;
    std::vector<uint32_t> m_storageBufferSizes;
    std::vector<SDL_GPUTransferBuffer*> m_storageTransferBuffers;

    void _releaseGPUResources() noexcept;
    void _moveFrom(Shader& other) noexcept;
    friend void _quit();
};

class Sampler
{
  public:
    Sampler() = delete;
    Sampler(
        const FilterMode minFilter = FilterMode::Default,
        const FilterMode magFilter = FilterMode::Default, const WrapMode wrapU = WrapMode::Clamp,
        const WrapMode wrapV = WrapMode::Clamp
    );
    ~Sampler();

    // Move-Only
    Sampler(const Sampler&) = delete;
    Sampler& operator=(const Sampler&) = delete;
    Sampler(Sampler&& other) noexcept;
    Sampler& operator=(Sampler&& other) noexcept;

    SDL_GPUSampler* getSDL() const noexcept;

  private:
    SDL_GPUSampler* m_sampler = nullptr;

    friend void _quit();
};

}  // namespace shaders
}  // namespace kn
