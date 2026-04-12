#pragma once

#include <SDL3/SDL.h>

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>
#include <string>
#include <type_traits>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
namespace shader_state
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

void _quit();
}  // namespace shader_state

class ShaderState
{
  public:
    ShaderState(const std::filesystem::path& fragmentFilePath, uint32_t uniformBufferCount = 0);
    ~ShaderState();

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
    SDL_GPUShader* m_fragShader;
    SDL_GPURenderState* m_renderState;

    // Allow shader_state namespace to access private members for cleanup
    friend void shader_state::_quit();

    // binding thing
    friend void shader_state::_bind(nb::module_& module);
};
}  // namespace kn
