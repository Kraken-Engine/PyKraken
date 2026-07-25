#include "kraken/input/Event.hpp"

#include <SDL3/SDL.h>

#include <unordered_map>

#include "kraken/core/Log.hpp"
#include "kraken/graphics/Window.hpp"
#include "kraken/input/Gamepad.hpp"
#include "kraken/input/Key.hpp"
#include "kraken/input/Mouse.hpp"

namespace kn
{
Event::Event(const uint32_t type)
    : type(type)
{
}

bool Event::is(const EventType t) const
{
    return type == static_cast<uint32_t>(t);
}

namespace event
{
// Track scheduled timers by event type
static std::unordered_map<uint32_t, SDL_TimerID> scheduledTimers;

void start_text_input()
{
    if (!SDL_StartTextInput(window::_get()))
    {
        throw std::runtime_error(std::string("Failed to start text input: ") + SDL_GetError());
    }
}

void stop_text_input()
{
    if (!SDL_StopTextInput(window::_get()))
    {
        throw std::runtime_error(std::string("Failed to stop text input: ") + SDL_GetError());
    }
}

const std::vector<Event> poll()
{
    gamepad::_clearStates();
    key::_clearStates();
    mouse::_clearStates();

    std::vector<Event> events;
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        Event e(event.type);

        gamepad::_handleEvents(event, e);
        key::_handleEvents(event, e);
        mouse::_handleEvents(event, e);

        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            if (window::_handlesClose())
                window::close();
            break;
        case SDL_EVENT_DISPLAY_ORIENTATION:
        case SDL_EVENT_DISPLAY_ADDED:
        case SDL_EVENT_DISPLAY_REMOVED:
        case SDL_EVENT_DISPLAY_MOVED:
        case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
        case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
        case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
        case SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED:
            e.data["display_id"] = event.display.displayID;
            e.data["data1"] = event.display.data1;
            e.data["data2"] = event.display.data2;
            break;
        case SDL_EVENT_WINDOW_SHOWN:
        case SDL_EVENT_WINDOW_HIDDEN:
        case SDL_EVENT_WINDOW_EXPOSED:
        case SDL_EVENT_WINDOW_MOVED:
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_MINIMIZED:
        case SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EVENT_WINDOW_RESTORED:
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
        case SDL_EVENT_WINDOW_FOCUS_LOST:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        case SDL_EVENT_WINDOW_HIT_TEST:
        case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
        case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
        case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
        case SDL_EVENT_WINDOW_OCCLUDED:
        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        case SDL_EVENT_WINDOW_DESTROYED:
        case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
            e.data["window_id"] = event.window.windowID;
            e.data["data1"] = event.window.data1;
            e.data["data2"] = event.window.data2;
            break;
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_UP:
        case SDL_EVENT_FINGER_MOTION:
        case SDL_EVENT_FINGER_CANCELED:
            e.data["touch_id"] = event.tfinger.touchID;
            e.data["finger_id"] = event.tfinger.fingerID;
            e.data["x"] = event.tfinger.x;
            e.data["y"] = event.tfinger.y;
            e.data["dx"] = event.tfinger.dx;
            e.data["dy"] = event.tfinger.dy;
            e.data["pressure"] = event.tfinger.pressure;
            e.data["window_id"] = event.tfinger.windowID;
            break;
        case SDL_EVENT_CLIPBOARD_UPDATE:
            e.data["num_mime_types"] = event.clipboard.num_mime_types;
            {
                std::vector<std::string> mime_types;
                mime_types.reserve(event.clipboard.num_mime_types);
                for (int i = 0; i < event.clipboard.num_mime_types; ++i)
                    mime_types.emplace_back(event.clipboard.mime_types[i]);
                e.data["mime_types"] = mime_types;
            }
            break;
        case SDL_EVENT_DROP_FILE:
        case SDL_EVENT_DROP_TEXT:
            e.data["data"] = event.drop.data;
        case SDL_EVENT_DROP_BEGIN:
        case SDL_EVENT_DROP_COMPLETE:
        case SDL_EVENT_DROP_POSITION:
            e.data["x"] = event.drop.x;
            e.data["y"] = event.drop.y;
            break;
        case SDL_EVENT_AUDIO_DEVICE_ADDED:
        case SDL_EVENT_AUDIO_DEVICE_REMOVED:
        case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
            e.data["which"] = event.adevice.which;
            e.data["recording"] = event.adevice.recording;
            break;
        case SDL_EVENT_SENSOR_UPDATE:
            e.data["which"] = event.sensor.which;
            {
                constexpr uint8_t SENSOR_DATA_SIZE = 6;
                std::vector<float> sensor_data;
                sensor_data.reserve(SENSOR_DATA_SIZE);
                for (int i = 0; i < SENSOR_DATA_SIZE; ++i)
                    sensor_data.push_back(event.sensor.data[i]);
                e.data["data"] = sensor_data;
            }
            break;
        case SDL_EVENT_PEN_PROXIMITY_IN:
        case SDL_EVENT_PEN_PROXIMITY_OUT:
            e.data["which"] = event.pproximity.which;
            break;
        case SDL_EVENT_PEN_DOWN:
        case SDL_EVENT_PEN_UP:
            e.data["which"] = event.ptouch.which;
            e.data["x"] = event.ptouch.x;
            e.data["y"] = event.ptouch.y;
            e.data["eraser"] = event.ptouch.eraser;
            break;
        case SDL_EVENT_PEN_BUTTON_DOWN:
        case SDL_EVENT_PEN_BUTTON_UP:
            e.data["which"] = event.pbutton.which;
            e.data["button"] = event.pbutton.button;
            e.data["x"] = event.pbutton.x;
            e.data["y"] = event.pbutton.y;
            break;
        case SDL_EVENT_PEN_MOTION:
            e.data["which"] = event.pmotion.which;
            e.data["x"] = event.pmotion.x;
            e.data["y"] = event.pmotion.y;
            break;
        case SDL_EVENT_PEN_AXIS:
            e.data["which"] = event.paxis.which;
            e.data["x"] = event.paxis.x;
            e.data["y"] = event.paxis.y;
            e.data["axis"] = static_cast<PenAxis>(event.paxis.axis);
            e.data["value"] = event.paxis.value;
            break;
        case SDL_EVENT_CAMERA_DEVICE_ADDED:
        case SDL_EVENT_CAMERA_DEVICE_REMOVED:
        case SDL_EVENT_CAMERA_DEVICE_APPROVED:
        case SDL_EVENT_CAMERA_DEVICE_DENIED:
            e.data["which"] = event.cdevice.which;
            break;
        case SDL_EVENT_PINCH_BEGIN:
        case SDL_EVENT_PINCH_UPDATE:
        case SDL_EVENT_PINCH_END:
            e.data["window_id"] = event.pinch.windowID;
            e.data["scale"] = event.pinch.scale;
            break;
        default:
            break;
        }

