#include "Gamepad.hpp"
#include "Event.hpp"
#include "Math.hpp"

constexpr int MAX_GAMEPADS = 4;
static std::array<std::optional<SDL_JoystickID>, MAX_GAMEPADS> _gamepadSlots;
static std::unordered_map<SDL_JoystickID, GamepadState> _connectedPads;

static bool verifySlot(int slot);

namespace gamepad
{
void _bind(py::module_& module)
{
    auto subGamepad = module.def_submodule("gamepad", "Gamepad related functions");

    subGamepad.def("is_pressed", &isPressed, "Check if a gamepad button is pressed",
                   py::arg("button"), py::arg("id") = 0);
    subGamepad.def("is_just_pressed", &isJustPressed, "Check if a gamepad button was just pressed",
                   py::arg("button"), py::arg("id") = 0);
    subGamepad.def("is_just_released", &isJustReleased,
                   "Check if a gamepad button was just released", py::arg("button"),
                   py::arg("id") = 0);
    subGamepad.def("get_left_stick", &getLeftStick, "Get the left stick position",
                   py::arg("id") = 0);
    subGamepad.def("get_right_stick", &getRightStick, "Get the right stick position",
                   py::arg("id") = 0);
    subGamepad.def("get_left_trigger", &getLeftTrigger, "Get the left trigger value",
                   py::arg("id") = 0);
    subGamepad.def("get_right_trigger", &getRightTrigger, "Get the right trigger value",
                   py::arg("id") = 0);
    subGamepad.def("set_dead_zone", static_cast<void (*)(float, int)>(&setDeadZone),
                   "Set the dead zone for sticks", py::arg("deadZone"), py::arg("slot") = 0);
    subGamepad.def("get_dead_zone", static_cast<float (*)(int)>(&getDeadZone),
                   "Get the dead zone for sticks", py::arg("slot") = 0);
    subGamepad.def("get_connected_slots", &getConnectedSlots, "Get connected gamepad slots");
}

bool isPressed(SDL_GamepadButton button, int slot)
{
    if (!verifySlot(slot))
        return false;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return SDL_GetGamepadButton(state.pad, button);
}

bool isJustPressed(SDL_GamepadButton button, int slot)
{
    if (!verifySlot(slot))
        return false;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    auto buttonIt = state.justPressed.find(button);
    return buttonIt != state.justPressed.end();
}

bool isJustReleased(SDL_GamepadButton button, int slot)
{
    if (!verifySlot(slot))
        return false;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    auto buttonIt = state.justReleased.find(button);
    return buttonIt != state.justReleased.end();
}

math::Vec2 getLeftStick(int slot)
{
    if (!verifySlot(slot))
        return {};

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    math::Vec2 axes = {SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_LEFTX),
                       SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_LEFTY)};
    axes /= SDL_MAX_SINT16;
    if (axes.getLength() > state.deadZone)
        return axes;

    return {};
}

math::Vec2 getRightStick(int slot)
{
    if (!verifySlot(slot))
        return {};

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    math::Vec2 axes = {SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_RIGHTX),
                       SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_RIGHTY)};
    axes /= SDL_MAX_SINT16;
    if (axes.getLength() > state.deadZone)
        return axes;

    return {};
}

double getLeftTrigger(int slot)
{
    if (!verifySlot(slot))
        return 0.0;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) /
           static_cast<double>(SDL_MAX_SINT16);
}

double getRightTrigger(int slot)
{
    if (!verifySlot(slot))
        return 0.0;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    const GamepadState& state = _connectedPads.at(id);

    return SDL_GetGamepadAxis(state.pad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) /
           static_cast<double>(SDL_MAX_SINT16);
}

void setDeadZone(float deadZone, int slot)
{
    if (!verifySlot(slot))
        return;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
    GamepadState& state = _connectedPads.at(id);

    state.deadZone = deadZone;
}

float getDeadZone(int slot)
{
    if (!verifySlot(slot))
        return 0.1f;

    SDL_JoystickID id = _gamepadSlots.at(slot).value();
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
    for (auto& [id, state] : _connectedPads)
    {
        state.justPressed.clear();
        state.justReleased.clear();
    }
}

void _handleEvents(const SDL_Event& sdle, event::knEvent& e)
{
    switch (sdle.type)
    {
    case SDL_EVENT_GAMEPAD_ADDED:
    {
        SDL_Gamepad* pad = SDL_OpenGamepad(sdle.gdevice.which);
        if (pad)
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
        }
        break;
    }
    case SDL_EVENT_GAMEPAD_REMOVED:
    {
        SDL_JoystickID id = sdle.gdevice.which;
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
        break;
    }
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
    {
        SDL_JoystickID id = sdle.gbutton.which;
        if (_connectedPads.find(id) == _connectedPads.end())
            return;

        GamepadState& state = _connectedPads.at(id);
        auto button = static_cast<SDL_GamepadButton>(sdle.gbutton.button);

        bool cond = (sdle.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN);
        if (sdle.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
            state.justPressed[button] = true;
        else
            state.justReleased[button] = true;
        break;
    }
    }
}
} // namespace gamepad

bool verifySlot(int slot)
{
    if (slot < 0 || slot >= MAX_GAMEPADS)
    {
        throw std::out_of_range("Gamepad slot out of range");
        return false;
    }

    if (!_gamepadSlots.at(slot).has_value())
        return false; // No gamepad connected in this slot

    return true;
}
