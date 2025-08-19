#include "Event.hpp"
#include "Gamepad.hpp"
#include "Key.hpp"
#include "Mouse.hpp"
#include "Window.hpp"
#include "_globals.hpp"

#include <SDL3/SDL.h>
#include <pybind11/stl.h>

namespace event
{

void _bind(py::module_& module)
{
    py::classh<knEvent>(module, "Event", R"doc(
Represents a single input event such as keyboard, mouse, or gamepad activity.

Attributes:
    type (int): Event type. Additional fields are accessed dynamically.
        )doc")

        .def_readonly("type", &knEvent::type, R"doc(
The event type (e.g., KEY_DOWN, MOUSE_BUTTON_UP).
        )doc")

        .def("__getattr__", &knEvent::getAttr, R"doc(
Dynamically access event attributes.

Examples:
    event.key
    event.button
    event.pos

Raises:
    AttributeError: If the requested attribute doesn't exist.
        )doc");

    auto subEvent = module.def_submodule("event", "Input event handling");

    subEvent.def("poll", &poll, R"doc(
Poll for all pending user input events.

This clears input states and returns a list of events that occurred since the last call.

Returns:
    list[Event]: A list of input event objects.
        )doc");
}

knEvent::knEvent(uint32_t type) : type(type) {}

std::vector<knEvent> poll()
{
    gamepad::_clearStates();
    key::_clearStates();
    mouse::_clearStates();

    std::vector<knEvent> events;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        knEvent e(event.type);

        gamepad::_handleEvents(event, e);
        key::_handleEvents(event, e);
        mouse::_handleEvents(event, e);

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            window::close();
            break;
        default:
            break;
        }

        events.push_back(std::move(e));
    }

    return events;
}

py::object knEvent::getAttr(const std::string& name) const
{
    if (name == "type")
        return py::int_(type);
    if (data.contains(name))
        return data[name.c_str()];
    throw py::attribute_error("Attribute '" + name + "' not found");
}
} // namespace event