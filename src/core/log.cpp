#include "kraken/core/Log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kn::log
{
bool _loggerEnabled = false;

void enable()
{
    if (_loggerEnabled)
    {
        warn("Logger already initialized");
        return;
    }

    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::level_enum::debug);
    spdlog::flush_on(spdlog::level::level_enum::warn);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    _loggerEnabled = true;
}

void disable()
{
    if (!_loggerEnabled)
        return;

    spdlog::set_default_logger(nullptr);
    spdlog::drop("console");

    _loggerEnabled = false;
}

}  // namespace kn::log
