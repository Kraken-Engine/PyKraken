#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <SDL3/SDL.h>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
enum class EventType : uint32_t;

struct Event
{
    using AttributeValue = std::variant<
        int, float, double, bool, uint32_t, int64_t, uint64_t, std::string, std::vector<float>,
        std::vector<std::string>, Scancode, Keycode, MouseButton, GamepadButton, GamepadAxis,
        PenAxis>;

    uint32_t type;
    std::unordered_map<std::string, AttributeValue> data;

    explicit Event(uint32_t type);

#ifdef KRAKEN_ENABLE_PYTHON
    [[nodiscard]] nb::object getAttr(const std::string& name) const;
#endif  // KRAKEN_ENABLE_PYTHON

    [[nodiscard]] bool is(const EventType t) const;

    template <typename T>
    T get(const std::string& name) const
    {
        if (auto it = data.find(name); it != data.end())
            return std::get<T>(it->second);

        throw std::runtime_error("Attribute '" + name + "' not found");
    }
};

namespace event
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

const std::vector<Event> poll();

void start_text_input();

void stop_text_input();

void push(const Event& event);

void schedule(const Event& event, uint32_t delay_ms, bool repeat = false);

void unschedule(const Event& event);

Event newCustom();
}  // namespace event

enum class EventType : uint32_t
{
    Quit = SDL_EVENT_QUIT,
    Terminating = SDL_EVENT_TERMINATING,
    LowMemory = SDL_EVENT_LOW_MEMORY,
    WillEnterBackground = SDL_EVENT_WILL_ENTER_BACKGROUND,
    DidEnterBackground = SDL_EVENT_DID_ENTER_BACKGROUND,
    WillEnterForeground = SDL_EVENT_WILL_ENTER_FOREGROUND,
    DidEnterForeground = SDL_EVENT_DID_ENTER_FOREGROUND,
    LocaleChanged = SDL_EVENT_LOCALE_CHANGED,
    SystemThemeChanged = SDL_EVENT_SYSTEM_THEME_CHANGED,

    // Display events
    DisplayOrientation = SDL_EVENT_DISPLAY_ORIENTATION,
    DisplayAdded = SDL_EVENT_DISPLAY_ADDED,
    DisplayRemoved = SDL_EVENT_DISPLAY_REMOVED,
    DisplayMoved = SDL_EVENT_DISPLAY_MOVED,
    DisplayDesktopModeChanged = SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,
    DisplayCurrentModeChanged = SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,
    DisplayContentScaleChanged = SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,
    DisplayUsableBoundsChanged = SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED,
    DisplayFirst = SDL_EVENT_DISPLAY_FIRST,
    DisplayLast = SDL_EVENT_DISPLAY_LAST,

    // Window events
    WindowShown = SDL_EVENT_WINDOW_SHOWN,
    WindowHidden = SDL_EVENT_WINDOW_HIDDEN,
    WindowExposed = SDL_EVENT_WINDOW_EXPOSED,
    WindowMoved = SDL_EVENT_WINDOW_MOVED,
    WindowResized = SDL_EVENT_WINDOW_RESIZED,
    WindowPixelSizeChanged = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
    WindowMetalViewResized = SDL_EVENT_WINDOW_METAL_VIEW_RESIZED,
    WindowMinimized = SDL_EVENT_WINDOW_MINIMIZED,
    WindowMaximized = SDL_EVENT_WINDOW_MAXIMIZED,
    WindowRestored = SDL_EVENT_WINDOW_RESTORED,
    WindowMouseEnter = SDL_EVENT_WINDOW_MOUSE_ENTER,
    WindowMouseLeave = SDL_EVENT_WINDOW_MOUSE_LEAVE,
    WindowFocusGained = SDL_EVENT_WINDOW_FOCUS_GAINED,
    WindowFocusLost = SDL_EVENT_WINDOW_FOCUS_LOST,
    WindowCloseRequested = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
    WindowHitTest = SDL_EVENT_WINDOW_HIT_TEST,
    WindowICCProfChanged = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
    WindowDisplayChanged = SDL_EVENT_WINDOW_DISPLAY_CHANGED,
    WindowDisplayScaleChanged = SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
    WindowSafeAreaChanged = SDL_EVENT_WINDOW_SAFE_AREA_CHANGED,
    WindowOccluded = SDL_EVENT_WINDOW_OCCLUDED,
    WindowEnterFullscreen = SDL_EVENT_WINDOW_ENTER_FULLSCREEN,
    WindowLeaveFullscreen = SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,
    WindowDestroyed = SDL_EVENT_WINDOW_DESTROYED,
    WindowHdrStateChanged = SDL_EVENT_WINDOW_HDR_STATE_CHANGED,
    WindowFirst = SDL_EVENT_WINDOW_FIRST,
    WindowLast = SDL_EVENT_WINDOW_LAST,

    // Keyboard events
    KeyDown = SDL_EVENT_KEY_DOWN,
    KeyUp = SDL_EVENT_KEY_UP,
    TextEditing = SDL_EVENT_TEXT_EDITING,
    TextInput = SDL_EVENT_TEXT_INPUT,
    KeymapChanged = SDL_EVENT_KEYMAP_CHANGED,
    KeyboardAdded = SDL_EVENT_KEYBOARD_ADDED,
    KeyboardRemoved = SDL_EVENT_KEYBOARD_REMOVED,
    TextEditingCandidates = SDL_EVENT_TEXT_EDITING_CANDIDATES,
    ScreenKeyboardShown = SDL_EVENT_SCREEN_KEYBOARD_SHOWN,
    ScreenKeyboardHidden = SDL_EVENT_SCREEN_KEYBOARD_HIDDEN,

