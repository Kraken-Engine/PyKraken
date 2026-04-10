#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif // KRAKEN_ENABLE_PYTHON

#include <string>
#include <vector>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif // KRAKEN_ENABLE_PYTHON

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
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif // KRAKEN_ENABLE_PYTHON

const std::vector<Event> poll();

void start_text_input();

void stop_text_input();

void push(const Event& event);

void schedule(const Event& event, uint32_t delay_ms, bool repeat = false);

void unschedule(const Event& event);

Event newCustom();
}  // namespace event
}  // namespace kn
