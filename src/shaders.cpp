#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/ndarray.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Renderer.hpp"
#include "Shaders.hpp"
#include "tools/shader_baker.hpp"

namespace kn::shaders
{

// Static registry to track all shader states for proper cleanup
static std::vector<Shader*> _shaderStates;

Shader load(const std::filesystem::path& fragmentBasePath, uint32_t uniformBufferCount)
{
    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(renderer::_getGPUDevice());
    if (formats == SDL_GPU_SHADERFORMAT_INVALID)
        throw std::runtime_error(
            "Couldn't get supported shader formats: " + std::string(SDL_GetError())
        );

    SDL_GPUShaderFormat shaderFormat = SDL_GPU_SHADERFORMAT_INVALID;
    const char* entrypoint;
    std::string extension;

    if (formats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        shaderFormat = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
        extension = ".spv";
    }
    else if (formats & SDL_GPU_SHADERFORMAT_MSL)
    {
        shaderFormat = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
        extension = ".msl";
    }
    else if (formats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        shaderFormat = SDL_GPU_SHADERFORMAT_DXIL;
        entrypoint = "main";
        extension = ".dxil";
    }
    else
    {
        throw std::runtime_error("No supported shader formats available on this GPU.");
    }

    std::filesystem::path fullPath = fragmentBasePath.string() + extension;

    size_t codeSize;
    void* code = SDL_LoadFile(fullPath.string().c_str(), &codeSize);
    if (!code)
        throw std::runtime_error("Failed to load shader from disk: " + fullPath.string());

    SDL_GPUShaderCreateInfo shaderInfo{};
    shaderInfo.code_size = codeSize;
    shaderInfo.code = static_cast<const Uint8*>(code);
    shaderInfo.entrypoint = entrypoint;
    shaderInfo.format = shaderFormat;
    shaderInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    shaderInfo.num_samplers = 1;          // Not usable yet for more than 1 sampler
    shaderInfo.num_storage_textures = 0;  // Not usable yet
    shaderInfo.num_storage_buffers = 0;   // Not usable yet
    shaderInfo.num_uniform_buffers = uniformBufferCount;

    SDL_GPUShader* fragShader = SDL_CreateGPUShader(renderer::_getGPUDevice(), &shaderInfo);
    if (!fragShader)
    {
        SDL_free(code);
        throw std::runtime_error("Failed to create shader: " + std::string(SDL_GetError()));
    }

    SDL_free(code);

    SDL_GPURenderStateCreateInfo renderStateInfo{};
    renderStateInfo.fragment_shader = fragShader;
    SDL_GPURenderState* renderState = SDL_CreateGPURenderState(renderer::_get(), &renderStateInfo);
    if (!renderState)
    {
        SDL_ReleaseGPUShader(renderer::_getGPUDevice(), fragShader);
        throw std::runtime_error("Failed to create render state: " + std::string(SDL_GetError()));
    }

    return Shader(fragShader, renderState);
}

Shader::Shader(SDL_GPUShader* fragShader, SDL_GPURenderState* renderState)
    : m_fragShader(fragShader),
      m_renderState(renderState)
{
    // Register this shader for cleanup
    if (m_fragShader || m_renderState)
        _shaderStates.push_back(this);
}

Shader::Shader(Shader&& other) noexcept
    : m_fragShader(other.m_fragShader),
      m_renderState(other.m_renderState)
{
    // Steal pointers and nullify original
    other.m_fragShader = nullptr;
    other.m_renderState = nullptr;

    // Update address in global registry
    auto it = std::find(_shaderStates.begin(), _shaderStates.end(), &other);
    if (it != _shaderStates.end())
        *it = this;  // Swap old pointer for new address
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing GPU resources first
        if (m_renderState)
            SDL_DestroyGPURenderState(m_renderState);
        if (m_fragShader)
            SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);

