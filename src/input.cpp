#include "Input.hpp"
#include "Gamepad.hpp"
#include "Key.hpp"
#include "Math.hpp"
#include "Mouse.hpp"

#include <algorithm>
#include <pybind11/stl.h>
#include <unordered_map>

template <class... Ts> struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace input
{
static std::unordered_map<std::string, std::vector<InputAction>> _inputBindings;

void _bind(py::module_& module)
{
    py::class_<InputAction>(module, "InputAction", R"doc(
Represents a single input trigger such as a key, mouse button, or gamepad control.
    )doc")

        .def(py::init<SDL_Scancode>(), py::arg("scancode"), R"doc(
Create an input action from a scancode.

Args:
    scancode (Scancode): Keyboard scancode.
        )doc")

        .def(py::init<KnKeycode>(), py::arg("keycode"), R"doc(
Create an input action from a keycode.

Args:
    keycode (Keycode): Keyboard keycode.
        )doc")

        .def(py::init<knMouseButton>(), py::arg("mouse_button"), R"doc(
Create an input action from a mouse button.

Args:
    mouse_button (MouseButton): Mouse button code.
        )doc")

        .def(py::init<SDL_GamepadButton, int>(), py::arg("gamepad_button"), py::arg("slot") = 0,
             R"doc(
Create an input action from a gamepad button.

Args:
    gamepad_button (GamepadButton): Gamepad button code.
    slot (int, optional): Gamepad slot (default is 0).
        )doc")

        .def(py::init<SDL_GamepadAxis, bool, int>(), py::arg("gamepad_axis"),
             py::arg("is_positive"), py::arg("slot") = 0, R"doc(
Create an input action from a gamepad axis direction.

Args:
    gamepad_axis (GamepadAxis): Gamepad axis code.
    is_positive (bool): True for positive direction, False for negative.
    slot (int, optional): Gamepad slot (default is 0).
        )doc");

    auto subInput = module.def_submodule("input", "Input handling and action binding");

    subInput.def("bind", &bindInput, py::arg("name"), py::arg("actions"), R"doc(
Bind a name to a list of InputActions.

Args:
    name (str): The identifier for this binding (e.g. "jump").
    actions (list[InputAction]): One or more InputActions to bind.
        )doc");

    subInput.def("unbind", &unbindInput, py::arg("name"), R"doc(
Unbind a previously registered input name.

Args:
    name (str): The binding name to remove.
        )doc");

    subInput.def("get_direction", &getDirection, py::arg("up"), py::arg("right"), py::arg("down"),
                 py::arg("left"), R"doc(
Get a directional vector based on named input actions.

This is typically used for WASD-style or D-pad movement.

Args:
    up (str): Name of action for upward movement.
    right (str): Name of action for rightward movement.
    down (str): Name of action for downward movement.
    left (str): Name of action for leftward movement.

Returns:
    Vec2: A normalized vector representing the intended direction.
        )doc");

    subInput.def("get_axis", &getAxis, py::arg("negative"), py::arg("positive"), R"doc(
Get a 1D axis value based on two opposing input actions.

Args:
    negative (str): Name of the negative direction action (e.g. "left").
    positive (str): Name of the positive direction action (e.g. "right").

Returns:
    float: Value in range [-1.0, 1.0] based on input.
        )doc");

    subInput.def("is_pressed", &isPressed, py::arg("name"), R"doc(
Check if the given action is currently being held.

Args:
    name (str): The name of the bound input.

Returns:
    bool: True if any action bound to the name is pressed.
        )doc");

    subInput.def("is_just_pressed", &isJustPressed, py::arg("name"), R"doc(
Check if the given action was just pressed this frame.

Args:
    name (str): The name of the bound input.

Returns:
    bool: True if pressed this frame only.
        )doc");

    subInput.def("is_just_released", &isJustReleased, py::arg("name"), R"doc(
Check if the given action was just released this frame.

Args:
    name (str): The name of the bound input.

Returns:
    bool: True if released this frame only.
        )doc");
}

InputAction::InputAction(const SDL_Scancode scan) : data(scan) {}

InputAction::InputAction(const KnKeycode key) : data(key) {}

InputAction::InputAction(const knMouseButton mButton) : data(mButton) {}

InputAction::InputAction(const SDL_GamepadButton cButton, const int slot)
    : data(cButton), padSlot(slot)
{
}

InputAction::InputAction(const SDL_GamepadAxis axis, const bool isPositive, const int slot)
    : data(std::make_pair(axis, isPositive)), padSlot(slot)
{
}

void bindInput(const std::string& name, const std::vector<InputAction>& actions)
{
    if (name.empty())
    {
        throw std::invalid_argument("Input name cannot be empty.");
        return;
    }

    _inputBindings[name] = actions;
}

void unbindInput(const std::string& name) { _inputBindings.erase(name); }

Vec2 getDirection(const std::string& up, const std::string& right, const std::string& down,
                  const std::string& left)
{
    Vec2 directionVec;
    const Vec2 leftStick = gamepad::getLeftStick();
    const Vec2 rightStick = gamepad::getRightStick();

    const auto processActions = [&](const std::string& name, double& axisValue, int direction)
    {
        const auto it = _inputBindings.find(name);
        if (it == _inputBindings.end())
            return;

        for (const auto& action : it->second)
        {
            std::visit(
                overloaded{
                    [&](SDL_Scancode scan)
                    {
                        if (key::isPressed(scan))
                            axisValue += direction;
                    },
                    [&](KnKeycode key)
                    {
                        if (key::isPressed(key))
                            axisValue += direction;
                    },
                    [&](knMouseButton mButton)
                    {
                        if (mouse::isPressed(mButton))
                            axisValue += direction;
                    },
                    [&](SDL_GamepadButton cButton)
                    {
                        if (gamepad::isPressed(cButton, action.padSlot))
                            axisValue += direction;
                    },
                    [&](const std::pair<SDL_GamepadAxis, bool>& axisPair)
                    {
                        auto [axis, isPositive] = axisPair;

                        auto process = [&](int a, double value)
                        {
                            if (axis == a &&
                                ((isPositive && value > 0) || (!isPositive && value < 0)))
                                axisValue += value;
                        };

                        process(SDL_GAMEPAD_AXIS_LEFTX, leftStick.x);
                        process(SDL_GAMEPAD_AXIS_LEFTY, leftStick.y);
                        process(SDL_GAMEPAD_AXIS_RIGHTX, rightStick.x);
                        process(SDL_GAMEPAD_AXIS_RIGHTY, rightStick.y);
                    },
                },
                action.data);
        }
    };

    processActions(up, directionVec.y, -1);
    processActions(right, directionVec.x, 1);
    processActions(down, directionVec.y, 1);
    processActions(left, directionVec.x, -1);

    directionVec.normalize();

    return directionVec;
}

double getAxis(const std::string& negative, const std::string& positive)
{
    double axisValue = 0.0;
    const Vec2 leftStick = gamepad::getLeftStick();
    const Vec2 rightStick = gamepad::getRightStick();

    const auto processActions = [&](const std::string& name, int direction)
    {
        const auto it = _inputBindings.find(name);
        if (it == _inputBindings.end())
            return;

        for (const auto& action : it->second)
        {
            std::visit(
                overloaded{
                    [&](SDL_Scancode scan)
                    {
                        if (key::isPressed(scan))
                            axisValue += direction;
                    },
                    [&](KnKeycode key)
                    {
                        if (key::isPressed(key))
                            axisValue += direction;
                    },
                    [&](knMouseButton mButton)
                    {
                        if (mouse::isPressed(mButton))
                            axisValue += direction;
                    },
                    [&](SDL_GamepadButton cButton)
                    {
                        if (gamepad::isPressed(cButton, action.padSlot))
                            axisValue += direction;
                    },
                    [&](const std::pair<SDL_GamepadAxis, bool>& axisPair)
                    {
                        auto [axis, isPositive] = axisPair;

                        auto process = [&](int a, double value)
                        {
                            if (axis == a &&
                                ((isPositive && value > 0) || (!isPositive && value < 0)))
                                axisValue += value * direction;
                        };

                        process(SDL_GAMEPAD_AXIS_LEFTX, leftStick.x);
                        process(SDL_GAMEPAD_AXIS_LEFTY, leftStick.y);
                        process(SDL_GAMEPAD_AXIS_RIGHTX, rightStick.x);
                        process(SDL_GAMEPAD_AXIS_RIGHTY, rightStick.y);
                    },
                },
                action.data);
        }
    };

    processActions(negative, -1);
    processActions(positive, 1);

    return std::clamp(axisValue, -1.0, 1.0);
}

bool isPressed(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return false;

    return std::any_of(
        it->second.begin(), it->second.end(),
        [](const InputAction& action)
        {
            return std::visit(
                overloaded{
                    [](SDL_Scancode scan) -> bool { return key::isPressed(scan); },
                    [](KnKeycode key) -> bool { return key::isPressed(key); },
                    [](knMouseButton mButton) -> bool { return mouse::isPressed(mButton); },
                    [](SDL_GamepadButton cButton) -> bool { return gamepad::isPressed(cButton); },
                    [](const std::pair<SDL_GamepadAxis, bool>& axisPair) -> bool { return false; },
                },
                action.data);
        });
}

bool isJustPressed(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return false;

    return std::any_of(
        it->second.begin(), it->second.end(),
        [](const InputAction& action)
        {
            return std::visit(
                overloaded{
                    [](SDL_Scancode scan) -> bool { return key::isJustPressed(scan); },
                    [](KnKeycode key) -> bool { return key::isJustPressed(key); },
                    [](knMouseButton mButton) -> bool { return mouse::isJustPressed(mButton); },
                    [](SDL_GamepadButton cButton) -> bool
                    { return gamepad::isJustPressed(cButton); },
                    [](const std::pair<SDL_GamepadAxis, bool>& axisPair) -> bool { return false; },
                },
                action.data);
        });
}

bool isJustReleased(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return false;

    return std::any_of(
        it->second.begin(), it->second.end(),
        [](const InputAction& action)
        {
            return std::visit(
                overloaded{
                    [](SDL_Scancode scan) -> bool { return key::isJustReleased(scan); },
                    [](KnKeycode key) -> bool { return key::isJustReleased(key); },
                    [](knMouseButton mButton) -> bool { return mouse::isJustReleased(mButton); },
                    [](SDL_GamepadButton cButton) -> bool
                    { return gamepad::isJustReleased(cButton); },
                    [](const std::pair<SDL_GamepadAxis, bool>& axisPair) -> bool { return false; },
                },
                action.data);
        });
}

} // namespace input
