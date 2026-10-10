#include "kraken/input/Input.hpp"

#include <algorithm>
#include <unordered_map>

#include "kraken/input/Gamepad.hpp"
#include "kraken/input/Key.hpp"
#include "kraken/input/Mouse.hpp"
#include "kraken/math/Math.hpp"

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace kn
{
InputAction::InputAction(const Scancode scan)
    : data(scan)
{
}

InputAction::InputAction(const Keycode key)
    : data(key)
{
}

InputAction::InputAction(const MouseButton mButton)
    : data(mButton)
{
}

InputAction::InputAction(const GamepadButton cButton, const int slot)
    : data(cButton),
      padSlot(slot)
{
}

InputAction::InputAction(const GamepadAxis axis, const bool isPositive, const int slot)
    : data(std::make_pair(axis, isPositive)),
      padSlot(slot)
{
}

int InputAction::getPadSlot() const
{
    return padSlot;
}

InputAction::InputData InputAction::getData() const
{
    return data;
}

namespace input
{

struct InputBinding
{
    std::vector<InputAction> actions;
    std::vector<uint8_t> held;
    size_t heldCount = 0;
    bool justPressed = false;
    bool justReleased = false;
};

static std::unordered_map<std::string, InputBinding> _inputBindings;

void bind(const std::string& name, const std::vector<InputAction>& actions)
{
    if (name.empty())
        throw std::invalid_argument("Input name cannot be empty.");

    InputBinding binding{actions, std::vector<uint8_t>(actions.size())};
    for (size_t i = 0; i < actions.size(); ++i)
    {
        binding.held[i] = std::visit(
            overloaded{
                [](Scancode v) { return key::isPressed(v); }, [](Keycode v)
                { return key::isPressed(v); }, [](MouseButton v) { return mouse::isPressed(v); },
                [&](GamepadButton v) { return gamepad::isPressed(v, actions[i].getPadSlot()); },
                [](const std::pair<GamepadAxis, bool>&) { return false; }
            },
            actions[i].getData()
        );
        binding.heldCount += binding.held[i];
    }
    _inputBindings.insert_or_assign(name, std::move(binding));
}

void unbind(const std::string& name)
{
    _inputBindings.erase(name);
}

// Returns the maximum activation strength [0, 1] across all bindings for a given action.
// Digital inputs (keys, buttons) contribute 1.0 when active.
// Analog inputs contribute their absolute deflection in the matching direction.
static double getStrength(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return 0.0;

    double strength = 0.0;

    for (const auto& action : it->second.actions)
    {
        std::visit(
            overloaded{
                [&](const Scancode scan)
                {
                    if (key::isPressed(scan))
                        strength = std::max(strength, 1.0);
                },
                [&](const Keycode key)
                {
                    if (key::isPressed(key))
                        strength = std::max(strength, 1.0);
                },
                [&](const MouseButton mButton)
                {
                    if (mouse::isPressed(mButton))
                        strength = std::max(strength, 1.0);
                },
                [&](const GamepadButton cButton)
                {
                    if (gamepad::isPressed(cButton, action.getPadSlot()))
                        strength = std::max(strength, 1.0);
                },
                [&](const std::pair<GamepadAxis, bool>& axisPair)
                {
                    // TODO: Consider trigger axes
                    auto [axis, isPositive] = axisPair;

                    auto process = [&](const GamepadAxis a, const double value)
                    {
                        if (axis == a && ((isPositive && value > 0) || (!isPositive && value < 0)))
                            strength = std::max(strength, std::abs(value));
                    };

                    const Vec2 leftStick = gamepad::getLeftStick(action.getPadSlot());
                    const Vec2 rightStick = gamepad::getRightStick(action.getPadSlot());

                    process(GamepadAxis::LeftX, leftStick.x);
                    process(GamepadAxis::LeftY, leftStick.y);
                    process(GamepadAxis::RightX, rightStick.x);
                    process(GamepadAxis::RightY, rightStick.y);
                },
            },
            action.getData()
        );
    }

    return strength;
}

Vec2 getDirection(
    const std::string& up, const std::string& right, const std::string& down,
    const std::string& left
)
{
    Vec2 direction{
        getStrength(right) - getStrength(left),
        getStrength(down) - getStrength(up),
    };

    if (direction.getLengthSquared() > 1.0)
        direction.normalize();

    return direction;
}

double getAxis(const std::string& negative, const std::string& positive)
{
    return std::clamp(getStrength(positive) - getStrength(negative), -1.0, 1.0);
}

// A poll is one input frame. Edges latch until the next poll, including taps
// and release/repress sequences entirely within a single frame.
void _beginFrame()
{
    for (auto& [name, binding] : _inputBindings)
        binding.justPressed = binding.justReleased = false;
}

void _reset()
{
    for (auto& [name, binding] : _inputBindings)
    {
        std::fill(binding.held.begin(), binding.held.end(), 0);
        binding.heldCount = 0;
        binding.justPressed = binding.justReleased = false;
    }
}

void _handleEvent(const SDL_Event& event)
{
    const bool keyboard = event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP;
    const bool mouse = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                       event.type == SDL_EVENT_MOUSE_BUTTON_UP;
    const bool pad = event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ||
                     event.type == SDL_EVENT_GAMEPAD_BUTTON_UP;
    const bool removed = event.type == SDL_EVENT_GAMEPAD_REMOVED;
    const bool focusLost = event.type == SDL_EVENT_WINDOW_FOCUS_LOST;
    if ((!keyboard && !mouse && !pad && !removed && !focusLost) || (keyboard && event.key.repeat))
        return;
    const int slot = (pad || removed)
                         ? gamepad::_getSlot(removed ? event.gdevice.which : event.gbutton.which)
                         : -1;
    const bool down = event.type == SDL_EVENT_KEY_DOWN ||
                      event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                      event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN;
    for (auto& [name, binding] : _inputBindings)
    {
        const bool wasHeld = binding.heldCount != 0;
        for (size_t i = 0; i < binding.actions.size(); ++i)
        {
            const auto& action = binding.actions[i];
            const bool matches = std::visit(
                overloaded{
                    [&](Scancode v)
                    {
                        return focusLost ||
                               (keyboard && static_cast<SDL_Scancode>(v) == event.key.scancode);
                    },
                    [&](Keycode v)
                    {
                        return focusLost ||
                               (keyboard && static_cast<SDL_Keycode>(v) == event.key.key);
                    },
                    [&](MouseButton v)
                    {
                        return focusLost || (mouse && static_cast<Uint8>(v) == event.button.button);
                    },
                    [&](GamepadButton v)
                    {
                        return slot >= 0 && action.getPadSlot() == slot &&
                               (removed || (pad && static_cast<Uint8>(v) == event.gbutton.button));
                    },
                    [](const std::pair<GamepadAxis, bool>&) { return false; }
                },
                action.getData()
            );
            if (matches && binding.held[i] != down)
            {
                if (down)
                    ++binding.heldCount;
                else
                    --binding.heldCount;
                binding.held[i] = down;
            }
        }
        const bool held = binding.heldCount != 0;
        binding.justPressed |= !wasHeld && held;
        binding.justReleased |= wasHeld && !held;
    }
}

bool isPressed(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    return it != _inputBindings.end() && it->second.heldCount != 0;
}

bool isJustPressed(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    return it != _inputBindings.end() && it->second.justPressed;
}

bool isJustReleased(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    return it != _inputBindings.end() && it->second.justReleased;
}

}  // namespace input
}  // namespace kn
