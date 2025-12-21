#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn::constants
{
void _bind(const py::module_& module);
}  // namespace kn::constants