    // Mouse events
    MouseMotion = SDL_EVENT_MOUSE_MOTION,
    MouseButtonDown = SDL_EVENT_MOUSE_BUTTON_DOWN,
    MouseButtonUp = SDL_EVENT_MOUSE_BUTTON_UP,
    MouseWheel = SDL_EVENT_MOUSE_WHEEL,
    MouseAdded = SDL_EVENT_MOUSE_ADDED,
    MouseRemoved = SDL_EVENT_MOUSE_REMOVED,

    // Joystick events
    JoystickAxisMotion = SDL_EVENT_JOYSTICK_AXIS_MOTION,
    JoystickBallMotion = SDL_EVENT_JOYSTICK_BALL_MOTION,
    JoystickHatMotion = SDL_EVENT_JOYSTICK_HAT_MOTION,
    JoystickButtonDown = SDL_EVENT_JOYSTICK_BUTTON_DOWN,
    JoystickButtonUp = SDL_EVENT_JOYSTICK_BUTTON_UP,
    JoystickAdded = SDL_EVENT_JOYSTICK_ADDED,
    JoystickRemoved = SDL_EVENT_JOYSTICK_REMOVED,
    JoystickBatteryUpdated = SDL_EVENT_JOYSTICK_BATTERY_UPDATED,
    JoystickUpdateComplete = SDL_EVENT_JOYSTICK_UPDATE_COMPLETE,

    // Gamepad events
    GamepadAxisMotion = SDL_EVENT_GAMEPAD_AXIS_MOTION,
    GamepadButtonDown = SDL_EVENT_GAMEPAD_BUTTON_DOWN,
    GamepadButtonUp = SDL_EVENT_GAMEPAD_BUTTON_UP,
    GamepadAdded = SDL_EVENT_GAMEPAD_ADDED,
    GamepadRemoved = SDL_EVENT_GAMEPAD_REMOVED,
    GamepadRemapped = SDL_EVENT_GAMEPAD_REMAPPED,
    GamepadTouchpadDown = SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN,
    GamepadTouchpadMotion = SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION,
    GamepadTouchpadUp = SDL_EVENT_GAMEPAD_TOUCHPAD_UP,
    GamepadSensorUpdate = SDL_EVENT_GAMEPAD_SENSOR_UPDATE,
    GamepadUpdateComplete = SDL_EVENT_GAMEPAD_UPDATE_COMPLETE,
    GamepadSteamHandleUpdated = SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED,

    // Touch events
    FingerDown = SDL_EVENT_FINGER_DOWN,
    FingerUp = SDL_EVENT_FINGER_UP,
    FingerMotion = SDL_EVENT_FINGER_MOTION,
    FingerCanceled = SDL_EVENT_FINGER_CANCELED,

    // Drag and drop events
    ClipboardUpdate = SDL_EVENT_CLIPBOARD_UPDATE,
    DropFile = SDL_EVENT_DROP_FILE,
    DropText = SDL_EVENT_DROP_TEXT,
    DropBegin = SDL_EVENT_DROP_BEGIN,
    DropComplete = SDL_EVENT_DROP_COMPLETE,
    DropPosition = SDL_EVENT_DROP_POSITION,

    // Audio device events
    AudioDeviceAdded = SDL_EVENT_AUDIO_DEVICE_ADDED,
    AudioDeviceRemoved = SDL_EVENT_AUDIO_DEVICE_REMOVED,
    AudioDeviceFormatChanged = SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,

    // Sensor events
    SensorUpdate = SDL_EVENT_SENSOR_UPDATE,

    // Pen events
    PenProximityIn = SDL_EVENT_PEN_PROXIMITY_IN,
    PenProximityOut = SDL_EVENT_PEN_PROXIMITY_OUT,
    PenDown = SDL_EVENT_PEN_DOWN,
    PenUp = SDL_EVENT_PEN_UP,
    PenButtonDown = SDL_EVENT_PEN_BUTTON_DOWN,
    PenButtonUp = SDL_EVENT_PEN_BUTTON_UP,
    PenMotion = SDL_EVENT_PEN_MOTION,
    PenAxis = SDL_EVENT_PEN_AXIS,

    // Camera events
    CameraDeviceAdded = SDL_EVENT_CAMERA_DEVICE_ADDED,
    CameraDeviceRemoved = SDL_EVENT_CAMERA_DEVICE_REMOVED,
    CameraDeviceApproved = SDL_EVENT_CAMERA_DEVICE_APPROVED,
    CameraDeviceDenied = SDL_EVENT_CAMERA_DEVICE_DENIED,

    // Render events
    RenderTargetsReset = SDL_EVENT_RENDER_TARGETS_RESET,
    RenderDeviceReset = SDL_EVENT_RENDER_DEVICE_RESET,
    RenderDeviceLost = SDL_EVENT_RENDER_DEVICE_LOST,

    // Pinch gesture events
    PinchBegin = SDL_EVENT_PINCH_BEGIN,
    PinchUpdate = SDL_EVENT_PINCH_UPDATE,
    PinchEnd = SDL_EVENT_PINCH_END,

    // Reserved / internal / user events
    Private0 = SDL_EVENT_PRIVATE0,
    Private1 = SDL_EVENT_PRIVATE1,
    Private2 = SDL_EVENT_PRIVATE2,
    Private3 = SDL_EVENT_PRIVATE3,
    PollSentinel = SDL_EVENT_POLL_SENTINEL,
    User = SDL_EVENT_USER,
    Last = SDL_EVENT_LAST,
};

}  // namespace kn
