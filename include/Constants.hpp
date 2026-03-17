#pragma once

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace kn::constants
{
void _bind(const nb::module_& module);
}  // namespace kn::constants
