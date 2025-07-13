from __future__ import annotations
import typing
from . import color
from . import ease
from . import event
from . import gamepad
from . import input
from . import key
from . import line
from . import math
from . import mouse
from . import rect
from . import time
from . import transform
from . import window
__all__ = ['AUDIO_DEVICE_ADDED', 'AUDIO_DEVICE_REMOVED', 'Anchor', 'BOTTOM_LEFT', 'BOTTOM_MID', 'BOTTOM_RIGHT', 'CAMERA_ADDED', 'CAMERA_APPROVED', 'CAMERA_DENIED', 'CAMERA_REMOVED', 'CENTER', 'C_BACK', 'C_DPADDOWN', 'C_DPADLEFT', 'C_DPADRIGHT', 'C_DPADUP', 'C_EAST', 'C_GUIDE', 'C_LEFTSHOULDER', 'C_LEFTSTICK', 'C_LTRIGGER', 'C_LX', 'C_LY', 'C_NORTH', 'C_PS3', 'C_PS4', 'C_PS5', 'C_RIGHTSHOULDER', 'C_RIGHTSTICK', 'C_RTRIGGER', 'C_RX', 'C_RY', 'C_SOUTH', 'C_STANDARD', 'C_START', 'C_SWITCHJOYCONLEFT', 'C_SWITCHJOYCONPAIR', 'C_SWITCHJOYCONRIGHT', 'C_SWITCHPRO', 'C_WEST', 'C_XBOX360', 'C_XBOXONE', 'Camera', 'Circle', 'Clock', 'Color', 'DROP_BEGIN', 'DROP_COMPLETE', 'DROP_FILE', 'DROP_POSITION', 'DROP_TEXT', 'EasingAnimation', 'Event', 'EventType', 'GAMEPAD_ADDED', 'GAMEPAD_AXIS_MOTION', 'GAMEPAD_BUTTON_DOWN', 'GAMEPAD_BUTTON_UP', 'GAMEPAD_REMOVED', 'GAMEPAD_TOUCHPAD_DOWN', 'GAMEPAD_TOUCHPAD_MOTION', 'GAMEPAD_TOUCHPAD_UP', 'GamepadAxis', 'GamepadButton', 'GamepadType', 'InputAction', 'KEYBOARD_ADDED', 'KEYBOARD_REMOVED', 'KEY_DOWN', 'KEY_UP', 'K_0', 'K_1', 'K_2', 'K_3', 'K_4', 'K_5', 'K_6', 'K_7', 'K_8', 'K_9', 'K_AGAIN', 'K_AMPERSAND', 'K_ASTERISK', 'K_AT', 'K_BACKSLASH', 'K_BACKSPACE', 'K_CAPS', 'K_CARET', 'K_COLON', 'K_COMMA', 'K_COPY', 'K_CUT', 'K_DBLQUOTE', 'K_DEL', 'K_DOLLAR', 'K_DOWN', 'K_END', 'K_EQ', 'K_ESC', 'K_EXCLAIM', 'K_F1', 'K_F10', 'K_F11', 'K_F12', 'K_F2', 'K_F3', 'K_F4', 'K_F5', 'K_F6', 'K_F7', 'K_F8', 'K_F9', 'K_FIND', 'K_GRAVE', 'K_GT', 'K_HASH', 'K_HOME', 'K_INS', 'K_KP_0', 'K_KP_1', 'K_KP_2', 'K_KP_3', 'K_KP_4', 'K_KP_5', 'K_KP_6', 'K_KP_7', 'K_KP_8', 'K_KP_9', 'K_KP_DIV', 'K_KP_ENTER', 'K_KP_MINUS', 'K_KP_MULT', 'K_KP_PERIOD', 'K_KP_PLUS', 'K_LALT', 'K_LBRACE', 'K_LBRACKET', 'K_LCTRL', 'K_LEFT', 'K_LGUI', 'K_LPAREN', 'K_LSHIFT', 'K_LT', 'K_MINUS', 'K_MUTE', 'K_NUMLOCK', 'K_PASTE', 'K_PAUSE', 'K_PERCENT', 'K_PERIOD', 'K_PGDOWN', 'K_PGUP', 'K_PIPE', 'K_PLUS', 'K_PRTSCR', 'K_QUESTION', 'K_RALT', 'K_RBRACE', 'K_RBRACKET', 'K_RCTRL', 'K_RETURN', 'K_RGUI', 'K_RIGHT', 'K_RPAREN', 'K_RSHIFT', 'K_SCRLK', 'K_SEMICOLON', 'K_SGLQUOTE', 'K_SLASH', 'K_SPACE', 'K_TAB', 'K_TILDE', 'K_UNDERSCORE', 'K_UNDO', 'K_UP', 'K_VOLDOWN', 'K_VOLUP', 'K_a', 'K_b', 'K_c', 'K_d', 'K_e', 'K_f', 'K_g', 'K_h', 'K_i', 'K_j', 'K_k', 'K_l', 'K_m', 'K_n', 'K_o', 'K_p', 'K_q', 'K_r', 'K_s', 'K_t', 'K_u', 'K_v', 'K_w', 'K_x', 'K_y', 'K_z', 'Keycode', 'Line', 'MID_LEFT', 'MID_RIGHT', 'MOUSE_ADDED', 'MOUSE_BUTTON_DOWN', 'MOUSE_BUTTON_UP', 'MOUSE_MOTION', 'MOUSE_REMOVED', 'MOUSE_WHEEL', 'M_LEFT', 'M_MIDDLE', 'M_RIGHT', 'M_SIDE1', 'M_SIDE2', 'MouseButton', 'PEN_AXIS', 'PEN_BUTTON_DOWN', 'PEN_BUTTON_UP', 'PEN_DOWN', 'PEN_MOTION', 'PEN_PROXIMITY_IN', 'PEN_PROXIMITY_OUT', 'PEN_UP', 'PolarCoordinate', 'QUIT', 'Rect', 'Renderer', 'S_0', 'S_1', 'S_2', 'S_3', 'S_4', 'S_5', 'S_6', 'S_7', 'S_8', 'S_9', 'S_AGAIN', 'S_APOSTROPHE', 'S_BACKSLASH', 'S_BACKSPACE', 'S_CAPS', 'S_COMMA', 'S_COPY', 'S_CUT', 'S_DEL', 'S_DOWN', 'S_END', 'S_EQ', 'S_ESC', 'S_F1', 'S_F10', 'S_F11', 'S_F12', 'S_F2', 'S_F3', 'S_F4', 'S_F5', 'S_F6', 'S_F7', 'S_F8', 'S_F9', 'S_FIND', 'S_GRAVE', 'S_HOME', 'S_INS', 'S_KP_0', 'S_KP_1', 'S_KP_2', 'S_KP_3', 'S_KP_4', 'S_KP_5', 'S_KP_6', 'S_KP_7', 'S_KP_8', 'S_KP_9', 'S_KP_DIV', 'S_KP_ENTER', 'S_KP_MINUS', 'S_KP_MULT', 'S_KP_PERIOD', 'S_KP_PLUS', 'S_LALT', 'S_LBRACKET', 'S_LCTRL', 'S_LEFT', 'S_LGUI', 'S_LSHIFT', 'S_MINUS', 'S_MUTE', 'S_NUMLOCK', 'S_PASTE', 'S_PAUSE', 'S_PERIOD', 'S_PGDOWN', 'S_PGUP', 'S_PRTSCR', 'S_RALT', 'S_RBRACKET', 'S_RCTRL', 'S_RETURN', 'S_RGUI', 'S_RIGHT', 'S_RSHIFT', 'S_SCRLK', 'S_SEMICOLON', 'S_SLASH', 'S_SPACE', 'S_TAB', 'S_UNDO', 'S_UP', 'S_VOLDOWN', 'S_VOLUP', 'S_a', 'S_b', 'S_c', 'S_d', 'S_e', 'S_f', 'S_g', 'S_h', 'S_i', 'S_j', 'S_k', 'S_l', 'S_m', 'S_n', 'S_o', 'S_p', 'S_q', 'S_r', 'S_s', 'S_t', 'S_u', 'S_v', 'S_w', 'S_x', 'S_y', 'S_z', 'Scancode', 'Surface', 'TEXT_EDITING', 'TEXT_INPUT', 'TOP_LEFT', 'TOP_MID', 'TOP_RIGHT', 'Texture', 'Vec2', 'WINDOW_ENTER_FULLSCREEN', 'WINDOW_EXPOSED', 'WINDOW_FOCUS_GAINED', 'WINDOW_FOCUS_LOST', 'WINDOW_HIDDEN', 'WINDOW_LEAVE_FULLSCREEN', 'WINDOW_MAXIMIZED', 'WINDOW_MINIMIZED', 'WINDOW_MOUSE_ENTER', 'WINDOW_MOUSE_LEAVE', 'WINDOW_MOVED', 'WINDOW_OCCLUDED', 'WINDOW_RESIZED', 'WINDOW_RESTORED', 'WINDOW_SHOWN', 'color', 'ease', 'event', 'gamepad', 'init', 'input', 'key', 'line', 'math', 'mouse', 'quit', 'rect', 'time', 'transform', 'window']
class Anchor:
    """
    Members:
    
      TOP_LEFT
    
      TOP_MID
    
      TOP_RIGHT
    
      MID_LEFT
    
      CENTER
    
      MID_RIGHT
    
      BOTTOM_LEFT
    
      BOTTOM_MID
    
      BOTTOM_RIGHT
    """
    BOTTOM_LEFT: typing.ClassVar[Anchor]  # value = <Anchor.BOTTOM_LEFT: 6>
    BOTTOM_MID: typing.ClassVar[Anchor]  # value = <Anchor.BOTTOM_MID: 7>
    BOTTOM_RIGHT: typing.ClassVar[Anchor]  # value = <Anchor.BOTTOM_RIGHT: 8>
    CENTER: typing.ClassVar[Anchor]  # value = <Anchor.CENTER: 4>
    MID_LEFT: typing.ClassVar[Anchor]  # value = <Anchor.MID_LEFT: 3>
    MID_RIGHT: typing.ClassVar[Anchor]  # value = <Anchor.MID_RIGHT: 5>
    TOP_LEFT: typing.ClassVar[Anchor]  # value = <Anchor.TOP_LEFT: 0>
    TOP_MID: typing.ClassVar[Anchor]  # value = <Anchor.TOP_MID: 1>
    TOP_RIGHT: typing.ClassVar[Anchor]  # value = <Anchor.TOP_RIGHT: 2>
    __members__: typing.ClassVar[dict[str, Anchor]]  # value = {'TOP_LEFT': <Anchor.TOP_LEFT: 0>, 'TOP_MID': <Anchor.TOP_MID: 1>, 'TOP_RIGHT': <Anchor.TOP_RIGHT: 2>, 'MID_LEFT': <Anchor.MID_LEFT: 3>, 'CENTER': <Anchor.CENTER: 4>, 'MID_RIGHT': <Anchor.MID_RIGHT: 5>, 'BOTTOM_LEFT': <Anchor.BOTTOM_LEFT: 6>, 'BOTTOM_MID': <Anchor.BOTTOM_MID: 7>, 'BOTTOM_RIGHT': <Anchor.BOTTOM_RIGHT: 8>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Camera:
    """
    
    Represents a 2D camera used for rendering.
    
    Controls the viewport's translation, allowing you to move the view of the world.
        
    """
    def __init__(self, pos: Vec2 = ...) -> None:
        """
        Create a camera at the given position.
        
        Args:
            pos (Vec2, optional): The camera's initial position. Default set to (0, 0).
        """
    def set(self) -> None:
        """
        Set this camera as the active one for rendering.
        
        Only one camera can be active at a time.
        """
    @property
    def pos(self) -> Vec2:
        """
        Get or set the camera's position.
        
        Returns:
            Vec2: The camera's current position.
        
        You can also assign a Vec2 or a (x, y) sequence to set the position.
        """
    @pos.setter
    def pos(self, arg1: Vec2) -> None:
        ...
class Circle:
    """
    
    Represents a circle shape with position and radius.
    
    Supports collision detection with points, rectangles, other circles, and lines.
        
    """
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, other: Circle) -> bool:
        """
        Check if two circles are equal.
        """
    def __getitem__(self, index: int) -> float:
        """
        Get component by index: 0 = x, 1 = y, 2 = radius.
        """
    @typing.overload
    def __init__(self, pos: Vec2, radius: float) -> None:
        """
        Create a circle at a given position and radius.
        
        Args:
            pos (Vec2): Center position of the circle.
            radius (float): Radius of the circle.
        """
    @typing.overload
    def __init__(self, arg0: typing.Sequence) -> None:
        """
        Create a circle from a nested sequence: ([x, y], radius).
        """
    def __iter__(self) -> typing.Iterator:
        """
        Return an iterator over (x, y, radius).
        """
    def __len__(self) -> int:
        """
        Always returns 3 for (x, y, radius).
        """
    def __ne__(self, other: Circle) -> bool:
        """
        Check if two circles are not equal.
        """
    def as_rect(self) -> Rect:
        """
        Return the smallest rectangle that fully contains the circle.
        """
    def collide_circle(self, circle: Circle) -> bool:
        """
        Check collision with another circle.
        """
    def collide_line(self, line: Line) -> bool:
        """
        Check collision with a line.
        """
    def collide_point(self, point: typing.Any) -> bool:
        """
        Check if a point lies inside the circle.
        
        Args:
            point (Vec2 or tuple): The point to test.
        """
    def collide_rect(self, rect: Rect) -> bool:
        """
        Check collision with a rectangle.
        """
    def contains(self, shape: typing.Any) -> bool:
        """
        Check if the circle fully contains the given shape.
        
        Args:
            shape (Vec2, Circle, or Rect): The shape to test.
        """
    def copy(self) -> Circle:
        """
        Return a copy of the circle.
        """
    @property
    def area(self) -> float:
        """
        Return the area of the circle.
        """
    @property
    def circumference(self) -> float:
        """
        Return the circumference of the circle.
        """
    @property
    def pos(self) -> Vec2:
        """
        The center position of the circle as a Vec2.
        """
    @pos.setter
    def pos(self, arg0: Vec2) -> None:
        ...
    @property
    def radius(self) -> float:
        """
        The radius of the circle.
        """
    @radius.setter
    def radius(self, arg0: float) -> None:
        ...
class Clock:
    """
    
    A clock for tracking frame time and controlling framerate.
    
    The Clock class is used to measure time between frames and optionally
    limit the framerate of your application. It's essential for creating
    smooth, frame-rate independent animations and game loops.
        
    """
    def __init__(self) -> None:
        """
        Create a new Clock instance.
        
        The clock starts measuring time immediately upon creation.
        """
    def get_fps(self) -> int:
        """
        Get the current frames per second of the program.
        
        Returns:
            int: The current FPS based on the last frame time.
        """
    def tick(self, frame_rate: int = 0) -> float:
        """
        Get the time since the last frame and optionally cap the framerate.
        
        This method should be called once per frame in your main loop. It returns
        the time elapsed since the last call to tick(), which can be used for
        frame-rate independent animations.
        
        Args:
            frame_rate (int, optional): Maximum framerate to enforce. If 0, no limit is applied.
                                       Defaults to 0 (unlimited).
        
        Returns:
            float: The time elapsed since the last tick() call, in seconds.
        """
class Color:
    """
    
    Represents an RGBA color.
    
    Each channel (r, g, b, a) is an 8-bit unsigned integer.
        
    """
    def __getitem__(self, index: int) -> int:
        """
        Access color channels by index (0=r, 1=g, 2=b, 3=a).
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Create a Color with default values (0, 0, 0, 255).
        """
    @typing.overload
    def __init__(self, r: int, g: int, b: int, a: int = 255) -> None:
        """
        Create a Color from RGBA components.
        
        Args:
            r (int): Red value [0-255].
            g (int): Green value [0-255].
            b (int): Blue value [0-255].
            a (int, optional): Alpha value [0-255]. Defaults to 255.
        """
    @typing.overload
    def __init__(self, arg0: typing.Any) -> None:
        """
        Create a Color from a hex string or a sequence of RGB(A) integers.
        
        Examples:
            Color("#ff00ff")
            Color([255, 0, 255])
            Color((255, 0, 255, 128))
        """
    def __iter__(self) -> typing.Iterator:
        """
        Return an iterator over (r, g, b, a).
        """
    def __len__(self) -> int:
        """
        Return the number of channels (always 4).
        """
    def __repr__(self) -> str:
        """
        Return a string suitable for debugging and recreation.
        """
    def __setitem__(self, index: int, value: int) -> None:
        """
        Set a color channel by index.
        """
    def __str__(self) -> str:
        """
        Return a human-readable string representation.
        """
    @property
    def a(self) -> int:
        """
        Alpha channel (0-255).
        """
    @a.setter
    def a(self, arg0: int) -> None:
        ...
    @property
    def b(self) -> int:
        """
        Blue channel (0-255).
        """
    @b.setter
    def b(self, arg0: int) -> None:
        ...
    @property
    def g(self) -> int:
        """
        Green channel (0-255).
        """
    @g.setter
    def g(self, arg0: int) -> None:
        ...
    @property
    def hex(self) -> str:
        """
        Get or set the color as a hex string (e.g. "#FF00FF" or "#FF00FF80").
        """
    @hex.setter
    def hex(self, arg1: str) -> None:
        ...
    @property
    def r(self) -> int:
        """
        Red channel (0-255).
        """
    @r.setter
    def r(self, arg0: int) -> None:
        ...
class EasingAnimation:
    """
    
    A class for animating values over time using easing functions.
    
    This class supports pausing, resuming, reversing, and checking progress.
        
    """
    def __init__(self, start: Vec2, end: Vec2, duration: float, easeFunc: typing.Callable[[float], float]) -> None:
        """
        Create an EasingAnimation.
        
        Args:
            start (Vec2): Starting position.
            end (Vec2): Ending position.
            duration (float): Time in seconds for full animation.
            easeFunc (Callable): Easing function that maps [0, 1] → [0, 1].
        """
    def pause(self) -> None:
        """
        Pause the animation's progression.
        """
    def restart(self) -> None:
        """
        Restart the animation from the beginning.
        """
    def resume(self) -> None:
        """
        Resume the animation from its current state.
        """
    def reverse(self) -> None:
        """
        Reverse the direction of the animation.
        """
    def step(self, delta: float) -> Vec2:
        """
        Advance the animation by delta time and return the current position.
        
        Args:
            delta (float): Time step to progress the animation.
        Returns:
            Vec2: Interpolated position.
        """
    @property
    def is_done(self) -> bool:
        """
        Check whether the animation has finished.
        """
class Event:
    """
    
    Represents a single input event such as keyboard, mouse, or gamepad activity.
    
    Attributes:
        type (int): Event type. Additional fields are accessed dynamically.
            
    """
    def __getattr__(self, arg0: str) -> typing.Any:
        """
        Dynamically access event attributes.
        
        Examples:
            event.key
            event.button
            event.pos
        
        Raises:
            AttributeError: If the requested attribute doesn't exist.
        """
    @property
    def type(self) -> int:
        """
        The event type (e.g., KEY_DOWN, MOUSE_BUTTON_UP).
        """
