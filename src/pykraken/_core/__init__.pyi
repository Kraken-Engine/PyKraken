from __future__ import annotations
import typing
from . import color
from . import draw
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
from . import window
__all__ = ['AUDIODEVICEADDED', 'AUDIODEVICEREMOVED', 'CAMERAADDED', 'CAMERAAPPROVED', 'CAMERADENIED', 'CAMERAREMOVED', 'C_BACK', 'C_DPADDOWN', 'C_DPADLEFT', 'C_DPADRIGHT', 'C_DPADUP', 'C_EAST', 'C_GUIDE', 'C_LEFTSHOULDER', 'C_LEFTSTICK', 'C_LTRIGGER', 'C_LX', 'C_LY', 'C_NORTH', 'C_PS3', 'C_PS4', 'C_PS5', 'C_RIGHTSHOULDER', 'C_RIGHTSTICK', 'C_RTRIGGER', 'C_RX', 'C_RY', 'C_SOUTH', 'C_STANDARD', 'C_START', 'C_SWITCHJOYCONLEFT', 'C_SWITCHJOYCONPAIR', 'C_SWITCHJOYCONRIGHT', 'C_SWITCHPRO', 'C_WEST', 'C_XBOX360', 'C_XBOXONE', 'Camera', 'Circle', 'Clock', 'Color', 'DROPBEGIN', 'DROPCOMPLETE', 'DROPFILE', 'DROPPOSITION', 'DROPTEXT', 'EasingAnimation', 'Event', 'EventType', 'GAMEPADADDED', 'GAMEPADAXISMOTION', 'GAMEPADBUTTONDOWN', 'GAMEPADBUTTONUP', 'GAMEPADREMOVED', 'GAMEPADTOUCHPADDOWN', 'GAMEPADTOUCHPADMOTION', 'GAMEPADTOUCHPADUP', 'GamepadAxis', 'GamepadButton', 'GamepadType', 'InputAction', 'KEYBOARD_ADDED', 'KEYBOARD_REMOVED', 'KEYDOWN', 'KEYUP', 'K_0', 'K_1', 'K_2', 'K_3', 'K_4', 'K_5', 'K_6', 'K_7', 'K_8', 'K_9', 'K_AGAIN', 'K_AMPERSAND', 'K_ASTERISK', 'K_AT', 'K_BACKSLASH', 'K_BACKSPACE', 'K_CAPS', 'K_CARET', 'K_COLON', 'K_COMMA', 'K_COPY', 'K_CUT', 'K_DBLQUOTE', 'K_DEL', 'K_DOLLAR', 'K_DOWN', 'K_END', 'K_EQ', 'K_ESC', 'K_EXCLAIM', 'K_F1', 'K_F10', 'K_F11', 'K_F12', 'K_F2', 'K_F3', 'K_F4', 'K_F5', 'K_F6', 'K_F7', 'K_F8', 'K_F9', 'K_FIND', 'K_GRAVE', 'K_GT', 'K_HASH', 'K_HOME', 'K_INS', 'K_KP_0', 'K_KP_1', 'K_KP_2', 'K_KP_3', 'K_KP_4', 'K_KP_5', 'K_KP_6', 'K_KP_7', 'K_KP_8', 'K_KP_9', 'K_KP_DIV', 'K_KP_ENTER', 'K_KP_MINUS', 'K_KP_MULT', 'K_KP_PERIOD', 'K_KP_PLUS', 'K_LALT', 'K_LBRACE', 'K_LBRACKET', 'K_LCTRL', 'K_LEFT', 'K_LGUI', 'K_LPAREN', 'K_LSHIFT', 'K_LT', 'K_MINUS', 'K_MUTE', 'K_NUMLOCK', 'K_PASTE', 'K_PAUSE', 'K_PERCENT', 'K_PERIOD', 'K_PGDOWN', 'K_PGUP', 'K_PIPE', 'K_PLUS', 'K_PRTSCR', 'K_QUESTION', 'K_RALT', 'K_RBRACE', 'K_RBRACKET', 'K_RCTRL', 'K_RETURN', 'K_RGUI', 'K_RIGHT', 'K_RPAREN', 'K_RSHIFT', 'K_SCRLK', 'K_SEMICOLON', 'K_SGLQUOTE', 'K_SLASH', 'K_SPACE', 'K_TAB', 'K_TILDE', 'K_UNDERSCORE', 'K_UNDO', 'K_UP', 'K_VOLDOWN', 'K_VOLUP', 'K_a', 'K_b', 'K_c', 'K_d', 'K_e', 'K_f', 'K_g', 'K_h', 'K_i', 'K_j', 'K_k', 'K_l', 'K_m', 'K_n', 'K_o', 'K_p', 'K_q', 'K_r', 'K_s', 'K_t', 'K_u', 'K_v', 'K_w', 'K_x', 'K_y', 'K_z', 'Keycode', 'Line', 'MOUSEADDED', 'MOUSEBUTTONDOWN', 'MOUSEBUTTONUP', 'MOUSEMOTION', 'MOUSEREMOVED', 'MOUSEWHEEL', 'M_LEFT', 'M_MIDDLE', 'M_RIGHT', 'M_SIDE1', 'M_SIDE2', 'MouseButton', 'PENAXIS', 'PENBUTTONDOWN', 'PENBUTTONUP', 'PENDOWN', 'PENMOTION', 'PENPROXIMITYIN', 'PENPROXIMITYOUT', 'PENUP', 'PolarCoordinate', 'QUIT', 'Rect', 'S_0', 'S_1', 'S_2', 'S_3', 'S_4', 'S_5', 'S_6', 'S_7', 'S_8', 'S_9', 'S_AGAIN', 'S_APOSTROPHE', 'S_BACKSLASH', 'S_BACKSPACE', 'S_CAPS', 'S_COMMA', 'S_COPY', 'S_CUT', 'S_DEL', 'S_DOWN', 'S_END', 'S_EQ', 'S_ESC', 'S_F1', 'S_F10', 'S_F11', 'S_F12', 'S_F2', 'S_F3', 'S_F4', 'S_F5', 'S_F6', 'S_F7', 'S_F8', 'S_F9', 'S_FIND', 'S_GRAVE', 'S_HOME', 'S_INS', 'S_KP_0', 'S_KP_1', 'S_KP_2', 'S_KP_3', 'S_KP_4', 'S_KP_5', 'S_KP_6', 'S_KP_7', 'S_KP_8', 'S_KP_9', 'S_KP_DIV', 'S_KP_ENTER', 'S_KP_MINUS', 'S_KP_MULT', 'S_KP_PERIOD', 'S_KP_PLUS', 'S_LALT', 'S_LBRACKET', 'S_LCTRL', 'S_LEFT', 'S_LGUI', 'S_LSHIFT', 'S_MINUS', 'S_MUTE', 'S_NUMLOCK', 'S_PASTE', 'S_PAUSE', 'S_PERIOD', 'S_PGDOWN', 'S_PGUP', 'S_PRTSCR', 'S_RALT', 'S_RBRACKET', 'S_RCTRL', 'S_RETURN', 'S_RGUI', 'S_RIGHT', 'S_RSHIFT', 'S_SCRLK', 'S_SEMICOLON', 'S_SLASH', 'S_SPACE', 'S_TAB', 'S_UNDO', 'S_UP', 'S_VOLDOWN', 'S_VOLUP', 'S_a', 'S_b', 'S_c', 'S_d', 'S_e', 'S_f', 'S_g', 'S_h', 'S_i', 'S_j', 'S_k', 'S_l', 'S_m', 'S_n', 'S_o', 'S_p', 'S_q', 'S_r', 'S_s', 'S_t', 'S_u', 'S_v', 'S_w', 'S_x', 'S_y', 'S_z', 'Scancode', 'TEXTEDITING', 'TEXTINPUT', 'Vec2', 'WINDOWENTERFULLSCREEN', 'WINDOWEXPOSED', 'WINDOWFOCUSGAINED', 'WINDOWFOCUSLOST', 'WINDOWHIDDEN', 'WINDOWLEAVEFULLSCREEN', 'WINDOWMAXIMIZED', 'WINDOWMINIMIZED', 'WINDOWMOUSEENTER', 'WINDOWMOUSELEAVE', 'WINDOWMOVED', 'WINDOWOCCLUDED', 'WINDOWRESIZED', 'WINDOWRESTORED', 'WINDOWSHOWN', 'color', 'draw', 'ease', 'event', 'gamepad', 'init', 'input', 'key', 'line', 'math', 'mouse', 'quit', 'rect', 'time', 'window']
class Camera:
    """
    
            Represents a 2D camera used for rendering. Controls viewport translation and scaling.
        
    """
    @staticmethod
    def __init__(*args, **kwargs):
        """
        
                    Default constructor for Camera.
        
                    Initializes the camera at position (0.0, 0.0).
                
        
                    Create a Camera at a specific position.
        
                    :param x: X-coordinate of the camera.
                    :type x: float
                    :param y: Y-coordinate of the camera.
                    :type y: float
                
        
                    Create a Camera using a Vec2 or a sequence of two values.
        
                    :param pos: A ``Vec2`` object or a sequence of two floats representing the camera's position.
                    :type pos: Union[Vec2, tuple[float, float]]
                
        """
    @staticmethod
    def set(*args, **kwargs):
        """
        
                    Set this camera as the active one for rendering.
        
                    Only one camera can be active at a time.
                
        """
    @property
    def pos(*args, **kwargs):
        """
                    Get or set the camera's position.
        
                    :getter: Get the camera's position as a Vec2.
                    :setter: Set the camera's position using a Vec2 or a sequence of two floats.
                    :type: Union[Vec2, tuple[float, float]]
        """
    @pos.setter
    def pos(*args, **kwargs):
        ...
