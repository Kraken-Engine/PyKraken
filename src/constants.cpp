#include "Constants.hpp"

#include <SDL3/SDL.h>
#include <pybind11/native_enum.h>

#include "Math.hpp"
#include "_globals.hpp"

const kn::Vec2 kn::Anchor::TOP_LEFT = {0.0, 0.0};
const kn::Vec2 kn::Anchor::TOP_MID = {0.5, 0.0};
const kn::Vec2 kn::Anchor::TOP_RIGHT = {1.0, 0.0};
const kn::Vec2 kn::Anchor::MID_LEFT = {0.0, 0.5};
const kn::Vec2 kn::Anchor::CENTER = {0.5, 0.5};
const kn::Vec2 kn::Anchor::MID_RIGHT = {1.0, 0.5};
const kn::Vec2 kn::Anchor::BOTTOM_LEFT = {0.0, 1.0};
const kn::Vec2 kn::Anchor::BOTTOM_MID = {0.5, 1.0};
const kn::Vec2 kn::Anchor::BOTTOM_RIGHT = {1.0, 1.0};

namespace kn::constants
{
void _bind(const py::module_& module)
{
    // Define Anchor "enum" (class with static constants)
    py::classh<Anchor>(module, "Anchor", R"doc(
Anchor positions returning Vec2 values for alignment.
    )doc")
        .def_property_readonly_static(
            "TOP_LEFT", [](const py::object&) { return Anchor::TOP_LEFT; }, R"doc(
Vec2 representing top-left anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "TOP_MID", [](const py::object&) { return Anchor::TOP_MID; }, R"doc(
Vec2 representing top-middle anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "TOP_RIGHT", [](const py::object&) { return Anchor::TOP_RIGHT; }, R"doc(
Vec2 representing top-right anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "MID_LEFT", [](const py::object&) { return Anchor::MID_LEFT; }, R"doc(
Vec2 representing middle-left anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "CENTER", [](const py::object&) { return Anchor::CENTER; }, R"doc(
Vec2 representing center anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "MID_RIGHT", [](const py::object&) { return Anchor::MID_RIGHT; }, R"doc(
Vec2 representing middle-right anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "BOTTOM_LEFT", [](const py::object&) { return Anchor::BOTTOM_LEFT; }, R"doc(
Vec2 representing bottom-left anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "BOTTOM_MID", [](const py::object&) { return Anchor::BOTTOM_MID; }, R"doc(
Vec2 representing bottom-middle anchor point.
        )doc"
        )
        .def_property_readonly_static(
            "BOTTOM_RIGHT", [](const py::object&) { return Anchor::BOTTOM_RIGHT; }, R"doc(
Vec2 representing bottom-right anchor point.
        )doc"
        );

    // Define Align enum
    py::native_enum<Align>(module, "Align", "enum.IntEnum", R"doc(
Horizontal alignment options for layout and text.
    )doc")
        .value("LEFT", Align::Left, "Left alignment")
        .value("CENTER", Align::Center, "Center alignment")
        .value("RIGHT", Align::Right, "Right alignment")
        .finalize();

    // Define event types
    py::native_enum<SDL_EventType>(module, "EventType", "enum.IntEnum", R"doc(
SDL event type constants for input and system events.
    )doc")
        .value("QUIT", SDL_EVENT_QUIT, "Quit requested")
        .value("TERMINATING", SDL_EVENT_TERMINATING, "Application is terminating")
        .value("LOW_MEMORY", SDL_EVENT_LOW_MEMORY, "Low memory warning")
        .value(
            "WILL_ENTER_BACKGROUND", SDL_EVENT_WILL_ENTER_BACKGROUND, "About to enter background"
        )
        .value("DID_ENTER_BACKGROUND", SDL_EVENT_DID_ENTER_BACKGROUND, "Entered background")
        .value(
            "WILL_ENTER_FOREGROUND", SDL_EVENT_WILL_ENTER_FOREGROUND, "About to enter foreground"
        )
        .value("DID_ENTER_FOREGROUND", SDL_EVENT_DID_ENTER_FOREGROUND, "Entered foreground")
        .value("LOCALE_CHANGED", SDL_EVENT_LOCALE_CHANGED, "Locale settings changed")
        .value("SYSTEM_THEME_CHANGED", SDL_EVENT_SYSTEM_THEME_CHANGED, "System theme changed")

        // Display events
        .value("DISPLAY_ORIENTATION", SDL_EVENT_DISPLAY_ORIENTATION, "Display orientation changed")
        .value("DISPLAY_ADDED", SDL_EVENT_DISPLAY_ADDED, "Display connected")
        .value("DISPLAY_REMOVED", SDL_EVENT_DISPLAY_REMOVED, "Display disconnected")
        .value("DISPLAY_MOVED", SDL_EVENT_DISPLAY_MOVED, "Display moved")
        .value(
            "DISPLAY_DESKTOP_MODE_CHANGED", SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,
            "Desktop display mode changed"
        )
        .value(
            "DISPLAY_CURRENT_MODE_CHANGED", SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,
            "Current display mode changed"
        )
        .value(
            "DISPLAY_CONTENT_SCALE_CHANGED", SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,
            "Display content scale changed"
        )
        .value(
            "DISPLAY_USABLE_BOUNDS_CHANGED", SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED,
            "Usable display bounds changed"
        )

        // Window events
        .value("WINDOW_SHOWN", SDL_EVENT_WINDOW_SHOWN, "Window shown")
        .value("WINDOW_HIDDEN", SDL_EVENT_WINDOW_HIDDEN, "Window hidden")
        .value("WINDOW_EXPOSED", SDL_EVENT_WINDOW_EXPOSED, "Window needs redraw")
        .value("WINDOW_MOVED", SDL_EVENT_WINDOW_MOVED, "Window moved")
        .value("WINDOW_RESIZED", SDL_EVENT_WINDOW_RESIZED, "Window resized")
        .value("WINDOW_MINIMIZED", SDL_EVENT_WINDOW_MINIMIZED, "Window minimized")
        .value("WINDOW_MAXIMIZED", SDL_EVENT_WINDOW_MAXIMIZED, "Window maximized")
        .value("WINDOW_RESTORED", SDL_EVENT_WINDOW_RESTORED, "Window restored")
        .value("WINDOW_MOUSE_ENTER", SDL_EVENT_WINDOW_MOUSE_ENTER, "Mouse entered window")
        .value("WINDOW_MOUSE_LEAVE", SDL_EVENT_WINDOW_MOUSE_LEAVE, "Mouse left window")
        .value("WINDOW_FOCUS_GAINED", SDL_EVENT_WINDOW_FOCUS_GAINED, "Window gained focus")
        .value("WINDOW_FOCUS_LOST", SDL_EVENT_WINDOW_FOCUS_LOST, "Window lost focus")
        .value("WINDOW_CLOSE_REQUESTED", SDL_EVENT_WINDOW_CLOSE_REQUESTED, "Window close requested")
        .value("WINDOW_HIT_TEST", SDL_EVENT_WINDOW_HIT_TEST, "Window hit test request")
        .value("WINDOW_ICCPROF_CHANGED", SDL_EVENT_WINDOW_ICCPROF_CHANGED, "ICC profile changed")
        .value("WINDOW_DISPLAY_CHANGED", SDL_EVENT_WINDOW_DISPLAY_CHANGED, "Window display changed")
        .value(
            "WINDOW_DISPLAY_SCALE_CHANGED", SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
            "Window display scale changed"
        )
        .value(
            "WINDOW_SAFE_AREA_CHANGED", SDL_EVENT_WINDOW_SAFE_AREA_CHANGED,
            "Window safe area changed"
        )
        .value("WINDOW_OCCLUDED", SDL_EVENT_WINDOW_OCCLUDED, "Window occluded")
        .value("WINDOW_ENTER_FULLSCREEN", SDL_EVENT_WINDOW_ENTER_FULLSCREEN, "Entered fullscreen")
        .value("WINDOW_LEAVE_FULLSCREEN", SDL_EVENT_WINDOW_LEAVE_FULLSCREEN, "Left fullscreen")
        .value("WINDOW_DESTROYED", SDL_EVENT_WINDOW_DESTROYED, "Window destroyed")
        .value("WINDOW_HDR_STATE_CHANGED", SDL_EVENT_WINDOW_HDR_STATE_CHANGED, "HDR state changed")

        // Keyboard events
        .value("KEY_DOWN", SDL_EVENT_KEY_DOWN, "Key pressed or repeating while held")
        .value("KEY_UP", SDL_EVENT_KEY_UP, "Key released")
        .value("TEXT_EDITING", SDL_EVENT_TEXT_EDITING, "Text editing in progress")
        .value("TEXT_INPUT", SDL_EVENT_TEXT_INPUT, "Text input committed")
        .value("KEYMAP_CHANGED", SDL_EVENT_KEYMAP_CHANGED, "Keymap changed")
        .value("KEYBOARD_ADDED", SDL_EVENT_KEYBOARD_ADDED, "Keyboard connected")
        .value("KEYBOARD_REMOVED", SDL_EVENT_KEYBOARD_REMOVED, "Keyboard disconnected")
        .value(
            "TEXT_EDITING_CANDIDATES", SDL_EVENT_TEXT_EDITING_CANDIDATES,
            "IME candidate list updated"
        )
        .value("SCREEN_KEYBOARD_SHOWN", SDL_EVENT_SCREEN_KEYBOARD_SHOWN, "On-screen keyboard shown")
        .value(
            "SCREEN_KEYBOARD_HIDDEN", SDL_EVENT_SCREEN_KEYBOARD_HIDDEN, "On-screen keyboard hidden"
        )

        // Mouse events
        .value("MOUSE_MOTION", SDL_EVENT_MOUSE_MOTION, "Mouse moved")
        .value("MOUSE_BUTTON_DOWN", SDL_EVENT_MOUSE_BUTTON_DOWN, "Mouse button pressed")
        .value("MOUSE_BUTTON_UP", SDL_EVENT_MOUSE_BUTTON_UP, "Mouse button released")
        .value("MOUSE_WHEEL", SDL_EVENT_MOUSE_WHEEL, "Mouse wheel scrolled")
        .value("MOUSE_ADDED", SDL_EVENT_MOUSE_ADDED, "Mouse connected")
        .value("MOUSE_REMOVED", SDL_EVENT_MOUSE_REMOVED, "Mouse disconnected")

        // Gamepad events
        .value("GAMEPAD_AXIS_MOTION", SDL_EVENT_GAMEPAD_AXIS_MOTION, "Gamepad axis moved")
        .value("GAMEPAD_BUTTON_DOWN", SDL_EVENT_GAMEPAD_BUTTON_DOWN, "Gamepad button pressed")
        .value("GAMEPAD_BUTTON_UP", SDL_EVENT_GAMEPAD_BUTTON_UP, "Gamepad button released")
        .value("GAMEPAD_ADDED", SDL_EVENT_GAMEPAD_ADDED, "Gamepad connected")
        .value("GAMEPAD_REMOVED", SDL_EVENT_GAMEPAD_REMOVED, "Gamepad disconnected")
        .value("GAMEPAD_REMAPPED", SDL_EVENT_GAMEPAD_REMAPPED, "Gamepad mapping updated")
        .value("GAMEPAD_TOUCHPAD_DOWN", SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN, "Touchpad pressed")
        .value("GAMEPAD_TOUCHPAD_MOTION", SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION, "Touchpad moved")
        .value("GAMEPAD_TOUCHPAD_UP", SDL_EVENT_GAMEPAD_TOUCHPAD_UP, "Touchpad released")
        .value("GAMEPAD_SENSOR_UPDATE", SDL_EVENT_GAMEPAD_SENSOR_UPDATE, "Gamepad sensor updated")
        .value(
            "GAMEPAD_UPDATE_COMPLETE", SDL_EVENT_GAMEPAD_UPDATE_COMPLETE, "Gamepad update complete"
        )
        .value(
            "GAMEPAD_STEAM_HANDLE_UPDATED", SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED,
            "Steam handle updated"
        )

        // Touch events
        .value("FINGER_DOWN", SDL_EVENT_FINGER_DOWN, "Finger touch began")
        .value("FINGER_UP", SDL_EVENT_FINGER_UP, "Finger touch ended")
        .value("FINGER_MOTION", SDL_EVENT_FINGER_MOTION, "Finger moved")
        .value("FINGER_CANCELED", SDL_EVENT_FINGER_CANCELED, "Finger touch canceled")

        // Drag and drop events
        .value("CLIPBOARD_UPDATE", SDL_EVENT_CLIPBOARD_UPDATE, "Clipboard content changed")
        .value("DROP_FILE", SDL_EVENT_DROP_FILE, "File dropped")
        .value("DROP_TEXT", SDL_EVENT_DROP_TEXT, "Text dropped")
        .value("DROP_BEGIN", SDL_EVENT_DROP_BEGIN, "Drag-and-drop started")
        .value("DROP_COMPLETE", SDL_EVENT_DROP_COMPLETE, "Drag-and-drop completed")
        .value("DROP_POSITION", SDL_EVENT_DROP_POSITION, "Drag-and-drop position updated")

        // Audio device events
        .value("AUDIO_DEVICE_ADDED", SDL_EVENT_AUDIO_DEVICE_ADDED, "Audio device connected")
        .value("AUDIO_DEVICE_REMOVED", SDL_EVENT_AUDIO_DEVICE_REMOVED, "Audio device disconnected")
        .value(
            "AUDIO_DEVICE_FORMAT_CHANGED", SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,
            "Audio device format changed"
        )

        // Sensor events
        .value("SENSOR_UPDATE", SDL_EVENT_SENSOR_UPDATE, "Sensor data updated")

        // Pen events
        .value("PEN_PROXIMITY_IN", SDL_EVENT_PEN_PROXIMITY_IN, "Pen entered proximity")
        .value("PEN_PROXIMITY_OUT", SDL_EVENT_PEN_PROXIMITY_OUT, "Pen left proximity")
        .value("PEN_DOWN", SDL_EVENT_PEN_DOWN, "Pen pressed")
        .value("PEN_UP", SDL_EVENT_PEN_UP, "Pen released")
        .value("PEN_BUTTON_DOWN", SDL_EVENT_PEN_BUTTON_DOWN, "Pen button pressed")
        .value("PEN_BUTTON_UP", SDL_EVENT_PEN_BUTTON_UP, "Pen button released")
        .value("PEN_MOTION", SDL_EVENT_PEN_MOTION, "Pen moved")
        .value("PEN_AXIS", SDL_EVENT_PEN_AXIS, "Pen axis data updated")

        // Camera events
        .value("CAMERA_DEVICE_ADDED", SDL_EVENT_CAMERA_DEVICE_ADDED, "Camera connected")
        .value("CAMERA_DEVICE_REMOVED", SDL_EVENT_CAMERA_DEVICE_REMOVED, "Camera disconnected")
        .value("CAMERA_DEVICE_APPROVED", SDL_EVENT_CAMERA_DEVICE_APPROVED, "Camera access approved")
        .value("CAMERA_DEVICE_DENIED", SDL_EVENT_CAMERA_DEVICE_DENIED, "Camera access denied")

        // Render events
        .value("RENDER_TARGETS_RESET", SDL_EVENT_RENDER_TARGETS_RESET, "Render targets reset")
        .value("RENDER_DEVICE_RESET", SDL_EVENT_RENDER_DEVICE_RESET, "Render device reset")
        .value("RENDER_DEVICE_LOST", SDL_EVENT_RENDER_DEVICE_LOST, "Render device lost")

        // Pinch gesture events
        .value("PINCH_BEGIN", SDL_EVENT_PINCH_BEGIN, "Pinch gesture began")
        .value("PINCH_UPDATE", SDL_EVENT_PINCH_UPDATE, "Pinch gesture updated")
        .value("PINCH_END", SDL_EVENT_PINCH_END, "Pinch gesture ended")

        .export_values()
        .finalize();

    // Mouse buttons
    py::native_enum<MouseButton>(module, "MouseButton", "enum.IntEnum", R"doc(
Mouse button identifiers.
    )doc")
        .value("M_LEFT", MouseButton::Left, "Left mouse button")
        .value("M_MIDDLE", MouseButton::Middle, "Middle mouse button")
        .value("M_RIGHT", MouseButton::Right, "Right mouse button")
        .value("M_SIDE1", MouseButton::Side1, "First side mouse button")
        .value("M_SIDE2", MouseButton::Side2, "Second side mouse button")
        .export_values()
        .finalize();

    // Pen Axis
    py::native_enum<SDL_PenAxis>(module, "PenAxis", "enum.IntEnum", R"doc(
Stylus/pen axis identifiers for pen motion data.
    )doc")
        .value("P_PRESSURE", SDL_PEN_AXIS_PRESSURE, "Pen pressure axis")
        .value("P_TILT_X", SDL_PEN_AXIS_XTILT, "Pen X tilt axis")
        .value("P_TILT_Y", SDL_PEN_AXIS_YTILT, "Pen Y tilt axis")
        .value("P_DISTANCE", SDL_PEN_AXIS_DISTANCE, "Pen distance from surface")
        .value("P_ROTATION", SDL_PEN_AXIS_ROTATION, "Pen rotation axis")
        .value("P_SLIDER", SDL_PEN_AXIS_SLIDER, "Pen slider axis")
        .value("P_TANGENTIAL_PRESSURE", SDL_PEN_AXIS_TANGENTIAL_PRESSURE, "Pen tangential pressure")
        .export_values()
        .finalize();

    // Scancodes
    py::native_enum<SDL_Scancode>(module, "Scancode", "enum.IntEnum", R"doc(
Keyboard scancodes representing physical key locations.
    )doc")
        .value("S_a", SDL_SCANCODE_A, "The physical A key")
        .value("S_b", SDL_SCANCODE_B, "The physical B key")
        .value("S_c", SDL_SCANCODE_C, "The physical C key")
        .value("S_d", SDL_SCANCODE_D, "The physical D key")
        .value("S_e", SDL_SCANCODE_E, "The physical E key")
        .value("S_f", SDL_SCANCODE_F, "The physical F key")
        .value("S_g", SDL_SCANCODE_G, "The physical G key")
        .value("S_h", SDL_SCANCODE_H, "The physical H key")
        .value("S_i", SDL_SCANCODE_I, "The physical I key")
        .value("S_j", SDL_SCANCODE_J, "The physical J key")
        .value("S_k", SDL_SCANCODE_K, "The physical K key")
        .value("S_l", SDL_SCANCODE_L, "The physical L key")
        .value("S_m", SDL_SCANCODE_M, "The physical M key")
        .value("S_n", SDL_SCANCODE_N, "The physical N key")
        .value("S_o", SDL_SCANCODE_O, "The physical O key")
        .value("S_p", SDL_SCANCODE_P, "The physical P key")
        .value("S_q", SDL_SCANCODE_Q, "The physical Q key")
        .value("S_r", SDL_SCANCODE_R, "The physical R key")
        .value("S_s", SDL_SCANCODE_S, "The physical S key")
        .value("S_t", SDL_SCANCODE_T, "The physical T key")
        .value("S_u", SDL_SCANCODE_U, "The physical U key")
        .value("S_v", SDL_SCANCODE_V, "The physical V key")
        .value("S_w", SDL_SCANCODE_W, "The physical W key")
        .value("S_x", SDL_SCANCODE_X, "The physical X key")
        .value("S_y", SDL_SCANCODE_Y, "The physical Y key")
        .value("S_z", SDL_SCANCODE_Z, "The physical Z key")

        .value("S_1", SDL_SCANCODE_1, "The physical 1 key")
        .value("S_2", SDL_SCANCODE_2, "The physical 2 key")
        .value("S_3", SDL_SCANCODE_3, "The physical 3 key")
        .value("S_4", SDL_SCANCODE_4, "The physical 4 key")
        .value("S_5", SDL_SCANCODE_5, "The physical 5 key")
        .value("S_6", SDL_SCANCODE_6, "The physical 6 key")
        .value("S_7", SDL_SCANCODE_7, "The physical 7 key")
        .value("S_8", SDL_SCANCODE_8, "The physical 8 key")
        .value("S_9", SDL_SCANCODE_9, "The physical 9 key")
        .value("S_0", SDL_SCANCODE_0, "The physical 0 key")

        .value("S_RETURN", SDL_SCANCODE_RETURN, "The physical Return key")
        .value("S_ESC", SDL_SCANCODE_ESCAPE, "The physical Escape key")
        .value("S_BACKSPACE", SDL_SCANCODE_BACKSPACE, "The physical Backspace key")
        .value("S_TAB", SDL_SCANCODE_TAB, "The physical Tab key")
        .value("S_SPACE", SDL_SCANCODE_SPACE, "The physical Space key")
        .value("S_MINUS", SDL_SCANCODE_MINUS, "The physical Minus key")
        .value("S_EQ", SDL_SCANCODE_EQUALS, "The physical Equals key")
        .value("S_LBRACKET", SDL_SCANCODE_LEFTBRACKET, "The physical Left Bracket key")
        .value("S_RBRACKET", SDL_SCANCODE_RIGHTBRACKET, "The physical Right Bracket key")
        .value("S_BACKSLASH", SDL_SCANCODE_BACKSLASH, "The physical Backslash key")
        .value("S_SEMICOLON", SDL_SCANCODE_SEMICOLON, "The physical Semicolon key")
        .value("S_APOSTROPHE", SDL_SCANCODE_APOSTROPHE, "The physical Apostrophe key")
        .value("S_GRAVE", SDL_SCANCODE_GRAVE, "The physical Grave key")
        .value("S_COMMA", SDL_SCANCODE_COMMA, "The physical Comma key")
        .value("S_PERIOD", SDL_SCANCODE_PERIOD, "The physical Period key")
        .value("S_SLASH", SDL_SCANCODE_SLASH, "The physical Slash key")
        .value("S_CAPS", SDL_SCANCODE_CAPSLOCK, "The physical Caps Lock key")

        .value("S_F1", SDL_SCANCODE_F1, "The physical F1 key")
        .value("S_F2", SDL_SCANCODE_F2, "The physical F2 key")
        .value("S_F3", SDL_SCANCODE_F3, "The physical F3 key")
        .value("S_F4", SDL_SCANCODE_F4, "The physical F4 key")
        .value("S_F5", SDL_SCANCODE_F5, "The physical F5 key")
        .value("S_F6", SDL_SCANCODE_F6, "The physical F6 key")
        .value("S_F7", SDL_SCANCODE_F7, "The physical F7 key")
        .value("S_F8", SDL_SCANCODE_F8, "The physical F8 key")
        .value("S_F9", SDL_SCANCODE_F9, "The physical F9 key")
        .value("S_F10", SDL_SCANCODE_F10, "The physical F10 key")
        .value("S_F11", SDL_SCANCODE_F11, "The physical F11 key")
        .value("S_F12", SDL_SCANCODE_F12, "The physical F12 key")
        .value("S_F13", SDL_SCANCODE_F13, "The physical F13 key")
        .value("S_F14", SDL_SCANCODE_F14, "The physical F14 key")
        .value("S_F15", SDL_SCANCODE_F15, "The physical F15 key")

        .value("S_PRTSCR", SDL_SCANCODE_PRINTSCREEN, "The physical Print Screen key")
        .value("S_SCRLK", SDL_SCANCODE_SCROLLLOCK, "The physical Scroll Lock key")
        .value("S_PAUSE", SDL_SCANCODE_PAUSE, "The physical Pause key")
        .value("S_INS", SDL_SCANCODE_INSERT, "The physical Insert key")
        .value("S_HOME", SDL_SCANCODE_HOME, "The physical Home key")
        .value("S_PGUP", SDL_SCANCODE_PAGEUP, "The physical Page Up key")
        .value("S_DEL", SDL_SCANCODE_DELETE, "The physical Delete key")
        .value("S_END", SDL_SCANCODE_END, "The physical End key")
        .value("S_PGDOWN", SDL_SCANCODE_PAGEDOWN, "The physical Page Down key")
        .value("S_RIGHT", SDL_SCANCODE_RIGHT, "The physical Right Arrow key")
        .value("S_LEFT", SDL_SCANCODE_LEFT, "The physical Left Arrow key")
        .value("S_DOWN", SDL_SCANCODE_DOWN, "The physical Down Arrow key")
        .value("S_UP", SDL_SCANCODE_UP, "The physical Up Arrow key")

        .value("S_NUMLOCK", SDL_SCANCODE_NUMLOCKCLEAR, "The physical Num Lock key")
        .value("S_KP_DIV", SDL_SCANCODE_KP_DIVIDE, "The physical keypad Divide key")
        .value("S_KP_MULT", SDL_SCANCODE_KP_MULTIPLY, "The physical keypad Multiply key")
        .value("S_KP_MINUS", SDL_SCANCODE_KP_MINUS, "The physical keypad Minus key")
        .value("S_KP_PLUS", SDL_SCANCODE_KP_PLUS, "The physical keypad Plus key")
        .value("S_KP_ENTER", SDL_SCANCODE_KP_ENTER, "The physical keypad Enter key")
        .value("S_KP_1", SDL_SCANCODE_KP_1, "The physical keypad 1 key")
        .value("S_KP_2", SDL_SCANCODE_KP_2, "The physical keypad 2 key")
        .value("S_KP_3", SDL_SCANCODE_KP_3, "The physical keypad 3 key")
        .value("S_KP_4", SDL_SCANCODE_KP_4, "The physical keypad 4 key")
        .value("S_KP_5", SDL_SCANCODE_KP_5, "The physical keypad 5 key")
        .value("S_KP_6", SDL_SCANCODE_KP_6, "The physical keypad 6 key")
        .value("S_KP_7", SDL_SCANCODE_KP_7, "The physical keypad 7 key")
        .value("S_KP_8", SDL_SCANCODE_KP_8, "The physical keypad 8 key")
        .value("S_KP_9", SDL_SCANCODE_KP_9, "The physical keypad 9 key")
        .value("S_KP_0", SDL_SCANCODE_KP_0, "The physical keypad 0 key")
        .value("S_KP_PERIOD", SDL_SCANCODE_KP_PERIOD, "The physical keypad Period key")

        .value("S_APPLICATION", SDL_SCANCODE_APPLICATION, "The physical Application key")
        .value("S_POWER", SDL_SCANCODE_POWER, "The physical Power key")
        .value("S_EXECUTE", SDL_SCANCODE_EXECUTE, "The physical Execute key")
        .value("S_HELP", SDL_SCANCODE_HELP, "The physical Help key")
        .value("S_MENU", SDL_SCANCODE_MENU, "The physical Menu key")
        .value("S_SELECT", SDL_SCANCODE_SELECT, "The physical Select key")
        .value("S_STOP", SDL_SCANCODE_STOP, "The physical Stop key")
        .value("S_AGAIN", SDL_SCANCODE_AGAIN, "The physical Again key")
        .value("S_UNDO", SDL_SCANCODE_UNDO, "The physical Undo key")
        .value("S_CUT", SDL_SCANCODE_CUT, "The physical Cut key")
        .value("S_COPY", SDL_SCANCODE_COPY, "The physical Copy key")
        .value("S_PASTE", SDL_SCANCODE_PASTE, "The physical Paste key")
        .value("S_FIND", SDL_SCANCODE_FIND, "The physical Find key")
        .value("S_MUTE", SDL_SCANCODE_MUTE, "The physical Mute key")
        .value("S_VOLUP", SDL_SCANCODE_VOLUMEUP, "The physical Volume Up key")
        .value("S_VOLDOWN", SDL_SCANCODE_VOLUMEDOWN, "The physical Volume Down key")

        .value("S_LCTRL", SDL_SCANCODE_LCTRL, "The physical Left Ctrl key")
        .value("S_LSHIFT", SDL_SCANCODE_LSHIFT, "The physical Left Shift key")
        .value("S_LALT", SDL_SCANCODE_LALT, "The physical Left Alt key")
        .value("S_LGUI", SDL_SCANCODE_LGUI, "The physical Left GUI/Windows key")
        .value("S_RCTRL", SDL_SCANCODE_RCTRL, "The physical Right Ctrl key")
        .value("S_RSHIFT", SDL_SCANCODE_RSHIFT, "The physical Right Shift key")
        .value("S_RALT", SDL_SCANCODE_RALT, "The physical Right Alt key")
        .value("S_RGUI", SDL_SCANCODE_RGUI, "The physical Right GUI/Windows key")

        .value("S_MODE", SDL_SCANCODE_MODE, "The physical Mode key")
        .value("S_SLEEP", SDL_SCANCODE_SLEEP, "The physical Sleep key")
        .value("S_WAKE", SDL_SCANCODE_WAKE, "The physical Wake key")
        .value("S_CHANNEL_INC", SDL_SCANCODE_CHANNEL_INCREMENT, "The physical Channel Up key")
        .value("S_CHANNEL_DEC", SDL_SCANCODE_CHANNEL_DECREMENT, "The physical Channel Down key")
        .value("S_MEDIA_PLAY", SDL_SCANCODE_MEDIA_PLAY, "The physical Media Play key")
        .value("S_MEDIA_PAUSE", SDL_SCANCODE_MEDIA_PAUSE, "The physical Media Pause key")
        .value("S_MEDIA_REC", SDL_SCANCODE_MEDIA_RECORD, "The physical Media Record key")
        .value(
            "S_MEDIA_FAST_FORWARD", SDL_SCANCODE_MEDIA_FAST_FORWARD,
            "The physical Media Fast Forward key"
        )
        .value("S_MEDIA_REWIND", SDL_SCANCODE_MEDIA_REWIND, "The physical Media Rewind key")
        .value("S_MEDIA_NEXT", SDL_SCANCODE_MEDIA_NEXT_TRACK, "The physical Media Next Track key")
        .value(
            "S_MEDIA_PREV", SDL_SCANCODE_MEDIA_PREVIOUS_TRACK,
            "The physical Media Previous Track key"
        )
        .value("S_MEDIA_STOP", SDL_SCANCODE_MEDIA_STOP, "The physical Media Stop key")
        .value("S_MEDIA_EJECT", SDL_SCANCODE_MEDIA_EJECT, "The physical Media Eject key")
        .value(
            "S_MEDIA_PLAY_PAUSE", SDL_SCANCODE_MEDIA_PLAY_PAUSE, "The physical Media Play/Pause key"
        )
        .value("S_MEDIA_SELECT", SDL_SCANCODE_MEDIA_SELECT, "The physical Media Select key")

        .value("S_SOFTLEFT", SDL_SCANCODE_SOFTLEFT, "The physical Soft Left key")
        .value("S_SOFTRIGHT", SDL_SCANCODE_SOFTRIGHT, "The physical Soft Right key")
        .value("S_CALL", SDL_SCANCODE_CALL, "The physical Call key")
        .value("S_ENDCALL", SDL_SCANCODE_ENDCALL, "The physical End Call key")

        .export_values()
        .finalize();

    // Keycodes
    py::native_enum<Keycode>(module, "Keycode", "enum.IntEnum", R"doc(
Keyboard keycodes representing logical keys.
    )doc")
        .value("K_UNKNOWN", Keycode::K_UNKNOWN, "Unknown key")
        .value("K_RETURN", Keycode::K_RETURN, "The symbolic Return key")
        .value("K_ESC", Keycode::K_ESC, "The symbolic Escape key")
        .value("K_BACKSPACE", Keycode::K_BACKSPACE, "The symbolic Backspace key")
        .value("K_TAB", Keycode::K_TAB, "The symbolic Tab key")
        .value("K_SPACE", Keycode::K_SPACE, "The symbolic Space key")
        .value("K_EXCLAIM", Keycode::K_EXCLAIM, "The symbolic Exclamation key")
        .value("K_DBLQUOTE", Keycode::K_DBLQUOTE, "The symbolic Double Quote key")
        .value("K_HASH", Keycode::K_HASH, "The symbolic Hash key")
        .value("K_DOLLAR", Keycode::K_DOLLAR, "The symbolic Dollar key")
        .value("K_PERCENT", Keycode::K_PERCENT, "The symbolic Percent key")
        .value("K_AMPERSAND", Keycode::K_AMPERSAND, "The symbolic Ampersand key")
        .value("K_SGLQUOTE", Keycode::K_SGLQUOTE, "The symbolic Single Quote key")
        .value("K_LPAREN", Keycode::K_LPAREN, "The symbolic Left Parenthesis key")
        .value("K_RPAREN", Keycode::K_RPAREN, "The symbolic Right Parenthesis key")
        .value("K_ASTERISK", Keycode::K_ASTERISK, "The symbolic Asterisk key")
        .value("K_PLUS", Keycode::K_PLUS, "The symbolic Plus key")
        .value("K_COMMA", Keycode::K_COMMA, "The symbolic Comma key")
        .value("K_MINUS", Keycode::K_MINUS, "The symbolic Minus key")
        .value("K_PERIOD", Keycode::K_PERIOD, "The symbolic Period key")
        .value("K_SLASH", Keycode::K_SLASH, "The symbolic Slash key")
        .value("K_0", Keycode::K_0, "The symbolic 0 key")
        .value("K_1", Keycode::K_1, "The symbolic 1 key")
        .value("K_2", Keycode::K_2, "The symbolic 2 key")
        .value("K_3", Keycode::K_3, "The symbolic 3 key")
        .value("K_4", Keycode::K_4, "The symbolic 4 key")
        .value("K_5", Keycode::K_5, "The symbolic 5 key")
        .value("K_6", Keycode::K_6, "The symbolic 6 key")
        .value("K_7", Keycode::K_7, "The symbolic 7 key")
        .value("K_8", Keycode::K_8, "The symbolic 8 key")
        .value("K_9", Keycode::K_9, "The symbolic 9 key")
        .value("K_COLON", Keycode::K_COLON, "The symbolic Colon key")
        .value("K_SEMICOLON", Keycode::K_SEMICOLON, "The symbolic Semicolon key")
        .value("K_LT", Keycode::K_LT, "The symbolic Less-than key")
        .value("K_EQ", Keycode::K_EQ, "The symbolic Equals key")
        .value("K_GT", Keycode::K_GT, "The symbolic Greater-than key")
        .value("K_QUESTION", Keycode::K_QUESTION, "The symbolic Question key")
        .value("K_AT", Keycode::K_AT, "The symbolic At key")
        .value("K_LBRACKET", Keycode::K_LBRACKET, "The symbolic Left Bracket key")
        .value("K_BACKSLASH", Keycode::K_BACKSLASH, "The symbolic Backslash key")
        .value("K_RBRACKET", Keycode::K_RBRACKET, "The symbolic Right Bracket key")
        .value("K_CARET", Keycode::K_CARET, "The symbolic Caret key")
        .value("K_UNDERSCORE", Keycode::K_UNDERSCORE, "The symbolic Underscore key")
        .value("K_GRAVE", Keycode::K_GRAVE, "The symbolic Grave key")
        .value("K_a", Keycode::K_a, "The symbolic A key")
        .value("K_b", Keycode::K_b, "The symbolic B key")
        .value("K_c", Keycode::K_c, "The symbolic C key")
        .value("K_d", Keycode::K_d, "The symbolic D key")
        .value("K_e", Keycode::K_e, "The symbolic E key")
        .value("K_f", Keycode::K_f, "The symbolic F key")
        .value("K_g", Keycode::K_g, "The symbolic G key")
        .value("K_h", Keycode::K_h, "The symbolic H key")
        .value("K_i", Keycode::K_i, "The symbolic I key")
        .value("K_j", Keycode::K_j, "The symbolic J key")
        .value("K_k", Keycode::K_k, "The symbolic K key")
        .value("K_l", Keycode::K_l, "The symbolic L key")
        .value("K_m", Keycode::K_m, "The symbolic M key")
        .value("K_n", Keycode::K_n, "The symbolic N key")
        .value("K_o", Keycode::K_o, "The symbolic O key")
        .value("K_p", Keycode::K_p, "The symbolic P key")
        .value("K_q", Keycode::K_q, "The symbolic Q key")
        .value("K_r", Keycode::K_r, "The symbolic R key")
        .value("K_s", Keycode::K_s, "The symbolic S key")
        .value("K_t", Keycode::K_t, "The symbolic T key")
        .value("K_u", Keycode::K_u, "The symbolic U key")
        .value("K_v", Keycode::K_v, "The symbolic V key")
        .value("K_w", Keycode::K_w, "The symbolic W key")
        .value("K_x", Keycode::K_x, "The symbolic X key")
        .value("K_y", Keycode::K_y, "The symbolic Y key")
        .value("K_z", Keycode::K_z, "The symbolic Z key")
        .value("K_LBRACE", Keycode::K_LBRACE, "The symbolic Left Brace key")
        .value("K_PIPE", Keycode::K_PIPE, "The symbolic Pipe key")
        .value("K_RBRACE", Keycode::K_RBRACE, "The symbolic Right Brace key")
        .value("K_TILDE", Keycode::K_TILDE, "The symbolic Tilde key")
        .value("K_DEL", Keycode::K_DEL, "The symbolic Delete key")
        .value("K_CAPS", Keycode::K_CAPS, "The symbolic Caps Lock key")
        .value("K_F1", Keycode::K_F1, "The symbolic F1 key")
        .value("K_F2", Keycode::K_F2, "The symbolic F2 key")
        .value("K_F3", Keycode::K_F3, "The symbolic F3 key")
        .value("K_F4", Keycode::K_F4, "The symbolic F4 key")
        .value("K_F5", Keycode::K_F5, "The symbolic F5 key")
        .value("K_F6", Keycode::K_F6, "The symbolic F6 key")
        .value("K_F7", Keycode::K_F7, "The symbolic F7 key")
        .value("K_F8", Keycode::K_F8, "The symbolic F8 key")
        .value("K_F9", Keycode::K_F9, "The symbolic F9 key")
        .value("K_F10", Keycode::K_F10, "The symbolic F10 key")
        .value("K_F11", Keycode::K_F11, "The symbolic F11 key")
        .value("K_F12", Keycode::K_F12, "The symbolic F12 key")
        .value("K_PRTSCR", Keycode::K_PRTSCR, "The symbolic Print Screen key")
        .value("K_SCRLK", Keycode::K_SCRLK, "The symbolic Scroll Lock key")
        .value("K_PAUSE", Keycode::K_PAUSE, "The symbolic Pause key")
        .value("K_INS", Keycode::K_INS, "The symbolic Insert key")
        .value("K_HOME", Keycode::K_HOME, "The symbolic Home key")
        .value("K_PGUP", Keycode::K_PGUP, "The symbolic Page Up key")
        .value("K_END", Keycode::K_END, "The symbolic End key")
        .value("K_PGDOWN", Keycode::K_PGDOWN, "The symbolic Page Down key")
        .value("K_RIGHT", Keycode::K_RIGHT, "The symbolic Right Arrow key")
        .value("K_LEFT", Keycode::K_LEFT, "The symbolic Left Arrow key")
        .value("K_DOWN", Keycode::K_DOWN, "The symbolic Down Arrow key")
        .value("K_UP", Keycode::K_UP, "The symbolic Up Arrow key")
        .value("K_NUMLOCK", Keycode::K_NUMLOCK, "The symbolic Num Lock key")
        .value("K_KP_DIV", Keycode::K_KP_DIV, "The symbolic keypad Divide key")
        .value("K_KP_MULT", Keycode::K_KP_MULT, "The symbolic keypad Multiply key")
        .value("K_KP_MINUS", Keycode::K_KP_MINUS, "The symbolic keypad Minus key")
        .value("K_KP_PLUS", Keycode::K_KP_PLUS, "The symbolic keypad Plus key")
        .value("K_KP_ENTER", Keycode::K_KP_ENTER, "The symbolic keypad Enter key")
        .value("K_KP_1", Keycode::K_KP_1, "The symbolic keypad 1 key")
        .value("K_KP_2", Keycode::K_KP_2, "The symbolic keypad 2 key")
        .value("K_KP_3", Keycode::K_KP_3, "The symbolic keypad 3 key")
        .value("K_KP_4", Keycode::K_KP_4, "The symbolic keypad 4 key")
        .value("K_KP_5", Keycode::K_KP_5, "The symbolic keypad 5 key")
        .value("K_KP_6", Keycode::K_KP_6, "The symbolic keypad 6 key")
        .value("K_KP_7", Keycode::K_KP_7, "The symbolic keypad 7 key")
        .value("K_KP_8", Keycode::K_KP_8, "The symbolic keypad 8 key")
        .value("K_KP_9", Keycode::K_KP_9, "The symbolic keypad 9 key")
        .value("K_KP_0", Keycode::K_KP_0, "The symbolic keypad 0 key")
        .value("K_KP_PERIOD", Keycode::K_KP_PERIOD, "The symbolic keypad Period key")
        .value("K_APPLICATION", Keycode::K_APPLICATION, "The symbolic Application key")
        .value("K_POWER", Keycode::K_POWER, "The symbolic Power key")
        .value("K_F13", Keycode::K_F13, "The symbolic F13 key")
        .value("K_F14", Keycode::K_F14, "The symbolic F14 key")
        .value("K_F15", Keycode::K_F15, "The symbolic F15 key")
        .value("K_EXECUTE", Keycode::K_EXECUTE, "The symbolic Execute key")
        .value("K_HELP", Keycode::K_HELP, "The symbolic Help key")
        .value("K_MENU", Keycode::K_MENU, "The symbolic Menu key")
        .value("K_SELECT", Keycode::K_SELECT, "The symbolic Select key")
        .value("K_STOP", Keycode::K_STOP, "The symbolic Stop key")
        .value("K_AGAIN", Keycode::K_AGAIN, "The symbolic Again key")
        .value("K_UNDO", Keycode::K_UNDO, "The symbolic Undo key")
        .value("K_CUT", Keycode::K_CUT, "The symbolic Cut key")
        .value("K_COPY", Keycode::K_COPY, "The symbolic Copy key")
        .value("K_PASTE", Keycode::K_PASTE, "The symbolic Paste key")
        .value("K_FIND", Keycode::K_FIND, "The symbolic Find key")
        .value("K_MUTE", Keycode::K_MUTE, "The symbolic Mute key")
        .value("K_VOLUP", Keycode::K_VOLUP, "The symbolic Volume Up key")
        .value("K_VOLDOWN", Keycode::K_VOLDOWN, "The symbolic Volume Down key")
        .value("K_LCTRL", Keycode::K_LCTRL, "The symbolic Left Ctrl key")
        .value("K_LSHIFT", Keycode::K_LSHIFT, "The symbolic Left Shift key")
        .value("K_LALT", Keycode::K_LALT, "The symbolic Left Alt key")
        .value("K_LGUI", Keycode::K_LGUI, "The symbolic Left GUI/Windows key")
        .value("K_RCTRL", Keycode::K_RCTRL, "The symbolic Right Ctrl key")
        .value("K_RSHIFT", Keycode::K_RSHIFT, "The symbolic Right Shift key")
        .value("K_RALT", Keycode::K_RALT, "The symbolic Right Alt key")
        .value("K_RGUI", Keycode::K_RGUI, "The symbolic Right GUI/Windows key")
        .value("K_MODE", Keycode::K_MODE, "The symbolic Mode key")
        .value("K_SLEEP", Keycode::K_SLEEP, "The symbolic Sleep key")
        .value("K_WAKE", Keycode::K_WAKE, "The symbolic Wake key")
        .value("K_CHANNEL_INC", Keycode::K_CHANNEL_INC, "The symbolic Channel Up key")
        .value("K_CHANNEL_DEC", Keycode::K_CHANNEL_DEC, "The symbolic Channel Down key")
        .value("K_MEDIA_PLAY", Keycode::K_MEDIA_PLAY, "The symbolic Media Play key")
        .value("K_MEDIA_PAUSE", Keycode::K_MEDIA_PAUSE, "The symbolic Media Pause key")
        .value("K_MEDIA_REC", Keycode::K_MEDIA_REC, "The symbolic Media Record key")
        .value("K_MEDIA_FF", Keycode::K_MEDIA_FF, "The symbolic Media Fast Forward key")
        .value("K_MEDIA_REWIND", Keycode::K_MEDIA_REWIND, "The symbolic Media Rewind key")
        .value("K_MEDIA_NEXT", Keycode::K_MEDIA_NEXT, "The symbolic Media Next Track key")
        .value("K_MEDIA_PREV", Keycode::K_MEDIA_PREV, "The symbolic Media Previous Track key")
        .value("K_MEDIA_STOP", Keycode::K_MEDIA_STOP, "The symbolic Media Stop key")
        .value("K_MEDIA_EJECT", Keycode::K_MEDIA_EJECT, "The symbolic Media Eject key")
        .value(
            "K_MEDIA_PLAY_PAUSE", Keycode::K_MEDIA_PLAY_PAUSE, "The symbolic Media Play/Pause key"
        )
        .value("K_MEDIA_SELECT", Keycode::K_MEDIA_SELECT, "The symbolic Media Select key")
        .value("K_SOFTLEFT", Keycode::K_SOFTLEFT, "The symbolic Soft Left key")
        .value("K_SOFTRIGHT", Keycode::K_SOFTRIGHT, "The symbolic Soft Right key")
        .value("K_CALL", Keycode::K_CALL, "The symbolic Call key")
        .value("K_ENDCALL", Keycode::K_ENDCALL, "The symbolic End Call key")
        .export_values()
        .finalize();

    // Gamepad buttons
    py::native_enum<SDL_GamepadButton>(module, "GamepadButton", "enum.IntEnum", R"doc(
Gamepad button identifiers.
    )doc")
        .value("C_SOUTH", SDL_GAMEPAD_BUTTON_SOUTH, "South face button")
        .value("C_EAST", SDL_GAMEPAD_BUTTON_EAST, "East face button")
        .value("C_WEST", SDL_GAMEPAD_BUTTON_WEST, "West face button")
        .value("C_NORTH", SDL_GAMEPAD_BUTTON_NORTH, "North face button")
        .value("C_BACK", SDL_GAMEPAD_BUTTON_BACK, "Back button")
        .value("C_GUIDE", SDL_GAMEPAD_BUTTON_GUIDE, "Guide button")
        .value("C_START", SDL_GAMEPAD_BUTTON_START, "Start button")
        .value("C_LSTICK", SDL_GAMEPAD_BUTTON_LEFT_STICK, "Left stick button")
        .value("C_RSTICK", SDL_GAMEPAD_BUTTON_RIGHT_STICK, "Right stick button")
        .value("C_LSHOULDER", SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, "Left shoulder button")
        .value("C_RSHOULDER", SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, "Right shoulder button")
        .value("C_DPAD_UP", SDL_GAMEPAD_BUTTON_DPAD_UP, "D-pad up")
        .value("C_DPAD_DOWN", SDL_GAMEPAD_BUTTON_DPAD_DOWN, "D-pad down")
        .value("C_DPAD_LEFT", SDL_GAMEPAD_BUTTON_DPAD_LEFT, "D-pad left")
        .value("C_DPAD_RIGHT", SDL_GAMEPAD_BUTTON_DPAD_RIGHT, "D-pad right")
        .export_values()
        .finalize();

    // Gamepad axes
    py::native_enum<SDL_GamepadAxis>(module, "GamepadAxis", "enum.IntEnum", R"doc(
Gamepad axis identifiers.
    )doc")
        .value("C_LX", SDL_GAMEPAD_AXIS_LEFTX, "Left stick X axis")
        .value("C_LY", SDL_GAMEPAD_AXIS_LEFTY, "Left stick Y axis")
        .value("C_RX", SDL_GAMEPAD_AXIS_RIGHTX, "Right stick X axis")
        .value("C_RY", SDL_GAMEPAD_AXIS_RIGHTY, "Right stick Y axis")
        .value("C_LTRIGGER", SDL_GAMEPAD_AXIS_LEFT_TRIGGER, "Left trigger axis")
        .value("C_RTRIGGER", SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, "Right trigger axis")
        .export_values()
        .finalize();

    // Gamepad types
    py::native_enum<SDL_GamepadType>(module, "GamepadType", "enum.IntEnum", R"doc(
Gamepad device type identifiers.
    )doc")
        .value("C_STANDARD", SDL_GAMEPAD_TYPE_STANDARD, "Standard gamepad")
        .value("C_XBOX_360", SDL_GAMEPAD_TYPE_XBOX360, "Xbox 360 gamepad")
        .value("C_XBOX_ONE", SDL_GAMEPAD_TYPE_XBOXONE, "Xbox One gamepad")
        .value("C_PS3", SDL_GAMEPAD_TYPE_PS3, "PlayStation 3 gamepad")
        .value("C_PS4", SDL_GAMEPAD_TYPE_PS4, "PlayStation 4 gamepad")
        .value("C_PS5", SDL_GAMEPAD_TYPE_PS5, "PlayStation 5 gamepad")
        .value(
            "C_SWITCH_PRO", SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO, "Nintendo Switch Pro controller"
        )
        .value(
            "C_SWITCH_JOYCON_LEFT", SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT,
            "Nintendo Switch Joy-Con left"
        )
        .value(
            "C_SWITCH_JOYCON_RIGHT", SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT,
            "Nintendo Switch Joy-Con right"
        )
        .value(
            "C_SWITCH_JOYCON_PAIR", SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR,
            "Nintendo Switch Joy-Con pair"
        )
        .export_values()
        .finalize();
}
}  // namespace kn::constants
