#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;

namespace kn
{
namespace shader_state
{
void _bind(py::module_& module);
void _quit();
} // namespace shader_state

class ShaderState
{
  public:
    ShaderState(const std::string& fragmentFilePath, Uint32 uniformBufferCount = 0,
                Uint32 samplerCount = 1);
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
} // namespace kn
