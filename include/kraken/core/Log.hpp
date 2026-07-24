#pragma once

#include <spdlog/spdlog.h>

namespace kn::log
{
void enable();
void disable();

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
