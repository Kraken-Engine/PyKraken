#include "Input.hpp"
#include "Gamepad.hpp"
#include "Key.hpp"
#include "Math.hpp"
#include "Mouse.hpp"

#include <algorithm>
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
    py::class_<InputAction>(module, "InputAction")
        .def(py::init<SDL_Scancode>(), py::arg("scancode"), "Create an InputAction from a scancode")
        .def(py::init<KnKeycode>(), py::arg("keycode"), "Create an InputAction from a keycode")
        .def(py::init<knMouseButton>(), py::arg("mouse_button"),
             "Create an InputAction from a mouse button")
        .def(py::init<SDL_GamepadButton, int>(), py::arg("gamepad_button"), py::arg("slot") = 0,
             "Create an InputAction from a gamepad button and slot")
        .def(py::init<SDL_GamepadAxis, bool, int>(), py::arg("gamepad_axis"),
             py::arg("is_positive"), py::arg("slot") = 0,
             "Create an InputAction from a gamepad axis and slot");

    auto subInput = module.def_submodule("input", "Input handling related functions");

    subInput.def(
        "bind",
        [](const py::str& name, const py::sequence& actions) -> void
        {
            if (actions.size() == 0)
                throw std::invalid_argument("Input actions cannot be empty.");

            std::vector<InputAction> inputActions;
            inputActions.reserve(actions.size());
            for (const auto& action : actions)
            {
                if (!py::isinstance<InputAction>(action))
                    throw std::invalid_argument("All actions must be of type InputAction.");
                inputActions.push_back(py::cast<InputAction>(action));
            }

            bindInput(name.cast<std::string>(), inputActions);
        },
        "Bind input actions to a name", py::arg("name"), py::arg("actions"));
    subInput.def("unbind", &unbindInput, "Unbind input actions from a name", py::arg("name"));
    subInput.def("get_direction", &getDirection, "Get a direction vector based on input actions",
                 py::arg("up"), py::arg("right"), py::arg("down"), py::arg("left"));
    subInput.def("get_axis", &getAxis, "Get an axis value based on input actions",
                 py::arg("negative"), py::arg("positive"));
    subInput.def("is_pressed", &isPressed, "Check if an input action is pressed", py::arg("name"));
    subInput.def("is_just_pressed", &isJustPressed, "Check if an input action was just pressed",
                 py::arg("name"));
    subInput.def("is_just_released", &isJustReleased, "Check if an input action was just released",
                 py::arg("name"));
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

math::Vec2 getDirection(const std::string& up, const std::string& right, const std::string& down,
                        const std::string& left)
{
    math::Vec2 directionVec;
    const auto leftStick = gamepad::getLeftStick();
    const auto rightStick = gamepad::getRightStick();

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

    if (!directionVec.isZero())
        directionVec.normalize();

    return directionVec;
}

double getAxis(const std::string& negative, const std::string& positive)
{
    double axisValue = 0.0;
    const auto leftStick = gamepad::getLeftStick();
    const auto rightStick = gamepad::getRightStick();

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