class EventType:
    """
    Members:
    
      QUIT
    
      WINDOW_SHOWN
    
      WINDOW_HIDDEN
    
      WINDOW_EXPOSED
    
      WINDOW_MOVED
    
      WINDOW_RESIZED
    
      WINDOW_MINIMIZED
    
      WINDOW_MAXIMIZED
    
      WINDOW_RESTORED
    
      WINDOW_MOUSE_ENTER
    
      WINDOW_MOUSE_LEAVE
    
      WINDOW_FOCUS_GAINED
    
      WINDOW_FOCUS_LOST
    
      WINDOW_OCCLUDED
    
      WINDOW_ENTER_FULLSCREEN
    
      WINDOW_LEAVE_FULLSCREEN
    
      KEY_DOWN
    
      KEY_UP
    
      TEXT_EDITING
    
      TEXT_INPUT
    
      KEYBOARD_ADDED
    
      KEYBOARD_REMOVED
    
      MOUSE_MOTION
    
      MOUSE_BUTTON_DOWN
    
      MOUSE_BUTTON_UP
    
      MOUSE_WHEEL
    
      MOUSE_ADDED
    
      MOUSE_REMOVED
    
      GAMEPAD_AXIS_MOTION
    
      GAMEPAD_BUTTON_DOWN
    
      GAMEPAD_BUTTON_UP
    
      GAMEPAD_ADDED
    
      GAMEPAD_REMOVED
    
      GAMEPAD_TOUCHPAD_DOWN
    
      GAMEPAD_TOUCHPAD_MOTION
    
      GAMEPAD_TOUCHPAD_UP
    
      DROP_FILE
    
      DROP_TEXT
    
      DROP_BEGIN
    
      DROP_COMPLETE
    
      DROP_POSITION
    
      AUDIO_DEVICE_ADDED
    
      AUDIO_DEVICE_REMOVED
    
      PEN_PROXIMITY_IN
    
      PEN_PROXIMITY_OUT
    
      PEN_DOWN
    
      PEN_UP
    
      PEN_BUTTON_DOWN
    
      PEN_BUTTON_UP
    
      PEN_MOTION
    
      PEN_AXIS
    
      CAMERA_ADDED
    
      CAMERA_REMOVED
    
      CAMERA_APPROVED
    
      CAMERA_DENIED
    """
    AUDIO_DEVICE_ADDED: typing.ClassVar[EventType]  # value = <EventType.AUDIO_DEVICE_ADDED: 4352>
    AUDIO_DEVICE_REMOVED: typing.ClassVar[EventType]  # value = <EventType.AUDIO_DEVICE_REMOVED: 4353>
    CAMERA_ADDED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_ADDED: 5120>
    CAMERA_APPROVED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_APPROVED: 5122>
    CAMERA_DENIED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_DENIED: 5123>
    CAMERA_REMOVED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_REMOVED: 5121>
    DROP_BEGIN: typing.ClassVar[EventType]  # value = <EventType.DROP_BEGIN: 4098>
    DROP_COMPLETE: typing.ClassVar[EventType]  # value = <EventType.DROP_COMPLETE: 4099>
    DROP_FILE: typing.ClassVar[EventType]  # value = <EventType.DROP_FILE: 4096>
    DROP_POSITION: typing.ClassVar[EventType]  # value = <EventType.DROP_POSITION: 4100>
    DROP_TEXT: typing.ClassVar[EventType]  # value = <EventType.DROP_TEXT: 4097>
    GAMEPAD_ADDED: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_ADDED: 1619>
    GAMEPAD_AXIS_MOTION: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_AXIS_MOTION: 1616>
    GAMEPAD_BUTTON_DOWN: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_BUTTON_DOWN: 1617>
    GAMEPAD_BUTTON_UP: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_BUTTON_UP: 1618>
    GAMEPAD_REMOVED: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_REMOVED: 1620>
    GAMEPAD_TOUCHPAD_DOWN: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_TOUCHPAD_DOWN: 1622>
    GAMEPAD_TOUCHPAD_MOTION: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_TOUCHPAD_MOTION: 1623>
    GAMEPAD_TOUCHPAD_UP: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_TOUCHPAD_UP: 1624>
    KEYBOARD_ADDED: typing.ClassVar[EventType]  # value = <EventType.KEYBOARD_ADDED: 773>
    KEYBOARD_REMOVED: typing.ClassVar[EventType]  # value = <EventType.KEYBOARD_REMOVED: 774>
    KEY_DOWN: typing.ClassVar[EventType]  # value = <EventType.KEY_DOWN: 768>
    KEY_UP: typing.ClassVar[EventType]  # value = <EventType.KEY_UP: 769>
    MOUSE_ADDED: typing.ClassVar[EventType]  # value = <EventType.MOUSE_ADDED: 1028>
    MOUSE_BUTTON_DOWN: typing.ClassVar[EventType]  # value = <EventType.MOUSE_BUTTON_DOWN: 1025>
    MOUSE_BUTTON_UP: typing.ClassVar[EventType]  # value = <EventType.MOUSE_BUTTON_UP: 1026>
    MOUSE_MOTION: typing.ClassVar[EventType]  # value = <EventType.MOUSE_MOTION: 1024>
    MOUSE_REMOVED: typing.ClassVar[EventType]  # value = <EventType.MOUSE_REMOVED: 1029>
    MOUSE_WHEEL: typing.ClassVar[EventType]  # value = <EventType.MOUSE_WHEEL: 1027>
    PEN_AXIS: typing.ClassVar[EventType]  # value = <EventType.PEN_AXIS: 4871>
    PEN_BUTTON_DOWN: typing.ClassVar[EventType]  # value = <EventType.PEN_BUTTON_DOWN: 4868>
    PEN_BUTTON_UP: typing.ClassVar[EventType]  # value = <EventType.PEN_BUTTON_UP: 4869>
    PEN_DOWN: typing.ClassVar[EventType]  # value = <EventType.PEN_DOWN: 4866>
    PEN_MOTION: typing.ClassVar[EventType]  # value = <EventType.PEN_MOTION: 4870>
    PEN_PROXIMITY_IN: typing.ClassVar[EventType]  # value = <EventType.PEN_PROXIMITY_IN: 4864>
    PEN_PROXIMITY_OUT: typing.ClassVar[EventType]  # value = <EventType.PEN_PROXIMITY_OUT: 4865>
    PEN_UP: typing.ClassVar[EventType]  # value = <EventType.PEN_UP: 4867>
    QUIT: typing.ClassVar[EventType]  # value = <EventType.QUIT: 256>
    TEXT_EDITING: typing.ClassVar[EventType]  # value = <EventType.TEXT_EDITING: 770>
    TEXT_INPUT: typing.ClassVar[EventType]  # value = <EventType.TEXT_INPUT: 771>
    WINDOW_ENTER_FULLSCREEN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_ENTER_FULLSCREEN: 535>
    WINDOW_EXPOSED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_EXPOSED: 516>
    WINDOW_FOCUS_GAINED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_FOCUS_GAINED: 526>
    WINDOW_FOCUS_LOST: typing.ClassVar[EventType]  # value = <EventType.WINDOW_FOCUS_LOST: 527>
    WINDOW_HIDDEN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_HIDDEN: 515>
    WINDOW_LEAVE_FULLSCREEN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_LEAVE_FULLSCREEN: 536>
    WINDOW_MAXIMIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MAXIMIZED: 522>
    WINDOW_MINIMIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MINIMIZED: 521>
    WINDOW_MOUSE_ENTER: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MOUSE_ENTER: 524>
    WINDOW_MOUSE_LEAVE: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MOUSE_LEAVE: 525>
    WINDOW_MOVED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MOVED: 517>
    WINDOW_OCCLUDED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_OCCLUDED: 534>
    WINDOW_RESIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_RESIZED: 518>
    WINDOW_RESTORED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_RESTORED: 523>
    WINDOW_SHOWN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_SHOWN: 514>
    __members__: typing.ClassVar[dict[str, EventType]]  # value = {'QUIT': <EventType.QUIT: 256>, 'WINDOW_SHOWN': <EventType.WINDOW_SHOWN: 514>, 'WINDOW_HIDDEN': <EventType.WINDOW_HIDDEN: 515>, 'WINDOW_EXPOSED': <EventType.WINDOW_EXPOSED: 516>, 'WINDOW_MOVED': <EventType.WINDOW_MOVED: 517>, 'WINDOW_RESIZED': <EventType.WINDOW_RESIZED: 518>, 'WINDOW_MINIMIZED': <EventType.WINDOW_MINIMIZED: 521>, 'WINDOW_MAXIMIZED': <EventType.WINDOW_MAXIMIZED: 522>, 'WINDOW_RESTORED': <EventType.WINDOW_RESTORED: 523>, 'WINDOW_MOUSE_ENTER': <EventType.WINDOW_MOUSE_ENTER: 524>, 'WINDOW_MOUSE_LEAVE': <EventType.WINDOW_MOUSE_LEAVE: 525>, 'WINDOW_FOCUS_GAINED': <EventType.WINDOW_FOCUS_GAINED: 526>, 'WINDOW_FOCUS_LOST': <EventType.WINDOW_FOCUS_LOST: 527>, 'WINDOW_OCCLUDED': <EventType.WINDOW_OCCLUDED: 534>, 'WINDOW_ENTER_FULLSCREEN': <EventType.WINDOW_ENTER_FULLSCREEN: 535>, 'WINDOW_LEAVE_FULLSCREEN': <EventType.WINDOW_LEAVE_FULLSCREEN: 536>, 'KEY_DOWN': <EventType.KEY_DOWN: 768>, 'KEY_UP': <EventType.KEY_UP: 769>, 'TEXT_EDITING': <EventType.TEXT_EDITING: 770>, 'TEXT_INPUT': <EventType.TEXT_INPUT: 771>, 'KEYBOARD_ADDED': <EventType.KEYBOARD_ADDED: 773>, 'KEYBOARD_REMOVED': <EventType.KEYBOARD_REMOVED: 774>, 'MOUSE_MOTION': <EventType.MOUSE_MOTION: 1024>, 'MOUSE_BUTTON_DOWN': <EventType.MOUSE_BUTTON_DOWN: 1025>, 'MOUSE_BUTTON_UP': <EventType.MOUSE_BUTTON_UP: 1026>, 'MOUSE_WHEEL': <EventType.MOUSE_WHEEL: 1027>, 'MOUSE_ADDED': <EventType.MOUSE_ADDED: 1028>, 'MOUSE_REMOVED': <EventType.MOUSE_REMOVED: 1029>, 'GAMEPAD_AXIS_MOTION': <EventType.GAMEPAD_AXIS_MOTION: 1616>, 'GAMEPAD_BUTTON_DOWN': <EventType.GAMEPAD_BUTTON_DOWN: 1617>, 'GAMEPAD_BUTTON_UP': <EventType.GAMEPAD_BUTTON_UP: 1618>, 'GAMEPAD_ADDED': <EventType.GAMEPAD_ADDED: 1619>, 'GAMEPAD_REMOVED': <EventType.GAMEPAD_REMOVED: 1620>, 'GAMEPAD_TOUCHPAD_DOWN': <EventType.GAMEPAD_TOUCHPAD_DOWN: 1622>, 'GAMEPAD_TOUCHPAD_MOTION': <EventType.GAMEPAD_TOUCHPAD_MOTION: 1623>, 'GAMEPAD_TOUCHPAD_UP': <EventType.GAMEPAD_TOUCHPAD_UP: 1624>, 'DROP_FILE': <EventType.DROP_FILE: 4096>, 'DROP_TEXT': <EventType.DROP_TEXT: 4097>, 'DROP_BEGIN': <EventType.DROP_BEGIN: 4098>, 'DROP_COMPLETE': <EventType.DROP_COMPLETE: 4099>, 'DROP_POSITION': <EventType.DROP_POSITION: 4100>, 'AUDIO_DEVICE_ADDED': <EventType.AUDIO_DEVICE_ADDED: 4352>, 'AUDIO_DEVICE_REMOVED': <EventType.AUDIO_DEVICE_REMOVED: 4353>, 'PEN_PROXIMITY_IN': <EventType.PEN_PROXIMITY_IN: 4864>, 'PEN_PROXIMITY_OUT': <EventType.PEN_PROXIMITY_OUT: 4865>, 'PEN_DOWN': <EventType.PEN_DOWN: 4866>, 'PEN_UP': <EventType.PEN_UP: 4867>, 'PEN_BUTTON_DOWN': <EventType.PEN_BUTTON_DOWN: 4868>, 'PEN_BUTTON_UP': <EventType.PEN_BUTTON_UP: 4869>, 'PEN_MOTION': <EventType.PEN_MOTION: 4870>, 'PEN_AXIS': <EventType.PEN_AXIS: 4871>, 'CAMERA_ADDED': <EventType.CAMERA_ADDED: 5120>, 'CAMERA_REMOVED': <EventType.CAMERA_REMOVED: 5121>, 'CAMERA_APPROVED': <EventType.CAMERA_APPROVED: 5122>, 'CAMERA_DENIED': <EventType.CAMERA_DENIED: 5123>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class GamepadAxis:
    """
    Members:
    
      C_LX
    
      C_LY
    
      C_RX
    
      C_RY
    
      C_LTRIGGER
    
      C_RTRIGGER
    """
    C_LTRIGGER: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_LTRIGGER: 4>
    C_LX: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_LX: 0>
    C_LY: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_LY: 1>
    C_RTRIGGER: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_RTRIGGER: 5>
    C_RX: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_RX: 2>
    C_RY: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_RY: 3>
    __members__: typing.ClassVar[dict[str, GamepadAxis]]  # value = {'C_LX': <GamepadAxis.C_LX: 0>, 'C_LY': <GamepadAxis.C_LY: 1>, 'C_RX': <GamepadAxis.C_RX: 2>, 'C_RY': <GamepadAxis.C_RY: 3>, 'C_LTRIGGER': <GamepadAxis.C_LTRIGGER: 4>, 'C_RTRIGGER': <GamepadAxis.C_RTRIGGER: 5>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class GamepadButton:
    """
    Members:
    
      C_SOUTH
    
      C_EAST
    
      C_WEST
    
      C_NORTH
    
      C_BACK
    
      C_GUIDE
    
      C_START
    
      C_LEFTSTICK
    
      C_RIGHTSTICK
    
      C_LEFTSHOULDER
    
      C_RIGHTSHOULDER
    
      C_DPADUP
    
      C_DPADDOWN
    
      C_DPADLEFT
    
      C_DPADRIGHT
    """
    C_BACK: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_BACK: 4>
    C_DPADDOWN: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPADDOWN: 12>
    C_DPADLEFT: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPADLEFT: 13>
    C_DPADRIGHT: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPADRIGHT: 14>
    C_DPADUP: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPADUP: 11>
    C_EAST: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_EAST: 1>
    C_GUIDE: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_GUIDE: 5>
    C_LEFTSHOULDER: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_LEFTSHOULDER: 9>
    C_LEFTSTICK: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_LEFTSTICK: 7>
    C_NORTH: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_NORTH: 3>
    C_RIGHTSHOULDER: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_RIGHTSHOULDER: 10>
    C_RIGHTSTICK: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_RIGHTSTICK: 8>
    C_SOUTH: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_SOUTH: 0>
    C_START: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_START: 6>
    C_WEST: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_WEST: 2>
    __members__: typing.ClassVar[dict[str, GamepadButton]]  # value = {'C_SOUTH': <GamepadButton.C_SOUTH: 0>, 'C_EAST': <GamepadButton.C_EAST: 1>, 'C_WEST': <GamepadButton.C_WEST: 2>, 'C_NORTH': <GamepadButton.C_NORTH: 3>, 'C_BACK': <GamepadButton.C_BACK: 4>, 'C_GUIDE': <GamepadButton.C_GUIDE: 5>, 'C_START': <GamepadButton.C_START: 6>, 'C_LEFTSTICK': <GamepadButton.C_LEFTSTICK: 7>, 'C_RIGHTSTICK': <GamepadButton.C_RIGHTSTICK: 8>, 'C_LEFTSHOULDER': <GamepadButton.C_LEFTSHOULDER: 9>, 'C_RIGHTSHOULDER': <GamepadButton.C_RIGHTSHOULDER: 10>, 'C_DPADUP': <GamepadButton.C_DPADUP: 11>, 'C_DPADDOWN': <GamepadButton.C_DPADDOWN: 12>, 'C_DPADLEFT': <GamepadButton.C_DPADLEFT: 13>, 'C_DPADRIGHT': <GamepadButton.C_DPADRIGHT: 14>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class GamepadType:
    """
    Members:
    
      C_STANDARD
    
      C_XBOX360
    
      C_XBOXONE
    
      C_PS3
    
      C_PS4
    
      C_PS5
    
      C_SWITCHPRO
    
      C_SWITCHJOYCONLEFT
    
      C_SWITCHJOYCONRIGHT
    
      C_SWITCHJOYCONPAIR
    """
    C_PS3: typing.ClassVar[GamepadType]  # value = <GamepadType.C_PS3: 4>
    C_PS4: typing.ClassVar[GamepadType]  # value = <GamepadType.C_PS4: 5>
    C_PS5: typing.ClassVar[GamepadType]  # value = <GamepadType.C_PS5: 6>
    C_STANDARD: typing.ClassVar[GamepadType]  # value = <GamepadType.C_STANDARD: 1>
    C_SWITCHJOYCONLEFT: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCHJOYCONLEFT: 8>
    C_SWITCHJOYCONPAIR: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCHJOYCONPAIR: 10>
    C_SWITCHJOYCONRIGHT: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCHJOYCONRIGHT: 9>
    C_SWITCHPRO: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCHPRO: 7>
    C_XBOX360: typing.ClassVar[GamepadType]  # value = <GamepadType.C_XBOX360: 2>
    C_XBOXONE: typing.ClassVar[GamepadType]  # value = <GamepadType.C_XBOXONE: 3>
    __members__: typing.ClassVar[dict[str, GamepadType]]  # value = {'C_STANDARD': <GamepadType.C_STANDARD: 1>, 'C_XBOX360': <GamepadType.C_XBOX360: 2>, 'C_XBOXONE': <GamepadType.C_XBOXONE: 3>, 'C_PS3': <GamepadType.C_PS3: 4>, 'C_PS4': <GamepadType.C_PS4: 5>, 'C_PS5': <GamepadType.C_PS5: 6>, 'C_SWITCHPRO': <GamepadType.C_SWITCHPRO: 7>, 'C_SWITCHJOYCONLEFT': <GamepadType.C_SWITCHJOYCONLEFT: 8>, 'C_SWITCHJOYCONRIGHT': <GamepadType.C_SWITCHJOYCONRIGHT: 9>, 'C_SWITCHJOYCONPAIR': <GamepadType.C_SWITCHJOYCONPAIR: 10>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class InputAction:
    """
    
    Represents a single input trigger such as a key, mouse button, or gamepad control.
        
    """
    @typing.overload
    def __init__(self, scancode: Scancode) -> None:
        """
        Create an input action from a scancode.
        
        Args:
            scancode (Scancode): Keyboard scancode.
        """
    @typing.overload
    def __init__(self, keycode: Keycode) -> None:
        """
        Create an input action from a keycode.
        
        Args:
            keycode (Keycode): Keyboard keycode.
        """
    @typing.overload
    def __init__(self, mouse_button: MouseButton) -> None:
        """
        Create an input action from a mouse button.
        
        Args:
            mouse_button (MouseButton): Mouse button code.
        """
    @typing.overload
    def __init__(self, gamepad_button: GamepadButton, slot: int = 0) -> None:
        """
        Create an input action from a gamepad button.
        
        Args:
            gamepad_button (GamepadButton): Gamepad button code.
            slot (int, optional): Gamepad slot (default is 0).
        """
    @typing.overload
    def __init__(self, gamepad_axis: GamepadAxis, is_positive: bool, slot: int = 0) -> None:
        """
        Create an input action from a gamepad axis direction.
        
        Args:
            gamepad_axis (GamepadAxis): Gamepad axis code.
            is_positive (bool): True for positive direction, False for negative.
            slot (int, optional): Gamepad slot (default is 0).
        """
