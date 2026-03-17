#pragma once

#include <nanobind/nanobind.h>
#include <spdlog/spdlog.h>

namespace nb = nanobind;

namespace kn::log
{
void _init();

void _bind(nb::module_& module);

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
