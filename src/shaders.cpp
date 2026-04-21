#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/ndarray.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <string>
#include <utility>

#include "Renderer.hpp"
#include "Shaders.hpp"
#include "Texture.hpp"
#include "_globals.hpp"
#include "tools/shader_baker.hpp"

static SDL_GPUFilter _toGPUFilter(const kn::FilterMode mode)
{
    switch (mode)
    {
    case kn::FilterMode::Nearest:
    case kn::FilterMode::PixelArt:
        return SDL_GPU_FILTER_NEAREST;
    case kn::FilterMode::Linear:
        return SDL_GPU_FILTER_LINEAR;
    default:
        return _toGPUFilter(kn::renderer::getDefaultFilterMode());
    }
}

static SDL_GPUSamplerAddressMode _toGPUWrap(const kn::WrapMode mode)
{
    switch (mode)
    {
    case kn::WrapMode::Repeat:
        return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    case kn::WrapMode::Mirror:
        return SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
    default:
        return SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    }
}

namespace kn::shaders
{
// Static registry to track all shader states for proper cleanup
static std::vector<Shader*> _shaderStates;

static void _registerShaderState(Shader* shader)
{
    if (std::find(_shaderStates.begin(), _shaderStates.end(), shader) == _shaderStates.end())
        _shaderStates.push_back(shader);
}

static void _unregisterShaderState(Shader* shader)
{
    auto it = std::find(_shaderStates.begin(), _shaderStates.end(), shader);
    if (it != _shaderStates.end())
        _shaderStates.erase(it);
}

Shader::Shader(
    const std::filesystem::path& fragmentBasePath, const uint32_t uniformBufferCount,
    const uint32_t samplerCount
)
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

    SDL_GPUShaderCreateInfo shaderInfo{
        .code_size = codeSize,
        .code = static_cast<const Uint8*>(code),
        .entrypoint = entrypoint,
        .format = shaderFormat,
        .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
        .num_samplers = samplerCount,
        .num_storage_textures = 0,  // Not usable yet
        .num_storage_buffers = 0,   // Not usable yet
        .num_uniform_buffers = uniformBufferCount,
        .props = 0,
    };

    m_samplerCount = samplerCount;
    m_samplerBindings.resize(samplerCount);

    m_fragShader = SDL_CreateGPUShader(renderer::_getGPUDevice(), &shaderInfo);
    if (!m_fragShader)
    {
        SDL_free(code);
        throw std::runtime_error("Failed to create shader: " + std::string(SDL_GetError()));
    }
    SDL_free(code);

    SDL_GPURenderStateCreateInfo renderStateInfo{
        .fragment_shader = m_fragShader,
        .num_sampler_bindings = 0,
        .sampler_bindings = nullptr,
        .num_storage_textures = 0,    // Not usable yet
        .storage_textures = nullptr,  // Not usable yet
        .num_storage_buffers = 0,     // Not usable yet
        .storage_buffers = nullptr,   // Not usable yet
        .props = 0,
    };
    m_renderState = SDL_CreateGPURenderState(renderer::_get(), &renderStateInfo);
    if (!m_renderState)
    {
        SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);
        m_fragShader = nullptr;
        throw std::runtime_error("Failed to create render state: " + std::string(SDL_GetError()));
    }

    _registerShaderState(this);
}

Shader::Shader(Shader&& other) noexcept
    : m_fragShader(other.m_fragShader),
      m_renderState(other.m_renderState),
      m_samplerCount(other.m_samplerCount),
      m_samplerBindings(std::move(other.m_samplerBindings))
{
    other.m_fragShader = nullptr;
    other.m_renderState = nullptr;
    other.m_samplerCount = 0;

    _unregisterShaderState(&other);
    if (m_renderState || m_fragShader)
        _registerShaderState(this);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        _unregisterShaderState(this);

        if (m_renderState)
            SDL_DestroyGPURenderState(m_renderState);
        if (m_fragShader)
            SDL_ReleaseGPUShader(renderer::_getGPUDevice(), m_fragShader);

        m_fragShader = other.m_fragShader;
        m_renderState = other.m_renderState;
        m_samplerCount = other.m_samplerCount;
        m_samplerBindings = std::move(other.m_samplerBindings);

        other.m_fragShader = nullptr;
        other.m_renderState = nullptr;
        other.m_samplerCount = 0;

        _unregisterShaderState(&other);
        if (m_renderState || m_fragShader)
            _registerShaderState(this);
    }

    return *this;
}

