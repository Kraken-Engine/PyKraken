#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON
#include <spdlog/spdlog.h>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn::log
{
void enable();
void disable();

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

extern bool _loggerEnabled;

template <typename... Args>
inline void info(fmt::format_string<Args...> fmt, Args&&... args)
{
    if (!_loggerEnabled)
        return;
    spdlog::info(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
inline void warn(fmt::format_string<Args...> fmt, Args&&... args)
{
    if (!_loggerEnabled)
        return;
    spdlog::warn(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
inline void error(fmt::format_string<Args...> fmt, Args&&... args)
{
    if (!_loggerEnabled)
        return;
    spdlog::error(fmt, std::forward<Args>(args)...);
}
}  // namespace kn::log
