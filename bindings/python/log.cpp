#include "kraken/core/Log.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "bindings/python/bindings.hpp"

namespace kn::log
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subLog = module.def_submodule("log", "Logging utilities");

    subLog.def("enable", &enable, R"doc(
Enable the logger.
    )doc");
    subLog.def("disable", &disable, R"doc(
Disable the logger.
    )doc");

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