class Circle:
    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __getitem__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __iter__(*args, **kwargs):
        ...
    @staticmethod
    def __len__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def as_rect(*args, **kwargs):
        ...
    @staticmethod
    def collide_circle(*args, **kwargs):
        ...
    @staticmethod
    def collide_line(*args, **kwargs):
        ...
    @staticmethod
    def collide_point(*args, **kwargs):
        ...
    @staticmethod
    def collide_rect(*args, **kwargs):
        ...
    @staticmethod
    def contains(*args, **kwargs):
        ...
    @staticmethod
    def copy(*args, **kwargs):
        ...
    @property
    def area(*args, **kwargs):
        ...
    @property
    def circumference(*args, **kwargs):
        ...
    @property
    def pos(*args, **kwargs):
        ...
    @pos.setter
    def pos(*args, **kwargs):
        ...
    @property
    def radius(*args, **kwargs):
        ...
    @radius.setter
    def radius(*args, **kwargs):
        ...
class Clock:
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def get_fps(*args, **kwargs):
        """
        Get the current frames per second of the program
        """
    @staticmethod
    def tick(*args, **kwargs):
        """
        Get the time since the last frame in seconds and optionally cap the framerate
        """
class Color:
    @staticmethod
    def __getitem__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __iter__(*args, **kwargs):
        ...
    @staticmethod
    def __len__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __setitem__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @property
    def a(*args, **kwargs):
        ...
    @a.setter
    def a(*args, **kwargs):
        ...
    @property
    def b(*args, **kwargs):
        ...
    @b.setter
    def b(*args, **kwargs):
        ...
    @property
    def g(*args, **kwargs):
        ...
    @g.setter
    def g(*args, **kwargs):
        ...
    @property
    def hex(*args, **kwargs):
        ...
    @hex.setter
    def hex(*args, **kwargs):
        ...
    @property
    def r(*args, **kwargs):
        ...
    @r.setter
    def r(*args, **kwargs):
        ...
