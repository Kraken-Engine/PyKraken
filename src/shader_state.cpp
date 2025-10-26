#include "Renderer.hpp"
#include "ShaderState.hpp"

namespace kn
{
ShaderState::ShaderState(const std::string& fragmentFilePath, Uint32 samplerCount,
                         Uint32 storageTextureCount, Uint32 storageBufferCount,
                         Uint32 uniformBufferCount)
{
    const char* ext = SDL_strrchr(fragmentFilePath.c_str(), '.');
    if (ext == nullptr)
    {
        SDL_Log("Warning: Shader file has no extension: %s", fragmentFilePath.c_str());
        throw std::runtime_error("Shader file has no extension");
    }

    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(renderer::_getGPUDevice());
    if (formats == SDL_GPU_SHADERFORMAT_INVALID)
    {
        SDL_Log("Couldn't get supported shader formats: %s", SDL_GetError());
        throw std::runtime_error("Couldn't get supported shader formats");
    }

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
        SDL_Log("Warning: Unknown shader extension '%s' or unsupported format", ext);
        throw std::runtime_error("Unknown shader extension or unsupported format");
    }

    size_t codeSize;
    void* code = SDL_LoadFile(fragmentFilePath.c_str(), &codeSize);
    if (code == nullptr)
    {
        SDL_Log("Failed to load shader from disk! %s", fragmentFilePath.c_str());
        throw std::runtime_error("Failed to load shader from disk");
    }

    SDL_GPUShaderCreateInfo shaderInfo{};
    shaderInfo.code_size = codeSize;
    shaderInfo.code = static_cast<const Uint8*>(code);
    shaderInfo.entrypoint = entrypoint;
    shaderInfo.format = shaderFormat;
    shaderInfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    shaderInfo.num_samplers = samplerCount;
    shaderInfo.num_storage_textures = storageTextureCount;
    shaderInfo.num_storage_buffers = storageBufferCount;
    shaderInfo.num_uniform_buffers = uniformBufferCount;

    m_fragShader = SDL_CreateGPUShader(renderer::_getGPUDevice(), &shaderInfo);
    if (m_fragShader == nullptr)
    {
        SDL_Log("Failed to create shader!");
        SDL_free(code);
        throw std::runtime_error("Failed to create shader");
    }

    SDL_free(code);

    SDL_GPURenderStateCreateInfo renderStateInfo{};
    renderStateInfo.fragment_shader = m_fragShader;
    m_renderState = SDL_CreateGPURenderState(renderer::_get(), &renderStateInfo);
    if (m_renderState == nullptr)
    {
        SDL_Log("Failed to create render state!");
        SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);
        throw std::runtime_error("Failed to create render state");
    }
}

ShaderState::~ShaderState()
{
    SDL_DestroyGPURenderState(m_renderState);
    SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);
}

void ShaderState::bind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), m_renderState))
    {
        throw std::runtime_error("Failed to bind shader state: " + std::string(SDL_GetError()));
    }
}

void ShaderState::unbind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), nullptr))
    {
        throw std::runtime_error("Failed to unbind shader state: " + std::string(SDL_GetError()));
    }
}

void ShaderState::setUniform(const Uint32 binding, const void* data, const size_t size) const
{
    SDL_SetGPURenderStateFragmentUniforms(m_renderState, binding, data, size);
}

namespace shader_state
{
void _bind(py::module_& module)
{
    py::class_<ShaderState>(module, "ShaderState",
                            "Encapsulates a GPU shader and its associated render state.")
        .def(py::init<const std::string&, Uint32, Uint32, Uint32, Uint32>(),
             py::arg("fragment_file_path"), py::arg("sampler_count"),
             py::arg("storage_texture_count"), py::arg("storage_buffer_count"),
             py::arg("uniform_buffer_count"))

        .def("bind", &ShaderState::bind, R"doc(
Binds this shader state to the current render pass, making it active for subsequent draw calls.
            )doc")
        .def("unbind", &ShaderState::unbind, R"doc(
Unbinds the current shader state, reverting to the default render state.
            )doc")
        .def(
            "set_uniform",
            [](const ShaderState& self, Uint32 binding, py::buffer dataBuf)
            {
                py::buffer_info info(dataBuf.request());
                if (info.ndim != 1)
                {
                    throw std::runtime_error("Data must be a 1D buffer or bytes object");
                }
                const void* ptr = info.ptr;
                const size_t nbytes = static_cast<size_t>(info.size * info.itemsize);
                self.setUniform(binding, ptr, nbytes);
            },
            py::arg("binding"), py::arg("data"), R"doc(
Sets uniform data for the fragment shader at the specified binding point.

Parameters:
    binding (int): The uniform buffer binding index.
    data (buffer): A buffer or bytes object containing the uniform data to upload.
            )doc");
}
} // namespace shader_state
} // namespace kn