class Keycode:
    """
    Members:
    
      K_BACKSPACE
    
      K_TAB
    
      K_RETURN
    
      K_ESC
    
      K_SPACE
    
      K_EXCLAIM
    
      K_DBLQUOTE
    
      K_HASH
    
      K_DOLLAR
    
      K_PERCENT
    
      K_AMPERSAND
    
      K_SGLQUOTE
    
      K_ASTERISK
    
      K_PLUS
    
      K_COMMA
    
      K_MINUS
    
      K_PERIOD
    
      K_SLASH
    
      K_0
    
      K_1
    
      K_2
    
      K_3
    
      K_4
    
      K_5
    
      K_6
    
      K_7
    
      K_8
    
      K_9
    
      K_COLON
    
      K_SEMICOLON
    
      K_LT
    
      K_EQ
    
      K_GT
    
      K_QUESTION
    
      K_AT
    
      K_LBRACKET
    
      K_BACKSLASH
    
      K_RBRACKET
    
      K_CARET
    
      K_UNDERSCORE
    
      K_GRAVE
    
      K_a
    
      K_b
    
      K_c
    
      K_d
    
      K_e
    
      K_f
    
      K_g
    
      K_h
    
      K_i
    
      K_j
    
      K_k
    
      K_l
    
      K_m
    
      K_n
    
      K_o
    
      K_p
    
      K_q
    
      K_r
    
      K_s
    
      K_t
    
      K_u
    
      K_v
    
      K_w
    
      K_x
    
      K_y
    
      K_z
    
      K_LBRACE
    
      K_PIPE
    
      K_RBRACE
    
      K_TILDE
    
      K_DEL
    
      K_CAPS
    
      K_F1
    
      K_F2
    
      K_F3
    
      K_F4
    
      K_F5
    
      K_F6
    
      K_F7
    
      K_F8
    
      K_F9
    
      K_F10
    
      K_F11
    
      K_F12
    
      K_PRTSCR
    
      K_SCRLK
    
      K_PAUSE
    
      K_INS
    
      K_HOME
    
      K_PGUP
    
      K_END
    
      K_PGDOWN
    
      K_RIGHT
    
      K_LEFT
    
      K_DOWN
    
      K_UP
    
      K_NUMLOCK
    
      K_KP_DIV
    
      K_KP_MULT
    
      K_KP_MINUS
    
      K_KP_PLUS
    
      K_KP_ENTER
    
      K_KP_1
    
      K_KP_2
    
      K_KP_3
    
      K_KP_4
    
      K_KP_5
    
      K_KP_6
    
      K_KP_7
    
      K_KP_8
    
      K_KP_9
    
      K_KP_0
    
      K_KP_PERIOD
    
      K_AGAIN
    
      K_UNDO
    
      K_CUT
    
      K_COPY
    
      K_PASTE
    
      K_FIND
    
      K_MUTE
    
      K_VOLUP
    
      K_VOLDOWN
    
      K_LPAREN
    
      K_RPAREN
    
      K_LCTRL
    
      K_LSHIFT
    
      K_LALT
    
      K_LGUI
    
      K_RCTRL
    
      K_RSHIFT
    
      K_RALT
    
      K_RGUI
    """
    K_0: typing.ClassVar[Keycode]  # value = <Keycode.K_0: 48>
    K_1: typing.ClassVar[Keycode]  # value = <Keycode.K_1: 49>
    K_2: typing.ClassVar[Keycode]  # value = <Keycode.K_2: 50>
    K_3: typing.ClassVar[Keycode]  # value = <Keycode.K_3: 51>
    K_4: typing.ClassVar[Keycode]  # value = <Keycode.K_4: 52>
    K_5: typing.ClassVar[Keycode]  # value = <Keycode.K_5: 53>
    K_6: typing.ClassVar[Keycode]  # value = <Keycode.K_6: 54>
    K_7: typing.ClassVar[Keycode]  # value = <Keycode.K_7: 55>
    K_8: typing.ClassVar[Keycode]  # value = <Keycode.K_8: 56>
    K_9: typing.ClassVar[Keycode]  # value = <Keycode.K_9: 57>
    K_AGAIN: typing.ClassVar[Keycode]  # value = <Keycode.K_AGAIN: 1073741945>
    K_AMPERSAND: typing.ClassVar[Keycode]  # value = <Keycode.K_AMPERSAND: 38>
    K_ASTERISK: typing.ClassVar[Keycode]  # value = <Keycode.K_ASTERISK: 42>
    K_AT: typing.ClassVar[Keycode]  # value = <Keycode.K_AT: 64>
    K_BACKSLASH: typing.ClassVar[Keycode]  # value = <Keycode.K_BACKSLASH: 92>
    K_BACKSPACE: typing.ClassVar[Keycode]  # value = <Keycode.K_BACKSPACE: 8>
    K_CAPS: typing.ClassVar[Keycode]  # value = <Keycode.K_CAPS: 1073741881>
    K_CARET: typing.ClassVar[Keycode]  # value = <Keycode.K_CARET: 94>
    K_COLON: typing.ClassVar[Keycode]  # value = <Keycode.K_COLON: 58>
    K_COMMA: typing.ClassVar[Keycode]  # value = <Keycode.K_COMMA: 44>
    K_COPY: typing.ClassVar[Keycode]  # value = <Keycode.K_COPY: 1073741948>
    K_CUT: typing.ClassVar[Keycode]  # value = <Keycode.K_CUT: 1073741947>
    K_DBLQUOTE: typing.ClassVar[Keycode]  # value = <Keycode.K_DBLQUOTE: 34>
    K_DEL: typing.ClassVar[Keycode]  # value = <Keycode.K_DEL: 127>
    K_DOLLAR: typing.ClassVar[Keycode]  # value = <Keycode.K_DOLLAR: 36>
    K_DOWN: typing.ClassVar[Keycode]  # value = <Keycode.K_DOWN: 1073741905>
    K_END: typing.ClassVar[Keycode]  # value = <Keycode.K_END: 1073741901>
    K_EQ: typing.ClassVar[Keycode]  # value = <Keycode.K_EQ: 61>
    K_ESC: typing.ClassVar[Keycode]  # value = <Keycode.K_ESC: 27>
    K_EXCLAIM: typing.ClassVar[Keycode]  # value = <Keycode.K_EXCLAIM: 33>
    K_F1: typing.ClassVar[Keycode]  # value = <Keycode.K_F1: 1073741882>
    K_F10: typing.ClassVar[Keycode]  # value = <Keycode.K_F10: 1073741891>
    K_F11: typing.ClassVar[Keycode]  # value = <Keycode.K_F11: 1073741892>
    K_F12: typing.ClassVar[Keycode]  # value = <Keycode.K_F12: 1073741893>
    K_F2: typing.ClassVar[Keycode]  # value = <Keycode.K_F2: 1073741883>
    K_F3: typing.ClassVar[Keycode]  # value = <Keycode.K_F3: 1073741884>
    K_F4: typing.ClassVar[Keycode]  # value = <Keycode.K_F4: 1073741885>
    K_F5: typing.ClassVar[Keycode]  # value = <Keycode.K_F5: 1073741886>
    K_F6: typing.ClassVar[Keycode]  # value = <Keycode.K_F6: 1073741887>
    K_F7: typing.ClassVar[Keycode]  # value = <Keycode.K_F7: 1073741888>
    K_F8: typing.ClassVar[Keycode]  # value = <Keycode.K_F8: 1073741889>
    K_F9: typing.ClassVar[Keycode]  # value = <Keycode.K_F9: 1073741890>
    K_FIND: typing.ClassVar[Keycode]  # value = <Keycode.K_FIND: 1073741950>
    K_GRAVE: typing.ClassVar[Keycode]  # value = <Keycode.K_GRAVE: 96>
    K_GT: typing.ClassVar[Keycode]  # value = <Keycode.K_GT: 62>
    K_HASH: typing.ClassVar[Keycode]  # value = <Keycode.K_HASH: 35>
    K_HOME: typing.ClassVar[Keycode]  # value = <Keycode.K_HOME: 1073741898>
    K_INS: typing.ClassVar[Keycode]  # value = <Keycode.K_INS: 1073741897>
    K_KP_0: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_0: 1073741922>
    K_KP_1: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_1: 1073741913>
    K_KP_2: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_2: 1073741914>
    K_KP_3: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_3: 1073741915>
    K_KP_4: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_4: 1073741916>
    K_KP_5: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_5: 1073741917>
    K_KP_6: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_6: 1073741918>
    K_KP_7: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_7: 1073741919>
    K_KP_8: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_8: 1073741920>
    K_KP_9: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_9: 1073741921>
    K_KP_DIV: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_DIV: 1073741908>
    K_KP_ENTER: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_ENTER: 1073741912>
    K_KP_MINUS: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_MINUS: 1073741910>
    K_KP_MULT: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_MULT: 1073741909>
    K_KP_PERIOD: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_PERIOD: 1073741923>
    K_KP_PLUS: typing.ClassVar[Keycode]  # value = <Keycode.K_KP_PLUS: 1073741911>
    K_LALT: typing.ClassVar[Keycode]  # value = <Keycode.K_LALT: 1073742050>
    K_LBRACE: typing.ClassVar[Keycode]  # value = <Keycode.K_LBRACE: 123>
    K_LBRACKET: typing.ClassVar[Keycode]  # value = <Keycode.K_LBRACKET: 91>
    K_LCTRL: typing.ClassVar[Keycode]  # value = <Keycode.K_LCTRL: 1073742048>
    K_LEFT: typing.ClassVar[Keycode]  # value = <Keycode.K_LEFT: 1073741904>
    K_LGUI: typing.ClassVar[Keycode]  # value = <Keycode.K_LGUI: 1073742051>
    K_LPAREN: typing.ClassVar[Keycode]  # value = <Keycode.K_LPAREN: 40>
    K_LSHIFT: typing.ClassVar[Keycode]  # value = <Keycode.K_LSHIFT: 1073742049>
    K_LT: typing.ClassVar[Keycode]  # value = <Keycode.K_LT: 60>
    K_MINUS: typing.ClassVar[Keycode]  # value = <Keycode.K_MINUS: 45>
    K_MUTE: typing.ClassVar[Keycode]  # value = <Keycode.K_MUTE: 1073741951>
    K_NUMLOCK: typing.ClassVar[Keycode]  # value = <Keycode.K_NUMLOCK: 1073741907>
    K_PASTE: typing.ClassVar[Keycode]  # value = <Keycode.K_PASTE: 1073741949>
    K_PAUSE: typing.ClassVar[Keycode]  # value = <Keycode.K_PAUSE: 1073741896>
    K_PERCENT: typing.ClassVar[Keycode]  # value = <Keycode.K_PERCENT: 37>
    K_PERIOD: typing.ClassVar[Keycode]  # value = <Keycode.K_PERIOD: 46>
    K_PGDOWN: typing.ClassVar[Keycode]  # value = <Keycode.K_PGDOWN: 1073741902>
    K_PGUP: typing.ClassVar[Keycode]  # value = <Keycode.K_PGUP: 1073741899>
    K_PIPE: typing.ClassVar[Keycode]  # value = <Keycode.K_PIPE: 124>
    K_PLUS: typing.ClassVar[Keycode]  # value = <Keycode.K_PLUS: 43>
    K_PRTSCR: typing.ClassVar[Keycode]  # value = <Keycode.K_PRTSCR: 1073741894>
    K_QUESTION: typing.ClassVar[Keycode]  # value = <Keycode.K_QUESTION: 63>
    K_RALT: typing.ClassVar[Keycode]  # value = <Keycode.K_RALT: 1073742054>
    K_RBRACE: typing.ClassVar[Keycode]  # value = <Keycode.K_RBRACE: 125>
    K_RBRACKET: typing.ClassVar[Keycode]  # value = <Keycode.K_RBRACKET: 93>
    K_RCTRL: typing.ClassVar[Keycode]  # value = <Keycode.K_RCTRL: 1073742052>
    K_RETURN: typing.ClassVar[Keycode]  # value = <Keycode.K_RETURN: 13>
    K_RGUI: typing.ClassVar[Keycode]  # value = <Keycode.K_RGUI: 1073742055>
    K_RIGHT: typing.ClassVar[Keycode]  # value = <Keycode.K_RIGHT: 1073741903>
    K_RPAREN: typing.ClassVar[Keycode]  # value = <Keycode.K_RPAREN: 41>
    K_RSHIFT: typing.ClassVar[Keycode]  # value = <Keycode.K_RSHIFT: 1073742053>
    K_SCRLK: typing.ClassVar[Keycode]  # value = <Keycode.K_SCRLK: 1073741895>
    K_SEMICOLON: typing.ClassVar[Keycode]  # value = <Keycode.K_SEMICOLON: 59>
    K_SGLQUOTE: typing.ClassVar[Keycode]  # value = <Keycode.K_SGLQUOTE: 39>
    K_SLASH: typing.ClassVar[Keycode]  # value = <Keycode.K_SLASH: 47>
    K_SPACE: typing.ClassVar[Keycode]  # value = <Keycode.K_SPACE: 32>
    K_TAB: typing.ClassVar[Keycode]  # value = <Keycode.K_TAB: 9>
    K_TILDE: typing.ClassVar[Keycode]  # value = <Keycode.K_TILDE: 126>
    K_UNDERSCORE: typing.ClassVar[Keycode]  # value = <Keycode.K_UNDERSCORE: 95>
    K_UNDO: typing.ClassVar[Keycode]  # value = <Keycode.K_UNDO: 1073741946>
    K_UP: typing.ClassVar[Keycode]  # value = <Keycode.K_UP: 1073741906>
    K_VOLDOWN: typing.ClassVar[Keycode]  # value = <Keycode.K_VOLDOWN: 1073741953>
    K_VOLUP: typing.ClassVar[Keycode]  # value = <Keycode.K_VOLUP: 1073741952>
    K_a: typing.ClassVar[Keycode]  # value = <Keycode.K_a: 97>
    K_b: typing.ClassVar[Keycode]  # value = <Keycode.K_b: 98>
    K_c: typing.ClassVar[Keycode]  # value = <Keycode.K_c: 99>
    K_d: typing.ClassVar[Keycode]  # value = <Keycode.K_d: 100>
    K_e: typing.ClassVar[Keycode]  # value = <Keycode.K_e: 101>
    K_f: typing.ClassVar[Keycode]  # value = <Keycode.K_f: 102>
    K_g: typing.ClassVar[Keycode]  # value = <Keycode.K_g: 103>
    K_h: typing.ClassVar[Keycode]  # value = <Keycode.K_h: 104>
    K_i: typing.ClassVar[Keycode]  # value = <Keycode.K_i: 105>
    K_j: typing.ClassVar[Keycode]  # value = <Keycode.K_j: 106>
    K_k: typing.ClassVar[Keycode]  # value = <Keycode.K_k: 107>
    K_l: typing.ClassVar[Keycode]  # value = <Keycode.K_l: 108>
    K_m: typing.ClassVar[Keycode]  # value = <Keycode.K_m: 109>
    K_n: typing.ClassVar[Keycode]  # value = <Keycode.K_n: 110>
    K_o: typing.ClassVar[Keycode]  # value = <Keycode.K_o: 111>
    K_p: typing.ClassVar[Keycode]  # value = <Keycode.K_p: 112>
    K_q: typing.ClassVar[Keycode]  # value = <Keycode.K_q: 113>
    K_r: typing.ClassVar[Keycode]  # value = <Keycode.K_r: 114>
    K_s: typing.ClassVar[Keycode]  # value = <Keycode.K_s: 115>
    K_t: typing.ClassVar[Keycode]  # value = <Keycode.K_t: 116>
    K_u: typing.ClassVar[Keycode]  # value = <Keycode.K_u: 117>
    K_v: typing.ClassVar[Keycode]  # value = <Keycode.K_v: 118>
    K_w: typing.ClassVar[Keycode]  # value = <Keycode.K_w: 119>
    K_x: typing.ClassVar[Keycode]  # value = <Keycode.K_x: 120>
    K_y: typing.ClassVar[Keycode]  # value = <Keycode.K_y: 121>
    K_z: typing.ClassVar[Keycode]  # value = <Keycode.K_z: 122>
    __members__: typing.ClassVar[dict[str, Keycode]]  # value = {'K_BACKSPACE': <Keycode.K_BACKSPACE: 8>, 'K_TAB': <Keycode.K_TAB: 9>, 'K_RETURN': <Keycode.K_RETURN: 13>, 'K_ESC': <Keycode.K_ESC: 27>, 'K_SPACE': <Keycode.K_SPACE: 32>, 'K_EXCLAIM': <Keycode.K_EXCLAIM: 33>, 'K_DBLQUOTE': <Keycode.K_DBLQUOTE: 34>, 'K_HASH': <Keycode.K_HASH: 35>, 'K_DOLLAR': <Keycode.K_DOLLAR: 36>, 'K_PERCENT': <Keycode.K_PERCENT: 37>, 'K_AMPERSAND': <Keycode.K_AMPERSAND: 38>, 'K_SGLQUOTE': <Keycode.K_SGLQUOTE: 39>, 'K_ASTERISK': <Keycode.K_ASTERISK: 42>, 'K_PLUS': <Keycode.K_PLUS: 43>, 'K_COMMA': <Keycode.K_COMMA: 44>, 'K_MINUS': <Keycode.K_MINUS: 45>, 'K_PERIOD': <Keycode.K_PERIOD: 46>, 'K_SLASH': <Keycode.K_SLASH: 47>, 'K_0': <Keycode.K_0: 48>, 'K_1': <Keycode.K_1: 49>, 'K_2': <Keycode.K_2: 50>, 'K_3': <Keycode.K_3: 51>, 'K_4': <Keycode.K_4: 52>, 'K_5': <Keycode.K_5: 53>, 'K_6': <Keycode.K_6: 54>, 'K_7': <Keycode.K_7: 55>, 'K_8': <Keycode.K_8: 56>, 'K_9': <Keycode.K_9: 57>, 'K_COLON': <Keycode.K_COLON: 58>, 'K_SEMICOLON': <Keycode.K_SEMICOLON: 59>, 'K_LT': <Keycode.K_LT: 60>, 'K_EQ': <Keycode.K_EQ: 61>, 'K_GT': <Keycode.K_GT: 62>, 'K_QUESTION': <Keycode.K_QUESTION: 63>, 'K_AT': <Keycode.K_AT: 64>, 'K_LBRACKET': <Keycode.K_LBRACKET: 91>, 'K_BACKSLASH': <Keycode.K_BACKSLASH: 92>, 'K_RBRACKET': <Keycode.K_RBRACKET: 93>, 'K_CARET': <Keycode.K_CARET: 94>, 'K_UNDERSCORE': <Keycode.K_UNDERSCORE: 95>, 'K_GRAVE': <Keycode.K_GRAVE: 96>, 'K_a': <Keycode.K_a: 97>, 'K_b': <Keycode.K_b: 98>, 'K_c': <Keycode.K_c: 99>, 'K_d': <Keycode.K_d: 100>, 'K_e': <Keycode.K_e: 101>, 'K_f': <Keycode.K_f: 102>, 'K_g': <Keycode.K_g: 103>, 'K_h': <Keycode.K_h: 104>, 'K_i': <Keycode.K_i: 105>, 'K_j': <Keycode.K_j: 106>, 'K_k': <Keycode.K_k: 107>, 'K_l': <Keycode.K_l: 108>, 'K_m': <Keycode.K_m: 109>, 'K_n': <Keycode.K_n: 110>, 'K_o': <Keycode.K_o: 111>, 'K_p': <Keycode.K_p: 112>, 'K_q': <Keycode.K_q: 113>, 'K_r': <Keycode.K_r: 114>, 'K_s': <Keycode.K_s: 115>, 'K_t': <Keycode.K_t: 116>, 'K_u': <Keycode.K_u: 117>, 'K_v': <Keycode.K_v: 118>, 'K_w': <Keycode.K_w: 119>, 'K_x': <Keycode.K_x: 120>, 'K_y': <Keycode.K_y: 121>, 'K_z': <Keycode.K_z: 122>, 'K_LBRACE': <Keycode.K_LBRACE: 123>, 'K_PIPE': <Keycode.K_PIPE: 124>, 'K_RBRACE': <Keycode.K_RBRACE: 125>, 'K_TILDE': <Keycode.K_TILDE: 126>, 'K_DEL': <Keycode.K_DEL: 127>, 'K_CAPS': <Keycode.K_CAPS: 1073741881>, 'K_F1': <Keycode.K_F1: 1073741882>, 'K_F2': <Keycode.K_F2: 1073741883>, 'K_F3': <Keycode.K_F3: 1073741884>, 'K_F4': <Keycode.K_F4: 1073741885>, 'K_F5': <Keycode.K_F5: 1073741886>, 'K_F6': <Keycode.K_F6: 1073741887>, 'K_F7': <Keycode.K_F7: 1073741888>, 'K_F8': <Keycode.K_F8: 1073741889>, 'K_F9': <Keycode.K_F9: 1073741890>, 'K_F10': <Keycode.K_F10: 1073741891>, 'K_F11': <Keycode.K_F11: 1073741892>, 'K_F12': <Keycode.K_F12: 1073741893>, 'K_PRTSCR': <Keycode.K_PRTSCR: 1073741894>, 'K_SCRLK': <Keycode.K_SCRLK: 1073741895>, 'K_PAUSE': <Keycode.K_PAUSE: 1073741896>, 'K_INS': <Keycode.K_INS: 1073741897>, 'K_HOME': <Keycode.K_HOME: 1073741898>, 'K_PGUP': <Keycode.K_PGUP: 1073741899>, 'K_END': <Keycode.K_END: 1073741901>, 'K_PGDOWN': <Keycode.K_PGDOWN: 1073741902>, 'K_RIGHT': <Keycode.K_RIGHT: 1073741903>, 'K_LEFT': <Keycode.K_LEFT: 1073741904>, 'K_DOWN': <Keycode.K_DOWN: 1073741905>, 'K_UP': <Keycode.K_UP: 1073741906>, 'K_NUMLOCK': <Keycode.K_NUMLOCK: 1073741907>, 'K_KP_DIV': <Keycode.K_KP_DIV: 1073741908>, 'K_KP_MULT': <Keycode.K_KP_MULT: 1073741909>, 'K_KP_MINUS': <Keycode.K_KP_MINUS: 1073741910>, 'K_KP_PLUS': <Keycode.K_KP_PLUS: 1073741911>, 'K_KP_ENTER': <Keycode.K_KP_ENTER: 1073741912>, 'K_KP_1': <Keycode.K_KP_1: 1073741913>, 'K_KP_2': <Keycode.K_KP_2: 1073741914>, 'K_KP_3': <Keycode.K_KP_3: 1073741915>, 'K_KP_4': <Keycode.K_KP_4: 1073741916>, 'K_KP_5': <Keycode.K_KP_5: 1073741917>, 'K_KP_6': <Keycode.K_KP_6: 1073741918>, 'K_KP_7': <Keycode.K_KP_7: 1073741919>, 'K_KP_8': <Keycode.K_KP_8: 1073741920>, 'K_KP_9': <Keycode.K_KP_9: 1073741921>, 'K_KP_0': <Keycode.K_KP_0: 1073741922>, 'K_KP_PERIOD': <Keycode.K_KP_PERIOD: 1073741923>, 'K_AGAIN': <Keycode.K_AGAIN: 1073741945>, 'K_UNDO': <Keycode.K_UNDO: 1073741946>, 'K_CUT': <Keycode.K_CUT: 1073741947>, 'K_COPY': <Keycode.K_COPY: 1073741948>, 'K_PASTE': <Keycode.K_PASTE: 1073741949>, 'K_FIND': <Keycode.K_FIND: 1073741950>, 'K_MUTE': <Keycode.K_MUTE: 1073741951>, 'K_VOLUP': <Keycode.K_VOLUP: 1073741952>, 'K_VOLDOWN': <Keycode.K_VOLDOWN: 1073741953>, 'K_LPAREN': <Keycode.K_LPAREN: 40>, 'K_RPAREN': <Keycode.K_RPAREN: 41>, 'K_LCTRL': <Keycode.K_LCTRL: 1073742048>, 'K_LSHIFT': <Keycode.K_LSHIFT: 1073742049>, 'K_LALT': <Keycode.K_LALT: 1073742050>, 'K_LGUI': <Keycode.K_LGUI: 1073742051>, 'K_RCTRL': <Keycode.K_RCTRL: 1073742052>, 'K_RSHIFT': <Keycode.K_RSHIFT: 1073742053>, 'K_RALT': <Keycode.K_RALT: 1073742054>, 'K_RGUI': <Keycode.K_RGUI: 1073742055>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Line:
    """
    
    A 2D line segment defined by two points: A and B.
    You can access or modify points using `.a`, `.b`, or directly via `.ax`, `.ay`, `.bx`, `.by`.
        
    """
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, other: Line) -> bool:
        """
        Check if two lines are equal.
        
        Args:
            other (Line): The other line to compare.
        
        Returns:
            bool: True if all components are equal.
        """
    def __getitem__(self, arg0: int) -> float:
        """
        Get coordinate by index:
            0 = ax, 1 = ay, 2 = bx, 3 = by
        
        Raises:
            IndexError: If index is not 0-3.
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Create a default line with all values set to 0.
        """
    @typing.overload
    def __init__(self, ax: float, ay: float, bx: float, by: float) -> None:
        """
        Create a line from two coordinate points.
        
        Args:
            ax (float): X-coordinate of point A.
            ay (float): Y-coordinate of point A.
            bx (float): X-coordinate of point B.
            by (float): Y-coordinate of point B.
        """
    @typing.overload
    def __init__(self, ax: float, ay: float, b: Vec2) -> None:
        """
        Create a line from A coordinates and a Vec2 B point.
        
        Args:
            ax (float): X-coordinate of point A.
            ay (float): Y-coordinate of point A.
            b (Vec2): Point B.
        """
    @typing.overload
    def __init__(self, a: Vec2, bx: float, by: float) -> None:
        """
        Create a line from a Vec2 A point and B coordinates.
        
        Args:
            a (Vec2): Point A.
            bx (float): X-coordinate of point B.
            by (float): Y-coordinate of point B.
        """
    @typing.overload
    def __init__(self, a: Vec2, b: Vec2) -> None:
        """
        Create a line from two Vec2 points.
        
        Args:
            a (Vec2): Point A.
            b (Vec2): Point B.
        """
    @typing.overload
    def __init__(self, arg0: typing.Sequence) -> None:
        """
        Create a line from two 2-element sequences: [[ax, ay], [bx, by]].
        
        Raises:
            ValueError: If either point is not a 2-element sequence.
        """
    def __iter__(self) -> typing.Iterator:
        ...
    def __len__(self) -> int:
        """
        Return the number of components (always 4).
        
        Returns:
            int: Always returns 4 (ax, ay, bx, by).
        """
    def __ne__(self, other: Line) -> bool:
        """
        Check if two lines are not equal.
        
        Args:
            other (Line): The other line to compare.
        
        Returns:
            bool: True if any component differs.
        """
    def copy(self) -> Line:
        """
        Return a copy of this line.
        """
    def move(self, offset: Vec2) -> None:
        """
        Move this line by a Vec2 or 2-element sequence.
        
        Args:
            offset (Vec2 | list[float]): The amount to move.
        """
    @property
    def a(self) -> tuple:
        """
        Get or set point A as a tuple or Vec2.
        """
    @a.setter
    def a(self, arg1: Vec2) -> None:
        ...
    @property
    def ax(self) -> float:
        """
        X-coordinate of point A.
        """
    @ax.setter
    def ax(self, arg0: float) -> None:
        ...
    @property
    def ay(self) -> float:
        """
        Y-coordinate of point A.
        """
    @ay.setter
    def ay(self, arg0: float) -> None:
        ...
    @property
    def b(self) -> tuple:
        """
        Get or set point B as a tuple or Vec2.
        """
    @b.setter
    def b(self, arg1: Vec2) -> None:
        ...
    @property
    def bx(self) -> float:
        """
        X-coordinate of point B.
        """
    @bx.setter
    def bx(self, arg0: float) -> None:
        ...
    @property
    def by(self) -> float:
        """
        Y-coordinate of point B.
        """
    @by.setter
    def by(self, arg0: float) -> None:
        ...
    @property
    def length(self) -> float:
        """
        The Euclidean length of the line segment.
        """