Shader::~Shader()
{
    _unregisterShaderState(this);

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

    if (m_samplerBindings.empty())
        return;

    if (!SDL_SetGPURenderStateSamplerBindings(
            m_renderState, static_cast<int>(m_samplerBindings.size()), m_samplerBindings.data()
        ))
        throw std::runtime_error("Failed to set sampler bindings: " + std::string(SDL_GetError()));
}

void Shader::unbind() const
{
    if (!SDL_SetGPURenderState(renderer::_get(), nullptr))
        throw std::runtime_error("Failed to unbind shader state: " + std::string(SDL_GetError()));
}

void Shader::setTextureSampler(
    const uint32_t binding, const Texture& texture, const Sampler& sampler
)
{
    if (!texture.hasUsage(TextureUsage::ShaderSampled))
        throw std::runtime_error("Texture is not usable as a shader resource");

    if (binding >= m_samplerCount)
        throw std::invalid_argument("Sampler binding index out of range");

    m_samplerBindings[binding] = {
        .texture = texture.getGPU(),
        .sampler = sampler.getSDL(),
    };
}

Sampler::Sampler(
    const FilterMode minFilter, const FilterMode magFilter, const WrapMode wrapU,
    const WrapMode wrapV
)
{
    SDL_GPUDevice* device = renderer::_getGPUDevice();
    if (!device)
        throw std::runtime_error("GPU device not initialized");

    SDL_GPUSamplerCreateInfo samplerInfo{
        .min_filter = _toGPUFilter(minFilter),
        .mag_filter = _toGPUFilter(magFilter),
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,  // Not usable yet
        .address_mode_u = _toGPUWrap(wrapU),
        .address_mode_v = _toGPUWrap(wrapV),
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,  // Not usable yet
        .mip_lod_bias = 0.0f,                                        // Not usable yet
        .max_anisotropy = 1.0f,                                      // Not usable yet
        .compare_op = SDL_GPU_COMPAREOP_NEVER,                       // Not usable yet
        .min_lod = 0.0f,                                             // Not usable yet
        .max_lod = 0.0f,                                             // Not usable yet
        .enable_anisotropy = false,                                  // Not usable yet
        .enable_compare = false,                                     // Not usable yet
        .props = 0,
    };

    m_sampler = SDL_CreateGPUSampler(device, &samplerInfo);
    if (!m_sampler)
        throw std::runtime_error("Failed to create sampler: " + std::string(SDL_GetError()));
}

Sampler::Sampler(Sampler&& other) noexcept
    : m_sampler(other.m_sampler)
{
    other.m_sampler = nullptr;
}

Sampler& Sampler::operator=(Sampler&& other) noexcept
{
    if (this != &other)
    {
        if (m_sampler)
            SDL_ReleaseGPUSampler(renderer::_getGPUDevice(), m_sampler);

        m_sampler = other.m_sampler;
        other.m_sampler = nullptr;
    }

    return *this;
}

Sampler::~Sampler()
{
    if (m_sampler)
    {
        SDL_ReleaseGPUSampler(renderer::_getGPUDevice(), m_sampler);
        m_sampler = nullptr;
    }
}