        // Remove "other" from the registry to prevent dangling pointers.
        auto it = std::find(_shaderStates.begin(), _shaderStates.end(), &other);
        if (it != _shaderStates.end())
            _shaderStates.erase(it);

        // Steal pointers
        m_fragShader = other.m_fragShader;
        m_renderState = other.m_renderState;

        // Nullify original
        other.m_fragShader = nullptr;
        other.m_renderState = nullptr;
    }

    return *this;
}

Shader::~Shader()
{
    // Remove from registry if still present
    auto& shaders = _shaderStates;
    auto it = std::find(shaders.begin(), shaders.end(), this);
    if (it != shaders.end())
        shaders.erase(it);

    // Only clean up GPU resources if the device still exists
    // If _quit() was called, resources were already freed
    if (m_renderState)
    {
        SDL_DestroyGPURenderState(m_renderState);
        m_renderState = nullptr;
    }

    if (m_fragShader)
    {
        SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);
        m_fragShader = nullptr;
    }
}

void Shader::bind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), m_renderState))
        throw std::runtime_error("Failed to bind shader state: " + std::string(SDL_GetError()));
}

void Shader::unbind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), nullptr))
        throw std::runtime_error("Failed to unbind shader state: " + std::string(SDL_GetError()));
}

void _quit()
{
    // Clean up all shader states before GPU device is destroyed
    for (Shader* shader : _shaderStates)
    {
        if (shader->m_renderState)
        {
            SDL_DestroyGPURenderState(shader->m_renderState);
            shader->m_renderState = nullptr;
        }

        if (shader->m_fragShader)
        {
            SDL_ReleaseGPUShader(renderer::_getGPUDevice(), shader->m_fragShader);
            shader->m_fragShader = nullptr;
        }
    }

    _shaderStates.clear();
}

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subShaders = module.def_submodule("shaders");

    subShaders.def("bake", &bake, "fragment_path"_a, "output_base_path"_a, R"doc(
Bake a shader from HLSL source to SPIR-V, DXIL, and MSL formats.

Args:
    fragment_path (str): Base file name of the input HLSL shader.
    output_base_path (str): Base file name to use for the generated outputs. The generated files will have .spv, .dxil, and .msl extensions.
        )doc");

    nb::class_<Shader>(subShaders, "Shader", R"doc(
Encapsulates a GPU shader and its associated render state.
        )doc")
        .def("bind", &Shader::bind, R"doc(
Binds this shader state to the current render pass, making it active for subsequent draw calls.
            )doc")
        .def("unbind", &Shader::unbind, R"doc(
Unbinds the current shader state, reverting to the default render state.
            )doc")
        .def(
            "set_uniform",
            [](const Shader& self, const Uint32 binding,
               nb::ndarray<nb::c_contig, nb::device::cpu> dataBuf)
            {
                if (dataBuf.ndim() != 1)
                    throw std::runtime_error("Data must be a 1D buffer or bytes object");

                const void* ptr = dataBuf.data();
                const Uint32 nbytes = static_cast<Uint32>(dataBuf.nbytes());

                SDL_SetGPURenderStateFragmentUniforms(self.m_renderState, binding, ptr, nbytes);
            },
            "binding"_a, "data"_a, R"doc(
Set uniform data for the fragment shader at the specified binding point.

Args:
    binding (int): Uniform buffer binding index.
    data (buffer): Buffer or bytes object containing the uniform data to upload.
            )doc"
        );

    // Factory function to create a Shader from a fragment file path
    subShaders.def("load", &load, "fragment_base_path"_a, "uniform_buffer_count"_a = 0, R"doc(
Load a shader from disk and prepare its render state.

Args:
    fragment_base_path (str): Base file name of the fragment shader. The appropriate backend extension will be appended automatically.
    uniform_buffer_count (int, optional): Number of uniform buffers used by the shader. Default is 0.

Returns:
    Shader: A new Shader instance.

Raises:
    RuntimeError: If the shader cannot be loaded or created.
        )doc");
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn::shaders
