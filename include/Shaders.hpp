#pragma once

#include <SDL3/SDL.h>

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>
#include <type_traits>
#include <vector>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

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

    template <typename StorageBufferType>
    void setStorageBufferData(const uint32_t binding, const StorageBufferType& data)
    {
        static_assert(
            std::is_trivially_copyable_v<StorageBufferType>,
            "Storage buffer data must be trivially copyable."
        );

        _setStorageBufferDataRaw(binding, &data, static_cast<uint32_t>(sizeof(StorageBufferType)));
    }

    template <typename UniformType>
    void setUniform(const uint32_t binding, const UniformType& data) const
    {
        static_assert(
            std::is_trivially_copyable_v<UniformType>, "Uniform data must be trivially copyable."
        );

        SDL_SetGPURenderStateFragmentUniforms(m_renderState, binding, &data, sizeof(UniformType));
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
    void _setStorageBufferDataRaw(uint32_t binding, const void* data, uint32_t len);

    friend void _quit();

#ifdef KRAKEN_ENABLE_PYTHON
    friend void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON
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

#ifdef KRAKEN_ENABLE_PYTHON
    friend void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON
};

}  // namespace shaders
}  // namespace kn
