#include "Log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace kn::log
{
bool _loggerEnabled = false;

void _init()
{
    if (_loggerEnabled)
        return;

    auto console = spdlog::stdout_color_mt("console");
    auto file = spdlog::basic_logger_mt("file", "kn-debug.log");

    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::warn);

    spdlog::info("Logger initialized");

    _loggerEnabled = true;
}

void _bind(py::module_& module)
{
    auto subLog = module.def_submodule("log", "Logging utilities");

    subLog.def("info", [](const char* fmt) { spdlog::info(fmt); }, py::arg("message"), R"doc(
Log an informational message.

Args:
    message (str): The message to log.
        )doc");
    subLog.def("warn", [](const char* fmt) { spdlog::warn(fmt); }, py::arg("message"), R"doc(
Log a warning message.

Args:
    message (str): The message to log.
        )doc");
    subLog.def("error", [](const char* fmt) { spdlog::error(fmt); }, py::arg("message"), R"doc(
Log an error message.

Args:
    message (str): The message to log.
        )doc");
}
} // namespace kn::log
