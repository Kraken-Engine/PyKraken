#include "Gamepad.hpp"

#include <pybind11/stl.h>

#include <ranges>

#include "Event.hpp"
#include "Math.hpp"

namespace kn::gamepad
{
static bool verifySlot(int slot);

constexpr int MAX_GAMEPADS = 4;
static std::array<std::optional<SDL_JoystickID>, MAX_GAMEPADS> _gamepadSlots;
static std::unordered_map<SDL_JoystickID, GamepadState> _connectedPads;

bool isPressed(const SDL_GamepadButton button, const int slot)
{
    if (!verifySlot(slot))
        return false;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return SDL_GetGamepadButton(state.pad, button);
}

bool isJustPressed(const SDL_GamepadButton button, const int slot)
{
    if (!verifySlot(slot))
        return false;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return state.justPressed.contains(button);
}

bool isJustReleased(const SDL_GamepadButton button, const int slot)
{
    if (!verifySlot(slot))
        return false;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return state.justReleased.contains(button);
}

Vec2 getLeftStick(const int slot)
{
    if (!verifySlot(slot))
        return {};

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    Vec2 axes =
        {SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_LEFTX),
         SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_LEFTY)};
    axes /= SDL_MAX_SINT16;
    if (axes.getLength() > state.deadZone)
        return axes;

    return {};
}

Vec2 getRightStick(const int slot)
{
    if (!verifySlot(slot))
        return {};

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    Vec2 axes =
        {SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_RIGHTX),
         SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_RIGHTY)};
    axes /= SDL_MAX_SINT16;
    if (axes.getLength() > state.deadZone)
        return axes;

    return {};
}

double getLeftTrigger(const int slot)
{
    if (!verifySlot(slot))
        return 0.0;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) /
           static_cast<double>(SDL_MAX_SINT16);
}

double getRightTrigger(const int slot)
{
    if (!verifySlot(slot))
        return 0.0;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) /
           static_cast<double>(SDL_MAX_SINT16);
}

void setDeadZone(const float deadZone, const int slot)
{
    if (!verifySlot(slot))
        return;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    GamepadState& state = _connectedPads.at(id);

    state.deadZone = deadZone;
}

float getDeadZone(const int slot)
{
    if (!verifySlot(slot))
        return 0.1f;

    const SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return state.deadZone;
}

std::vector<int> getConnectedSlots()
{
    std::vector<int> slots;
    for (int i = 0; i < MAX_GAMEPADS; ++i)
    {
        if (_gamepadSlots.at(i).has_value())
        {
            slots.push_back(i);
        }
    }
    return slots;
}

void _clearStates()
{
    for (auto& state : _connectedPads | std::views::values)
    {
        state.justPressed.clear();
        state.justReleased.clear();
    }
}

