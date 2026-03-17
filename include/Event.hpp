#pragma once

#include <nanobind/nanobind.h>

#include <string>
#include <vector>

namespace nb = nanobind;

namespace kn
{
struct Event
{
    uint32_t type;
    nb::dict data;

    explicit Event(uint32_t type);

    [[nodiscard]] nb::object getAttr(const std::string& name) const;
};

namespace event
{
void _bind(nb::module_& module);

const std::vector<Event> poll();

void start_text_input();

void stop_text_input();

void push(const Event& event);

void schedule(const Event& event, uint32_t delay_ms, bool repeat = false);

void unschedule(const Event& event);

Event newCustom();
}  // namespace event
}  // namespace kn