SDL_GPUSampler* Sampler::getSDL() const noexcept
{
    return m_sampler;
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

    nb::enum_<WrapMode>(subShaders, "WrapMode", R"doc(
Texture address mode used by shader samplers.
    )doc")
        .value("CLAMP", WrapMode::Clamp, "Clamp texture coordinates to the edge")
        .value("MIRROR", WrapMode::Mirror, "Mirror texture coordinates")
        .value("REPEAT", WrapMode::Repeat, "Repeat texture coordinates");

    subShaders.def("bake", &bake, "fragment_path"_a, "output_base_path"_a, R"doc(
Bake a shader from HLSL source to SPIR-V, DXIL, and MSL formats.

It is recommended to use the pykraken cli tool for shader baking rather than calling this directly.

Args:
    fragment_path (str): Base file name of the input HLSL shader.
    output_base_path (str): Base file name to use for the generated outputs. The generated files will have .spv, .dxil, and .msl extensions.
        )doc");

    nb::class_<Shader>(subShaders, "Shader", R"doc(
Encapsulates a GPU shader and its associated render state.
        )doc")
        .def(
            nb::init<const std::filesystem::path&, uint32_t, uint32_t>(), "fragment_base_path"_a,
            "uniform_buffer_count"_a = 0, "sampler_count"_a = 1, R"doc(
Create a Shader instance from a fragment shader file.

Args:
    fragment_base_path (str): Base file name of the fragment shader. The appropriate backend extension will be appended automatically.
    uniform_buffer_count (int, optional): Number of uniform buffers used by the shader. Default is 0.
    sampler_count (int, optional): Number of samplers used by the shader. Default is 1.

Raises:
    RuntimeError: If the shader cannot be loaded or created.
            )doc"
        )

        .def("bind", &Shader::bind, R"doc(
Binds this shader state to the current render pass, making it active for subsequent draw calls.

Raises:
    RuntimeError: If the shader state cannot be bound.
            )doc")
        .def("unbind", &Shader::unbind, R"doc(
Unbinds the current shader state, reverting to the default render state.
            )doc")
        .def(
            "set_uniform",
            [](const Shader& self, const Uint32 binding, nb::object data)
            {
                Py_buffer view;

                if (PyObject_GetBuffer(data.ptr(), &view, PyBUF_CONTIG_RO) != 0)
                    throw nb::type_error("Expected a buffer-compatible object for uniform data");

                try
                {
                    if (view.buf == nullptr || view.len < 0)
                        throw nb::type_error("Invalid buffer object");

                    SDL_SetGPURenderStateFragmentUniforms(
                        self.m_renderState, binding, view.buf, static_cast<uint32_t>(view.len)
                    );
                }
                catch (...)
                {
                    PyBuffer_Release(&view);
                    throw;
                }

                PyBuffer_Release(&view);
            },
            "binding"_a, "data"_a,
            nb::sig("def set_uniform(self, binding: int, data: collections.abc.Buffer, /) -> None"),
            R"doc(
Set uniform data for the fragment shader at the specified binding point.

Args:
    binding (int): Uniform buffer binding index.
    data (Buffer): Buffer-compatible object containing the uniform bytes.

Raises:
    TypeError: If the object does not provide compatible uniform data.
    RuntimeError: If the uniform data cannot be set.
            )doc"
        )
        .def(
            "set_texture_sampler", &Shader::setTextureSampler, "binding"_a, "texture"_a,
            "sampler"_a, R"doc(
Set the texture and sampler used for a fragment shader texture binding.

Args:
    binding (int): Sampler binding index.
    texture (Texture): Texture to bind. Must have the `ShaderSampled` usage flag.
    sampler (Sampler): Sampler to use for the texture.

Raises:
    RuntimeError: If the texture is not usable as a shader resource.
    ValueError: If the binding index is out of range.
            )doc"
        );

    nb::class_<Sampler>(subShaders, "Sampler", R"doc(
Encapsulates a GPU sampler object used by shaders.
        )doc")
        .def(
            nb::init<const FilterMode, const FilterMode, const WrapMode, const WrapMode>(),
            "min_filter"_a = FilterMode::Default, "mag_filter"_a = FilterMode::Default,
            "wrap_u"_a = WrapMode::Clamp, "wrap_v"_a = WrapMode::Clamp, R"doc(
Create a sampler with the requested filtering and wrapping modes.

Args:
    min_filter (FilterMode, optional): Minification filter. Default is renderer default.
    mag_filter (FilterMode, optional): Magnification filter. Default is renderer default.
    wrap_u (WrapMode, optional): Horizontal wrap mode. Default is clamp.
    wrap_v (WrapMode, optional): Vertical wrap mode. Default is clamp.

Raises:
    RuntimeError: If the sampler cannot be created.
            )doc"
        );
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn::shaders
