#include "kraken/input/Event.hpp"

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <unordered_map>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/graphics/Window.hpp"
#include "kraken/input/Gamepad.hpp"
#include "kraken/input/Key.hpp"
#include "kraken/input/Mouse.hpp"

namespace kn::event
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Event>(module, "Event", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY), R"doc(
Represents a single input event such as keyboard, mouse, or gamepad activity.

Attributes:
    type (EventType | int): Event type. Built-in Kraken events are returned as
        EventType values. Custom user events are returned as integers.
        )doc")

        .def_prop_ro(
            "type",
            [](const Event& e) -> nb::object
            {
                if (e.type < SDL_EVENT_USER)
                    return nb::cast(static_cast<EventType>(e.type));
                return nb::int_(e.type);
            },
            nb::sig("def type(self) -> EventType | int"), R"doc(
The event type.

Built-in Kraken events are returned as EventType values.
Custom user events are returned as integers.
            )doc"
        )

        .def(
            "__getattr__",
            [](const Event& event, const std::string& name) -> nb::object
            {
                if (const auto it = event.data.find(name); it != event.data.end())
                {
                    return std::visit(
                        [](const auto& value) -> nb::object { return nb::cast(value); }, it->second
                    );
                }
                throw nb::attribute_error(("Attribute '" + name + "' not found").c_str());
            }
        );

    auto subEvent = module.def_submodule("event", "Input event handling");

    subEvent.def("poll", &poll, R"doc(
Poll for all pending user input events.

This clears input states and returns a list of events that occurred since the last call.

Returns:
    list[Event]: A list of input event objects.
        )doc");

    subEvent.def("new_custom", &newCustom, R"doc(
Create a new custom event type.

Returns:
    Event: The newly registered custom Event.

Raises:
    RuntimeError: If registration fails.
        )doc");

    subEvent.def("push", &push, "event"_a, R"doc(
Push a custom event to the event queue.

Args:
    event (Event): The custom event to push to the queue.

Raises:
    ValueError: If the event is not a custom event type.
    RuntimeError: If the event could not be queued.
        )doc");

    subEvent.def(
        "schedule", &schedule, "event"_a, "delay_ms"_a, "repeat"_a = false,
        R"doc(
Schedule a custom event to be pushed after a delay. Will overwrite any existing timer for the same event.

Args:
    event (Event): The custom event to schedule.
    delay_ms (int): Delay in milliseconds before the event is pushed.
    repeat (bool, optional): If True, the event will be pushed repeatedly at the
        specified interval. If False, the event is pushed only once. Defaults to False.

Raises:
    ValueError: If the event is not a custom event type.
    RuntimeError: If the timer could not be created.
        )doc"
    );

    subEvent.def("unschedule", &unschedule, "event"_a, R"doc(
Cancel a scheduled event timer.

Args:
    event (Event): The custom event whose timer should be cancelled.
        )doc");

    subEvent.def("start_text_input", &start_text_input, R"doc(
Start text input for TEXT_INPUT and TEXT_EDITING events.

Raises:
    RuntimeError: If text input could not be started.
        )doc");

    subEvent.def("stop_text_input", &stop_text_input, R"doc(
Stop text input for TEXT_INPUT and TEXT_EDITING events.

Raises:
    RuntimeError: If text input could not be stopped.
        )doc");
}
}  // namespace kn::event