class MouseButton:
    """
    Members:
    
      M_LEFT
    
      M_MIDDLE
    
      M_RIGHT
    
      M_SIDE1
    
      M_SIDE2
    """
    M_LEFT: typing.ClassVar[MouseButton]  # value = <MouseButton.M_LEFT: 1>
    M_MIDDLE: typing.ClassVar[MouseButton]  # value = <MouseButton.M_MIDDLE: 2>
    M_RIGHT: typing.ClassVar[MouseButton]  # value = <MouseButton.M_RIGHT: 3>
    M_SIDE1: typing.ClassVar[MouseButton]  # value = <MouseButton.M_SIDE1: 4>
    M_SIDE2: typing.ClassVar[MouseButton]  # value = <MouseButton.M_SIDE2: 5>
    __members__: typing.ClassVar[dict[str, MouseButton]]  # value = {'M_LEFT': <MouseButton.M_LEFT: 1>, 'M_MIDDLE': <MouseButton.M_MIDDLE: 2>, 'M_RIGHT': <MouseButton.M_RIGHT: 3>, 'M_SIDE1': <MouseButton.M_SIDE1: 4>, 'M_SIDE2': <MouseButton.M_SIDE2: 5>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class PolarCoordinate:
    """
    
    Represents a polar coordinate with angle and radius components.
    
    A polar coordinate system uses an angle (in radians) and radius to define a position
    relative to a fixed origin point.
        
    """
    def __eq__(self, arg0: PolarCoordinate) -> bool:
        """
        Check if two PolarCoordinates are equal.
        
        Args:
            other (PolarCoordinate): The other PolarCoordinate to compare.
        
        Returns:
            bool: True if both angle and radius are equal.
        """
    def __getitem__(self, index: int) -> float:
        """
        Access polar coordinate components by index.
        
        Args:
            index (int): Index (0=angle, 1=radius).
        
        Returns:
            float: The component value.
        
        Raises:
            IndexError: If index is not 0 or 1.
        """
    def __hash__(self) -> int:
        """
        Return a hash value for the PolarCoordinate.
        
        Returns:
            int: Hash value based on angle and radius.
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Create a PolarCoordinate with default values (0.0, 0.0).
        """
    @typing.overload
    def __init__(self, angle: float, radius: float) -> None:
        """
        Create a PolarCoordinate from angle and radius.
        
        Args:
            angle (float): The angle in radians.
            radius (float): The radius/distance from origin.
        """
    @typing.overload
    def __init__(self, arg0: typing.Sequence) -> None:
        """
        Create a PolarCoordinate from a sequence of two elements.
        
        Args:
            sequence: A sequence (list, tuple) containing [angle, radius].
        
        Raises:
            RuntimeError: If sequence doesn't contain exactly 2 elements.
        """
    def __iter__(self) -> typing.Iterator:
        """
        Return an iterator over (angle, radius).
        
        Returns:
            iterator: Iterator that yields angle first, then radius.
        """
    def __len__(self) -> int:
        """
        Return the number of components (always 2).
        
        Returns:
            int: Always returns 2 (angle and radius).
        """
    def __ne__(self, arg0: PolarCoordinate) -> bool:
        """
        Check if two PolarCoordinates are not equal.
        
        Args:
            other (PolarCoordinate): The other PolarCoordinate to compare.
        
        Returns:
            bool: True if angle or radius are different.
        """
    def __repr__(self) -> str:
        """
        Return a string suitable for debugging and recreation.
        
        Returns:
            str: String in format "PolarCoordinate(angle, radius)".
        """
    def __setitem__(self, index: int, value: float) -> None:
        """
        Set polar coordinate components by index.
        
        Args:
            index (int): Index (0=angle, 1=radius).
            value (float): The new value to set.
        
        Raises:
            IndexError: If index is not 0 or 1.
        """
    def __str__(self) -> str:
        """
        Return a human-readable string representation.
        
        Returns:
            str: String in format "(angle, radius)".
        """
    def to_cartesian(self) -> Vec2:
        """
        Convert polar coordinates to Cartesian coordinates.
        
        Returns:
            Vec2: The equivalent Cartesian coordinates as a Vec2.
        """
    @property
    def angle(self) -> float:
        """
        The angle component in radians.
        """
    @angle.setter
    def angle(self, arg0: float) -> None:
        ...
    @property
    def radius(self) -> float:
        """
        The radius component (distance from origin).
        """
    @radius.setter
    def radius(self, arg0: float) -> None:
        ...
