#include "kraken/graphics/Shaders.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <algorithm>
#include <limits>
#include <string>
#include <utility>

#include "bindings/python/bindings.hpp"
#include "kraken/core/_globals.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Texture.hpp"
#include "tools/shader_baker.hpp"

namespace kn::shaders
{
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
            nb::init<
                const std::filesystem::path&, uint32_t, uint32_t, const std::vector<uint32_t>&>(),
            "fragment_base_path"_a, "uniform_buffer_count"_a = 0, "sampler_count"_a = 1,
            "storage_buffer_sizes"_a = nb::list{}, R"doc(
Create a Shader instance from a fragment shader file.

Args:
    fragment_base_path (str): Base file name of the fragment shader. The appropriate backend extension will be appended automatically.
    uniform_buffer_count (int, optional): Number of uniform buffers used by the shader. Default is 0.
    sampler_count (int, optional): Number of samplers used by the shader. Default is 1.
    storage_buffer_sizes (Sequence[int], optional): The storage buffer sizes in bytes used by the shader. Default is an empty sequence.

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
                    throw nb::type_error(
                        "Expected a buffer-compatible object for storage buffer data"
                    );

                try
                {
                    if (view.buf == nullptr || view.len < 0)
                        throw nb::type_error("Invalid buffer object");

                    self.setUniformData(binding, view.buf, static_cast<uint32_t>(view.len));
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
            "set_storage_buffer_data",
            [](Shader& self, const Uint32 binding, nb::object data)
            {
                Py_buffer view;

                if (PyObject_GetBuffer(data.ptr(), &view, PyBUF_CONTIG_RO) != 0)
                    throw nb::type_error("Expected a buffer-compatible object for uniform data");

                try
                {
                    if (view.buf == nullptr || view.len < 0)
                        throw nb::type_error("Invalid buffer object");

                    if (static_cast<unsigned long long>(view.len) >
                        std::numeric_limits<uint32_t>::max())
                        throw std::runtime_error("Storage buffer data is too large.");

                    self.setStorageBufferData(binding, view.buf, static_cast<uint32_t>(view.len));
                }
                catch (...)
                {
                    PyBuffer_Release(&view);
                    throw;
                }

                PyBuffer_Release(&view);
            },
            "binding"_a, "data"_a,
            nb::sig(
                "def set_storage_buffer_data(self, binding: int, data: collections.abc.Buffer, /) "
                "-> None"
            ),
            R"doc(
Sets the data for a data storage buffer for the fragment shader at the specified binding.

Currently untested with GLSL.

Args:
    binding (int): Shader storage buffer binding.
    data (Buffer): Buffer-compatible object containing the bytes to be given to the shader buffer.

Raises:
    TypeError: If the object does not provide compatible storage buffer data.
    RuntimeError: If the storage buffer data cannot be set.
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
}  // namespace kn::shaders