class EasingAnimation:
    @staticmethod
    def __init__(*args, **kwargs):
        """
        Create an EasingAnimation with start and end positions, duration, and easing function
        """
    @staticmethod
    def pause(*args, **kwargs):
        """
        Pause the animation
        """
    @staticmethod
    def restart(*args, **kwargs):
        """
        Restart the animation
        """
    @staticmethod
    def resume(*args, **kwargs):
        """
        Resume the animation
        """
    @staticmethod
    def reverse(*args, **kwargs):
        """
        Reverse the direction of the animation
        """
    @staticmethod
    def step(*args, **kwargs):
        """
        Update the animation with the given delta time and get the current position in the animation
        """
    @property
    def is_done(*args, **kwargs):
        """
        Check if the animation is done
        """
class Event:
    @staticmethod
    def __getattr__(*args, **kwargs):
        ...
    @property
    def type(*args, **kwargs):
        ...
class EventType:
    """
    Members:
    
      QUIT
    
      WINDOWSHOWN
    
      WINDOWHIDDEN
    
      WINDOWEXPOSED
    
      WINDOWMOVED
    
      WINDOWRESIZED
    
      WINDOWMINIMIZED
    
      WINDOWMAXIMIZED
    
      WINDOWRESTORED
    
      WINDOWMOUSEENTER
    
      WINDOWMOUSELEAVE
    
      WINDOWFOCUSGAINED
    
      WINDOWFOCUSLOST
    
      WINDOWOCCLUDED
    
      WINDOWENTERFULLSCREEN
    
      WINDOWLEAVEFULLSCREEN
    
      KEYDOWN
    
      KEYUP
    
      TEXTEDITING
    
      TEXTINPUT
    
      KEYBOARD_ADDED
    
      KEYBOARD_REMOVED
    
      MOUSEMOTION
    
      MOUSEBUTTONDOWN
    
      MOUSEBUTTONUP
    
      MOUSEWHEEL
    
      MOUSEADDED
    
      MOUSEREMOVED
    
      GAMEPADAXISMOTION
    
      GAMEPADBUTTONDOWN
    
      GAMEPADBUTTONUP
    
      GAMEPADADDED
    
      GAMEPADREMOVED
    
      GAMEPADTOUCHPADDOWN
    
      GAMEPADTOUCHPADMOTION
    
      GAMEPADTOUCHPADUP
    
      DROPFILE
    
      DROPTEXT
    
      DROPBEGIN
    
      DROPCOMPLETE
    
      DROPPOSITION
    
      AUDIODEVICEADDED
    
      AUDIODEVICEREMOVED
    
      PENPROXIMITYIN
    
      PENPROXIMITYOUT
    
      PENDOWN
    
      PENUP
    
      PENBUTTONDOWN
    
      PENBUTTONUP
    
      PENMOTION
    
      PENAXIS
    
      CAMERAADDED
    
      CAMERAREMOVED
    
      CAMERAAPPROVED
    
      CAMERADENIED
    """
    AUDIODEVICEADDED: typing.ClassVar[EventType]  # value = <EventType.AUDIODEVICEADDED: 4352>
    AUDIODEVICEREMOVED: typing.ClassVar[EventType]  # value = <EventType.AUDIODEVICEREMOVED: 4353>
    CAMERAADDED: typing.ClassVar[EventType]  # value = <EventType.CAMERAADDED: 5120>
    CAMERAAPPROVED: typing.ClassVar[EventType]  # value = <EventType.CAMERAAPPROVED: 5122>
    CAMERADENIED: typing.ClassVar[EventType]  # value = <EventType.CAMERADENIED: 5123>
    CAMERAREMOVED: typing.ClassVar[EventType]  # value = <EventType.CAMERAREMOVED: 5121>
    DROPBEGIN: typing.ClassVar[EventType]  # value = <EventType.DROPBEGIN: 4098>
    DROPCOMPLETE: typing.ClassVar[EventType]  # value = <EventType.DROPCOMPLETE: 4099>
    DROPFILE: typing.ClassVar[EventType]  # value = <EventType.DROPFILE: 4096>
    DROPPOSITION: typing.ClassVar[EventType]  # value = <EventType.DROPPOSITION: 4100>
    DROPTEXT: typing.ClassVar[EventType]  # value = <EventType.DROPTEXT: 4097>
    GAMEPADADDED: typing.ClassVar[EventType]  # value = <EventType.GAMEPADADDED: 1619>
    GAMEPADAXISMOTION: typing.ClassVar[EventType]  # value = <EventType.GAMEPADAXISMOTION: 1616>
    GAMEPADBUTTONDOWN: typing.ClassVar[EventType]  # value = <EventType.GAMEPADBUTTONDOWN: 1617>
    GAMEPADBUTTONUP: typing.ClassVar[EventType]  # value = <EventType.GAMEPADBUTTONUP: 1618>
    GAMEPADREMOVED: typing.ClassVar[EventType]  # value = <EventType.GAMEPADREMOVED: 1620>
    GAMEPADTOUCHPADDOWN: typing.ClassVar[EventType]  # value = <EventType.GAMEPADTOUCHPADDOWN: 1622>
    GAMEPADTOUCHPADMOTION: typing.ClassVar[EventType]  # value = <EventType.GAMEPADTOUCHPADMOTION: 1623>
    GAMEPADTOUCHPADUP: typing.ClassVar[EventType]  # value = <EventType.GAMEPADTOUCHPADUP: 1624>
    KEYBOARD_ADDED: typing.ClassVar[EventType]  # value = <EventType.KEYBOARD_ADDED: 773>
    KEYBOARD_REMOVED: typing.ClassVar[EventType]  # value = <EventType.KEYBOARD_REMOVED: 774>
    KEYDOWN: typing.ClassVar[EventType]  # value = <EventType.KEYDOWN: 768>
    KEYUP: typing.ClassVar[EventType]  # value = <EventType.KEYUP: 769>
    MOUSEADDED: typing.ClassVar[EventType]  # value = <EventType.MOUSEADDED: 1028>
    MOUSEBUTTONDOWN: typing.ClassVar[EventType]  # value = <EventType.MOUSEBUTTONDOWN: 1025>
    MOUSEBUTTONUP: typing.ClassVar[EventType]  # value = <EventType.MOUSEBUTTONUP: 1026>
    MOUSEMOTION: typing.ClassVar[EventType]  # value = <EventType.MOUSEMOTION: 1024>
    MOUSEREMOVED: typing.ClassVar[EventType]  # value = <EventType.MOUSEREMOVED: 1029>
    MOUSEWHEEL: typing.ClassVar[EventType]  # value = <EventType.MOUSEWHEEL: 1027>
    PENAXIS: typing.ClassVar[EventType]  # value = <EventType.PENAXIS: 4871>
    PENBUTTONDOWN: typing.ClassVar[EventType]  # value = <EventType.PENBUTTONDOWN: 4868>
    PENBUTTONUP: typing.ClassVar[EventType]  # value = <EventType.PENBUTTONUP: 4869>
    PENDOWN: typing.ClassVar[EventType]  # value = <EventType.PENDOWN: 4866>
    PENMOTION: typing.ClassVar[EventType]  # value = <EventType.PENMOTION: 4870>
    PENPROXIMITYIN: typing.ClassVar[EventType]  # value = <EventType.PENPROXIMITYIN: 4864>
    PENPROXIMITYOUT: typing.ClassVar[EventType]  # value = <EventType.PENPROXIMITYOUT: 4865>
    PENUP: typing.ClassVar[EventType]  # value = <EventType.PENUP: 4867>
    QUIT: typing.ClassVar[EventType]  # value = <EventType.QUIT: 256>
    TEXTEDITING: typing.ClassVar[EventType]  # value = <EventType.TEXTEDITING: 770>
    TEXTINPUT: typing.ClassVar[EventType]  # value = <EventType.TEXTINPUT: 771>
    WINDOWENTERFULLSCREEN: typing.ClassVar[EventType]  # value = <EventType.WINDOWENTERFULLSCREEN: 535>
    WINDOWEXPOSED: typing.ClassVar[EventType]  # value = <EventType.WINDOWEXPOSED: 516>
    WINDOWFOCUSGAINED: typing.ClassVar[EventType]  # value = <EventType.WINDOWFOCUSGAINED: 526>
    WINDOWFOCUSLOST: typing.ClassVar[EventType]  # value = <EventType.WINDOWFOCUSLOST: 527>
    WINDOWHIDDEN: typing.ClassVar[EventType]  # value = <EventType.WINDOWHIDDEN: 515>
    WINDOWLEAVEFULLSCREEN: typing.ClassVar[EventType]  # value = <EventType.WINDOWLEAVEFULLSCREEN: 536>
    WINDOWMAXIMIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOWMAXIMIZED: 522>
    WINDOWMINIMIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOWMINIMIZED: 521>
    WINDOWMOUSEENTER: typing.ClassVar[EventType]  # value = <EventType.WINDOWMOUSEENTER: 524>
    WINDOWMOUSELEAVE: typing.ClassVar[EventType]  # value = <EventType.WINDOWMOUSELEAVE: 525>
    WINDOWMOVED: typing.ClassVar[EventType]  # value = <EventType.WINDOWMOVED: 517>
    WINDOWOCCLUDED: typing.ClassVar[EventType]  # value = <EventType.WINDOWOCCLUDED: 534>
    WINDOWRESIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOWRESIZED: 518>
    WINDOWRESTORED: typing.ClassVar[EventType]  # value = <EventType.WINDOWRESTORED: 523>
    WINDOWSHOWN: typing.ClassVar[EventType]  # value = <EventType.WINDOWSHOWN: 514>
    __members__: typing.ClassVar[dict[str, EventType]]  # value = {'QUIT': <EventType.QUIT: 256>, 'WINDOWSHOWN': <EventType.WINDOWSHOWN: 514>, 'WINDOWHIDDEN': <EventType.WINDOWHIDDEN: 515>, 'WINDOWEXPOSED': <EventType.WINDOWEXPOSED: 516>, 'WINDOWMOVED': <EventType.WINDOWMOVED: 517>, 'WINDOWRESIZED': <EventType.WINDOWRESIZED: 518>, 'WINDOWMINIMIZED': <EventType.WINDOWMINIMIZED: 521>, 'WINDOWMAXIMIZED': <EventType.WINDOWMAXIMIZED: 522>, 'WINDOWRESTORED': <EventType.WINDOWRESTORED: 523>, 'WINDOWMOUSEENTER': <EventType.WINDOWMOUSEENTER: 524>, 'WINDOWMOUSELEAVE': <EventType.WINDOWMOUSELEAVE: 525>, 'WINDOWFOCUSGAINED': <EventType.WINDOWFOCUSGAINED: 526>, 'WINDOWFOCUSLOST': <EventType.WINDOWFOCUSLOST: 527>, 'WINDOWOCCLUDED': <EventType.WINDOWOCCLUDED: 534>, 'WINDOWENTERFULLSCREEN': <EventType.WINDOWENTERFULLSCREEN: 535>, 'WINDOWLEAVEFULLSCREEN': <EventType.WINDOWLEAVEFULLSCREEN: 536>, 'KEYDOWN': <EventType.KEYDOWN: 768>, 'KEYUP': <EventType.KEYUP: 769>, 'TEXTEDITING': <EventType.TEXTEDITING: 770>, 'TEXTINPUT': <EventType.TEXTINPUT: 771>, 'KEYBOARD_ADDED': <EventType.KEYBOARD_ADDED: 773>, 'KEYBOARD_REMOVED': <EventType.KEYBOARD_REMOVED: 774>, 'MOUSEMOTION': <EventType.MOUSEMOTION: 1024>, 'MOUSEBUTTONDOWN': <EventType.MOUSEBUTTONDOWN: 1025>, 'MOUSEBUTTONUP': <EventType.MOUSEBUTTONUP: 1026>, 'MOUSEWHEEL': <EventType.MOUSEWHEEL: 1027>, 'MOUSEADDED': <EventType.MOUSEADDED: 1028>, 'MOUSEREMOVED': <EventType.MOUSEREMOVED: 1029>, 'GAMEPADAXISMOTION': <EventType.GAMEPADAXISMOTION: 1616>, 'GAMEPADBUTTONDOWN': <EventType.GAMEPADBUTTONDOWN: 1617>, 'GAMEPADBUTTONUP': <EventType.GAMEPADBUTTONUP: 1618>, 'GAMEPADADDED': <EventType.GAMEPADADDED: 1619>, 'GAMEPADREMOVED': <EventType.GAMEPADREMOVED: 1620>, 'GAMEPADTOUCHPADDOWN': <EventType.GAMEPADTOUCHPADDOWN: 1622>, 'GAMEPADTOUCHPADMOTION': <EventType.GAMEPADTOUCHPADMOTION: 1623>, 'GAMEPADTOUCHPADUP': <EventType.GAMEPADTOUCHPADUP: 1624>, 'DROPFILE': <EventType.DROPFILE: 4096>, 'DROPTEXT': <EventType.DROPTEXT: 4097>, 'DROPBEGIN': <EventType.DROPBEGIN: 4098>, 'DROPCOMPLETE': <EventType.DROPCOMPLETE: 4099>, 'DROPPOSITION': <EventType.DROPPOSITION: 4100>, 'AUDIODEVICEADDED': <EventType.AUDIODEVICEADDED: 4352>, 'AUDIODEVICEREMOVED': <EventType.AUDIODEVICEREMOVED: 4353>, 'PENPROXIMITYIN': <EventType.PENPROXIMITYIN: 4864>, 'PENPROXIMITYOUT': <EventType.PENPROXIMITYOUT: 4865>, 'PENDOWN': <EventType.PENDOWN: 4866>, 'PENUP': <EventType.PENUP: 4867>, 'PENBUTTONDOWN': <EventType.PENBUTTONDOWN: 4868>, 'PENBUTTONUP': <EventType.PENBUTTONUP: 4869>, 'PENMOTION': <EventType.PENMOTION: 4870>, 'PENAXIS': <EventType.PENAXIS: 4871>, 'CAMERAADDED': <EventType.CAMERAADDED: 5120>, 'CAMERAREMOVED': <EventType.CAMERAREMOVED: 5121>, 'CAMERAAPPROVED': <EventType.CAMERAAPPROVED: 5122>, 'CAMERADENIED': <EventType.CAMERADENIED: 5123>}
    @staticmethod
    def __and__(*args, **kwargs):
        ...
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __invert__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __or__(*args, **kwargs):
        ...
    @staticmethod
    def __rand__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __ror__(*args, **kwargs):
        ...
    @staticmethod
    def __rxor__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def __xor__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
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
    @staticmethod
    def __and__(*args, **kwargs):
        ...
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __invert__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __or__(*args, **kwargs):
        ...
    @staticmethod
    def __rand__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __ror__(*args, **kwargs):
        ...
    @staticmethod
    def __rxor__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def __xor__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
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
    @staticmethod
    def __and__(*args, **kwargs):
        ...
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __invert__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __or__(*args, **kwargs):
        ...
    @staticmethod
    def __rand__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __ror__(*args, **kwargs):
        ...
    @staticmethod
    def __rxor__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def __xor__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
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
    @staticmethod
    def __and__(*args, **kwargs):
        ...
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __invert__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __or__(*args, **kwargs):
        ...
    @staticmethod
    def __rand__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __ror__(*args, **kwargs):
        ...
    @staticmethod
    def __rxor__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def __xor__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
        ...