void _handleEvents(const SDL_Event& sdlEvent, const Event& e)
{
    switch (sdlEvent.type)
    {
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        {
            const SDL_JoystickID id = sdlEvent.gaxis.which;
            if (!_connectedPads.contains(id))
                return;

            e.data["which"] = id;
            e.data["axis"] = sdlEvent.gaxis.axis;
            e.data["value"] = sdlEvent.gaxis.value;
            break;
        }
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
        {
            const SDL_JoystickID id = sdlEvent.gbutton.which;
            if (!_connectedPads.contains(id))
                return;

            GamepadState& state = _connectedPads.at(id);
            const auto button = static_cast<SDL_GamepadButton>(sdlEvent.gbutton.button);

            if (sdlEvent.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
                state.justPressed[button] = true;
            else
                state.justReleased[button] = true;

            e.data["which"] = id;
            e.data["button"] = button;
            break;
        }
        case SDL_EVENT_GAMEPAD_ADDED:
            if (SDL_Gamepad* pad = SDL_OpenGamepad(sdlEvent.gdevice.which))
            {
                SDL_JoystickID id = SDL_GetGamepadID(pad);

                for (int i = 0; i < MAX_GAMEPADS; ++i)
                {
                    if (!_gamepadSlots.at(i).has_value())
                    {
                        _gamepadSlots[i] = id;
                        _connectedPads[id].pad = pad;
                        break;
                    }
                }
                e.data["which"] = id;
            }
            break;
        case SDL_EVENT_GAMEPAD_REMOVED:
        {
            const SDL_JoystickID id = sdlEvent.gdevice.which;
            SDL_CloseGamepad(_connectedPads.at(id).pad);
            _connectedPads.erase(id);

            for (auto& slot : _gamepadSlots)
            {
                if (slot.has_value() && slot.value() == id)
                {
                    slot.reset();
                    break;
                }
            }
            e.data["which"] = id;
            break;
        }
        case SDL_EVENT_GAMEPAD_REMAPPED:
        case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
        case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
            e.data["which"] = sdlEvent.gdevice.which;
            break;
        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
        case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
            e.data["which"] = sdlEvent.gtouchpad.which;
            e.data["touchpad"] = sdlEvent.gtouchpad.touchpad;
            e.data["finger"] = sdlEvent.gtouchpad.finger;
            e.data["x"] = sdlEvent.gtouchpad.x;
            e.data["y"] = sdlEvent.gtouchpad.y;
            e.data["pressure"] = sdlEvent.gtouchpad.pressure;
            break;
        case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
            e.data["which"] = sdlEvent.gsensor.which;
            e.data["sensor"] = sdlEvent.gsensor.sensor;
            e.data["data"] = std::vector<float>{sdlEvent.gsensor.data, sdlEvent.gsensor.data + 3};
            e.data["timestamp"] = sdlEvent.gsensor.sensor_timestamp;
            break;
        default:
            break;
    }
}

bool verifySlot(const int slot)
{
    if (slot < 0 || slot >= MAX_GAMEPADS)
        throw std::out_of_range("Gamepad slot out of range");

    if (!_gamepadSlots.at(slot).has_value())
        return false;  // No gamepad connected in this slot

    return true;
}

void _bind(py::module_& module)
{
    auto subGamepad = module.def_submodule("gamepad", "Gamepad input handling functions");

    subGamepad.def("is_pressed", &isPressed, py::arg("button"), py::arg("slot") = 0, R"doc(
Check if a gamepad button is currently being held down.

Args:
    button (GamepadButton): The button code.
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    bool: True if the button is pressed.
    )doc");

    subGamepad.def(
        "is_just_pressed", &isJustPressed, py::arg("button"), py::arg("slot") = 0,
        R"doc(
Check if a gamepad button was pressed during this frame.

Args:
    button (GamepadButton): The button code.
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    bool: True if the button was just pressed.
    )doc"
    );

    subGamepad.def(
        "is_just_released", &isJustReleased, py::arg("button"), py::arg("slot") = 0,
        R"doc(
Check if a gamepad button was released during this frame.

Args:
    button (GamepadButton): The button code.
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    bool: True if the button was just released.
    )doc"
    );

    subGamepad.def("get_left_stick", &getLeftStick, py::arg("slot") = 0, R"doc(
Get the left analog stick position.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    Vec2: A vector of stick input normalized to [-1, 1], or (0, 0) if inside dead zone.
    )doc");

    subGamepad.def("get_right_stick", &getRightStick, py::arg("slot") = 0, R"doc(
Get the right analog stick position.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    Vec2: A vector of stick input normalized to [-1, 1], or (0, 0) if inside dead zone.
    )doc");

    subGamepad.def("get_left_trigger", &getLeftTrigger, py::arg("slot") = 0, R"doc(
Get the left trigger's current pressure value.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    float: Trigger value in range [0.0, 1.0].
    )doc");

    subGamepad.def("get_right_trigger", &getRightTrigger, py::arg("slot") = 0, R"doc(
Get the right trigger's current pressure value.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    float: Trigger value in range [0.0, 1.0].
    )doc");

    subGamepad.def("set_deadzone", &setDeadZone, py::arg("deadzone"), py::arg("slot") = 0, R"doc(
Set the dead zone threshold for a gamepad's analog sticks.

Args:
    deadzone (float): Value from 0.0 to 1.0 where movement is ignored.
    slot (int, optional): Gamepad slot ID (default is 0).
    )doc");

    subGamepad.def("get_deadzone", &getDeadZone, py::arg("slot") = 0, R"doc(
Get the current dead zone value for a gamepad's analog sticks.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    float: Deadzone threshold.
    )doc");

    subGamepad.def("get_connected_slots", &getConnectedSlots, R"doc(
Get a list of connected gamepad slot indices.

Returns:
    list[int]: A list of slot IDs with active gamepads.
    )doc");
}
}  // namespace kn::gamepad
