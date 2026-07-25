#include "kraken/input/Key.hpp"

#include <algorithm>
#include <unordered_map>

#include "kraken/core/_globals.hpp"
#include "kraken/input/Event.hpp"

namespace kn::key
{
static bool _scancodePressed[SDL_SCANCODE_COUNT] = {};
static bool _scancodeReleased[SDL_SCANCODE_COUNT] = {};
static std::unordered_map<SDL_Keycode, bool> _keycodePressed;
static std::unordered_map<SDL_Keycode, bool> _keycodeReleased;

bool isPressed(const Scancode scancode)
{
    return SDL_GetKeyboardState(nullptr)[static_cast<SDL_Scancode>(scancode)];
}

bool isJustPressed(const Scancode scancode)
{
    return _scancodePressed[static_cast<SDL_Scancode>(scancode)];
}

bool isJustReleased(const Scancode scancode)
{
    return _scancodeReleased[static_cast<SDL_Scancode>(scancode)];
}

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

void _handleEvents(const SDL_Event& sdlEvent, Event& e)
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
        e.data["scan"] = static_cast<Scancode>(sdlEvent.key.scancode);
        e.data["repeat"] = sdlEvent.key.repeat;
        e.data["mod"] = sdlEvent.key.mod;
        e.data["window_id"] = sdlEvent.key.windowID;
        break;
    case SDL_EVENT_TEXT_EDITING:
        e.data["window_id"] = sdlEvent.edit.windowID;
        e.data["text"] = sdlEvent.edit.text;
        e.data["start"] = sdlEvent.edit.start;
        e.data["length"] = sdlEvent.edit.length;
        break;
    case SDL_EVENT_TEXT_INPUT:
        e.data["window_id"] = sdlEvent.text.windowID;
        e.data["text"] = sdlEvent.text.text;
        break;
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
        e.data["which"] = sdlEvent.kdevice.which;
        break;
    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
        e.data["window_id"] = sdlEvent.edit_candidates.windowID;
        e.data["candidates"] = std::vector<std::string>(
            sdlEvent.edit_candidates.candidates,
            sdlEvent.edit_candidates.candidates + sdlEvent.edit_candidates.num_candidates
        );
        e.data["num_candidates"] = sdlEvent.edit_candidates.num_candidates;
        e.data["selected_candidate"] = sdlEvent.edit_candidates.selected_candidate;
        e.data["horizontal"] = sdlEvent.edit_candidates.horizontal;
        e.data["padding1"] = sdlEvent.edit_candidates.padding1;
        e.data["padding2"] = sdlEvent.edit_candidates.padding2;
        e.data["padding3"] = sdlEvent.edit_candidates.padding3;
        break;
    default:
        break;
    }
}

}  // namespace kn::key