class Rect:
    """
    
    Represents a rectangle with position and size.
    
    A Rect is defined by its top-left corner position (x, y) and dimensions (w, h).
    Supports various geometric operations, collision detection, and positioning methods.
        
    """
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        """
        Check if the rectangle has positive area.
        
        Returns:
            bool: True if both width and height are greater than 0.
        """
    def __eq__(self, other: Rect) -> bool:
        """
        Check if two rectangles are equal.
        
        Args:
            other (Rect): The other rectangle to compare.
        
        Returns:
            bool: True if all components (x, y, w, h) are equal.
        """
    def __getitem__(self, index: int) -> float:
        """
        Access rectangle components by index.
        
        Args:
            index (int): Index (0=x, 1=y, 2=w, 3=h).
        
        Returns:
            float: The component value.
        
        Raises:
            IndexError: If index is not 0, 1, 2, or 3.
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Create a Rect with default values (0, 0, 0, 0).
        """
    @typing.overload
    def __init__(self, x: float, y: float, w: float, h: float) -> None:
        """
        Create a Rect with specified position and dimensions.
        
        Args:
            x (float): The x coordinate of the top-left corner.
            y (float): The y coordinate of the top-left corner.
            w (float): The width of the rectangle.
            h (float): The height of the rectangle.
        """
    @typing.overload
    def __init__(self, x: float, y: float, size: Vec2) -> None:
        """
        Create a Rect with specified position and size vector.
        
        Args:
            x (float): The x coordinate of the top-left corner.
            y (float): The y coordinate of the top-left corner.
            size (Vec2): The size as a Vec2 (width, height).
        """
    @typing.overload
    def __init__(self, pos: Vec2, w: float, h: float) -> None:
        """
        Create a Rect with specified position vector and dimensions.
        
        Args:
            pos (Vec2): The position as a Vec2 (x, y).
            w (float): The width of the rectangle.
            h (float): The height of the rectangle.
        """
    @typing.overload
    def __init__(self, pos: Vec2, size: Vec2) -> None:
        """
        Create a Rect with specified position and size vectors.
        
        Args:
            pos (Vec2): The position as a Vec2 (x, y).
            size (Vec2): The size as a Vec2 (width, height).
        """
    @typing.overload
    def __init__(self, arg0: typing.Sequence) -> None:
        """
        Create a Rect from a sequence of four elements.
        
        Args:
            sequence: A sequence (list, tuple) containing [x, y, w, h].
        
        Raises:
            RuntimeError: If sequence doesn't contain exactly 4 elements.
        """
    def __iter__(self) -> typing.Iterator:
        """
        Return an iterator over (x, y, w, h).
        
        Returns:
            iterator: Iterator that yields x, y, w, h in order.
        """
    def __len__(self) -> int:
        """
        Return the number of components (always 4).
        
        Returns:
            int: Always returns 4 (x, y, w, h).
        """
    def __ne__(self, other: Rect) -> bool:
        """
        Check if two rectangles are not equal.
        
        Args:
            other (Rect): The other rectangle to compare.
        
        Returns:
            bool: True if any component differs.
        """
    def __repr__(self) -> str:
        """
        Return a string suitable for debugging and recreation.
        
        Returns:
            str: String in format "Rect(x=..., y=..., w=..., h=...)".
        """
    def __str__(self) -> str:
        """
        Return a human-readable string representation.
        
        Returns:
            str: String in format "[x, y, w, h]".
        """
    @typing.overload
    def clamp(self, other: Rect) -> None:
        """
        Clamp this rectangle to be within another rectangle.
        
        Args:
            other (Rect): The rectangle to clamp within.
        
        Raises:
            ValueError: If this rectangle is larger than the clamp area.
        """
    @typing.overload
    def clamp(self, min: Vec2, max: Vec2) -> None:
        """
        Clamp this rectangle to be within the specified bounds.
        
        Args:
            min (Vec2): The minimum bounds as (min_x, min_y).
            max (Vec2): The maximum bounds as (max_x, max_y).
        
        Raises:
            ValueError: If min >= max or rectangle is larger than the clamp area.
        """
    def collide_point(self, point: Vec2) -> bool:
        """
        Check if a point is inside this rectangle.
        
        Args:
            point (Vec2): The point to check.
        
        Returns:
            bool: True if the point is inside this rectangle.
        """
    def collide_rect(self, other: Rect) -> bool:
        """
        Check if this rectangle collides with another rectangle.
        
        Args:
            other (Rect): The rectangle to check collision with.
        
        Returns:
            bool: True if the rectangles overlap.
        """
    def contains(self, other: Rect) -> bool:
        """
        Check if this rectangle completely contains another rectangle.
        
        Args:
            other (Rect): The rectangle to check.
        
        Returns:
            bool: True if this rectangle completely contains the other.
        """
    def copy(self) -> Rect:
        """
        Create a copy of this rectangle.
        
        Returns:
            Rect: A new Rect with the same position and size.
        """
    def fit(self, other: Rect) -> None:
        """
        Scale this rectangle to fit inside another rectangle while maintaining aspect ratio.
        
        Args:
            other (Rect): The rectangle to fit inside.
        
        Raises:
            ValueError: If other rectangle has non-positive dimensions.
        """
    def inflate(self, offset: Vec2) -> None:
        """
        Inflate the rectangle by the given offset.
        
        The rectangle grows in all directions. The position is adjusted to keep the center
        in the same place.
        
        Args:
            offset (Vec2): The amount to inflate by as (dw, dh).
        """
    def move(self, offset: Vec2) -> None:
        """
        Move the rectangle by the given offset.
        
        Args:
            offset (Vec2): The offset to move by as (dx, dy).
        """
    @typing.overload
    def scale_by(self, factor: float) -> None:
        """
        Scale the rectangle by a uniform factor.
        
        Args:
            factor (float): The scaling factor (must be > 0).
        
        Raises:
            ValueError: If factor is <= 0.
        """
    @typing.overload
    def scale_by(self, factor: Vec2) -> None:
        """
        Scale the rectangle by different factors for width and height.
        
        Args:
            factor (Vec2): The scaling factors as (scale_x, scale_y).
        
        Raises:
            ValueError: If any factor is <= 0.
        """
    def scale_to(self, size: Vec2) -> None:
        """
        Scale the rectangle to the specified size.
        
        Args:
            size (Vec2): The new size as (width, height).
        
        Raises:
            ValueError: If width or height is <= 0.
        """
    @property
    def bottom(self) -> float:
        """
        The y coordinate of the bottom edge.
        """
    @bottom.setter
    def bottom(self, arg1: float) -> None:
        ...
    @property
    def bottom_left(self) -> tuple:
        """
        The position of the bottom-left corner as (x, y).
        """
    @bottom_left.setter
    def bottom_left(self, arg1: Vec2) -> None:
        ...
    @property
    def bottom_mid(self) -> tuple:
        """
        The position of the bottom-middle point as (x, y).
        """
    @bottom_mid.setter
    def bottom_mid(self, arg1: Vec2) -> None:
        ...
    @property
    def bottom_right(self) -> tuple:
        """
        The position of the bottom-right corner as (x, y).
        """
    @bottom_right.setter
    def bottom_right(self, arg1: Vec2) -> None:
        ...
    @property
    def center(self) -> tuple:
        """
        The position of the center point as (x, y).
        """
    @center.setter
    def center(self, arg1: Vec2) -> None:
        ...
    @property
    def h(self) -> float:
        """
        The height of the rectangle.
        """
    @h.setter
    def h(self, arg0: float) -> None:
        ...
    @property
    def left(self) -> float:
        """
        The x coordinate of the left edge.
        """
    @left.setter
    def left(self, arg1: float) -> None:
        ...
    @property
    def mid_left(self) -> tuple:
        """
        The position of the middle-left point as (x, y).
        """
    @mid_left.setter
    def mid_left(self, arg1: Vec2) -> None:
        ...
    @property
    def mid_right(self) -> tuple:
        """
        The position of the middle-right point as (x, y).
        """
    @mid_right.setter
    def mid_right(self, arg1: Vec2) -> None:
        ...
    @property
    def right(self) -> float:
        """
        The x coordinate of the right edge.
        """
    @right.setter
    def right(self, arg1: float) -> None:
        ...
    @property
    def size(self) -> tuple:
        """
        The size of the rectangle as (width, height).
        """
    @size.setter
    def size(self, arg1: Vec2) -> None:
        ...
    @property
    def top(self) -> float:
        """
        The y coordinate of the top edge.
        """
    @top.setter
    def top(self, arg1: float) -> None:
        ...
    @property
    def top_left(self) -> tuple:
        """
        The position of the top-left corner as (x, y).
        """
    @top_left.setter
    def top_left(self, arg1: Vec2) -> None:
        ...
    @property
    def top_mid(self) -> tuple:
        """
        The position of the top-middle point as (x, y).
        """
    @top_mid.setter
    def top_mid(self, arg1: Vec2) -> None:
        ...
    @property
    def top_right(self) -> tuple:
        """
        The position of the top-right corner as (x, y).
        """
    @top_right.setter
    def top_right(self, arg1: Vec2) -> None:
        ...
    @property
    def w(self) -> float:
        """
        The width of the rectangle.
        """
    @w.setter
    def w(self, arg0: float) -> None:
        ...
    @property
    def x(self) -> float:
        """
        The x coordinate of the top-left corner.
        """
    @x.setter
    def x(self, arg0: float) -> None:
        ...
    @property
    def y(self) -> float:
        """
        The y coordinate of the top-left corner.
        """
    @y.setter
    def y(self, arg0: float) -> None:
        ...
class Renderer:
    """
    
    A 2D graphics renderer for drawing shapes, textures, and other visual elements.
    
    The Renderer manages the rendering pipeline, handles camera transformations,
    and provides methods for drawing various primitives and textures to the screen.
        
    """
    def __init__(self, resolution: Vec2) -> None:
        """
        Create a Renderer with the specified resolution.
        
        Args:
            resolution (Vec2): The rendering resolution as (width, height).
        
        Raises:
            ValueError: If resolution width or height is <= 0.
            RuntimeError: If renderer creation fails.
        """
    def clear(self, color: Color = ...) -> None:
        """
        Clear the renderer with the specified color.
        
        Args:
            color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).
        
        Raises:
            ValueError: If color values are not between 0 and 255.
        """
    @typing.overload
    def draw(self, point: Vec2, color: Color) -> None:
        """
        Draw a single point to the renderer.
        
        Args:
            point (Vec2): The position of the point.
            color (Color): The color of the point.
        
        Raises:
            RuntimeError: If point rendering fails.
        """
    @typing.overload
    def draw(self, texture: Texture, dst_rect: Rect, src_rect: Rect = ...) -> None:
        """
        Draw a texture with specified destination and source rectangles.
        
        Args:
            texture (Texture): The texture to draw.
            dst_rect (Rect): The destination rectangle on the renderer.
            src_rect (Rect, optional): The source rectangle from the texture. 
                                      Defaults to entire texture if not specified.
        
        Raises:
            RuntimeError: If texture doesn't belong to this renderer.
        """
    @typing.overload
    def draw(self, texture: Texture, pos: Vec2 = ..., anchor: Anchor = Anchor.CENTER) -> None:
        """
        Draw a texture at the specified position with anchor alignment.
        
        Args:
            texture (Texture): The texture to draw.
            pos (Vec2, optional): The position to draw at. Defaults to (0, 0).
            anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
        
        Raises:
            RuntimeError: If texture doesn't belong to this renderer.
        """
    @typing.overload
    def draw(self, circle: Circle, color: Color, thickness: int = 0) -> None:
        """
        Draw a circle to the renderer.
        
        Args:
            circle (Circle): The circle to draw.
            color (Color): The color of the circle.
            thickness (int, optional): The line thickness. If 0 or >= radius, draws filled circle.
                                      Defaults to 0 (filled).
        """
    @typing.overload
    def draw(self, line: Line, color: Color, thickness: int = 1) -> None:
        """
        Draw a line to the renderer.
        
        Args:
            line (Line): The line to draw.
            color (Color): The color of the line.
            thickness (int, optional): The line thickness in pixels. Defaults to 1.
        """
    @typing.overload
    def draw(self, rect: Rect, color: Color, thickness: int = 0) -> None:
        """
        Draw a rectangle to the renderer.
        
        Args:
            rect (Rect): The rectangle to draw.
            color (Color): The color of the rectangle.
            thickness (int, optional): The line thickness. If 0 or >= half width/height, 
                                      draws filled rectangle. Defaults to 0 (filled).
        """
    def get_resolution(self) -> Vec2:
        """
        Get the resolution of the renderer.
        
        Returns:
            Vec2: The current rendering resolution as (width, height).
        """
    def present(self) -> None:
        """
        Present the rendered content to the screen.
        
        This finalizes the current frame and displays it. Should be called after
        all drawing operations for the frame are complete.
        """
    def to_viewport(self, coordinate: Vec2) -> Vec2:
        """
        Convert window coordinates to viewport coordinates.
        
        Args:
            coordinate (Vec2): The window coordinate to convert.
        
        Returns:
            Vec2: The equivalent viewport coordinate.
        """
