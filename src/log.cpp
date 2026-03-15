#include "Log.hpp"

#include <nanobind/stl/string.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kn::log
{
bool _loggerEnabled = false;

void _init()
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

void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subLog = module.def_submodule("log", "Logging utilities");

    subLog.def("info", [](const std::string& fmt) { info("{}", fmt); }, "message"_a, R"doc(
Log an informational message.

Args:
    message (str): The message to log.
        )doc");
    subLog.def("warn", [](const std::string& fmt) { warn("{}", fmt); }, "message"_a, R"doc(
Log a warning message.

Args:
    message (str): The message to log.
        )doc");
    subLog.def("error", [](const std::string& fmt) { error("{}", fmt); }, "message"_a, R"doc(
Log an error message.

Args:
    message (str): The message to log.
        )doc");
}
}  // namespace kn::log
