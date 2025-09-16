#include "Event.hpp"
#include "_globals.hpp"

#include "Key.hpp"
#include <unordered_map>

namespace kn::key
{
static bool _scancodePressed[SDL_SCANCODE_COUNT] = {};
static bool _scancodeReleased[SDL_SCANCODE_COUNT] = {};
static std::unordered_map<SDL_Keycode, bool> _keycodePressed;
static std::unordered_map<SDL_Keycode, bool> _keycodeReleased;

bool isPressed(const SDL_Scancode scancode) { return SDL_GetKeyboardState(nullptr)[scancode]; }

bool isJustPressed(const SDL_Scancode scancode) { return _scancodePressed[scancode]; }

bool isJustReleased(const SDL_Scancode scancode) { return _scancodeReleased[scancode]; }

bool isPressed(Keycode keycode)
{
    const SDL_Scancode scancode =
        SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(keycode), nullptr);
    return SDL_GetKeyboardState(nullptr)[scancode];
}

bool isJustPressed(Keycode keycode)
{
    const auto it = _keycodePressed.find(static_cast<SDL_Keycode>(keycode));
    return it != _keycodePressed.end() && it->second;
}

bool isJustReleased(Keycode keycode)
{
    const auto it = _keycodeReleased.find(static_cast<SDL_Keycode>(keycode));
    return it != _keycodeReleased.end() && it->second;
}

void _clearStates()
{
    std::ranges::fill(_scancodePressed, false);
    std::ranges::fill(_scancodeReleased, false);
    _keycodePressed.clear();
    _keycodeReleased.clear();
}

void _handleEvents(const SDL_Event& sdlEvent, const Event& e)
{
    switch (sdlEvent.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        if (sdlEvent.type == SDL_EVENT_KEY_DOWN && !sdlEvent.key.repeat)
        {
            _scancodePressed[sdlEvent.key.scancode] = true;
            _keycodePressed[sdlEvent.key.key] = true;
        }
        else if (sdlEvent.type == SDL_EVENT_KEY_UP)
        {
            _scancodeReleased[sdlEvent.key.scancode] = true;
            _keycodeReleased[sdlEvent.key.key] = true;
        }
        e.data["which"] = sdlEvent.key.which;
        e.data["key"] = static_cast<Keycode>(sdlEvent.key.key);
        e.data["scan"] = sdlEvent.key.scancode;
        break;
    case SDL_EVENT_TEXT_EDITING:
        e.data["text"] = sdlEvent.edit.text;
        e.data["start"] = sdlEvent.edit.start;
        e.data["length"] = sdlEvent.edit.length;
        break;
    case SDL_EVENT_TEXT_INPUT:
        e.data["text"] = sdlEvent.text.text;
        break;
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
        e.data["which"] = sdlEvent.kdevice.which;
        break;
    default:
        break;
    }
}

void _bind(py::module_& module)
{
    auto subKey = module.def_submodule("key", "Keyboard key state checks");

    subKey.def("is_pressed", py::overload_cast<SDL_Scancode>(&isPressed), py::arg("scancode"),
               R"doc(
Check if a key is currently held down (by scancode).

Args:
    scancode (Scancode): The physical key (e.g., S_w).

Returns:
    bool: True if the key is held.
        )doc");

    subKey.def("is_just_pressed", py::overload_cast<SDL_Scancode>(&isJustPressed),
               py::arg("scancode"), R"doc(
Check if a key was pressed this frame (by scancode).

Args:
    scancode (Scancode): The physical key.

Returns:
    bool: True if the key was newly pressed.
        )doc");

    subKey.def("is_just_released", py::overload_cast<SDL_Scancode>(&isJustReleased),
               py::arg("scancode"), R"doc(
Check if a key was released this frame (by scancode).

Args:
    scancode (Scancode): The physical key.

Returns:
    bool: True if the key was newly released.
        )doc");

    subKey.def("is_pressed", py::overload_cast<Keycode>(&isPressed), py::arg("keycode"),
               R"doc(
Check if a key is currently held down (by keycode).

Args:
    keycode (Keycode): The symbolic key (e.g., K_SPACE).

Returns:
    bool: True if the key is held.
        )doc");

    subKey.def("is_just_pressed", py::overload_cast<Keycode>(&isJustPressed), py::arg("keycode"),
               R"doc(
Check if a key was pressed this frame (by keycode).

Args:
    keycode (Keycode): The symbolic key.

Returns:
    bool: True if the key was newly pressed.
        )doc");

    subKey.def("is_just_released", py::overload_cast<Keycode>(&isJustReleased), py::arg("keycode"),
               R"doc(
Check if a key was released this frame (by keycode).

Args:
    keycode (Keycode): The symbolic key.

Returns:
    bool: True if the key was newly released.
        )doc");
}
} // namespace kn::key