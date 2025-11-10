#pragma once
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
struct Event
{
    uint32_t type;
    py::dict data;

    explicit Event(uint32_t type);

    [[nodiscard]] py::object getAttr(const std::string& name) const;
};

namespace event
{
void _bind(py::module_& module);

std::vector<Event> poll();

Event create_custom();
} // namespace event
} // namespace kn