class Scancode:
    """
    Members:
    
      S_a
    
      S_b
    
      S_c
    
      S_d
    
      S_e
    
      S_f
    
      S_g
    
      S_h
    
      S_i
    
      S_j
    
      S_k
    
      S_l
    
      S_m
    
      S_n
    
      S_o
    
      S_p
    
      S_q
    
      S_r
    
      S_s
    
      S_t
    
      S_u
    
      S_v
    
      S_w
    
      S_x
    
      S_y
    
      S_z
    
      S_1
    
      S_2
    
      S_3
    
      S_4
    
      S_5
    
      S_6
    
      S_7
    
      S_8
    
      S_9
    
      S_0
    
      S_RETURN
    
      S_ESC
    
      S_BACKSPACE
    
      S_TAB
    
      S_SPACE
    
      S_MINUS
    
      S_EQ
    
      S_LBRACKET
    
      S_RBRACKET
    
      S_BACKSLASH
    
      S_SEMICOLON
    
      S_APOSTROPHE
    
      S_GRAVE
    
      S_COMMA
    
      S_PERIOD
    
      S_SLASH
    
      S_CAPS
    
      S_F1
    
      S_F2
    
      S_F3
    
      S_F4
    
      S_F5
    
      S_F6
    
      S_F7
    
      S_F8
    
      S_F9
    
      S_F10
    
      S_F11
    
      S_F12
    
      S_PRTSCR
    
      S_SCRLK
    
      S_PAUSE
    
      S_INS
    
      S_HOME
    
      S_PGUP
    
      S_DEL
    
      S_END
    
      S_PGDOWN
    
      S_RIGHT
    
      S_LEFT
    
      S_DOWN
    
      S_UP
    
      S_NUMLOCK
    
      S_KP_DIV
    
      S_KP_MULT
    
      S_KP_MINUS
    
      S_KP_PLUS
    
      S_KP_ENTER
    
      S_KP_1
    
      S_KP_2
    
      S_KP_3
    
      S_KP_4
    
      S_KP_5
    
      S_KP_6
    
      S_KP_7
    
      S_KP_8
    
      S_KP_9
    
      S_KP_0
    
      S_KP_PERIOD
    
      S_AGAIN
    
      S_UNDO
    
      S_CUT
    
      S_COPY
    
      S_PASTE
    
      S_FIND
    
      S_MUTE
    
      S_VOLUP
    
      S_VOLDOWN
    
      S_LCTRL
    
      S_LSHIFT
    
      S_LALT
    
      S_LGUI
    
      S_RCTRL
    
      S_RSHIFT
    
      S_RALT
    
      S_RGUI
    """
    S_0: typing.ClassVar[Scancode]  # value = <Scancode.S_0: 39>
    S_1: typing.ClassVar[Scancode]  # value = <Scancode.S_1: 30>
    S_2: typing.ClassVar[Scancode]  # value = <Scancode.S_2: 31>
    S_3: typing.ClassVar[Scancode]  # value = <Scancode.S_3: 32>
    S_4: typing.ClassVar[Scancode]  # value = <Scancode.S_4: 33>
    S_5: typing.ClassVar[Scancode]  # value = <Scancode.S_5: 34>
    S_6: typing.ClassVar[Scancode]  # value = <Scancode.S_6: 35>
    S_7: typing.ClassVar[Scancode]  # value = <Scancode.S_7: 36>
    S_8: typing.ClassVar[Scancode]  # value = <Scancode.S_8: 37>
    S_9: typing.ClassVar[Scancode]  # value = <Scancode.S_9: 38>
    S_AGAIN: typing.ClassVar[Scancode]  # value = <Scancode.S_AGAIN: 121>
    S_APOSTROPHE: typing.ClassVar[Scancode]  # value = <Scancode.S_APOSTROPHE: 52>
    S_BACKSLASH: typing.ClassVar[Scancode]  # value = <Scancode.S_BACKSLASH: 49>
    S_BACKSPACE: typing.ClassVar[Scancode]  # value = <Scancode.S_BACKSPACE: 42>
    S_CAPS: typing.ClassVar[Scancode]  # value = <Scancode.S_CAPS: 57>
    S_COMMA: typing.ClassVar[Scancode]  # value = <Scancode.S_COMMA: 54>
    S_COPY: typing.ClassVar[Scancode]  # value = <Scancode.S_COPY: 124>
    S_CUT: typing.ClassVar[Scancode]  # value = <Scancode.S_CUT: 123>
    S_DEL: typing.ClassVar[Scancode]  # value = <Scancode.S_DEL: 76>
    S_DOWN: typing.ClassVar[Scancode]  # value = <Scancode.S_DOWN: 81>
    S_END: typing.ClassVar[Scancode]  # value = <Scancode.S_END: 77>
    S_EQ: typing.ClassVar[Scancode]  # value = <Scancode.S_EQ: 46>
    S_ESC: typing.ClassVar[Scancode]  # value = <Scancode.S_ESC: 41>
    S_F1: typing.ClassVar[Scancode]  # value = <Scancode.S_F1: 58>
    S_F10: typing.ClassVar[Scancode]  # value = <Scancode.S_F10: 67>
    S_F11: typing.ClassVar[Scancode]  # value = <Scancode.S_F11: 68>
    S_F12: typing.ClassVar[Scancode]  # value = <Scancode.S_F12: 69>
    S_F2: typing.ClassVar[Scancode]  # value = <Scancode.S_F2: 59>
    S_F3: typing.ClassVar[Scancode]  # value = <Scancode.S_F3: 60>
    S_F4: typing.ClassVar[Scancode]  # value = <Scancode.S_F4: 61>
    S_F5: typing.ClassVar[Scancode]  # value = <Scancode.S_F5: 62>
    S_F6: typing.ClassVar[Scancode]  # value = <Scancode.S_F6: 63>
    S_F7: typing.ClassVar[Scancode]  # value = <Scancode.S_F7: 64>
    S_F8: typing.ClassVar[Scancode]  # value = <Scancode.S_F8: 65>
    S_F9: typing.ClassVar[Scancode]  # value = <Scancode.S_F9: 66>
    S_FIND: typing.ClassVar[Scancode]  # value = <Scancode.S_FIND: 126>
    S_GRAVE: typing.ClassVar[Scancode]  # value = <Scancode.S_GRAVE: 53>
    S_HOME: typing.ClassVar[Scancode]  # value = <Scancode.S_HOME: 74>
    S_INS: typing.ClassVar[Scancode]  # value = <Scancode.S_INS: 73>
    S_KP_0: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_0: 98>
    S_KP_1: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_1: 89>
    S_KP_2: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_2: 90>
    S_KP_3: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_3: 91>
    S_KP_4: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_4: 92>
    S_KP_5: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_5: 93>
    S_KP_6: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_6: 94>
    S_KP_7: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_7: 95>
    S_KP_8: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_8: 96>
    S_KP_9: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_9: 97>
    S_KP_DIV: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_DIV: 84>
    S_KP_ENTER: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_ENTER: 88>
    S_KP_MINUS: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_MINUS: 86>
    S_KP_MULT: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_MULT: 85>
    S_KP_PERIOD: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_PERIOD: 99>
    S_KP_PLUS: typing.ClassVar[Scancode]  # value = <Scancode.S_KP_PLUS: 87>
    S_LALT: typing.ClassVar[Scancode]  # value = <Scancode.S_LALT: 226>
    S_LBRACKET: typing.ClassVar[Scancode]  # value = <Scancode.S_LBRACKET: 47>
    S_LCTRL: typing.ClassVar[Scancode]  # value = <Scancode.S_LCTRL: 224>
    S_LEFT: typing.ClassVar[Scancode]  # value = <Scancode.S_LEFT: 80>
    S_LGUI: typing.ClassVar[Scancode]  # value = <Scancode.S_LGUI: 227>
    S_LSHIFT: typing.ClassVar[Scancode]  # value = <Scancode.S_LSHIFT: 225>
    S_MINUS: typing.ClassVar[Scancode]  # value = <Scancode.S_MINUS: 45>
    S_MUTE: typing.ClassVar[Scancode]  # value = <Scancode.S_MUTE: 127>
    S_NUMLOCK: typing.ClassVar[Scancode]  # value = <Scancode.S_NUMLOCK: 83>
    S_PASTE: typing.ClassVar[Scancode]  # value = <Scancode.S_PASTE: 125>
    S_PAUSE: typing.ClassVar[Scancode]  # value = <Scancode.S_PAUSE: 72>
    S_PERIOD: typing.ClassVar[Scancode]  # value = <Scancode.S_PERIOD: 55>
    S_PGDOWN: typing.ClassVar[Scancode]  # value = <Scancode.S_PGDOWN: 78>
    S_PGUP: typing.ClassVar[Scancode]  # value = <Scancode.S_PGUP: 75>
    S_PRTSCR: typing.ClassVar[Scancode]  # value = <Scancode.S_PRTSCR: 70>
    S_RALT: typing.ClassVar[Scancode]  # value = <Scancode.S_RALT: 230>
    S_RBRACKET: typing.ClassVar[Scancode]  # value = <Scancode.S_RBRACKET: 48>
    S_RCTRL: typing.ClassVar[Scancode]  # value = <Scancode.S_RCTRL: 228>
    S_RETURN: typing.ClassVar[Scancode]  # value = <Scancode.S_RETURN: 40>
    S_RGUI: typing.ClassVar[Scancode]  # value = <Scancode.S_RGUI: 231>
    S_RIGHT: typing.ClassVar[Scancode]  # value = <Scancode.S_RIGHT: 79>
    S_RSHIFT: typing.ClassVar[Scancode]  # value = <Scancode.S_RSHIFT: 229>
    S_SCRLK: typing.ClassVar[Scancode]  # value = <Scancode.S_SCRLK: 71>
    S_SEMICOLON: typing.ClassVar[Scancode]  # value = <Scancode.S_SEMICOLON: 51>
    S_SLASH: typing.ClassVar[Scancode]  # value = <Scancode.S_SLASH: 56>
    S_SPACE: typing.ClassVar[Scancode]  # value = <Scancode.S_SPACE: 44>
    S_TAB: typing.ClassVar[Scancode]  # value = <Scancode.S_TAB: 43>
    S_UNDO: typing.ClassVar[Scancode]  # value = <Scancode.S_UNDO: 122>
    S_UP: typing.ClassVar[Scancode]  # value = <Scancode.S_UP: 82>
    S_VOLDOWN: typing.ClassVar[Scancode]  # value = <Scancode.S_VOLDOWN: 129>
    S_VOLUP: typing.ClassVar[Scancode]  # value = <Scancode.S_VOLUP: 128>
    S_a: typing.ClassVar[Scancode]  # value = <Scancode.S_a: 4>
    S_b: typing.ClassVar[Scancode]  # value = <Scancode.S_b: 5>
    S_c: typing.ClassVar[Scancode]  # value = <Scancode.S_c: 6>
    S_d: typing.ClassVar[Scancode]  # value = <Scancode.S_d: 7>
    S_e: typing.ClassVar[Scancode]  # value = <Scancode.S_e: 8>
    S_f: typing.ClassVar[Scancode]  # value = <Scancode.S_f: 9>
    S_g: typing.ClassVar[Scancode]  # value = <Scancode.S_g: 10>
    S_h: typing.ClassVar[Scancode]  # value = <Scancode.S_h: 11>
    S_i: typing.ClassVar[Scancode]  # value = <Scancode.S_i: 12>
    S_j: typing.ClassVar[Scancode]  # value = <Scancode.S_j: 13>
    S_k: typing.ClassVar[Scancode]  # value = <Scancode.S_k: 14>
    S_l: typing.ClassVar[Scancode]  # value = <Scancode.S_l: 15>
    S_m: typing.ClassVar[Scancode]  # value = <Scancode.S_m: 16>
    S_n: typing.ClassVar[Scancode]  # value = <Scancode.S_n: 17>
    S_o: typing.ClassVar[Scancode]  # value = <Scancode.S_o: 18>
    S_p: typing.ClassVar[Scancode]  # value = <Scancode.S_p: 19>
    S_q: typing.ClassVar[Scancode]  # value = <Scancode.S_q: 20>
    S_r: typing.ClassVar[Scancode]  # value = <Scancode.S_r: 21>
    S_s: typing.ClassVar[Scancode]  # value = <Scancode.S_s: 22>
    S_t: typing.ClassVar[Scancode]  # value = <Scancode.S_t: 23>
    S_u: typing.ClassVar[Scancode]  # value = <Scancode.S_u: 24>
    S_v: typing.ClassVar[Scancode]  # value = <Scancode.S_v: 25>
    S_w: typing.ClassVar[Scancode]  # value = <Scancode.S_w: 26>
    S_x: typing.ClassVar[Scancode]  # value = <Scancode.S_x: 27>
    S_y: typing.ClassVar[Scancode]  # value = <Scancode.S_y: 28>
    S_z: typing.ClassVar[Scancode]  # value = <Scancode.S_z: 29>
    __members__: typing.ClassVar[dict[str, Scancode]]  # value = {'S_a': <Scancode.S_a: 4>, 'S_b': <Scancode.S_b: 5>, 'S_c': <Scancode.S_c: 6>, 'S_d': <Scancode.S_d: 7>, 'S_e': <Scancode.S_e: 8>, 'S_f': <Scancode.S_f: 9>, 'S_g': <Scancode.S_g: 10>, 'S_h': <Scancode.S_h: 11>, 'S_i': <Scancode.S_i: 12>, 'S_j': <Scancode.S_j: 13>, 'S_k': <Scancode.S_k: 14>, 'S_l': <Scancode.S_l: 15>, 'S_m': <Scancode.S_m: 16>, 'S_n': <Scancode.S_n: 17>, 'S_o': <Scancode.S_o: 18>, 'S_p': <Scancode.S_p: 19>, 'S_q': <Scancode.S_q: 20>, 'S_r': <Scancode.S_r: 21>, 'S_s': <Scancode.S_s: 22>, 'S_t': <Scancode.S_t: 23>, 'S_u': <Scancode.S_u: 24>, 'S_v': <Scancode.S_v: 25>, 'S_w': <Scancode.S_w: 26>, 'S_x': <Scancode.S_x: 27>, 'S_y': <Scancode.S_y: 28>, 'S_z': <Scancode.S_z: 29>, 'S_1': <Scancode.S_1: 30>, 'S_2': <Scancode.S_2: 31>, 'S_3': <Scancode.S_3: 32>, 'S_4': <Scancode.S_4: 33>, 'S_5': <Scancode.S_5: 34>, 'S_6': <Scancode.S_6: 35>, 'S_7': <Scancode.S_7: 36>, 'S_8': <Scancode.S_8: 37>, 'S_9': <Scancode.S_9: 38>, 'S_0': <Scancode.S_0: 39>, 'S_RETURN': <Scancode.S_RETURN: 40>, 'S_ESC': <Scancode.S_ESC: 41>, 'S_BACKSPACE': <Scancode.S_BACKSPACE: 42>, 'S_TAB': <Scancode.S_TAB: 43>, 'S_SPACE': <Scancode.S_SPACE: 44>, 'S_MINUS': <Scancode.S_MINUS: 45>, 'S_EQ': <Scancode.S_EQ: 46>, 'S_LBRACKET': <Scancode.S_LBRACKET: 47>, 'S_RBRACKET': <Scancode.S_RBRACKET: 48>, 'S_BACKSLASH': <Scancode.S_BACKSLASH: 49>, 'S_SEMICOLON': <Scancode.S_SEMICOLON: 51>, 'S_APOSTROPHE': <Scancode.S_APOSTROPHE: 52>, 'S_GRAVE': <Scancode.S_GRAVE: 53>, 'S_COMMA': <Scancode.S_COMMA: 54>, 'S_PERIOD': <Scancode.S_PERIOD: 55>, 'S_SLASH': <Scancode.S_SLASH: 56>, 'S_CAPS': <Scancode.S_CAPS: 57>, 'S_F1': <Scancode.S_F1: 58>, 'S_F2': <Scancode.S_F2: 59>, 'S_F3': <Scancode.S_F3: 60>, 'S_F4': <Scancode.S_F4: 61>, 'S_F5': <Scancode.S_F5: 62>, 'S_F6': <Scancode.S_F6: 63>, 'S_F7': <Scancode.S_F7: 64>, 'S_F8': <Scancode.S_F8: 65>, 'S_F9': <Scancode.S_F9: 66>, 'S_F10': <Scancode.S_F10: 67>, 'S_F11': <Scancode.S_F11: 68>, 'S_F12': <Scancode.S_F12: 69>, 'S_PRTSCR': <Scancode.S_PRTSCR: 70>, 'S_SCRLK': <Scancode.S_SCRLK: 71>, 'S_PAUSE': <Scancode.S_PAUSE: 72>, 'S_INS': <Scancode.S_INS: 73>, 'S_HOME': <Scancode.S_HOME: 74>, 'S_PGUP': <Scancode.S_PGUP: 75>, 'S_DEL': <Scancode.S_DEL: 76>, 'S_END': <Scancode.S_END: 77>, 'S_PGDOWN': <Scancode.S_PGDOWN: 78>, 'S_RIGHT': <Scancode.S_RIGHT: 79>, 'S_LEFT': <Scancode.S_LEFT: 80>, 'S_DOWN': <Scancode.S_DOWN: 81>, 'S_UP': <Scancode.S_UP: 82>, 'S_NUMLOCK': <Scancode.S_NUMLOCK: 83>, 'S_KP_DIV': <Scancode.S_KP_DIV: 84>, 'S_KP_MULT': <Scancode.S_KP_MULT: 85>, 'S_KP_MINUS': <Scancode.S_KP_MINUS: 86>, 'S_KP_PLUS': <Scancode.S_KP_PLUS: 87>, 'S_KP_ENTER': <Scancode.S_KP_ENTER: 88>, 'S_KP_1': <Scancode.S_KP_1: 89>, 'S_KP_2': <Scancode.S_KP_2: 90>, 'S_KP_3': <Scancode.S_KP_3: 91>, 'S_KP_4': <Scancode.S_KP_4: 92>, 'S_KP_5': <Scancode.S_KP_5: 93>, 'S_KP_6': <Scancode.S_KP_6: 94>, 'S_KP_7': <Scancode.S_KP_7: 95>, 'S_KP_8': <Scancode.S_KP_8: 96>, 'S_KP_9': <Scancode.S_KP_9: 97>, 'S_KP_0': <Scancode.S_KP_0: 98>, 'S_KP_PERIOD': <Scancode.S_KP_PERIOD: 99>, 'S_AGAIN': <Scancode.S_AGAIN: 121>, 'S_UNDO': <Scancode.S_UNDO: 122>, 'S_CUT': <Scancode.S_CUT: 123>, 'S_COPY': <Scancode.S_COPY: 124>, 'S_PASTE': <Scancode.S_PASTE: 125>, 'S_FIND': <Scancode.S_FIND: 126>, 'S_MUTE': <Scancode.S_MUTE: 127>, 'S_VOLUP': <Scancode.S_VOLUP: 128>, 'S_VOLDOWN': <Scancode.S_VOLDOWN: 129>, 'S_LCTRL': <Scancode.S_LCTRL: 224>, 'S_LSHIFT': <Scancode.S_LSHIFT: 225>, 'S_LALT': <Scancode.S_LALT: 226>, 'S_LGUI': <Scancode.S_LGUI: 227>, 'S_RCTRL': <Scancode.S_RCTRL: 228>, 'S_RSHIFT': <Scancode.S_RSHIFT: 229>, 'S_RALT': <Scancode.S_RALT: 230>, 'S_RGUI': <Scancode.S_RGUI: 231>}
    def __and__(self, other: typing.Any) -> typing.Any:
        ...
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __ge__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __gt__(self, other: typing.Any) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: int) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __invert__(self) -> typing.Any:
        ...
    def __le__(self, other: typing.Any) -> bool:
        ...
    def __lt__(self, other: typing.Any) -> bool:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __or__(self, other: typing.Any) -> typing.Any:
        ...
    def __rand__(self, other: typing.Any) -> typing.Any:
        ...
    def __repr__(self) -> str:
        ...
    def __ror__(self, other: typing.Any) -> typing.Any:
        ...
    def __rxor__(self, other: typing.Any) -> typing.Any:
        ...
    def __setstate__(self, state: int) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __xor__(self, other: typing.Any) -> typing.Any:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Surface:
    """
    
    Represents a 2D pixel buffer for image manipulation and blitting operations.
    
    A Surface is a collection of pixels that can be manipulated, drawn on, and used as a source
    for texture creation or blitting to other surfaces. Supports pixel-level operations,
    color key transparency, and alpha blending.
        
    """
    @typing.overload
    def __init__(self, size: Vec2) -> None:
        """
        Create a new Surface with the specified dimensions.
        
        Args:
            size (Vec2): The size of the surface as (width, height).
        
        Raises:
            RuntimeError: If surface creation fails.
        """
    @typing.overload
    def __init__(self, file_path: str) -> None:
        """
        Create a Surface by loading an image from a file.
        
        Args:
            file_path (str): Path to the image file to load.
        
        Raises:
            RuntimeError: If the file cannot be loaded or doesn't exist.
        """
    @typing.overload
    def blit(self, source: Surface, pos: Vec2, anchor: Anchor = Anchor.CENTER, src_rect: Rect = ...) -> None:
        """
        Blit (copy) another surface onto this surface at the specified position with anchor alignment.
        
        Args:
            source (Surface): The source surface to blit from.
            pos (Vec2): The position to blit to.
            anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
            src_rect (Rect, optional): The source rectangle to blit from. Defaults to entire source surface.
        
        Raises:
            RuntimeError: If the blit operation fails.
        """
    @typing.overload
    def blit(self, source: Surface, dst_rect: Rect, src_rect: Rect = ...) -> None:
        """
        Blit (copy) another surface onto this surface with specified destination and source rectangles.
        
        Args:
            source (Surface): The source surface to blit from.
            dst_rect (Rect): The destination rectangle on this surface.
            src_rect (Rect, optional): The source rectangle to blit from. Defaults to entire source surface.
        
        Raises:
            RuntimeError: If the blit operation fails.
        """
    def copy(self) -> Surface:
        """
        Create a copy of this surface.
        
        Returns:
            Surface: A new Surface that is an exact copy of this one.
        
        Raises:
            RuntimeError: If surface copying fails.
        """
    def fill(self, color: Color) -> None:
        """
        Fill the entire surface with a solid color.
        
        Args:
            color (Color): The color to fill the surface with.
        """
    def get_at(self, coord: Vec2) -> Color:
        """
        Get the color of a pixel at the specified coordinates.
        
        Args:
            coord (Vec2): The coordinates of the pixel as (x, y).
        
        Returns:
            Color: The color of the pixel at the specified coordinates.
        
        Raises:
            IndexError: If coordinates are outside the surface bounds.
        """
    def set_at(self, coord: Vec2, color: Color) -> None:
        """
        Set the color of a pixel at the specified coordinates.
        
        Args:
            coord (Vec2): The coordinates of the pixel as (x, y).
            color (Color): The color to set the pixel to.
        
        Raises:
            IndexError: If coordinates are outside the surface bounds.
        """
    @property
    def alpha_mod(self) -> int:
        """
        The alpha modulation value for the surface.
        
        Controls the overall transparency of the surface. Values range from 0 (fully transparent)
        to 255 (fully opaque).
        
        Returns:
            int: The current alpha modulation value [0-255].
        
        Raises:
            RuntimeError: If getting the alpha value fails.
        """
    @alpha_mod.setter
    def alpha_mod(self, arg1: int) -> None:
        ...
    @property
    def color_key(self) -> Color:
        """
        The color key for transparency.
        
        When set, pixels of this color will be treated as transparent during blitting operations.
        Used for simple transparency effects.
        
        Returns:
            Color: The current color key.
        
        Raises:
            RuntimeError: If getting the color key fails.
        """
    @color_key.setter
    def color_key(self, arg1: Color) -> None:
        ...
    @property
    def height(self) -> int:
        """
        The height of the surface in pixels.
        
        Returns:
            int: The surface height.
        """
    @property
    def rect(self) -> Rect:
        """
        A rectangle representing the surface bounds.
        
        Returns:
            Rect: A rectangle with position (0, 0) and the surface's dimensions.
        """
    @property
    def size(self) -> Vec2:
        """
        The size of the surface as a Vec2.
        
        Returns:
            Vec2: The surface size as (width, height).
        """
    @property
    def width(self) -> int:
        """
        The width of the surface in pixels.
        
        Returns:
            int: The surface width.
        """
class Texture:
    """
    
    Represents a hardware-accelerated image that can be efficiently rendered.
    
    Textures are optimized for fast rendering operations and support various effects
    like rotation, flipping, tinting, alpha blending, and different blend modes.
    They are created from image files or surfaces and must be associated with a renderer.
        
    """
    class Flip:
        """
        
        Controls horizontal and vertical flipping of a texture during rendering.
        
        Used to mirror textures along the horizontal and/or vertical axes without
        creating additional texture data.
            
        """
        @property
        def h(self) -> bool:
            """
            Enable or disable horizontal flipping.
            
            When True, the texture is mirrored horizontally (left-right flip).
            """
        @h.setter
        def h(self, arg0: bool) -> None:
            ...
        @property
        def v(self) -> bool:
            """
            Enable or disable vertical flipping.
            
            When True, the texture is mirrored vertically (top-bottom flip).
            """
        @v.setter
        def v(self, arg0: bool) -> None:
            ...
    @typing.overload
    def __init__(self, renderer: Renderer, file_path: str) -> None:
        """
        Create a Texture by loading an image from a file.
        
        Args:
            renderer (Renderer): The renderer that will own this texture.
            file_path (str): Path to the image file to load.
        
        Raises:
            ValueError: If file_path is empty.
            RuntimeError: If the file cannot be loaded or texture creation fails.
        """
    @typing.overload
    def __init__(self, renderer: Renderer, surface: Surface) -> None:
        """
        Create a Texture from an existing Surface.
        
        Args:
            renderer (Renderer): The renderer that will own this texture.
            surface (Surface): The surface to convert to a texture.
        
        Raises:
            RuntimeError: If texture creation from surface fails.
        """
    def get_alpha(self) -> float:
        """
        Get the current alpha modulation value.
        
        Returns:
            float: The current alpha value.
        """
    def get_rect(self) -> Rect:
        """
        Get a rectangle representing the texture bounds.
        
        Returns:
            Rect: A rectangle with position (0, 0) and the texture's dimensions.
        """
    def get_size(self) -> tuple:
        """
        Get the size of the texture.
        
        Returns:
            tuple[float, float]: The texture size as (width, height).
        """
    def get_tint(self) -> Color:
        """
        Get the current color tint applied to the texture.
        
        Returns:
            Color: The current tint color.
        """
    def make_additive(self) -> None:
        """
        Set the texture to use additive blending mode.
        
        In additive mode, the texture's colors are added to the destination,
        creating bright, glowing effects.
        """
    def make_multiply(self) -> None:
        """
        Set the texture to use multiply blending mode.
        
        In multiply mode, the texture's colors are multiplied with the destination,
        creating darkening and shadow effects.
        """
    def make_normal(self) -> None:
        """
        Set the texture to use normal (alpha) blending mode.
        
        This is the default blending mode for standard transparency effects.
        """
    def set_alpha(self, alpha: float) -> None:
        """
        Set the alpha (transparency) modulation for the texture.
        
        Args:
            alpha (float): The alpha value, typically in range [0.0, 1.0] where
                          0.0 is fully transparent and 1.0 is fully opaque.
        """
    def set_tint(self, color: Color) -> None:
        """
        Set the color tint applied to the texture during rendering.
        
        The tint color is multiplied with the texture's pixels, allowing for
        color effects and lighting.
        
        Args:
            color (Color): The tint color to apply.
        """
    @property
    def angle(self) -> float:
        """
        The rotation angle in degrees for rendering.
        
        When the texture is drawn, it will be rotated by this angle around its center.
        """
    @angle.setter
    def angle(self, arg0: float) -> None:
        ...
    @property
    def flip(self) -> Texture.Flip:
        """
        The flip settings for horizontal and vertical mirroring.
        
        Controls whether the texture is flipped horizontally and/or vertically during rendering.
        """
    @flip.setter
    def flip(self, arg0: Texture.Flip) -> None:
        ...
