#include "Constants.hpp"

#include <SDL3/SDL.h>

#include "Event.hpp"
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

#ifdef KRAKEN_ENABLE_PYTHON
namespace kn::constants
{
void _bind(const nb::module_& module)
{
    // Define Anchor "enum" (class with static constants)
    nb::class_<Anchor>(module, "Anchor", R"doc(
Anchor positions returning Vec2 values for alignment.
    )doc")
        .def_prop_ro_static(
            "TOP_LEFT", [](const nb::object&) { return Anchor::TOP_LEFT; }, "(0.0, 0.0)"
        )
        .def_prop_ro_static(
            "TOP_MID", [](const nb::object&) { return Anchor::TOP_MID; }, "(0.5, 0.0)"
        )
        .def_prop_ro_static(
            "TOP_RIGHT", [](const nb::object&) { return Anchor::TOP_RIGHT; }, "(1.0, 0.0)"
        )
        .def_prop_ro_static(
            "MID_LEFT", [](const nb::object&) { return Anchor::MID_LEFT; }, "(0.0, 0.5)"
        )
        .def_prop_ro_static(
            "CENTER", [](const nb::object&) { return Anchor::CENTER; }, "(0.5, 0.5)"
        )
        .def_prop_ro_static(
            "MID_RIGHT", [](const nb::object&) { return Anchor::MID_RIGHT; }, "(1.0, 0.5)"
        )
        .def_prop_ro_static(
            "BOTTOM_LEFT", [](const nb::object&) { return Anchor::BOTTOM_LEFT; }, "(0.0, 1.0)"
        )
        .def_prop_ro_static(
            "BOTTOM_MID", [](const nb::object&) { return Anchor::BOTTOM_MID; }, "(0.5, 1.0)"
        )
        .def_prop_ro_static(
            "BOTTOM_RIGHT", [](const nb::object&) { return Anchor::BOTTOM_RIGHT; }, "(1.0, 1.0)"
        );

    // Define TextAlign enum
    nb::enum_<TextAlign>(module, "TextAlign", R"doc(
Horizontal alignment options for text.
    )doc")
        .value("LEFT", TextAlign::Left, "Left alignment")
        .value("CENTER", TextAlign::Center, "Center alignment")
        .value("RIGHT", TextAlign::Right, "Right alignment");

    // Define event types
    nb::enum_<EventType>(module, "EventType", R"doc(
SDL event type constants for input and system events.
    )doc")
        .value("QUIT", EventType::Quit, "Quit requested")
        .value("TERMINATING", EventType::Terminating, "Application is terminating")
        .value("LOW_MEMORY", EventType::LowMemory, "Low memory warning")
        .value("WILL_ENTER_BACKGROUND", EventType::WillEnterBackground, "About to enter background")
        .value("DID_ENTER_BACKGROUND", EventType::DidEnterBackground, "Entered background")
        .value("WILL_ENTER_FOREGROUND", EventType::WillEnterForeground, "About to enter foreground")
        .value("DID_ENTER_FOREGROUND", EventType::DidEnterForeground, "Entered foreground")
        .value("LOCALE_CHANGED", EventType::LocaleChanged, "Locale settings changed")
        .value("SYSTEM_THEME_CHANGED", EventType::SystemThemeChanged, "System theme changed")

        // Display events
        .value("DISPLAY_ORIENTATION", EventType::DisplayOrientation, "Display orientation changed")
        .value("DISPLAY_ADDED", EventType::DisplayAdded, "Display connected")
        .value("DISPLAY_REMOVED", EventType::DisplayRemoved, "Display disconnected")
        .value("DISPLAY_MOVED", EventType::DisplayMoved, "Display moved")
        .value(
            "DISPLAY_DESKTOP_MODE_CHANGED", EventType::DisplayDesktopModeChanged,
            "Desktop display mode changed"
        )
        .value(
            "DISPLAY_CURRENT_MODE_CHANGED", EventType::DisplayCurrentModeChanged,
            "Current display mode changed"
        )
        .value(
            "DISPLAY_CONTENT_SCALE_CHANGED", EventType::DisplayContentScaleChanged,
            "Display content scale changed"
        )
        .value(
            "DISPLAY_USABLE_BOUNDS_CHANGED", EventType::DisplayUsableBoundsChanged,
            "Usable display bounds changed"
        )
        .value("DISPLAY_FIRST", EventType::DisplayFirst, "First display event")
        .value("DISPLAY_LAST", EventType::DisplayLast, "Last display event")

        // Window events
        .value("WINDOW_SHOWN", EventType::WindowShown, "Window shown")
        .value("WINDOW_HIDDEN", EventType::WindowHidden, "Window hidden")
        .value("WINDOW_EXPOSED", EventType::WindowExposed, "Window needs redraw")
        .value("WINDOW_MOVED", EventType::WindowMoved, "Window moved")
        .value("WINDOW_RESIZED", EventType::WindowResized, "Window resized")
        .value(
            "WINDOW_PIXEL_SIZE_CHANGED", EventType::WindowPixelSizeChanged,
            "Window pixel size changed"
        )
        .value(
            "WINDOW_METAL_VIEW_RESIZED", EventType::WindowMetalViewResized,
            "Window Metal view resized"
        )
        .value("WINDOW_MINIMIZED", EventType::WindowMinimized, "Window minimized")
        .value("WINDOW_MAXIMIZED", EventType::WindowMaximized, "Window maximized")
        .value("WINDOW_RESTORED", EventType::WindowRestored, "Window restored")
        .value("WINDOW_MOUSE_ENTER", EventType::WindowMouseEnter, "Mouse entered window")
        .value("WINDOW_MOUSE_LEAVE", EventType::WindowMouseLeave, "Mouse left window")
        .value("WINDOW_FOCUS_GAINED", EventType::WindowFocusGained, "Window gained focus")
        .value("WINDOW_FOCUS_LOST", EventType::WindowFocusLost, "Window lost focus")
        .value("WINDOW_CLOSE_REQUESTED", EventType::WindowCloseRequested, "Window close requested")
        .value("WINDOW_HIT_TEST", EventType::WindowHitTest, "Window hit test request")
        .value("WINDOW_ICCPROF_CHANGED", EventType::WindowICCProfChanged, "ICC profile changed")
        .value("WINDOW_DISPLAY_CHANGED", EventType::WindowDisplayChanged, "Window display changed")
        .value(
            "WINDOW_DISPLAY_SCALE_CHANGED", EventType::WindowDisplayScaleChanged,
            "Window display scale changed"
        )
        .value(
            "WINDOW_SAFE_AREA_CHANGED", EventType::WindowSafeAreaChanged, "Window safe area changed"
        )
        .value("WINDOW_OCCLUDED", EventType::WindowOccluded, "Window occluded")
        .value("WINDOW_ENTER_FULLSCREEN", EventType::WindowEnterFullscreen, "Entered fullscreen")
        .value("WINDOW_LEAVE_FULLSCREEN", EventType::WindowLeaveFullscreen, "Left fullscreen")
        .value("WINDOW_DESTROYED", EventType::WindowDestroyed, "Window destroyed")
        .value("WINDOW_HDR_STATE_CHANGED", EventType::WindowHdrStateChanged, "HDR state changed")
        .value("WINDOW_FIRST", EventType::WindowFirst, "First window event")
        .value("WINDOW_LAST", EventType::WindowLast, "Last window event")

        // Keyboard events
        .value("KEY_DOWN", EventType::KeyDown, "Key pressed or repeating while held")
        .value("KEY_UP", EventType::KeyUp, "Key released")
        .value("TEXT_EDITING", EventType::TextEditing, "Text editing in progress")
        .value("TEXT_INPUT", EventType::TextInput, "Text input committed")
        .value("KEYMAP_CHANGED", EventType::KeymapChanged, "Keymap changed")
        .value("KEYBOARD_ADDED", EventType::KeyboardAdded, "Keyboard connected")
        .value("KEYBOARD_REMOVED", EventType::KeyboardRemoved, "Keyboard disconnected")
        .value(
            "TEXT_EDITING_CANDIDATES", EventType::TextEditingCandidates,
            "IME candidate list updated"
        )
        .value("SCREEN_KEYBOARD_SHOWN", EventType::ScreenKeyboardShown, "On-screen keyboard shown")
        .value(
            "SCREEN_KEYBOARD_HIDDEN", EventType::ScreenKeyboardHidden, "On-screen keyboard hidden"
        )

        // Mouse events
        .value("MOUSE_MOTION", EventType::MouseMotion, "Mouse moved")
        .value("MOUSE_BUTTON_DOWN", EventType::MouseButtonDown, "Mouse button pressed")
        .value("MOUSE_BUTTON_UP", EventType::MouseButtonUp, "Mouse button released")
        .value("MOUSE_WHEEL", EventType::MouseWheel, "Mouse wheel scrolled")
        .value("MOUSE_ADDED", EventType::MouseAdded, "Mouse connected")
        .value("MOUSE_REMOVED", EventType::MouseRemoved, "Mouse disconnected")

        // Joystick events
        .value("JOYSTICK_AXIS_MOTION", EventType::JoystickAxisMotion, "Joystick axis motion")
        .value("JOYSTICK_BALL_MOTION", EventType::JoystickBallMotion, "Joystick trackball motion")
        .value("JOYSTICK_HAT_MOTION", EventType::JoystickHatMotion, "Joystick hat motion")
        .value("JOYSTICK_BUTTON_DOWN", EventType::JoystickButtonDown, "Joystick button pressed")
        .value("JOYSTICK_BUTTON_UP", EventType::JoystickButtonUp, "Joystick button released")
        .value("JOYSTICK_ADDED", EventType::JoystickAdded, "Joystick connected")
        .value("JOYSTICK_REMOVED", EventType::JoystickRemoved, "Joystick disconnected")
        .value(
            "JOYSTICK_BATTERY_UPDATED", EventType::JoystickBatteryUpdated,
            "Joystick battery updated"
        )
        .value(
            "JOYSTICK_UPDATE_COMPLETE", EventType::JoystickUpdateComplete,
            "Joystick update complete"
        )

        // Gamepad events
        .value("GAMEPAD_AXIS_MOTION", EventType::GamepadAxisMotion, "Gamepad axis moved")
        .value("GAMEPAD_BUTTON_DOWN", EventType::GamepadButtonDown, "Gamepad button pressed")
        .value("GAMEPAD_BUTTON_UP", EventType::GamepadButtonUp, "Gamepad button released")
        .value("GAMEPAD_ADDED", EventType::GamepadAdded, "Gamepad connected")
        .value("GAMEPAD_REMOVED", EventType::GamepadRemoved, "Gamepad disconnected")
        .value("GAMEPAD_REMAPPED", EventType::GamepadRemapped, "Gamepad mapping updated")
        .value("GAMEPAD_TOUCHPAD_DOWN", EventType::GamepadTouchpadDown, "Touchpad pressed")
        .value("GAMEPAD_TOUCHPAD_MOTION", EventType::GamepadTouchpadMotion, "Touchpad moved")
        .value("GAMEPAD_TOUCHPAD_UP", EventType::GamepadTouchpadUp, "Touchpad released")
        .value("GAMEPAD_SENSOR_UPDATE", EventType::GamepadSensorUpdate, "Gamepad sensor updated")
        .value(
            "GAMEPAD_UPDATE_COMPLETE", EventType::GamepadUpdateComplete, "Gamepad update complete"
        )
        .value(
            "GAMEPAD_STEAM_HANDLE_UPDATED", EventType::GamepadSteamHandleUpdated,
            "Steam handle updated"
        )

        // Touch events
        .value("FINGER_DOWN", EventType::FingerDown, "Finger touch began")
        .value("FINGER_UP", EventType::FingerUp, "Finger touch ended")
        .value("FINGER_MOTION", EventType::FingerMotion, "Finger moved")
        .value("FINGER_CANCELED", EventType::FingerCanceled, "Finger touch canceled")

        // Drag and drop events
        .value("CLIPBOARD_UPDATE", EventType::ClipboardUpdate, "Clipboard content changed")
        .value("DROP_FILE", EventType::DropFile, "File dropped")
        .value("DROP_TEXT", EventType::DropText, "Text dropped")
        .value("DROP_BEGIN", EventType::DropBegin, "Drag-and-drop started")
        .value("DROP_COMPLETE", EventType::DropComplete, "Drag-and-drop completed")
        .value("DROP_POSITION", EventType::DropPosition, "Drag-and-drop position updated")

        // Audio device events
        .value("AUDIO_DEVICE_ADDED", EventType::AudioDeviceAdded, "Audio device connected")
        .value("AUDIO_DEVICE_REMOVED", EventType::AudioDeviceRemoved, "Audio device disconnected")
        .value(
            "AUDIO_DEVICE_FORMAT_CHANGED", EventType::AudioDeviceFormatChanged,
            "Audio device format changed"
        )

        // Sensor events
        .value("SENSOR_UPDATE", EventType::SensorUpdate, "Sensor data updated")

        // Pen events
        .value("PEN_PROXIMITY_IN", EventType::PenProximityIn, "Pen entered proximity")
        .value("PEN_PROXIMITY_OUT", EventType::PenProximityOut, "Pen left proximity")
        .value("PEN_DOWN", EventType::PenDown, "Pen pressed")
        .value("PEN_UP", EventType::PenUp, "Pen released")
        .value("PEN_BUTTON_DOWN", EventType::PenButtonDown, "Pen button pressed")
        .value("PEN_BUTTON_UP", EventType::PenButtonUp, "Pen button released")
        .value("PEN_MOTION", EventType::PenMotion, "Pen moved")
        .value("PEN_AXIS", EventType::PenAxis, "Pen axis data updated")

        // Camera events
        .value("CAMERA_DEVICE_ADDED", EventType::CameraDeviceAdded, "Camera connected")
        .value("CAMERA_DEVICE_REMOVED", EventType::CameraDeviceRemoved, "Camera disconnected")
        .value("CAMERA_DEVICE_APPROVED", EventType::CameraDeviceApproved, "Camera access approved")
        .value("CAMERA_DEVICE_DENIED", EventType::CameraDeviceDenied, "Camera access denied")

        // Render events
        .value("RENDER_TARGETS_RESET", EventType::RenderTargetsReset, "Render targets reset")
        .value("RENDER_DEVICE_RESET", EventType::RenderDeviceReset, "Render device reset")
        .value("RENDER_DEVICE_LOST", EventType::RenderDeviceLost, "Render device lost")

        // Pinch gesture events
        .value("PINCH_BEGIN", EventType::PinchBegin, "Pinch gesture began")
        .value("PINCH_UPDATE", EventType::PinchUpdate, "Pinch gesture updated")
        .value("PINCH_END", EventType::PinchEnd, "Pinch gesture ended")

        // Reserved / internal / user events
        .value("PRIVATE0", EventType::Private0, "Private event 0")
        .value("PRIVATE1", EventType::Private1, "Private event 1")
        .value("PRIVATE2", EventType::Private2, "Private event 2")
        .value("PRIVATE3", EventType::Private3, "Private event 3")
        .value("POLL_SENTINEL", EventType::PollSentinel, "Internal poll sentinel")
        .value("USER", EventType::User, "User event base")
        .value("LAST", EventType::Last, "Last event value")

        .export_values();

    // Mouse buttons
    nb::enum_<MouseButton>(module, "MouseButton", R"doc(
Mouse button identifiers.
    )doc")
        .value("M_LEFT", MouseButton::Left, "Left mouse button")
        .value("M_MIDDLE", MouseButton::Middle, "Middle mouse button")
        .value("M_RIGHT", MouseButton::Right, "Right mouse button")
        .value("M_SIDE1", MouseButton::Side1, "First side mouse button")
        .value("M_SIDE2", MouseButton::Side2, "Second side mouse button")
        .export_values();

    // Pen Axis
    nb::enum_<PenAxis>(module, "PenAxis", R"doc(
Stylus/pen axis identifiers for pen motion data.
    )doc")
        .value("P_PRESSURE", PenAxis::Pressure, "Pen pressure axis")
        .value("P_TILT_X", PenAxis::XTilt, "Pen X tilt axis")
        .value("P_TILT_Y", PenAxis::YTilt, "Pen Y tilt axis")
        .value("P_DISTANCE", PenAxis::Distance, "Pen distance from surface")
        .value("P_ROTATION", PenAxis::Rotation, "Pen rotation axis")
        .value("P_SLIDER", PenAxis::Slider, "Pen slider axis")
        .value("P_TANGENTIAL_PRESSURE", PenAxis::TangentialPressure, "Pen tangential pressure")
        .export_values();

    // Scancodes
    nb::enum_<Scancode>(module, "Scancode", R"doc(
Keyboard scancodes representing physical key locations.
    )doc")
        .value("S_a", Scancode::A, "The physical A key")
        .value("S_b", Scancode::B, "The physical B key")
        .value("S_c", Scancode::C, "The physical C key")
        .value("S_d", Scancode::D, "The physical D key")
        .value("S_e", Scancode::E, "The physical E key")
        .value("S_f", Scancode::F, "The physical F key")
        .value("S_g", Scancode::G, "The physical G key")
        .value("S_h", Scancode::H, "The physical H key")
        .value("S_i", Scancode::I, "The physical I key")
        .value("S_j", Scancode::J, "The physical J key")
        .value("S_k", Scancode::K, "The physical K key")
        .value("S_l", Scancode::L, "The physical L key")
        .value("S_m", Scancode::M, "The physical M key")
        .value("S_n", Scancode::N, "The physical N key")
        .value("S_o", Scancode::O, "The physical O key")
        .value("S_p", Scancode::P, "The physical P key")
        .value("S_q", Scancode::Q, "The physical Q key")
        .value("S_r", Scancode::R, "The physical R key")
        .value("S_s", Scancode::S, "The physical S key")
        .value("S_t", Scancode::T, "The physical T key")
        .value("S_u", Scancode::U, "The physical U key")
        .value("S_v", Scancode::V, "The physical V key")
        .value("S_w", Scancode::W, "The physical W key")
        .value("S_x", Scancode::X, "The physical X key")
        .value("S_y", Scancode::Y, "The physical Y key")
        .value("S_z", Scancode::Z, "The physical Z key")

        .value("S_1", Scancode::Num1, "The physical 1 key")
        .value("S_2", Scancode::Num2, "The physical 2 key")
        .value("S_3", Scancode::Num3, "The physical 3 key")
        .value("S_4", Scancode::Num4, "The physical 4 key")
        .value("S_5", Scancode::Num5, "The physical 5 key")
        .value("S_6", Scancode::Num6, "The physical 6 key")
        .value("S_7", Scancode::Num7, "The physical 7 key")
        .value("S_8", Scancode::Num8, "The physical 8 key")
        .value("S_9", Scancode::Num9, "The physical 9 key")
        .value("S_0", Scancode::Num0, "The physical 0 key")

        .value("S_RETURN", Scancode::Return, "The physical Return key")
        .value("S_ESC", Scancode::Escape, "The physical Escape key")
        .value("S_BACKSPACE", Scancode::Backspace, "The physical Backspace key")
        .value("S_TAB", Scancode::Tab, "The physical Tab key")
        .value("S_SPACE", Scancode::Space, "The physical Space key")
        .value("S_MINUS", Scancode::Minus, "The physical Minus key")
        .value("S_EQ", Scancode::Equals, "The physical Equals key")
        .value("S_LBRACKET", Scancode::LeftBracket, "The physical Left Bracket key")
        .value("S_RBRACKET", Scancode::RightBracket, "The physical Right Bracket key")
        .value("S_BACKSLASH", Scancode::Backslash, "The physical Backslash key")
        .value("S_SEMICOLON", Scancode::Semicolon, "The physical Semicolon key")
        .value("S_APOSTROPHE", Scancode::Apostrophe, "The physical Apostrophe key")
        .value("S_GRAVE", Scancode::Grave, "The physical Grave key")
        .value("S_COMMA", Scancode::Comma, "The physical Comma key")
        .value("S_PERIOD", Scancode::Period, "The physical Period key")
        .value("S_SLASH", Scancode::Slash, "The physical Slash key")
        .value("S_CAPS", Scancode::CapsLock, "The physical Caps Lock key")

        .value("S_F1", Scancode::F1, "The physical F1 key")
        .value("S_F2", Scancode::F2, "The physical F2 key")
        .value("S_F3", Scancode::F3, "The physical F3 key")
        .value("S_F4", Scancode::F4, "The physical F4 key")
        .value("S_F5", Scancode::F5, "The physical F5 key")
        .value("S_F6", Scancode::F6, "The physical F6 key")
        .value("S_F7", Scancode::F7, "The physical F7 key")
        .value("S_F8", Scancode::F8, "The physical F8 key")
        .value("S_F9", Scancode::F9, "The physical F9 key")
        .value("S_F10", Scancode::F10, "The physical F10 key")
        .value("S_F11", Scancode::F11, "The physical F11 key")
        .value("S_F12", Scancode::F12, "The physical F12 key")
        .value("S_F13", Scancode::F13, "The physical F13 key")
        .value("S_F14", Scancode::F14, "The physical F14 key")
        .value("S_F15", Scancode::F15, "The physical F15 key")

        .value("S_PRTSCR", Scancode::PrintScreen, "The physical Print Screen key")
        .value("S_SCRLK", Scancode::ScrollLock, "The physical Scroll Lock key")
        .value("S_PAUSE", Scancode::Pause, "The physical Pause key")
        .value("S_INS", Scancode::Insert, "The physical Insert key")
        .value("S_HOME", Scancode::Home, "The physical Home key")
        .value("S_PGUP", Scancode::PageUp, "The physical Page Up key")
        .value("S_DEL", Scancode::Delete, "The physical Delete key")
        .value("S_END", Scancode::End, "The physical End key")
        .value("S_PGDOWN", Scancode::PageDown, "The physical Page Down key")
        .value("S_RIGHT", Scancode::Right, "The physical Right Arrow key")
        .value("S_LEFT", Scancode::Left, "The physical Left Arrow key")
        .value("S_DOWN", Scancode::Down, "The physical Down Arrow key")
        .value("S_UP", Scancode::Up, "The physical Up Arrow key")

        .value("S_NUMLOCK", Scancode::NumLock, "The physical Num Lock key")
        .value("S_KP_DIV", Scancode::KpDivide, "The physical keypad Divide key")
        .value("S_KP_MULT", Scancode::KpMultiply, "The physical keypad Multiply key")
        .value("S_KP_MINUS", Scancode::KpMinus, "The physical keypad Minus key")
        .value("S_KP_PLUS", Scancode::KpPlus, "The physical keypad Plus key")
        .value("S_KP_ENTER", Scancode::KpEnter, "The physical keypad Enter key")
        .value("S_KP_1", Scancode::Kp1, "The physical keypad 1 key")
        .value("S_KP_2", Scancode::Kp2, "The physical keypad 2 key")
        .value("S_KP_3", Scancode::Kp3, "The physical keypad 3 key")
        .value("S_KP_4", Scancode::Kp4, "The physical keypad 4 key")
        .value("S_KP_5", Scancode::Kp5, "The physical keypad 5 key")
        .value("S_KP_6", Scancode::Kp6, "The physical keypad 6 key")
        .value("S_KP_7", Scancode::Kp7, "The physical keypad 7 key")
        .value("S_KP_8", Scancode::Kp8, "The physical keypad 8 key")
        .value("S_KP_9", Scancode::Kp9, "The physical keypad 9 key")
        .value("S_KP_0", Scancode::Kp0, "The physical keypad 0 key")
        .value("S_KP_PERIOD", Scancode::KpPeriod, "The physical keypad Period key")

        .value("S_APPLICATION", Scancode::Application, "The physical Application key")
        .value("S_POWER", Scancode::Power, "The physical Power key")
        .value("S_EXECUTE", Scancode::Execute, "The physical Execute key")
        .value("S_HELP", Scancode::Help, "The physical Help key")
        .value("S_MENU", Scancode::Menu, "The physical Menu key")
        .value("S_SELECT", Scancode::Select, "The physical Select key")
        .value("S_STOP", Scancode::Stop, "The physical Stop key")
        .value("S_AGAIN", Scancode::Again, "The physical Again key")
        .value("S_UNDO", Scancode::Undo, "The physical Undo key")
        .value("S_CUT", Scancode::Cut, "The physical Cut key")
        .value("S_COPY", Scancode::Copy, "The physical Copy key")
        .value("S_PASTE", Scancode::Paste, "The physical Paste key")
        .value("S_FIND", Scancode::Find, "The physical Find key")
        .value("S_MUTE", Scancode::Mute, "The physical Mute key")
        .value("S_VOLUP", Scancode::VolUp, "The physical Volume Up key")
        .value("S_VOLDOWN", Scancode::VolDown, "The physical Volume Down key")

        .value("S_LCTRL", Scancode::LeftCtrl, "The physical Left Ctrl key")
        .value("S_LSHIFT", Scancode::LeftShift, "The physical Left Shift key")
        .value("S_LALT", Scancode::LeftAlt, "The physical Left Alt key")
        .value("S_LGUI", Scancode::LeftGui, "The physical Left GUI/Windows key")
        .value("S_RCTRL", Scancode::RightCtrl, "The physical Right Ctrl key")
        .value("S_RSHIFT", Scancode::RightShift, "The physical Right Shift key")
        .value("S_RALT", Scancode::RightAlt, "The physical Right Alt key")
        .value("S_RGUI", Scancode::RightGui, "The physical Right GUI/Windows key")

        .value("S_MODE", Scancode::Mode, "The physical Mode key")
        .value("S_SLEEP", Scancode::Sleep, "The physical Sleep key")
        .value("S_WAKE", Scancode::Wake, "The physical Wake key")
        .value("S_CHANNEL_INC", Scancode::ChannelInc, "The physical Channel Up key")
        .value("S_CHANNEL_DEC", Scancode::ChannelDec, "The physical Channel Down key")
        .value("S_MEDIA_PLAY", Scancode::MediaPlay, "The physical Media Play key")
        .value("S_MEDIA_PAUSE", Scancode::MediaPause, "The physical Media Pause key")
        .value("S_MEDIA_REC", Scancode::MediaRec, "The physical Media Record key")
        .value(
            "S_MEDIA_FAST_FORWARD", Scancode::MediaFastForward,
            "The physical Media Fast Forward key"
        )
        .value("S_MEDIA_REWIND", Scancode::MediaRewind, "The physical Media Rewind key")
        .value("S_MEDIA_NEXT", Scancode::MediaNext, "The physical Media Next Track key")
        .value("S_MEDIA_PREV", Scancode::MediaPrev, "The physical Media Previous Track key")
        .value("S_MEDIA_STOP", Scancode::MediaStop, "The physical Media Stop key")
        .value("S_MEDIA_EJECT", Scancode::MediaEject, "The physical Media Eject key")
        .value("S_MEDIA_PLAY_PAUSE", Scancode::MediaPlayPause, "The physical Media Play/Pause key")
        .value("S_MEDIA_SELECT", Scancode::MediaSelect, "The physical Media Select key")

        .value("S_SOFTLEFT", Scancode::SoftLeft, "The physical Soft Left key")
        .value("S_SOFTRIGHT", Scancode::SoftRight, "The physical Soft Right key")
        .value("S_CALL", Scancode::Call, "The physical Call key")
        .value("S_ENDCALL", Scancode::EndCall, "The physical End Call key")

        .export_values();

    // Keycodes
    nb::enum_<Keycode>(module, "Keycode", R"doc(
Keyboard keycodes representing logical keys.
    )doc")
        .value("K_UNKNOWN", Keycode::Unknown, "Unknown key")
        .value("K_RETURN", Keycode::Return, "The symbolic Return key")
        .value("K_ESC", Keycode::Esc, "The symbolic Escape key")
        .value("K_BACKSPACE", Keycode::Backspace, "The symbolic Backspace key")
        .value("K_TAB", Keycode::Tab, "The symbolic Tab key")
        .value("K_SPACE", Keycode::Space, "The symbolic Space key")
        .value("K_EXCLAIM", Keycode::Exclaim, "The symbolic Exclamation key")
        .value("K_DBLQUOTE", Keycode::DblQuote, "The symbolic Double Quote key")
        .value("K_HASH", Keycode::Hash, "The symbolic Hash key")
        .value("K_DOLLAR", Keycode::Dollar, "The symbolic Dollar key")
        .value("K_PERCENT", Keycode::Percent, "The symbolic Percent key")
        .value("K_AMPERSAND", Keycode::Ampersand, "The symbolic Ampersand key")
        .value("K_SGLQUOTE", Keycode::SglQuote, "The symbolic Single Quote key")
        .value("K_LPAREN", Keycode::LParen, "The symbolic Left Parenthesis key")
        .value("K_RPAREN", Keycode::RParen, "The symbolic Right Parenthesis key")
        .value("K_ASTERISK", Keycode::Asterisk, "The symbolic Asterisk key")
        .value("K_PLUS", Keycode::Plus, "The symbolic Plus key")
        .value("K_COMMA", Keycode::Comma, "The symbolic Comma key")
        .value("K_MINUS", Keycode::Minus, "The symbolic Minus key")
        .value("K_PERIOD", Keycode::Period, "The symbolic Period key")
        .value("K_SLASH", Keycode::Slash, "The symbolic Slash key")
        .value("K_0", Keycode::Num0, "The symbolic 0 key")
        .value("K_1", Keycode::Num1, "The symbolic 1 key")
        .value("K_2", Keycode::Num2, "The symbolic 2 key")
        .value("K_3", Keycode::Num3, "The symbolic 3 key")
        .value("K_4", Keycode::Num4, "The symbolic 4 key")
        .value("K_5", Keycode::Num5, "The symbolic 5 key")
        .value("K_6", Keycode::Num6, "The symbolic 6 key")
        .value("K_7", Keycode::Num7, "The symbolic 7 key")
        .value("K_8", Keycode::Num8, "The symbolic 8 key")
        .value("K_9", Keycode::Num9, "The symbolic 9 key")
        .value("K_COLON", Keycode::Colon, "The symbolic Colon key")
        .value("K_SEMICOLON", Keycode::Semicolon, "The symbolic Semicolon key")
        .value("K_LT", Keycode::LessThan, "The symbolic Less-than key")
        .value("K_EQ", Keycode::Equals, "The symbolic Equals key")
        .value("K_GT", Keycode::GreaterThan, "The symbolic Greater-than key")
        .value("K_QUESTION", Keycode::Question, "The symbolic Question key")
        .value("K_AT", Keycode::At, "The symbolic At key")
        .value("K_LBRACKET", Keycode::LeftBracket, "The symbolic Left Bracket key")
        .value("K_BACKSLASH", Keycode::Backslash, "The symbolic Backslash key")
        .value("K_RBRACKET", Keycode::RightBracket, "The symbolic Right Bracket key")
        .value("K_CARET", Keycode::Caret, "The symbolic Caret key")
        .value("K_UNDERSCORE", Keycode::Underscore, "The symbolic Underscore key")
        .value("K_GRAVE", Keycode::Grave, "The symbolic Grave key")
        .value("K_a", Keycode::A, "The symbolic A key")
        .value("K_b", Keycode::B, "The symbolic B key")
        .value("K_c", Keycode::C, "The symbolic C key")
        .value("K_d", Keycode::D, "The symbolic D key")
        .value("K_e", Keycode::E, "The symbolic E key")
        .value("K_f", Keycode::F, "The symbolic F key")
        .value("K_g", Keycode::G, "The symbolic G key")
        .value("K_h", Keycode::H, "The symbolic H key")
        .value("K_i", Keycode::I, "The symbolic I key")
        .value("K_j", Keycode::J, "The symbolic J key")
        .value("K_k", Keycode::K, "The symbolic K key")
        .value("K_l", Keycode::L, "The symbolic L key")
        .value("K_m", Keycode::M, "The symbolic M key")
        .value("K_n", Keycode::N, "The symbolic N key")
        .value("K_o", Keycode::O, "The symbolic O key")
        .value("K_p", Keycode::P, "The symbolic P key")
        .value("K_q", Keycode::Q, "The symbolic Q key")
        .value("K_r", Keycode::R, "The symbolic R key")
        .value("K_s", Keycode::S, "The symbolic S key")
        .value("K_t", Keycode::T, "The symbolic T key")
        .value("K_u", Keycode::U, "The symbolic U key")
        .value("K_v", Keycode::V, "The symbolic V key")
        .value("K_w", Keycode::W, "The symbolic W key")
        .value("K_x", Keycode::X, "The symbolic X key")
        .value("K_y", Keycode::Y, "The symbolic Y key")
        .value("K_z", Keycode::Z, "The symbolic Z key")
        .value("K_LBRACE", Keycode::LeftBrace, "The symbolic Left Brace key")
        .value("K_PIPE", Keycode::Pipe, "The symbolic Pipe key")
        .value("K_RBRACE", Keycode::RightBrace, "The symbolic Right Brace key")
        .value("K_TILDE", Keycode::Tilde, "The symbolic Tilde key")
        .value("K_DEL", Keycode::Del, "The symbolic Delete key")
        .value("K_CAPS", Keycode::CapsLock, "The symbolic Caps Lock key")
        .value("K_F1", Keycode::F1, "The symbolic F1 key")
        .value("K_F2", Keycode::F2, "The symbolic F2 key")
        .value("K_F3", Keycode::F3, "The symbolic F3 key")
        .value("K_F4", Keycode::F4, "The symbolic F4 key")
        .value("K_F5", Keycode::F5, "The symbolic F5 key")
        .value("K_F6", Keycode::F6, "The symbolic F6 key")
        .value("K_F7", Keycode::F7, "The symbolic F7 key")
        .value("K_F8", Keycode::F8, "The symbolic F8 key")
        .value("K_F9", Keycode::F9, "The symbolic F9 key")
        .value("K_F10", Keycode::F10, "The symbolic F10 key")
        .value("K_F11", Keycode::F11, "The symbolic F11 key")
        .value("K_F12", Keycode::F12, "The symbolic F12 key")
        .value("K_PRTSCR", Keycode::PrintScreen, "The symbolic Print Screen key")
        .value("K_SCRLK", Keycode::ScrollLock, "The symbolic Scroll Lock key")
        .value("K_PAUSE", Keycode::Pause, "The symbolic Pause key")
        .value("K_INS", Keycode::Insert, "The symbolic Insert key")
        .value("K_HOME", Keycode::Home, "The symbolic Home key")
        .value("K_PGUP", Keycode::PageUp, "The symbolic Page Up key")
        .value("K_END", Keycode::End, "The symbolic End key")
        .value("K_PGDOWN", Keycode::PageDown, "The symbolic Page Down key")
        .value("K_RIGHT", Keycode::Right, "The symbolic Right Arrow key")
        .value("K_LEFT", Keycode::Left, "The symbolic Left Arrow key")
        .value("K_DOWN", Keycode::Down, "The symbolic Down Arrow key")
        .value("K_UP", Keycode::Up, "The symbolic Up Arrow key")
        .value("K_NUMLOCK", Keycode::NumLock, "The symbolic Num Lock key")
        .value("K_KP_DIV", Keycode::KpDivide, "The symbolic keypad Divide key")
        .value("K_KP_MULT", Keycode::KpMultiply, "The symbolic keypad Multiply key")
        .value("K_KP_MINUS", Keycode::KpMinus, "The symbolic keypad Minus key")
        .value("K_KP_PLUS", Keycode::KpPlus, "The symbolic keypad Plus key")
        .value("K_KP_ENTER", Keycode::KpEnter, "The symbolic keypad Enter key")
        .value("K_KP_1", Keycode::Kp1, "The symbolic keypad 1 key")
        .value("K_KP_2", Keycode::Kp2, "The symbolic keypad 2 key")
        .value("K_KP_3", Keycode::Kp3, "The symbolic keypad 3 key")
        .value("K_KP_4", Keycode::Kp4, "The symbolic keypad 4 key")
        .value("K_KP_5", Keycode::Kp5, "The symbolic keypad 5 key")
        .value("K_KP_6", Keycode::Kp6, "The symbolic keypad 6 key")
        .value("K_KP_7", Keycode::Kp7, "The symbolic keypad 7 key")
        .value("K_KP_8", Keycode::Kp8, "The symbolic keypad 8 key")
        .value("K_KP_9", Keycode::Kp9, "The symbolic keypad 9 key")
        .value("K_KP_0", Keycode::Kp0, "The symbolic keypad 0 key")
        .value("K_KP_PERIOD", Keycode::KpPeriod, "The symbolic keypad Period key")
        .value("K_APPLICATION", Keycode::Application, "The symbolic Application key")
        .value("K_POWER", Keycode::Power, "The symbolic Power key")
        .value("K_F13", Keycode::F13, "The symbolic F13 key")
        .value("K_F14", Keycode::F14, "The symbolic F14 key")
        .value("K_F15", Keycode::F15, "The symbolic F15 key")
        .value("K_EXECUTE", Keycode::Execute, "The symbolic Execute key")
        .value("K_HELP", Keycode::Help, "The symbolic Help key")
        .value("K_MENU", Keycode::Menu, "The symbolic Menu key")
        .value("K_SELECT", Keycode::Select, "The symbolic Select key")
        .value("K_STOP", Keycode::Stop, "The symbolic Stop key")
        .value("K_AGAIN", Keycode::Again, "The symbolic Again key")
        .value("K_UNDO", Keycode::Undo, "The symbolic Undo key")
        .value("K_CUT", Keycode::Cut, "The symbolic Cut key")
        .value("K_COPY", Keycode::Copy, "The symbolic Copy key")
        .value("K_PASTE", Keycode::Paste, "The symbolic Paste key")
        .value("K_FIND", Keycode::Find, "The symbolic Find key")
        .value("K_MUTE", Keycode::Mute, "The symbolic Mute key")
        .value("K_VOLUP", Keycode::VolUp, "The symbolic Volume Up key")
        .value("K_VOLDOWN", Keycode::VolDown, "The symbolic Volume Down key")
        .value("K_LCTRL", Keycode::LeftCtrl, "The symbolic Left Ctrl key")
        .value("K_LSHIFT", Keycode::LeftShift, "The symbolic Left Shift key")
        .value("K_LALT", Keycode::LeftAlt, "The symbolic Left Alt key")
        .value("K_LGUI", Keycode::LeftGui, "The symbolic Left GUI/Windows key")
        .value("K_RCTRL", Keycode::RightCtrl, "The symbolic Right Ctrl key")
        .value("K_RSHIFT", Keycode::RightShift, "The symbolic Right Shift key")
        .value("K_RALT", Keycode::RightAlt, "The symbolic Right Alt key")
        .value("K_RGUI", Keycode::RightGui, "The symbolic Right GUI/Windows key")
        .value("K_MODE", Keycode::Mode, "The symbolic Mode key")
        .value("K_SLEEP", Keycode::Sleep, "The symbolic Sleep key")
        .value("K_WAKE", Keycode::Wake, "The symbolic Wake key")
        .value("K_CHANNEL_INC", Keycode::ChannelInc, "The symbolic Channel Up key")
        .value("K_CHANNEL_DEC", Keycode::ChannelDec, "The symbolic Channel Down key")
        .value("K_MEDIA_PLAY", Keycode::MediaPlay, "The symbolic Media Play key")
        .value("K_MEDIA_PAUSE", Keycode::MediaPause, "The symbolic Media Pause key")
        .value("K_MEDIA_REC", Keycode::MediaRec, "The symbolic Media Record key")
        .value("K_MEDIA_FF", Keycode::MediaFastForward, "The symbolic Media Fast Forward key")
        .value("K_MEDIA_REWIND", Keycode::MediaRewind, "The symbolic Media Rewind key")
        .value("K_MEDIA_NEXT", Keycode::MediaNext, "The symbolic Media Next Track key")
        .value("K_MEDIA_PREV", Keycode::MediaPrev, "The symbolic Media Previous Track key")
        .value("K_MEDIA_STOP", Keycode::MediaStop, "The symbolic Media Stop key")
        .value("K_MEDIA_EJECT", Keycode::MediaEject, "The symbolic Media Eject key")
        .value("K_MEDIA_PLAY_PAUSE", Keycode::MediaPlayPause, "The symbolic Media Play/Pause key")
        .value("K_MEDIA_SELECT", Keycode::MediaSelect, "The symbolic Media Select key")
        .value("K_SOFTLEFT", Keycode::SoftLeft, "The symbolic Soft Left key")
        .value("K_SOFTRIGHT", Keycode::SoftRight, "The symbolic Soft Right key")
        .value("K_CALL", Keycode::Call, "The symbolic Call key")
        .value("K_ENDCALL", Keycode::EndCall, "The symbolic End Call key")
        .export_values();

    // Gamepad buttons
    nb::enum_<GamepadButton>(module, "GamepadButton", R"doc(
Gamepad button identifiers.
    )doc")
        .value("C_SOUTH", GamepadButton::South, "South face button")
        .value("C_EAST", GamepadButton::East, "East face button")
        .value("C_WEST", GamepadButton::West, "West face button")
        .value("C_NORTH", GamepadButton::North, "North face button")
        .value("C_BACK", GamepadButton::Back, "Back button")
        .value("C_GUIDE", GamepadButton::Guide, "Guide button")
        .value("C_START", GamepadButton::Start, "Start button")
        .value("C_LSTICK", GamepadButton::LeftStick, "Left stick button")
        .value("C_RSTICK", GamepadButton::RightStick, "Right stick button")
        .value("C_LSHOULDER", GamepadButton::LeftShoulder, "Left shoulder button")
        .value("C_RSHOULDER", GamepadButton::RightShoulder, "Right shoulder button")
        .value("C_DPAD_UP", GamepadButton::DpadUp, "D-pad up")
        .value("C_DPAD_DOWN", GamepadButton::DpadDown, "D-pad down")
        .value("C_DPAD_LEFT", GamepadButton::DpadLeft, "D-pad left")
        .value("C_DPAD_RIGHT", GamepadButton::DpadRight, "D-pad right")
        .export_values();

    // Gamepad axes
    nb::enum_<GamepadAxis>(module, "GamepadAxis", R"doc(
Gamepad axis identifiers.
    )doc")
        .value("C_LX", GamepadAxis::LeftX, "Left stick X axis")
        .value("C_LY", GamepadAxis::LeftY, "Left stick Y axis")
        .value("C_RX", GamepadAxis::RightX, "Right stick X axis")
        .value("C_RY", GamepadAxis::RightY, "Right stick Y axis")
        .value("C_LTRIGGER", GamepadAxis::LeftTrigger, "Left trigger axis")
        .value("C_RTRIGGER", GamepadAxis::RightTrigger, "Right trigger axis")
        .export_values();

    // Gamepad types
    nb::enum_<GamepadType>(module, "GamepadType", R"doc(
Gamepad device type identifiers.
    )doc")
        .value("C_UNKNOWN", GamepadType::Unknown, "Unknown gamepad type")
        .value("C_STANDARD", GamepadType::Standard, "Standard gamepad")
        .value("C_XBOX_360", GamepadType::Xbox360, "Xbox 360 gamepad")
        .value("C_XBOX_ONE", GamepadType::XboxOne, "Xbox One gamepad")
        .value("C_PS3", GamepadType::Ps3, "PlayStation 3 gamepad")
        .value("C_PS4", GamepadType::Ps4, "PlayStation 4 gamepad")
        .value("C_PS5", GamepadType::Ps5, "PlayStation 5 gamepad")
        .value("C_SWITCH_PRO", GamepadType::SwitchPro, "Nintendo Switch Pro controller")
        .value(
            "C_SWITCH_JOYCON_LEFT", GamepadType::SwitchJoyconLeft, "Nintendo Switch Joy-Con left"
        )
        .value(
            "C_SWITCH_JOYCON_RIGHT", GamepadType::SwitchJoyconRight, "Nintendo Switch Joy-Con right"
        )
        .value(
            "C_SWITCH_JOYCON_PAIR", GamepadType::SwitchJoyconPair, "Nintendo Switch Joy-Con pair"
        )
        .value("C_GAMECUBE", GamepadType::GameCube, "Nintendo GameCube controller")
        .export_values();
}
}  // namespace kn::constants
#endif  // KRAKEN_ENABLE_PYTHON
