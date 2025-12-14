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

bool start_text_input();

bool stop_text_input();

bool push(const Event& event);

bool schedule(const Event& event, uint32_t delay_ms, bool repeat = false);

bool unschedule(const Event& event);

Event newCustom();
} // namespace event
} // namespace kn