class Vec2:
    """
    
    Represents a 2D vector with x and y components.
    
    Vec2 is used for positions, directions, velocities, and other 2D vector operations.
    Supports arithmetic operations, comparisons, and various mathematical functions.
        
    """
    def __add__(self, other: typing.Any) -> Vec2:
        """
        Add another Vec2 or sequence to this Vec2.
        
        Args:
            other (Vec2 or sequence): The Vec2 or sequence [x, y] to add.
        
        Returns:
            Vec2: A new Vec2 with the result of the addition.
        
        Raises:
            TypeError: If other is not a Vec2 or 2-element sequence.
        """
    def __bool__(self) -> bool:
        """
        Check if the vector is not zero.
        
        Returns:
            bool: True if the vector is not zero, False if it is zero.
        """
    def __eq__(self, other: Vec2) -> bool:
        """
        Check if two Vec2s are equal (within tolerance).
        
        Args:
            other (Vec2): The other Vec2 to compare.
        
        Returns:
            bool: True if vectors are equal within tolerance.
        """
    def __ge__(self, other: Vec2) -> bool:
        """
        Check if this Vec2 is component-wise greater than or equal to another.
        
        Args:
            other (Vec2): The other Vec2 to compare.
        
        Returns:
            bool: True if not component-wise less than other.
        """
    def __getitem__(self, index: int) -> float:
        """
        Access vector components by index.
        
        Args:
            index (int): Index (0=x, 1=y).
        
        Returns:
            float: The component value.
        
        Raises:
            IndexError: If index is not 0 or 1.
        """
    def __gt__(self, other: Vec2) -> bool:
        """
        Check if this Vec2 is component-wise greater than another.
        
        Args:
            other (Vec2): The other Vec2 to compare.
        
        Returns:
            bool: True if both x and y are greater than other's x and y.
        """
    def __hash__(self) -> int:
        """
        Return a hash value for the Vec2.
        
        Returns:
            int: Hash value based on x and y components.
        """
    def __iadd__(self, other: typing.Any) -> Vec2:
        """
        In-place addition (self += other).
        
        Args:
            other (Vec2 or sequence): The Vec2 or sequence [x, y] to add.
        
        Returns:
            Vec2: Reference to self after modification.
        
        Raises:
            TypeError: If other is not a Vec2 or 2-element sequence.
        """
    def __imul__(self, scalar: float) -> Vec2:
        """
        In-place multiplication by a scalar value (self *= scalar).
        
        Args:
            scalar (float): The scalar to multiply by.
        
        Returns:
            Vec2: Reference to self after modification.
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Create a zero vector (0, 0).
        """
    @typing.overload
    def __init__(self, value: float) -> None:
        """
        Create a Vec2 with both x and y set to the same value.
        
        Args:
            value (float): Value to set for both x and y components.
        """
    @typing.overload
    def __init__(self, x: float, y: float) -> None:
        """
        Create a Vec2 with given x and y values.
        
        Args:
            x (float): The x component.
            y (float): The y component.
        """
    @typing.overload
    def __init__(self, arg0: typing.Sequence) -> None:
        """
        Create a Vec2 from a sequence of two elements.
        
        Args:
            sequence: A sequence (list, tuple) containing [x, y].
        
        Raises:
            RuntimeError: If sequence doesn't contain exactly 2 elements.
        """
    def __isub__(self, other: typing.Any) -> Vec2:
        """
        In-place subtraction (self -= other).
        
        Args:
            other (Vec2 or sequence): The Vec2 or sequence [x, y] to subtract.
        
        Returns:
            Vec2: Reference to self after modification.
        
        Raises:
            TypeError: If other is not a Vec2 or 2-element sequence.
        """
    def __iter__(self) -> typing.Iterator:
        """
        Return an iterator over (x, y).
        
        Returns:
            iterator: Iterator that yields x first, then y.
        """
    def __itruediv__(self, scalar: float) -> Vec2:
        """
        In-place division by a scalar value (self /= scalar).
        
        Args:
            scalar (float): The scalar to divide by.
        
        Returns:
            Vec2: Reference to self after modification.
        """
    def __le__(self, other: Vec2) -> bool:
        """
        Check if this Vec2 is component-wise less than or equal to another.
        
        Args:
            other (Vec2): The other Vec2 to compare.
        
        Returns:
            bool: True if not component-wise greater than other.
        """
    def __len__(self) -> int:
        """
        Return the number of components (always 2).
        
        Returns:
            int: Always returns 2 (x and y).
        """
    def __lt__(self, other: Vec2) -> bool:
        """
        Check if this Vec2 is component-wise less than another.
        
        Args:
            other (Vec2): The other Vec2 to compare.
        
        Returns:
            bool: True if both x and y are less than other's x and y.
        """
    def __mul__(self, scalar: float) -> Vec2:
        """
        Multiply the vector by a scalar value.
        
        Args:
            scalar (float): The scalar to multiply by.
        
        Returns:
            Vec2: A new Vec2 with multiplied components.
        """
    def __ne__(self, other: Vec2) -> bool:
        """
        Check if two Vec2s are not equal.
        
        Args:
            other (Vec2): The other Vec2 to compare.
        
        Returns:
            bool: True if vectors are not equal.
        """
    def __neg__(self) -> Vec2:
        """
        Return the negation of this vector (-self).
        
        Returns:
            Vec2: A new Vec2 with negated x and y components.
        """
    def __radd__(self, other: typing.Any) -> Vec2:
        """
        Right-hand addition (other + self).
        
        Args:
            other (Vec2 or sequence): The Vec2 or sequence [x, y] to add.
        
        Returns:
            Vec2: A new Vec2 with the result of the addition.
        
        Raises:
            TypeError: If other is not a Vec2 or 2-element sequence.
        """
    def __repr__(self) -> str:
        """
        Return a string suitable for debugging and recreation.
        
        Returns:
            str: String in format "Vec2(x, y)".
        """
    def __rmul__(self: float, scalar: Vec2) -> Vec2:
        """
        Right-hand multiplication (scalar * self).
        
        Args:
            scalar (float): The scalar to multiply by.
        
        Returns:
            Vec2: A new Vec2 with multiplied components.
        """
    def __rsub__(self, other: typing.Any) -> Vec2:
        """
        Right-hand subtraction (other - self).
        
        Args:
            other (Vec2 or sequence): The Vec2 or sequence [x, y] to subtract from.
        
        Returns:
            Vec2: A new Vec2 with the result of the subtraction.
        
        Raises:
            TypeError: If other is not a Vec2 or 2-element sequence.
        """
    def __setitem__(self, index: int, value: float) -> None:
        """
        Set vector components by index.
        
        Args:
            index (int): Index (0=x, 1=y).
            value (float): The new value to set.
        
        Raises:
            IndexError: If index is not 0 or 1.
        """
    def __str__(self) -> str:
        """
        Return a human-readable string representation.
        
        Returns:
            str: String in format "<x, y>".
        """
    def __sub__(self, other: typing.Any) -> Vec2:
        """
        Subtract another Vec2 or sequence from this Vec2.
        
        Args:
            other (Vec2 or sequence): The Vec2 or sequence [x, y] to subtract.
        
        Returns:
            Vec2: A new Vec2 with the result of the subtraction.
        
        Raises:
            TypeError: If other is not a Vec2 or 2-element sequence.
        """
    def __truediv__(self, scalar: float) -> Vec2:
        """
        Divide the vector by a scalar value.
        
        Args:
            scalar (float): The scalar to divide by.
        
        Returns:
            Vec2: A new Vec2 with divided components.
        """
    def distance_to(self, other: Vec2) -> float:
        """
        Calculate the distance to another vector.
        
        Args:
            other (Vec2): The other vector.
        
        Returns:
            float: The Euclidean distance between the vectors.
        """
    def normalize(self) -> None:
        """
        Normalize the vector to unit length in-place.
        
        If the vector is zero, it remains unchanged.
        """
    def rotate(self, radians: float) -> None:
        """
        Rotate the vector by the given angle in radians.
        
        Args:
            radians (float): The angle to rotate by in radians.
        """
    def scale_to_length(self, length: float) -> None:
        """
        Scale the vector to the specified length in-place.
        
        Args:
            length (float): The target length.
        """
    def to_polar(self) -> PolarCoordinate:
        """
        Convert to polar coordinates.
        
        Returns:
            PolarCoordinate: A polar coordinate representation (angle, length).
        """
    @property
    def angle(self) -> float:
        """
        Get the angle of the vector in radians.
        
        Returns:
            float: The angle from the positive x-axis to this vector.
        """
    @property
    def length(self) -> float:
        """
        Get the length (magnitude) of the vector.
        
        Returns:
            float: The Euclidean length of the vector.
        """
    @property
    def x(self) -> float:
        """
        The x component of the vector.
        """
    @x.setter
    def x(self, arg0: float) -> None:
        ...
    @property
    def y(self) -> float:
        """
        The y component of the vector.
        """
    @y.setter
    def y(self, arg0: float) -> None:
        ...
def init() -> None:
    """
    Initialize the Kraken Engine.
    
    This sets up internal systems and must be called before using any other features.
    """
def quit() -> None:
    """
    Shut down the Kraken Engine and clean up resources.
    
    Call this once you're done using the engine to avoid memory leaks.
    """
