#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <filesystem>
#include <string>

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
    ShaderState(const std::filesystem::path& fragmentFilePath, Uint32 uniformBufferCount = 0);
    ~ShaderState();

    void bind() const;
    void unbind() const;

    void setUniform(const Uint32 binding, const void* data, const size_t size) const;

  private:
    SDL_GPUShader* m_fragShader;
    SDL_GPURenderState* m_renderState;

    // Allow shader_state namespace to access private members for cleanup
    friend void shader_state::_quit();
};
}  // namespace kn
