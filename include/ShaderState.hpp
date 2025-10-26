#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

namespace kn
{
class ShaderState
{
  public:
    ShaderState(const std::string& fragmentFilePath, Uint32 samplerCount,
                Uint32 storageTextureCount, Uint32 storageBufferCount, Uint32 uniformBufferCount);
    ~ShaderState();

    void bind() const;

    void unbind() const;

    void setUniform(const Uint32 binding, const void* data, const size_t size) const;

  private:
    SDL_GPUShader* m_fragShader;
    SDL_GPURenderState* m_renderState;
};
namespace shader_state
{
void _bind(py::module_& module);
} // namespace shader_state
} // namespace kn
