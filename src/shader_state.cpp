#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/ndarray.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <vector>

#include "Renderer.hpp"
#include "ShaderState.hpp"

namespace kn
{
// Static registry to track all shader states for proper cleanup
static std::vector<ShaderState*> _shaderStates;

ShaderState::ShaderState(
    const std::filesystem::path& fragmentFilePath, const Uint32 uniformBufferCount
)
{
    const char* ext = SDL_strrchr(fragmentFilePath.string().c_str(), '.');
    if (!ext)
        throw std::runtime_error("Shader file has no extension: " + fragmentFilePath.string());

    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(renderer::_getGPUDevice());
    if (formats == SDL_GPU_SHADERFORMAT_INVALID)
        throw std::runtime_error(
            "Couldn't get supported shader formats: " + std::string(SDL_GetError())
        );

    SDL_GPUShaderFormat shaderFormat = SDL_GPU_SHADERFORMAT_INVALID;
    const char* entrypoint;
    if (formats & SDL_GPU_SHADERFORMAT_SPIRV && SDL_strcmp(ext, ".spv") == 0)
    {
        shaderFormat = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
    }
    else if (formats & SDL_GPU_SHADERFORMAT_MSL && SDL_strcmp(ext, ".msl") == 0)
    {
        shaderFormat = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    }
    else if (formats & SDL_GPU_SHADERFORMAT_DXIL && SDL_strcmp(ext, ".dxil") == 0)
    {
        shaderFormat = SDL_GPU_SHADERFORMAT_DXIL;
        entrypoint = "main";
    }
    else
    {
        throw std::runtime_error(
            "Unknown shader extension " + std::string(ext) + " or unsupported format"
        );
    }

    size_t codeSize;
    void* code = SDL_LoadFile(fragmentFilePath.string().c_str(), &codeSize);
    if (!code)
        throw std::runtime_error("Failed to load shader from disk: " + fragmentFilePath.string());

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

    m_fragShader = SDL_CreateGPUShader(renderer::_getGPUDevice(), &shaderInfo);
    if (!m_fragShader)
    {
        SDL_free(code);
        throw std::runtime_error("Failed to create shader: " + std::string(SDL_GetError()));
    }

    SDL_free(code);

    SDL_GPURenderStateCreateInfo renderStateInfo{};
    renderStateInfo.fragment_shader = m_fragShader;
    m_renderState = SDL_CreateGPURenderState(renderer::_get(), &renderStateInfo);
    if (!m_renderState)
    {
        SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);
        throw std::runtime_error("Failed to create render state: " + std::string(SDL_GetError()));
    }

    // Register this shader state for cleanup
    _shaderStates.push_back(this);
}

ShaderState::~ShaderState()
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

void ShaderState::bind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), m_renderState))
        throw std::runtime_error("Failed to bind shader state: " + std::string(SDL_GetError()));
}

void ShaderState::unbind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), nullptr))
        throw std::runtime_error("Failed to unbind shader state: " + std::string(SDL_GetError()));
}

void ShaderState::setUniform(const Uint32 binding, const void* data, const size_t size) const
{
    SDL_SetGPURenderStateFragmentUniforms(m_renderState, binding, data, size);
}

namespace shader_state
{
void _quit()
{
    // Clean up all shader states before GPU device is destroyed
    for (ShaderState* shader : _shaderStates)
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

    nb::class_<ShaderState>(module, "ShaderState", R"doc(
Encapsulates a GPU shader and its associated render state.
        )doc")
        .def(
            nb::init<const std::filesystem::path&, Uint32>(), "fragment_file_path"_a,
            "uniform_buffer_count"_a = 0, R"doc(
Create a ShaderState from the specified fragment shader file.

Args:
    fragment_file_path (str | os.PathLike[str]): Path to the fragment shader file.
    uniform_buffer_count (int, optional): Number of uniform buffers used by the shader. Default is 0.
            )doc"
        )

        .def("bind", &ShaderState::bind, R"doc(
Binds this shader state to the current render pass, making it active for subsequent draw calls.
            )doc")
        .def("unbind", &ShaderState::unbind, R"doc(
Unbinds the current shader state, reverting to the default render state.
            )doc")
        .def(
            "set_uniform",
            [](const ShaderState& self, Uint32 binding,
               nb::ndarray<nb::c_contig, nb::device::cpu> dataBuf)
            {
                if (dataBuf.ndim() != 1)
                {
                    throw std::runtime_error("Data must be a 1D buffer or bytes object");
                }
                const void* ptr = dataBuf.data();
                const size_t nbytes = dataBuf.nbytes();
                self.setUniform(binding, ptr, nbytes);
            },
            "binding"_a, "data"_a, R"doc(
Set uniform data for the fragment shader at the specified binding point.

Args:
    binding (int): Uniform buffer binding index.
    data (buffer): Buffer or bytes object containing the uniform data to upload.
            )doc"
        );
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace shader_state
}  // namespace kn
