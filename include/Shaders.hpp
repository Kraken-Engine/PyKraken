#pragma once

#include <SDL3/SDL.h>

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>
#include <type_traits>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
namespace shaders
{
class Shader;

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

void _quit();

Shader load(const std::filesystem::path& fragmentBasePath, uint32_t uniformBufferCount = 0);

class Shader
{
  public:
    Shader() = delete;
    ~Shader();

    // Move-Only
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void bind() const;
    void unbind() const;

    template <typename UniformType>
    void setUniform(const uint32_t binding, const UniformType& data) const
    {
        static_assert(
            std::is_trivially_copyable_v<UniformType>, "Uniform data must be trivially copyable"
        );
        SDL_SetGPURenderStateFragmentUniforms(m_renderState, binding, &data, sizeof(UniformType));
    }

  private:
    Shader(SDL_GPUShader* m_fragShader, SDL_GPURenderState* m_renderState);

    SDL_GPUShader* m_fragShader;
    SDL_GPURenderState* m_renderState;

    friend void _quit();
    friend void _bind(nb::module_& module);

    friend Shader load(const std::filesystem::path& fragmentBasePath, uint32_t uniformBufferCount);
};
}  // namespace shaders
}  // namespace kn