        events.push_back(std::move(e));
    }

    return events;
}

Event newCustom()
{
    const uint32_t eventType = SDL_RegisterEvents(1);
    if (eventType == 0)
    {
        throw std::runtime_error("Failed to register custom event type");
    }

    return Event(eventType);
}

void push(const Event& event)
{
    if (event.type < SDL_EVENT_USER || event.type >= SDL_EVENT_LAST)
    {
        throw std::invalid_argument("Cannot push non-custom event types");
    }

    SDL_Event sdl_event{};
    sdl_event.type = event.type;

    if (!SDL_PushEvent(&sdl_event))
    {
        throw std::runtime_error(std::string("Failed to push event: ") + SDL_GetError());
    }
}

void schedule(const Event& event, uint32_t delay_ms, bool repeat)
{
    if (event.type < SDL_EVENT_USER || event.type >= SDL_EVENT_LAST)
    {
        throw std::invalid_argument("Cannot schedule non-custom event types");
    }

    // Cancel any existing timer for this event type
    auto it = scheduledTimers.find(event.type);
    if (it != scheduledTimers.end())
    {
        SDL_RemoveTimer(it->second);
        scheduledTimers.erase(it);
    }

    SDL_Event* sdl_event = new SDL_Event{};
    sdl_event->type = event.type;

    Uint32 sdl_delay = static_cast<Uint32>(delay_ms);
    SDL_TimerID timerID;

    if (repeat)
    {
        timerID = SDL_AddTimer(
            sdl_delay,
            [](void* userData, SDL_TimerID timerID, Uint32 interval) -> Uint32
            {
                SDL_PushEvent(static_cast<SDL_Event*>(userData));
                return interval;  // Continue the timer
            },
            sdl_event
        );
    }
    else
    {
        timerID = SDL_AddTimer(
            sdl_delay,
            [](void* userData, SDL_TimerID timerID, Uint32 interval) -> Uint32
            {
                auto* evt = static_cast<SDL_Event*>(userData);
                SDL_PushEvent(evt);

                // Remove from tracking map since timer is done
                uint32_t eventType = evt->type;
                delete evt;
                scheduledTimers.erase(eventType);

                return 0;  // Stop the timer
            },
            sdl_event
        );
    }

    if (timerID == 0)
    {
        delete sdl_event;
        throw std::runtime_error(std::string("Failed to create timer: ") + SDL_GetError());
    }

    // Track the timer
    scheduledTimers[event.type] = timerID;
}

void unschedule(const Event& event)
{
    if (event.type < SDL_EVENT_USER || event.type >= SDL_EVENT_LAST)
    {
        throw std::invalid_argument("Cannot unschedule non-custom event types");
    }

    auto it = scheduledTimers.find(event.type);
    if (it != scheduledTimers.end())
    {
        SDL_RemoveTimer(it->second);
        scheduledTimers.erase(it);
    }
}

}  // namespace event
}  // namespace kn