class InputAction:
    @staticmethod
    def __init__(*args, **kwargs):
        """
        Create an InputAction from a scancode
        Create an InputAction from a keycode
        Create an InputAction from a mouse button
        Create an InputAction from a gamepad button and slot
        Create an InputAction from a gamepad axis and slot
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
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
        ...
class Line:
    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __getitem__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __iter__(*args, **kwargs):
        ...
    @staticmethod
    def __len__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def copy(*args, **kwargs):
        ...
    @staticmethod
    def move(*args, **kwargs):
        ...
    @property
    def a(*args, **kwargs):
        ...
    @a.setter
    def a(*args, **kwargs):
        ...
    @property
    def ax(*args, **kwargs):
        ...
    @ax.setter
    def ax(*args, **kwargs):
        ...
    @property
    def ay(*args, **kwargs):
        ...
    @ay.setter
    def ay(*args, **kwargs):
        ...
    @property
    def b(*args, **kwargs):
        ...
    @b.setter
    def b(*args, **kwargs):
        ...
    @property
    def bx(*args, **kwargs):
        ...
    @bx.setter
    def bx(*args, **kwargs):
        ...
    @property
    def by(*args, **kwargs):
        ...
    @by.setter
    def by(*args, **kwargs):
        ...
    @property
    def length(*args, **kwargs):
        ...
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
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
        ...
class PolarCoordinate:
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __getitem__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        """
        Construct from sequence (angle, radius)
        Construct from angle and radius
        """
    @staticmethod
    def __iter__(*args, **kwargs):
        ...
    @staticmethod
    def __len__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __setitem__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def to_cartesian(*args, **kwargs):
        """
        Convert to Cartesian coordinates (Vec2)
        """
    @property
    def angle(*args, **kwargs):
        ...
    @angle.setter
    def angle(*args, **kwargs):
        ...
    @property
    def radius(*args, **kwargs):
        ...
    @radius.setter
    def radius(*args, **kwargs):
        ...
class Rect:
    __hash__: typing.ClassVar[None] = None
    @staticmethod
    def __bool__(*args, **kwargs):
        ...
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __getitem__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __iter__(*args, **kwargs):
        ...
    @staticmethod
    def __len__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def clamp(*args, **kwargs):
        ...
    @staticmethod
    def collide_point(*args, **kwargs):
        ...
    @staticmethod
    def collide_rect(*args, **kwargs):
        ...
    @staticmethod
    def contains(*args, **kwargs):
        ...
    @staticmethod
    def copy(*args, **kwargs):
        ...
    @staticmethod
    def fit(*args, **kwargs):
        ...
    @staticmethod
    def inflate(*args, **kwargs):
        ...
    @staticmethod
    def move(*args, **kwargs):
        ...
    @staticmethod
    def scale_by(*args, **kwargs):
        ...
    @staticmethod
    def scale_to(*args, **kwargs):
        ...
    @property
    def bottom(*args, **kwargs):
        ...
    @bottom.setter
    def bottom(*args, **kwargs):
        ...
    @property
    def bottom_left(*args, **kwargs):
        ...
    @bottom_left.setter
    def bottom_left(*args, **kwargs):
        ...
    @property
    def bottom_mid(*args, **kwargs):
        ...
    @bottom_mid.setter
    def bottom_mid(*args, **kwargs):
        ...
    @property
    def bottom_right(*args, **kwargs):
        ...
    @bottom_right.setter
    def bottom_right(*args, **kwargs):
        ...
    @property
    def center(*args, **kwargs):
        ...
    @center.setter
    def center(*args, **kwargs):
        ...
    @property
    def h(*args, **kwargs):
        ...
    @h.setter
    def h(*args, **kwargs):
        ...
    @property
    def left(*args, **kwargs):
        ...
    @left.setter
    def left(*args, **kwargs):
        ...
    @property
    def mid_left(*args, **kwargs):
        ...
    @mid_left.setter
    def mid_left(*args, **kwargs):
        ...
    @property
    def mid_right(*args, **kwargs):
        ...
    @mid_right.setter
    def mid_right(*args, **kwargs):
        ...
    @property
    def right(*args, **kwargs):
        ...
    @right.setter
    def right(*args, **kwargs):
        ...
    @property
    def size(*args, **kwargs):
        ...
    @size.setter
    def size(*args, **kwargs):
        ...
    @property
    def top(*args, **kwargs):
        ...
    @top.setter
    def top(*args, **kwargs):
        ...
    @property
    def top_left(*args, **kwargs):
        ...
    @top_left.setter
    def top_left(*args, **kwargs):
        ...
    @property
    def top_mid(*args, **kwargs):
        ...
    @top_mid.setter
    def top_mid(*args, **kwargs):
        ...
    @property
    def top_right(*args, **kwargs):
        ...
    @top_right.setter
    def top_right(*args, **kwargs):
        ...
    @property
    def w(*args, **kwargs):
        ...
    @w.setter
    def w(*args, **kwargs):
        ...
    @property
    def x(*args, **kwargs):
        ...
    @x.setter
    def x(*args, **kwargs):
        ...
    @property
    def y(*args, **kwargs):
        ...
    @y.setter
    def y(*args, **kwargs):
        ...
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
    @staticmethod
    def __and__(*args, **kwargs):
        ...
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getstate__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __index__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        ...
    @staticmethod
    def __int__(*args, **kwargs):
        ...
    @staticmethod
    def __invert__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __or__(*args, **kwargs):
        ...
    @staticmethod
    def __rand__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __ror__(*args, **kwargs):
        ...
    @staticmethod
    def __rxor__(*args, **kwargs):
        ...
    @staticmethod
    def __setstate__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def __xor__(*args, **kwargs):
        ...
    @property
    def name(*args, **kwargs):
        ...
    @property
    def value(*args, **kwargs):
        ...
class Vec2:
    @staticmethod
    def __add__(*args, **kwargs):
        ...
    @staticmethod
    def __bool__(*args, **kwargs):
        """
        Check if the vector is not zero
        """
    @staticmethod
    def __eq__(*args, **kwargs):
        ...
    @staticmethod
    def __ge__(*args, **kwargs):
        ...
    @staticmethod
    def __getitem__(*args, **kwargs):
        ...
    @staticmethod
    def __gt__(*args, **kwargs):
        ...
    @staticmethod
    def __hash__(*args, **kwargs):
        ...
    @staticmethod
    def __iadd__(*args, **kwargs):
        ...
    @staticmethod
    def __imul__(*args, **kwargs):
        ...
    @staticmethod
    def __init__(*args, **kwargs):
        """
        Create a zero vector
        Create a Vec2 with both x and y set to the same value
        Create a Vec2 with given x and y values
        Create a Vec2 from a sequence of two elements
        """
    @staticmethod
    def __isub__(*args, **kwargs):
        ...
    @staticmethod
    def __iter__(*args, **kwargs):
        ...
    @staticmethod
    def __itruediv__(*args, **kwargs):
        ...
    @staticmethod
    def __le__(*args, **kwargs):
        ...
    @staticmethod
    def __len__(*args, **kwargs):
        ...
    @staticmethod
    def __lt__(*args, **kwargs):
        ...
    @staticmethod
    def __mul__(*args, **kwargs):
        ...
    @staticmethod
    def __ne__(*args, **kwargs):
        ...
    @staticmethod
    def __neg__(*args, **kwargs):
        ...
    @staticmethod
    def __radd__(*args, **kwargs):
        ...
    @staticmethod
    def __repr__(*args, **kwargs):
        ...
    @staticmethod
    def __rmul__(*args, **kwargs):
        ...
    @staticmethod
    def __rsub__(*args, **kwargs):
        ...
    @staticmethod
    def __setitem__(*args, **kwargs):
        ...
    @staticmethod
    def __str__(*args, **kwargs):
        ...
    @staticmethod
    def __sub__(*args, **kwargs):
        ...
    @staticmethod
    def __truediv__(*args, **kwargs):
        ...
    @staticmethod
    def distance_to(*args, **kwargs):
        ...
    @staticmethod
    def normalize(*args, **kwargs):
        ...
    @staticmethod
    def rotate(*args, **kwargs):
        ...
    @staticmethod
    def scale_to_length(*args, **kwargs):
        ...
    @staticmethod
    def to_polar(*args, **kwargs):
        """
        Return a polar coordinate pair (angle, length)
        """
    @property
    def angle(*args, **kwargs):
        """
        Get the angle of the vector in radians
        """
    @property
    def length(*args, **kwargs):
        """
        Get the length of the vector
        """
    @property
    def x(*args, **kwargs):
        ...
    @x.setter
    def x(*args, **kwargs):
        ...
    @property
    def y(*args, **kwargs):
        ...
    @y.setter
    def y(*args, **kwargs):
        ...
def init(*args, **kwargs):
    """
    Initialize Kraken Engine
    """
def quit(*args, **kwargs):
    """
    Quit Kraken Engine
    """
AUDIODEVICEADDED: EventType  # value = <EventType.AUDIODEVICEADDED: 4352>
AUDIODEVICEREMOVED: EventType  # value = <EventType.AUDIODEVICEREMOVED: 4353>
CAMERAADDED: EventType  # value = <EventType.CAMERAADDED: 5120>
CAMERAAPPROVED: EventType  # value = <EventType.CAMERAAPPROVED: 5122>
CAMERADENIED: EventType  # value = <EventType.CAMERADENIED: 5123>
CAMERAREMOVED: EventType  # value = <EventType.CAMERAREMOVED: 5121>
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
DROPBEGIN: EventType  # value = <EventType.DROPBEGIN: 4098>
DROPCOMPLETE: EventType  # value = <EventType.DROPCOMPLETE: 4099>
DROPFILE: EventType  # value = <EventType.DROPFILE: 4096>
DROPPOSITION: EventType  # value = <EventType.DROPPOSITION: 4100>
DROPTEXT: EventType  # value = <EventType.DROPTEXT: 4097>
GAMEPADADDED: EventType  # value = <EventType.GAMEPADADDED: 1619>
GAMEPADAXISMOTION: EventType  # value = <EventType.GAMEPADAXISMOTION: 1616>
GAMEPADBUTTONDOWN: EventType  # value = <EventType.GAMEPADBUTTONDOWN: 1617>
GAMEPADBUTTONUP: EventType  # value = <EventType.GAMEPADBUTTONUP: 1618>
GAMEPADREMOVED: EventType  # value = <EventType.GAMEPADREMOVED: 1620>
GAMEPADTOUCHPADDOWN: EventType  # value = <EventType.GAMEPADTOUCHPADDOWN: 1622>
GAMEPADTOUCHPADMOTION: EventType  # value = <EventType.GAMEPADTOUCHPADMOTION: 1623>
GAMEPADTOUCHPADUP: EventType  # value = <EventType.GAMEPADTOUCHPADUP: 1624>
KEYBOARD_ADDED: EventType  # value = <EventType.KEYBOARD_ADDED: 773>
KEYBOARD_REMOVED: EventType  # value = <EventType.KEYBOARD_REMOVED: 774>
KEYDOWN: EventType  # value = <EventType.KEYDOWN: 768>
KEYUP: EventType  # value = <EventType.KEYUP: 769>
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
MOUSEADDED: EventType  # value = <EventType.MOUSEADDED: 1028>
MOUSEBUTTONDOWN: EventType  # value = <EventType.MOUSEBUTTONDOWN: 1025>
MOUSEBUTTONUP: EventType  # value = <EventType.MOUSEBUTTONUP: 1026>
MOUSEMOTION: EventType  # value = <EventType.MOUSEMOTION: 1024>
MOUSEREMOVED: EventType  # value = <EventType.MOUSEREMOVED: 1029>
MOUSEWHEEL: EventType  # value = <EventType.MOUSEWHEEL: 1027>
M_LEFT: MouseButton  # value = <MouseButton.M_LEFT: 1>
M_MIDDLE: MouseButton  # value = <MouseButton.M_MIDDLE: 2>
M_RIGHT: MouseButton  # value = <MouseButton.M_RIGHT: 3>
M_SIDE1: MouseButton  # value = <MouseButton.M_SIDE1: 4>
M_SIDE2: MouseButton  # value = <MouseButton.M_SIDE2: 5>
PENAXIS: EventType  # value = <EventType.PENAXIS: 4871>
PENBUTTONDOWN: EventType  # value = <EventType.PENBUTTONDOWN: 4868>
PENBUTTONUP: EventType  # value = <EventType.PENBUTTONUP: 4869>
PENDOWN: EventType  # value = <EventType.PENDOWN: 4866>
PENMOTION: EventType  # value = <EventType.PENMOTION: 4870>
PENPROXIMITYIN: EventType  # value = <EventType.PENPROXIMITYIN: 4864>
PENPROXIMITYOUT: EventType  # value = <EventType.PENPROXIMITYOUT: 4865>
PENUP: EventType  # value = <EventType.PENUP: 4867>
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
TEXTEDITING: EventType  # value = <EventType.TEXTEDITING: 770>
TEXTINPUT: EventType  # value = <EventType.TEXTINPUT: 771>
WINDOWENTERFULLSCREEN: EventType  # value = <EventType.WINDOWENTERFULLSCREEN: 535>
WINDOWEXPOSED: EventType  # value = <EventType.WINDOWEXPOSED: 516>
WINDOWFOCUSGAINED: EventType  # value = <EventType.WINDOWFOCUSGAINED: 526>
WINDOWFOCUSLOST: EventType  # value = <EventType.WINDOWFOCUSLOST: 527>
WINDOWHIDDEN: EventType  # value = <EventType.WINDOWHIDDEN: 515>
WINDOWLEAVEFULLSCREEN: EventType  # value = <EventType.WINDOWLEAVEFULLSCREEN: 536>
WINDOWMAXIMIZED: EventType  # value = <EventType.WINDOWMAXIMIZED: 522>
WINDOWMINIMIZED: EventType  # value = <EventType.WINDOWMINIMIZED: 521>
WINDOWMOUSEENTER: EventType  # value = <EventType.WINDOWMOUSEENTER: 524>
WINDOWMOUSELEAVE: EventType  # value = <EventType.WINDOWMOUSELEAVE: 525>
WINDOWMOVED: EventType  # value = <EventType.WINDOWMOVED: 517>
WINDOWOCCLUDED: EventType  # value = <EventType.WINDOWOCCLUDED: 534>
WINDOWRESIZED: EventType  # value = <EventType.WINDOWRESIZED: 518>
WINDOWRESTORED: EventType  # value = <EventType.WINDOWRESTORED: 523>
WINDOWSHOWN: EventType  # value = <EventType.WINDOWSHOWN: 514>
