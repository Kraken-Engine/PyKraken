#include "Key.hpp"
#include "Event.hpp"
#include "_globals.hpp"

#include <unordered_map>

static bool _scancodePressed[SDL_SCANCODE_COUNT] = {};
static bool _scancodeReleased[SDL_SCANCODE_COUNT] = {};
static std::unordered_map<SDL_Keycode, bool> _keycodePressed;
static std::unordered_map<SDL_Keycode, bool> _keycodeReleased;

namespace key
{
void _bind(py::module_& module)
{
    auto subKey = module.def_submodule("key", "Keyboard key state checks");

    subKey.def("is_pressed", static_cast<bool (*)(SDL_Scancode)>(&isPressed), py::arg("scancode"),
               R"doc(
Check if a key is currently held down (by scancode).

Args:
    scancode (Scancode): The physical key (e.g., S_w).

Returns:
    bool: True if the key is held.
        )doc");

    subKey.def("is_just_pressed", static_cast<bool (*)(SDL_Scancode)>(&isJustPressed),
               py::arg("scancode"), R"doc(
Check if a key was pressed this frame (by scancode).

Args:
    scancode (Scancode): The physical key.

Returns:
    bool: True if the key was newly pressed.
        )doc");

    subKey.def("is_just_released", static_cast<bool (*)(SDL_Scancode)>(&isJustReleased),
               py::arg("scancode"), R"doc(
Check if a key was released this frame (by scancode).

Args:
    scancode (Scancode): The physical key.

Returns:
    bool: True if the key was newly released.
        )doc");

    subKey.def("is_pressed", static_cast<bool (*)(KnKeycode)>(&isPressed), py::arg("keycode"),
               R"doc(
Check if a key is currently held down (by keycode).

Args:
    keycode (Keycode): The symbolic key (e.g., K_SPACE).

Returns:
    bool: True if the key is held.
        )doc");

    subKey.def("is_just_pressed", static_cast<bool (*)(KnKeycode)>(&isJustPressed),
               py::arg("keycode"), R"doc(
Check if a key was pressed this frame (by keycode).

Args:
    keycode (Keycode): The symbolic key.

Returns:
    bool: True if the key was newly pressed.
        )doc");

    subKey.def("is_just_released", static_cast<bool (*)(KnKeycode)>(&isJustReleased),
               py::arg("keycode"), R"doc(
Check if a key was released this frame (by keycode).

Args:
    keycode (Keycode): The symbolic key.

Returns:
    bool: True if the key was newly released.
        )doc");
}

bool isPressed(SDL_Scancode scancode) { return SDL_GetKeyboardState(nullptr)[scancode]; }

bool isJustPressed(SDL_Scancode scancode) { return _scancodePressed[scancode]; }

bool isJustReleased(SDL_Scancode scancode) { return _scancodeReleased[scancode]; }

bool isPressed(KnKeycode keycode)
{
    SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keycode), nullptr);
    return SDL_GetKeyboardState(nullptr)[scancode];
}

bool isJustPressed(KnKeycode keycode)
{
    auto it = _keycodePressed.find(static_cast<SDL_Keycode>(keycode));
    return it != _keycodePressed.end() && it->second;
}

bool isJustReleased(KnKeycode keycode)
{
    auto it = _keycodeReleased.find(static_cast<SDL_Keycode>(keycode));
    return it != _keycodeReleased.end() && it->second;
}

void _clearStates()
{
    std::fill(std::begin(_scancodePressed), std::end(_scancodePressed), false);
    std::fill(std::begin(_scancodeReleased), std::end(_scancodeReleased), false);
    _keycodePressed.clear();
    _keycodeReleased.clear();
}

void _handleEvents(const SDL_Event& sdle, event::knEvent& e)
{
    switch (sdle.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        if (sdle.type == SDL_EVENT_KEY_DOWN && !sdle.key.repeat)
        {
            _scancodePressed[sdle.key.scancode] = true;
            _keycodePressed[sdle.key.key] = true;
        }
        else if (sdle.type == SDL_EVENT_KEY_UP)
        {
            _scancodeReleased[sdle.key.scancode] = true;
            _keycodeReleased[sdle.key.key] = true;
        }
        e.data["key"] = py::cast(static_cast<KnKeycode>(sdle.key.key));
        e.data["scan"] = py::cast(sdle.key.scancode);
        break;
    default:
        break;
    }
}
} // namespace key