AUDIO_DEVICE_ADDED: EventType  # value = <EventType.AUDIO_DEVICE_ADDED: 4352>
AUDIO_DEVICE_REMOVED: EventType  # value = <EventType.AUDIO_DEVICE_REMOVED: 4353>
BOTTOM_LEFT: Anchor  # value = <Anchor.BOTTOM_LEFT: 6>
BOTTOM_MID: Anchor  # value = <Anchor.BOTTOM_MID: 7>
BOTTOM_RIGHT: Anchor  # value = <Anchor.BOTTOM_RIGHT: 8>
CAMERA_ADDED: EventType  # value = <EventType.CAMERA_ADDED: 5120>
CAMERA_APPROVED: EventType  # value = <EventType.CAMERA_APPROVED: 5122>
CAMERA_DENIED: EventType  # value = <EventType.CAMERA_DENIED: 5123>
CAMERA_REMOVED: EventType  # value = <EventType.CAMERA_REMOVED: 5121>
CENTER: Anchor  # value = <Anchor.CENTER: 4>
C_BACK: GamepadButton  # value = <GamepadButton.C_BACK: 4>
C_DPADDOWN: GamepadButton  # value = <GamepadButton.C_DPADDOWN: 12>
C_DPADLEFT: GamepadButton  # value = <GamepadButton.C_DPADLEFT: 13>
C_DPADRIGHT: GamepadButton  # value = <GamepadButton.C_DPADRIGHT: 14>
C_DPADUP: GamepadButton  # value = <GamepadButton.C_DPADUP: 11>
C_EAST: GamepadButton  # value = <GamepadButton.C_EAST: 1>
C_GUIDE: GamepadButton  # value = <GamepadButton.C_GUIDE: 5>
C_LEFTSHOULDER: GamepadButton  # value = <GamepadButton.C_LEFTSHOULDER: 9>
C_LEFTSTICK: GamepadButton  # value = <GamepadButton.C_LEFTSTICK: 7>
C_LTRIGGER: GamepadAxis  # value = <GamepadAxis.C_LTRIGGER: 4>
C_LX: GamepadAxis  # value = <GamepadAxis.C_LX: 0>
C_LY: GamepadAxis  # value = <GamepadAxis.C_LY: 1>
C_NORTH: GamepadButton  # value = <GamepadButton.C_NORTH: 3>
C_PS3: GamepadType  # value = <GamepadType.C_PS3: 4>
C_PS4: GamepadType  # value = <GamepadType.C_PS4: 5>
C_PS5: GamepadType  # value = <GamepadType.C_PS5: 6>
C_RIGHTSHOULDER: GamepadButton  # value = <GamepadButton.C_RIGHTSHOULDER: 10>
C_RIGHTSTICK: GamepadButton  # value = <GamepadButton.C_RIGHTSTICK: 8>
C_RTRIGGER: GamepadAxis  # value = <GamepadAxis.C_RTRIGGER: 5>
C_RX: GamepadAxis  # value = <GamepadAxis.C_RX: 2>
C_RY: GamepadAxis  # value = <GamepadAxis.C_RY: 3>
C_SOUTH: GamepadButton  # value = <GamepadButton.C_SOUTH: 0>
C_STANDARD: GamepadType  # value = <GamepadType.C_STANDARD: 1>
C_START: GamepadButton  # value = <GamepadButton.C_START: 6>
C_SWITCHJOYCONLEFT: GamepadType  # value = <GamepadType.C_SWITCHJOYCONLEFT: 8>
C_SWITCHJOYCONPAIR: GamepadType  # value = <GamepadType.C_SWITCHJOYCONPAIR: 10>
C_SWITCHJOYCONRIGHT: GamepadType  # value = <GamepadType.C_SWITCHJOYCONRIGHT: 9>
C_SWITCHPRO: GamepadType  # value = <GamepadType.C_SWITCHPRO: 7>
C_WEST: GamepadButton  # value = <GamepadButton.C_WEST: 2>
C_XBOX360: GamepadType  # value = <GamepadType.C_XBOX360: 2>
C_XBOXONE: GamepadType  # value = <GamepadType.C_XBOXONE: 3>
DROP_BEGIN: EventType  # value = <EventType.DROP_BEGIN: 4098>
DROP_COMPLETE: EventType  # value = <EventType.DROP_COMPLETE: 4099>
DROP_FILE: EventType  # value = <EventType.DROP_FILE: 4096>
DROP_POSITION: EventType  # value = <EventType.DROP_POSITION: 4100>
DROP_TEXT: EventType  # value = <EventType.DROP_TEXT: 4097>
GAMEPAD_ADDED: EventType  # value = <EventType.GAMEPAD_ADDED: 1619>
GAMEPAD_AXIS_MOTION: EventType  # value = <EventType.GAMEPAD_AXIS_MOTION: 1616>
GAMEPAD_BUTTON_DOWN: EventType  # value = <EventType.GAMEPAD_BUTTON_DOWN: 1617>
GAMEPAD_BUTTON_UP: EventType  # value = <EventType.GAMEPAD_BUTTON_UP: 1618>
GAMEPAD_REMOVED: EventType  # value = <EventType.GAMEPAD_REMOVED: 1620>
GAMEPAD_TOUCHPAD_DOWN: EventType  # value = <EventType.GAMEPAD_TOUCHPAD_DOWN: 1622>
GAMEPAD_TOUCHPAD_MOTION: EventType  # value = <EventType.GAMEPAD_TOUCHPAD_MOTION: 1623>
GAMEPAD_TOUCHPAD_UP: EventType  # value = <EventType.GAMEPAD_TOUCHPAD_UP: 1624>
KEYBOARD_ADDED: EventType  # value = <EventType.KEYBOARD_ADDED: 773>
KEYBOARD_REMOVED: EventType  # value = <EventType.KEYBOARD_REMOVED: 774>
KEY_DOWN: EventType  # value = <EventType.KEY_DOWN: 768>
KEY_UP: EventType  # value = <EventType.KEY_UP: 769>
K_0: Keycode  # value = <Keycode.K_0: 48>
K_1: Keycode  # value = <Keycode.K_1: 49>
K_2: Keycode  # value = <Keycode.K_2: 50>
K_3: Keycode  # value = <Keycode.K_3: 51>
K_4: Keycode  # value = <Keycode.K_4: 52>
K_5: Keycode  # value = <Keycode.K_5: 53>
K_6: Keycode  # value = <Keycode.K_6: 54>
K_7: Keycode  # value = <Keycode.K_7: 55>
K_8: Keycode  # value = <Keycode.K_8: 56>
K_9: Keycode  # value = <Keycode.K_9: 57>
K_AGAIN: Keycode  # value = <Keycode.K_AGAIN: 1073741945>
K_AMPERSAND: Keycode  # value = <Keycode.K_AMPERSAND: 38>
K_ASTERISK: Keycode  # value = <Keycode.K_ASTERISK: 42>
K_AT: Keycode  # value = <Keycode.K_AT: 64>
K_BACKSLASH: Keycode  # value = <Keycode.K_BACKSLASH: 92>
K_BACKSPACE: Keycode  # value = <Keycode.K_BACKSPACE: 8>
K_CAPS: Keycode  # value = <Keycode.K_CAPS: 1073741881>
K_CARET: Keycode  # value = <Keycode.K_CARET: 94>
K_COLON: Keycode  # value = <Keycode.K_COLON: 58>
K_COMMA: Keycode  # value = <Keycode.K_COMMA: 44>
K_COPY: Keycode  # value = <Keycode.K_COPY: 1073741948>
K_CUT: Keycode  # value = <Keycode.K_CUT: 1073741947>
K_DBLQUOTE: Keycode  # value = <Keycode.K_DBLQUOTE: 34>
K_DEL: Keycode  # value = <Keycode.K_DEL: 127>
K_DOLLAR: Keycode  # value = <Keycode.K_DOLLAR: 36>
K_DOWN: Keycode  # value = <Keycode.K_DOWN: 1073741905>
K_END: Keycode  # value = <Keycode.K_END: 1073741901>
K_EQ: Keycode  # value = <Keycode.K_EQ: 61>
K_ESC: Keycode  # value = <Keycode.K_ESC: 27>
K_EXCLAIM: Keycode  # value = <Keycode.K_EXCLAIM: 33>
K_F1: Keycode  # value = <Keycode.K_F1: 1073741882>
K_F10: Keycode  # value = <Keycode.K_F10: 1073741891>
K_F11: Keycode  # value = <Keycode.K_F11: 1073741892>
K_F12: Keycode  # value = <Keycode.K_F12: 1073741893>
K_F2: Keycode  # value = <Keycode.K_F2: 1073741883>
K_F3: Keycode  # value = <Keycode.K_F3: 1073741884>
K_F4: Keycode  # value = <Keycode.K_F4: 1073741885>
K_F5: Keycode  # value = <Keycode.K_F5: 1073741886>
K_F6: Keycode  # value = <Keycode.K_F6: 1073741887>
K_F7: Keycode  # value = <Keycode.K_F7: 1073741888>
K_F8: Keycode  # value = <Keycode.K_F8: 1073741889>
K_F9: Keycode  # value = <Keycode.K_F9: 1073741890>
K_FIND: Keycode  # value = <Keycode.K_FIND: 1073741950>
K_GRAVE: Keycode  # value = <Keycode.K_GRAVE: 96>
K_GT: Keycode  # value = <Keycode.K_GT: 62>
K_HASH: Keycode  # value = <Keycode.K_HASH: 35>
K_HOME: Keycode  # value = <Keycode.K_HOME: 1073741898>
K_INS: Keycode  # value = <Keycode.K_INS: 1073741897>
K_KP_0: Keycode  # value = <Keycode.K_KP_0: 1073741922>
K_KP_1: Keycode  # value = <Keycode.K_KP_1: 1073741913>
K_KP_2: Keycode  # value = <Keycode.K_KP_2: 1073741914>
K_KP_3: Keycode  # value = <Keycode.K_KP_3: 1073741915>
K_KP_4: Keycode  # value = <Keycode.K_KP_4: 1073741916>
K_KP_5: Keycode  # value = <Keycode.K_KP_5: 1073741917>
K_KP_6: Keycode  # value = <Keycode.K_KP_6: 1073741918>
K_KP_7: Keycode  # value = <Keycode.K_KP_7: 1073741919>
K_KP_8: Keycode  # value = <Keycode.K_KP_8: 1073741920>
K_KP_9: Keycode  # value = <Keycode.K_KP_9: 1073741921>
K_KP_DIV: Keycode  # value = <Keycode.K_KP_DIV: 1073741908>
K_KP_ENTER: Keycode  # value = <Keycode.K_KP_ENTER: 1073741912>
K_KP_MINUS: Keycode  # value = <Keycode.K_KP_MINUS: 1073741910>
K_KP_MULT: Keycode  # value = <Keycode.K_KP_MULT: 1073741909>
K_KP_PERIOD: Keycode  # value = <Keycode.K_KP_PERIOD: 1073741923>
K_KP_PLUS: Keycode  # value = <Keycode.K_KP_PLUS: 1073741911>
K_LALT: Keycode  # value = <Keycode.K_LALT: 1073742050>
K_LBRACE: Keycode  # value = <Keycode.K_LBRACE: 123>
K_LBRACKET: Keycode  # value = <Keycode.K_LBRACKET: 91>
K_LCTRL: Keycode  # value = <Keycode.K_LCTRL: 1073742048>
K_LEFT: Keycode  # value = <Keycode.K_LEFT: 1073741904>
K_LGUI: Keycode  # value = <Keycode.K_LGUI: 1073742051>
K_LPAREN: Keycode  # value = <Keycode.K_LPAREN: 40>
K_LSHIFT: Keycode  # value = <Keycode.K_LSHIFT: 1073742049>
K_LT: Keycode  # value = <Keycode.K_LT: 60>
K_MINUS: Keycode  # value = <Keycode.K_MINUS: 45>
K_MUTE: Keycode  # value = <Keycode.K_MUTE: 1073741951>
K_NUMLOCK: Keycode  # value = <Keycode.K_NUMLOCK: 1073741907>
K_PASTE: Keycode  # value = <Keycode.K_PASTE: 1073741949>
K_PAUSE: Keycode  # value = <Keycode.K_PAUSE: 1073741896>
K_PERCENT: Keycode  # value = <Keycode.K_PERCENT: 37>
K_PERIOD: Keycode  # value = <Keycode.K_PERIOD: 46>
K_PGDOWN: Keycode  # value = <Keycode.K_PGDOWN: 1073741902>
K_PGUP: Keycode  # value = <Keycode.K_PGUP: 1073741899>
K_PIPE: Keycode  # value = <Keycode.K_PIPE: 124>
K_PLUS: Keycode  # value = <Keycode.K_PLUS: 43>
K_PRTSCR: Keycode  # value = <Keycode.K_PRTSCR: 1073741894>
K_QUESTION: Keycode  # value = <Keycode.K_QUESTION: 63>
K_RALT: Keycode  # value = <Keycode.K_RALT: 1073742054>
K_RBRACE: Keycode  # value = <Keycode.K_RBRACE: 125>
K_RBRACKET: Keycode  # value = <Keycode.K_RBRACKET: 93>
K_RCTRL: Keycode  # value = <Keycode.K_RCTRL: 1073742052>
K_RETURN: Keycode  # value = <Keycode.K_RETURN: 13>
K_RGUI: Keycode  # value = <Keycode.K_RGUI: 1073742055>
K_RIGHT: Keycode  # value = <Keycode.K_RIGHT: 1073741903>
K_RPAREN: Keycode  # value = <Keycode.K_RPAREN: 41>
K_RSHIFT: Keycode  # value = <Keycode.K_RSHIFT: 1073742053>
K_SCRLK: Keycode  # value = <Keycode.K_SCRLK: 1073741895>
K_SEMICOLON: Keycode  # value = <Keycode.K_SEMICOLON: 59>
K_SGLQUOTE: Keycode  # value = <Keycode.K_SGLQUOTE: 39>
K_SLASH: Keycode  # value = <Keycode.K_SLASH: 47>
K_SPACE: Keycode  # value = <Keycode.K_SPACE: 32>
K_TAB: Keycode  # value = <Keycode.K_TAB: 9>
K_TILDE: Keycode  # value = <Keycode.K_TILDE: 126>
K_UNDERSCORE: Keycode  # value = <Keycode.K_UNDERSCORE: 95>
K_UNDO: Keycode  # value = <Keycode.K_UNDO: 1073741946>
K_UP: Keycode  # value = <Keycode.K_UP: 1073741906>
K_VOLDOWN: Keycode  # value = <Keycode.K_VOLDOWN: 1073741953>
K_VOLUP: Keycode  # value = <Keycode.K_VOLUP: 1073741952>
K_a: Keycode  # value = <Keycode.K_a: 97>
K_b: Keycode  # value = <Keycode.K_b: 98>
K_c: Keycode  # value = <Keycode.K_c: 99>
K_d: Keycode  # value = <Keycode.K_d: 100>
K_e: Keycode  # value = <Keycode.K_e: 101>
K_f: Keycode  # value = <Keycode.K_f: 102>
K_g: Keycode  # value = <Keycode.K_g: 103>
K_h: Keycode  # value = <Keycode.K_h: 104>
K_i: Keycode  # value = <Keycode.K_i: 105>
K_j: Keycode  # value = <Keycode.K_j: 106>
K_k: Keycode  # value = <Keycode.K_k: 107>
K_l: Keycode  # value = <Keycode.K_l: 108>
K_m: Keycode  # value = <Keycode.K_m: 109>
K_n: Keycode  # value = <Keycode.K_n: 110>
K_o: Keycode  # value = <Keycode.K_o: 111>
K_p: Keycode  # value = <Keycode.K_p: 112>
K_q: Keycode  # value = <Keycode.K_q: 113>
K_r: Keycode  # value = <Keycode.K_r: 114>
K_s: Keycode  # value = <Keycode.K_s: 115>
K_t: Keycode  # value = <Keycode.K_t: 116>
K_u: Keycode  # value = <Keycode.K_u: 117>
K_v: Keycode  # value = <Keycode.K_v: 118>
K_w: Keycode  # value = <Keycode.K_w: 119>
K_x: Keycode  # value = <Keycode.K_x: 120>
K_y: Keycode  # value = <Keycode.K_y: 121>
K_z: Keycode  # value = <Keycode.K_z: 122>
MID_LEFT: Anchor  # value = <Anchor.MID_LEFT: 3>
MID_RIGHT: Anchor  # value = <Anchor.MID_RIGHT: 5>
MOUSE_ADDED: EventType  # value = <EventType.MOUSE_ADDED: 1028>
MOUSE_BUTTON_DOWN: EventType  # value = <EventType.MOUSE_BUTTON_DOWN: 1025>
MOUSE_BUTTON_UP: EventType  # value = <EventType.MOUSE_BUTTON_UP: 1026>
MOUSE_MOTION: EventType  # value = <EventType.MOUSE_MOTION: 1024>
MOUSE_REMOVED: EventType  # value = <EventType.MOUSE_REMOVED: 1029>
MOUSE_WHEEL: EventType  # value = <EventType.MOUSE_WHEEL: 1027>
M_LEFT: MouseButton  # value = <MouseButton.M_LEFT: 1>
M_MIDDLE: MouseButton  # value = <MouseButton.M_MIDDLE: 2>
M_RIGHT: MouseButton  # value = <MouseButton.M_RIGHT: 3>
M_SIDE1: MouseButton  # value = <MouseButton.M_SIDE1: 4>
M_SIDE2: MouseButton  # value = <MouseButton.M_SIDE2: 5>
PEN_AXIS: EventType  # value = <EventType.PEN_AXIS: 4871>
PEN_BUTTON_DOWN: EventType  # value = <EventType.PEN_BUTTON_DOWN: 4868>
PEN_BUTTON_UP: EventType  # value = <EventType.PEN_BUTTON_UP: 4869>
PEN_DOWN: EventType  # value = <EventType.PEN_DOWN: 4866>
PEN_MOTION: EventType  # value = <EventType.PEN_MOTION: 4870>
PEN_PROXIMITY_IN: EventType  # value = <EventType.PEN_PROXIMITY_IN: 4864>
PEN_PROXIMITY_OUT: EventType  # value = <EventType.PEN_PROXIMITY_OUT: 4865>
PEN_UP: EventType  # value = <EventType.PEN_UP: 4867>
QUIT: EventType  # value = <EventType.QUIT: 256>
S_0: Scancode  # value = <Scancode.S_0: 39>
S_1: Scancode  # value = <Scancode.S_1: 30>
S_2: Scancode  # value = <Scancode.S_2: 31>
S_3: Scancode  # value = <Scancode.S_3: 32>
S_4: Scancode  # value = <Scancode.S_4: 33>
S_5: Scancode  # value = <Scancode.S_5: 34>
S_6: Scancode  # value = <Scancode.S_6: 35>
S_7: Scancode  # value = <Scancode.S_7: 36>
S_8: Scancode  # value = <Scancode.S_8: 37>
S_9: Scancode  # value = <Scancode.S_9: 38>
S_AGAIN: Scancode  # value = <Scancode.S_AGAIN: 121>
S_APOSTROPHE: Scancode  # value = <Scancode.S_APOSTROPHE: 52>
S_BACKSLASH: Scancode  # value = <Scancode.S_BACKSLASH: 49>
S_BACKSPACE: Scancode  # value = <Scancode.S_BACKSPACE: 42>
S_CAPS: Scancode  # value = <Scancode.S_CAPS: 57>
S_COMMA: Scancode  # value = <Scancode.S_COMMA: 54>
S_COPY: Scancode  # value = <Scancode.S_COPY: 124>
S_CUT: Scancode  # value = <Scancode.S_CUT: 123>
S_DEL: Scancode  # value = <Scancode.S_DEL: 76>
S_DOWN: Scancode  # value = <Scancode.S_DOWN: 81>
S_END: Scancode  # value = <Scancode.S_END: 77>
S_EQ: Scancode  # value = <Scancode.S_EQ: 46>
S_ESC: Scancode  # value = <Scancode.S_ESC: 41>
S_F1: Scancode  # value = <Scancode.S_F1: 58>
S_F10: Scancode  # value = <Scancode.S_F10: 67>
S_F11: Scancode  # value = <Scancode.S_F11: 68>
S_F12: Scancode  # value = <Scancode.S_F12: 69>
S_F2: Scancode  # value = <Scancode.S_F2: 59>
S_F3: Scancode  # value = <Scancode.S_F3: 60>
S_F4: Scancode  # value = <Scancode.S_F4: 61>
S_F5: Scancode  # value = <Scancode.S_F5: 62>
S_F6: Scancode  # value = <Scancode.S_F6: 63>
S_F7: Scancode  # value = <Scancode.S_F7: 64>
S_F8: Scancode  # value = <Scancode.S_F8: 65>
S_F9: Scancode  # value = <Scancode.S_F9: 66>
S_FIND: Scancode  # value = <Scancode.S_FIND: 126>
S_GRAVE: Scancode  # value = <Scancode.S_GRAVE: 53>
S_HOME: Scancode  # value = <Scancode.S_HOME: 74>
S_INS: Scancode  # value = <Scancode.S_INS: 73>
S_KP_0: Scancode  # value = <Scancode.S_KP_0: 98>
S_KP_1: Scancode  # value = <Scancode.S_KP_1: 89>
S_KP_2: Scancode  # value = <Scancode.S_KP_2: 90>
S_KP_3: Scancode  # value = <Scancode.S_KP_3: 91>
S_KP_4: Scancode  # value = <Scancode.S_KP_4: 92>
S_KP_5: Scancode  # value = <Scancode.S_KP_5: 93>
S_KP_6: Scancode  # value = <Scancode.S_KP_6: 94>
S_KP_7: Scancode  # value = <Scancode.S_KP_7: 95>
S_KP_8: Scancode  # value = <Scancode.S_KP_8: 96>
S_KP_9: Scancode  # value = <Scancode.S_KP_9: 97>
S_KP_DIV: Scancode  # value = <Scancode.S_KP_DIV: 84>
S_KP_ENTER: Scancode  # value = <Scancode.S_KP_ENTER: 88>
S_KP_MINUS: Scancode  # value = <Scancode.S_KP_MINUS: 86>
S_KP_MULT: Scancode  # value = <Scancode.S_KP_MULT: 85>
S_KP_PERIOD: Scancode  # value = <Scancode.S_KP_PERIOD: 99>
S_KP_PLUS: Scancode  # value = <Scancode.S_KP_PLUS: 87>
S_LALT: Scancode  # value = <Scancode.S_LALT: 226>
S_LBRACKET: Scancode  # value = <Scancode.S_LBRACKET: 47>
S_LCTRL: Scancode  # value = <Scancode.S_LCTRL: 224>
S_LEFT: Scancode  # value = <Scancode.S_LEFT: 80>
S_LGUI: Scancode  # value = <Scancode.S_LGUI: 227>
S_LSHIFT: Scancode  # value = <Scancode.S_LSHIFT: 225>
S_MINUS: Scancode  # value = <Scancode.S_MINUS: 45>
S_MUTE: Scancode  # value = <Scancode.S_MUTE: 127>
S_NUMLOCK: Scancode  # value = <Scancode.S_NUMLOCK: 83>
S_PASTE: Scancode  # value = <Scancode.S_PASTE: 125>
S_PAUSE: Scancode  # value = <Scancode.S_PAUSE: 72>
S_PERIOD: Scancode  # value = <Scancode.S_PERIOD: 55>
S_PGDOWN: Scancode  # value = <Scancode.S_PGDOWN: 78>
S_PGUP: Scancode  # value = <Scancode.S_PGUP: 75>
S_PRTSCR: Scancode  # value = <Scancode.S_PRTSCR: 70>
S_RALT: Scancode  # value = <Scancode.S_RALT: 230>
S_RBRACKET: Scancode  # value = <Scancode.S_RBRACKET: 48>
S_RCTRL: Scancode  # value = <Scancode.S_RCTRL: 228>
S_RETURN: Scancode  # value = <Scancode.S_RETURN: 40>
S_RGUI: Scancode  # value = <Scancode.S_RGUI: 231>
S_RIGHT: Scancode  # value = <Scancode.S_RIGHT: 79>
S_RSHIFT: Scancode  # value = <Scancode.S_RSHIFT: 229>
S_SCRLK: Scancode  # value = <Scancode.S_SCRLK: 71>
S_SEMICOLON: Scancode  # value = <Scancode.S_SEMICOLON: 51>
S_SLASH: Scancode  # value = <Scancode.S_SLASH: 56>
S_SPACE: Scancode  # value = <Scancode.S_SPACE: 44>
S_TAB: Scancode  # value = <Scancode.S_TAB: 43>
S_UNDO: Scancode  # value = <Scancode.S_UNDO: 122>
S_UP: Scancode  # value = <Scancode.S_UP: 82>
S_VOLDOWN: Scancode  # value = <Scancode.S_VOLDOWN: 129>
S_VOLUP: Scancode  # value = <Scancode.S_VOLUP: 128>
S_a: Scancode  # value = <Scancode.S_a: 4>
S_b: Scancode  # value = <Scancode.S_b: 5>
S_c: Scancode  # value = <Scancode.S_c: 6>
S_d: Scancode  # value = <Scancode.S_d: 7>
S_e: Scancode  # value = <Scancode.S_e: 8>
S_f: Scancode  # value = <Scancode.S_f: 9>
S_g: Scancode  # value = <Scancode.S_g: 10>
S_h: Scancode  # value = <Scancode.S_h: 11>
S_i: Scancode  # value = <Scancode.S_i: 12>
S_j: Scancode  # value = <Scancode.S_j: 13>
S_k: Scancode  # value = <Scancode.S_k: 14>
S_l: Scancode  # value = <Scancode.S_l: 15>
S_m: Scancode  # value = <Scancode.S_m: 16>
S_n: Scancode  # value = <Scancode.S_n: 17>
S_o: Scancode  # value = <Scancode.S_o: 18>
S_p: Scancode  # value = <Scancode.S_p: 19>
S_q: Scancode  # value = <Scancode.S_q: 20>
S_r: Scancode  # value = <Scancode.S_r: 21>
S_s: Scancode  # value = <Scancode.S_s: 22>
S_t: Scancode  # value = <Scancode.S_t: 23>
S_u: Scancode  # value = <Scancode.S_u: 24>
S_v: Scancode  # value = <Scancode.S_v: 25>
S_w: Scancode  # value = <Scancode.S_w: 26>
S_x: Scancode  # value = <Scancode.S_x: 27>
S_y: Scancode  # value = <Scancode.S_y: 28>
S_z: Scancode  # value = <Scancode.S_z: 29>
TEXT_EDITING: EventType  # value = <EventType.TEXT_EDITING: 770>
TEXT_INPUT: EventType  # value = <EventType.TEXT_INPUT: 771>
TOP_LEFT: Anchor  # value = <Anchor.TOP_LEFT: 0>
TOP_MID: Anchor  # value = <Anchor.TOP_MID: 1>
TOP_RIGHT: Anchor  # value = <Anchor.TOP_RIGHT: 2>
WINDOW_ENTER_FULLSCREEN: EventType  # value = <EventType.WINDOW_ENTER_FULLSCREEN: 535>
WINDOW_EXPOSED: EventType  # value = <EventType.WINDOW_EXPOSED: 516>
WINDOW_FOCUS_GAINED: EventType  # value = <EventType.WINDOW_FOCUS_GAINED: 526>
WINDOW_FOCUS_LOST: EventType  # value = <EventType.WINDOW_FOCUS_LOST: 527>
WINDOW_HIDDEN: EventType  # value = <EventType.WINDOW_HIDDEN: 515>
WINDOW_LEAVE_FULLSCREEN: EventType  # value = <EventType.WINDOW_LEAVE_FULLSCREEN: 536>
WINDOW_MAXIMIZED: EventType  # value = <EventType.WINDOW_MAXIMIZED: 522>
WINDOW_MINIMIZED: EventType  # value = <EventType.WINDOW_MINIMIZED: 521>
WINDOW_MOUSE_ENTER: EventType  # value = <EventType.WINDOW_MOUSE_ENTER: 524>
WINDOW_MOUSE_LEAVE: EventType  # value = <EventType.WINDOW_MOUSE_LEAVE: 525>
WINDOW_MOVED: EventType  # value = <EventType.WINDOW_MOVED: 517>
WINDOW_OCCLUDED: EventType  # value = <EventType.WINDOW_OCCLUDED: 534>
WINDOW_RESIZED: EventType  # value = <EventType.WINDOW_RESIZED: 518>
WINDOW_RESTORED: EventType  # value = <EventType.WINDOW_RESTORED: 523>
WINDOW_SHOWN: EventType  # value = <EventType.WINDOW_SHOWN: 514>
