#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace kn::constants
{
void _bind(const nb::module_& module);
}  // namespace kn::constants
#endif  // KRAKEN_ENABLE_PYTHON
