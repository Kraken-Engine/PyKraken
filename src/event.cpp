#include "Gamepad.hpp"
#include "Key.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

#include "Event.hpp"
#include <SDL3/SDL.h>
#include <pybind11/stl.h>

namespace kn
{
Event::Event(const uint32_t type) : type(type) {}

py::object Event::getAttr(const std::string& name) const
{
    if (name == "type")
        return py::int_(type);
    if (data.contains(name))
        return data[name.c_str()];
    throw py::attribute_error("Attribute '" + name + "' not found");
}

namespace event
{
std::vector<Event> poll()
{
    gamepad::_clearStates();
    key::_clearStates();
    mouse::_clearStates();

    std::vector<Event> events;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        Event e(event.type);

        gamepad::_handleEvents(event, e);
        key::_handleEvents(event, e);
        mouse::_handleEvents(event, e);

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            window::close();
            break;
        case SDL_EVENT_DROP_FILE:
        case SDL_EVENT_DROP_TEXT:
            e.data["data"] = event.drop.data;
        case SDL_EVENT_DROP_BEGIN:
        case SDL_EVENT_DROP_COMPLETE:
        case SDL_EVENT_DROP_POSITION:
            e.data["x"] = event.drop.x;
            e.data["y"] = event.drop.y;
            break;
        case SDL_EVENT_AUDIO_DEVICE_ADDED:
        case SDL_EVENT_AUDIO_DEVICE_REMOVED:
        case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
            e.data["which"] = event.adevice.which;
            e.data["recording"] = event.adevice.recording;
            break;
        case SDL_EVENT_PEN_PROXIMITY_IN:
        case SDL_EVENT_PEN_PROXIMITY_OUT:
            e.data["which"] = event.pproximity.which;
            break;
        case SDL_EVENT_PEN_DOWN:
        case SDL_EVENT_PEN_UP:
            e.data["which"] = event.ptouch.which;
            e.data["x"] = event.ptouch.x;
            e.data["y"] = event.ptouch.y;
            e.data["eraser"] = event.ptouch.eraser;
            break;
        case SDL_EVENT_PEN_BUTTON_DOWN:
        case SDL_EVENT_PEN_BUTTON_UP:
            e.data["which"] = event.pbutton.which;
            e.data["button"] = event.pbutton.button;
            e.data["x"] = event.pbutton.x;
            e.data["y"] = event.pbutton.y;
            break;
        case SDL_EVENT_PEN_MOTION:
            e.data["which"] = event.pmotion.which;
            e.data["x"] = event.pmotion.x;
            e.data["y"] = event.pmotion.y;
            break;
        case SDL_EVENT_PEN_AXIS:
            e.data["which"] = event.paxis.which;
            e.data["x"] = event.paxis.x;
            e.data["y"] = event.paxis.y;
            e.data["axis"] = event.paxis.axis;
            e.data["value"] = event.paxis.value;
            break;
        case SDL_EVENT_CAMERA_DEVICE_ADDED:
        case SDL_EVENT_CAMERA_DEVICE_REMOVED:
        case SDL_EVENT_CAMERA_DEVICE_APPROVED:
        case SDL_EVENT_CAMERA_DEVICE_DENIED:
            e.data["which"] = event.cdevice.which;
            break;
        default:
            break;
        }

        events.push_back(std::move(e));
    }

    return events;
}

void _bind(py::module_& module)
{
    py::classh<Event>(module, "Event", R"doc(
Represents a single input event such as keyboard, mouse, or gamepad activity.

Attributes:
    type (int): Event type. Additional fields are accessed dynamically.
        )doc")

        .def_readonly("type", &Event::type, R"doc(
The event type (e.g., KEY_DOWN, MOUSE_BUTTON_UP).
        )doc")

        .def("__getattr__", &Event::getAttr, R"doc(
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
} // namespace event
} // namespace kn
