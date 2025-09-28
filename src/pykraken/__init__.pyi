from __future__ import annotations
from pykraken._core import Anchor
from pykraken._core import Animation
from pykraken._core import AnimationController
from pykraken._core import Audio
from pykraken._core import AudioStream
from pykraken._core import Camera
from pykraken._core import Circle
from pykraken._core import Color
from pykraken._core import EasingAnimation
from pykraken._core import Event
from pykraken._core import EventType
from pykraken._core import Font
from pykraken._core import Frame
from pykraken._core import GamepadAxis
from pykraken._core import GamepadButton
from pykraken._core import GamepadType
from pykraken._core import InputAction
from pykraken._core import Keycode
from pykraken._core import Layer
from pykraken._core import Line
from pykraken._core import Mask
from pykraken._core import MouseButton
from pykraken._core import PenAxis
from pykraken._core import PixelArray
from pykraken._core import PolarCoordinate
from pykraken._core import Polygon
from pykraken._core import Rect
from pykraken._core import Scancode
from pykraken._core import Texture
from pykraken._core import Tile
from pykraken._core import TileMap
from pykraken._core import Timer
from pykraken._core import Vec2
from pykraken._core import color
from pykraken._core import draw
from pykraken._core import ease
from pykraken._core import event
from pykraken._core import gamepad
from pykraken._core import init
from pykraken._core import input
from pykraken._core import key
from pykraken._core import line
from pykraken._core import math
from pykraken._core import mouse
from pykraken._core import quit
from pykraken._core import rect
from pykraken._core import renderer
from pykraken._core import time
from pykraken._core import transform
from pykraken._core import window
from . import _core
__all__: list[str] = ['AUDIO_DEVICE_ADDED', 'AUDIO_DEVICE_FORMAT_CHANGED', 'AUDIO_DEVICE_REMOVED', 'Anchor', 'Animation', 'AnimationController', 'Audio', 'AudioStream', 'BOTTOM_LEFT', 'BOTTOM_MID', 'BOTTOM_RIGHT', 'CAMERA_ADDED', 'CAMERA_APPROVED', 'CAMERA_DENIED', 'CAMERA_REMOVED', 'CENTER', 'C_BACK', 'C_DPAD_DOWN', 'C_DPAD_LEFT', 'C_DPAD_RIGHT', 'C_DPAD_UP', 'C_EAST', 'C_GUIDE', 'C_LSHOULDER', 'C_LSTICK', 'C_LTRIGGER', 'C_LX', 'C_LY', 'C_NORTH', 'C_PS3', 'C_PS4', 'C_PS5', 'C_RSHOULDER', 'C_RSTICK', 'C_RTRIGGER', 'C_RX', 'C_RY', 'C_SOUTH', 'C_STANDARD', 'C_START', 'C_SWITCH_JOYCON_LEFT', 'C_SWITCH_JOYCON_PAIR', 'C_SWITCH_JOYCON_RIGHT', 'C_SWITCH_PRO', 'C_WEST', 'C_XBOX_360', 'C_XBOX_ONE', 'Camera', 'Circle', 'Color', 'DROP_BEGIN', 'DROP_COMPLETE', 'DROP_FILE', 'DROP_POSITION', 'DROP_TEXT', 'EasingAnimation', 'Event', 'EventType', 'Font', 'Frame', 'GAMEPAD_ADDED', 'GAMEPAD_AXIS_MOTION', 'GAMEPAD_BUTTON_DOWN', 'GAMEPAD_BUTTON_UP', 'GAMEPAD_REMOVED', 'GAMEPAD_TOUCHPAD_DOWN', 'GAMEPAD_TOUCHPAD_MOTION', 'GAMEPAD_TOUCHPAD_UP', 'GamepadAxis', 'GamepadButton', 'GamepadType', 'InputAction', 'KEYBOARD_ADDED', 'KEYBOARD_REMOVED', 'KEY_DOWN', 'KEY_UP', 'K_0', 'K_1', 'K_2', 'K_3', 'K_4', 'K_5', 'K_6', 'K_7', 'K_8', 'K_9', 'K_AGAIN', 'K_AMPERSAND', 'K_ASTERISK', 'K_AT', 'K_BACKSLASH', 'K_BACKSPACE', 'K_CAPS', 'K_CARET', 'K_COLON', 'K_COMMA', 'K_COPY', 'K_CUT', 'K_DBLQUOTE', 'K_DEL', 'K_DOLLAR', 'K_DOWN', 'K_END', 'K_EQ', 'K_ESC', 'K_EXCLAIM', 'K_F1', 'K_F10', 'K_F11', 'K_F12', 'K_F2', 'K_F3', 'K_F4', 'K_F5', 'K_F6', 'K_F7', 'K_F8', 'K_F9', 'K_FIND', 'K_GRAVE', 'K_GT', 'K_HASH', 'K_HOME', 'K_INS', 'K_KP_0', 'K_KP_1', 'K_KP_2', 'K_KP_3', 'K_KP_4', 'K_KP_5', 'K_KP_6', 'K_KP_7', 'K_KP_8', 'K_KP_9', 'K_KP_DIV', 'K_KP_ENTER', 'K_KP_MINUS', 'K_KP_MULT', 'K_KP_PERIOD', 'K_KP_PLUS', 'K_LALT', 'K_LBRACE', 'K_LBRACKET', 'K_LCTRL', 'K_LEFT', 'K_LGUI', 'K_LPAREN', 'K_LSHIFT', 'K_LT', 'K_MINUS', 'K_MUTE', 'K_NUMLOCK', 'K_PASTE', 'K_PAUSE', 'K_PERCENT', 'K_PERIOD', 'K_PGDOWN', 'K_PGUP', 'K_PIPE', 'K_PLUS', 'K_PRTSCR', 'K_QUESTION', 'K_RALT', 'K_RBRACE', 'K_RBRACKET', 'K_RCTRL', 'K_RETURN', 'K_RGUI', 'K_RIGHT', 'K_RPAREN', 'K_RSHIFT', 'K_SCRLK', 'K_SEMICOLON', 'K_SGLQUOTE', 'K_SLASH', 'K_SPACE', 'K_TAB', 'K_TILDE', 'K_UNDERSCORE', 'K_UNDO', 'K_UP', 'K_VOLDOWN', 'K_VOLUP', 'K_a', 'K_b', 'K_c', 'K_d', 'K_e', 'K_f', 'K_g', 'K_h', 'K_i', 'K_j', 'K_k', 'K_l', 'K_m', 'K_n', 'K_o', 'K_p', 'K_q', 'K_r', 'K_s', 'K_t', 'K_u', 'K_v', 'K_w', 'K_x', 'K_y', 'K_z', 'Keycode', 'Layer', 'Line', 'MID_LEFT', 'MID_RIGHT', 'MOUSE_ADDED', 'MOUSE_BUTTON_DOWN', 'MOUSE_BUTTON_UP', 'MOUSE_MOTION', 'MOUSE_REMOVED', 'MOUSE_WHEEL', 'M_LEFT', 'M_MIDDLE', 'M_RIGHT', 'M_SIDE1', 'M_SIDE2', 'Mask', 'MouseButton', 'PEN_AXIS', 'PEN_BUTTON_DOWN', 'PEN_BUTTON_UP', 'PEN_DOWN', 'PEN_MOTION', 'PEN_PROXIMITY_IN', 'PEN_PROXIMITY_OUT', 'PEN_UP', 'P_DISTANCE', 'P_PRESSURE', 'P_ROTATION', 'P_SLIDER', 'P_TANGENTIAL_PRESSURE', 'P_TILT_X', 'P_TILT_Y', 'PenAxis', 'PixelArray', 'PolarCoordinate', 'Polygon', 'QUIT', 'Rect', 'S_0', 'S_1', 'S_2', 'S_3', 'S_4', 'S_5', 'S_6', 'S_7', 'S_8', 'S_9', 'S_AGAIN', 'S_APOSTROPHE', 'S_BACKSLASH', 'S_BACKSPACE', 'S_CAPS', 'S_COMMA', 'S_COPY', 'S_CUT', 'S_DEL', 'S_DOWN', 'S_END', 'S_EQ', 'S_ESC', 'S_F1', 'S_F10', 'S_F11', 'S_F12', 'S_F2', 'S_F3', 'S_F4', 'S_F5', 'S_F6', 'S_F7', 'S_F8', 'S_F9', 'S_FIND', 'S_GRAVE', 'S_HOME', 'S_INS', 'S_KP_0', 'S_KP_1', 'S_KP_2', 'S_KP_3', 'S_KP_4', 'S_KP_5', 'S_KP_6', 'S_KP_7', 'S_KP_8', 'S_KP_9', 'S_KP_DIV', 'S_KP_ENTER', 'S_KP_MINUS', 'S_KP_MULT', 'S_KP_PERIOD', 'S_KP_PLUS', 'S_LALT', 'S_LBRACKET', 'S_LCTRL', 'S_LEFT', 'S_LGUI', 'S_LSHIFT', 'S_MINUS', 'S_MUTE', 'S_NUMLOCK', 'S_PASTE', 'S_PAUSE', 'S_PERIOD', 'S_PGDOWN', 'S_PGUP', 'S_PRTSCR', 'S_RALT', 'S_RBRACKET', 'S_RCTRL', 'S_RETURN', 'S_RGUI', 'S_RIGHT', 'S_RSHIFT', 'S_SCRLK', 'S_SEMICOLON', 'S_SLASH', 'S_SPACE', 'S_TAB', 'S_UNDO', 'S_UP', 'S_VOLDOWN', 'S_VOLUP', 'S_a', 'S_b', 'S_c', 'S_d', 'S_e', 'S_f', 'S_g', 'S_h', 'S_i', 'S_j', 'S_k', 'S_l', 'S_m', 'S_n', 'S_o', 'S_p', 'S_q', 'S_r', 'S_s', 'S_t', 'S_u', 'S_v', 'S_w', 'S_x', 'S_y', 'S_z', 'Scancode', 'TEXT_EDITING', 'TEXT_INPUT', 'TOP_LEFT', 'TOP_MID', 'TOP_RIGHT', 'Texture', 'Tile', 'TileMap', 'Timer', 'Vec2', 'WINDOW_ENTER_FULLSCREEN', 'WINDOW_EXPOSED', 'WINDOW_FOCUS_GAINED', 'WINDOW_FOCUS_LOST', 'WINDOW_HIDDEN', 'WINDOW_LEAVE_FULLSCREEN', 'WINDOW_MAXIMIZED', 'WINDOW_MINIMIZED', 'WINDOW_MOUSE_ENTER', 'WINDOW_MOUSE_LEAVE', 'WINDOW_MOVED', 'WINDOW_OCCLUDED', 'WINDOW_RESIZED', 'WINDOW_RESTORED', 'WINDOW_SHOWN', 'color', 'draw', 'ease', 'event', 'gamepad', 'init', 'input', 'key', 'line', 'math', 'mouse', 'quit', 'rect', 'renderer', 'time', 'transform', 'window']
AUDIO_DEVICE_ADDED: _core.EventType  # value = <EventType.AUDIO_DEVICE_ADDED: 4352>
AUDIO_DEVICE_FORMAT_CHANGED: _core.EventType  # value = <EventType.AUDIO_DEVICE_FORMAT_CHANGED: 4354>
AUDIO_DEVICE_REMOVED: _core.EventType  # value = <EventType.AUDIO_DEVICE_REMOVED: 4353>
BOTTOM_LEFT: _core.Anchor  # value = <Anchor.BOTTOM_LEFT: 6>
BOTTOM_MID: _core.Anchor  # value = <Anchor.BOTTOM_MID: 7>
BOTTOM_RIGHT: _core.Anchor  # value = <Anchor.BOTTOM_RIGHT: 8>
CAMERA_ADDED: _core.EventType  # value = <EventType.CAMERA_ADDED: 5120>
CAMERA_APPROVED: _core.EventType  # value = <EventType.CAMERA_APPROVED: 5122>
CAMERA_DENIED: _core.EventType  # value = <EventType.CAMERA_DENIED: 5123>
CAMERA_REMOVED: _core.EventType  # value = <EventType.CAMERA_REMOVED: 5121>
CENTER: _core.Anchor  # value = <Anchor.CENTER: 4>
C_BACK: _core.GamepadButton  # value = <GamepadButton.C_BACK: 4>
C_DPAD_DOWN: _core.GamepadButton  # value = <GamepadButton.C_DPAD_DOWN: 12>
C_DPAD_LEFT: _core.GamepadButton  # value = <GamepadButton.C_DPAD_LEFT: 13>
C_DPAD_RIGHT: _core.GamepadButton  # value = <GamepadButton.C_DPAD_RIGHT: 14>
C_DPAD_UP: _core.GamepadButton  # value = <GamepadButton.C_DPAD_UP: 11>
C_EAST: _core.GamepadButton  # value = <GamepadButton.C_EAST: 1>
C_GUIDE: _core.GamepadButton  # value = <GamepadButton.C_GUIDE: 5>
C_LSHOULDER: _core.GamepadButton  # value = <GamepadButton.C_LSHOULDER: 9>
C_LSTICK: _core.GamepadButton  # value = <GamepadButton.C_LSTICK: 7>
C_LTRIGGER: _core.GamepadAxis  # value = <GamepadAxis.C_LTRIGGER: 4>
C_LX: _core.GamepadAxis  # value = <GamepadAxis.C_LX: 0>
C_LY: _core.GamepadAxis  # value = <GamepadAxis.C_LY: 1>
C_NORTH: _core.GamepadButton  # value = <GamepadButton.C_NORTH: 3>
C_PS3: _core.GamepadType  # value = <GamepadType.C_PS3: 4>
C_PS4: _core.GamepadType  # value = <GamepadType.C_PS4: 5>
C_PS5: _core.GamepadType  # value = <GamepadType.C_PS5: 6>
C_RSHOULDER: _core.GamepadButton  # value = <GamepadButton.C_RSHOULDER: 10>
C_RSTICK: _core.GamepadButton  # value = <GamepadButton.C_RSTICK: 8>
C_RTRIGGER: _core.GamepadAxis  # value = <GamepadAxis.C_RTRIGGER: 5>
C_RX: _core.GamepadAxis  # value = <GamepadAxis.C_RX: 2>
C_RY: _core.GamepadAxis  # value = <GamepadAxis.C_RY: 3>
C_SOUTH: _core.GamepadButton  # value = <GamepadButton.C_SOUTH: 0>
C_STANDARD: _core.GamepadType  # value = <GamepadType.C_STANDARD: 1>
C_START: _core.GamepadButton  # value = <GamepadButton.C_START: 6>
C_SWITCH_JOYCON_LEFT: _core.GamepadType  # value = <GamepadType.C_SWITCH_JOYCON_LEFT: 8>
C_SWITCH_JOYCON_PAIR: _core.GamepadType  # value = <GamepadType.C_SWITCH_JOYCON_PAIR: 10>
C_SWITCH_JOYCON_RIGHT: _core.GamepadType  # value = <GamepadType.C_SWITCH_JOYCON_RIGHT: 9>
C_SWITCH_PRO: _core.GamepadType  # value = <GamepadType.C_SWITCH_PRO: 7>
C_WEST: _core.GamepadButton  # value = <GamepadButton.C_WEST: 2>
C_XBOX_360: _core.GamepadType  # value = <GamepadType.C_XBOX_360: 2>
C_XBOX_ONE: _core.GamepadType  # value = <GamepadType.C_XBOX_ONE: 3>
DROP_BEGIN: _core.EventType  # value = <EventType.DROP_BEGIN: 4098>
DROP_COMPLETE: _core.EventType  # value = <EventType.DROP_COMPLETE: 4099>
DROP_FILE: _core.EventType  # value = <EventType.DROP_FILE: 4096>
DROP_POSITION: _core.EventType  # value = <EventType.DROP_POSITION: 4100>
DROP_TEXT: _core.EventType  # value = <EventType.DROP_TEXT: 4097>
GAMEPAD_ADDED: _core.EventType  # value = <EventType.GAMEPAD_ADDED: 1619>
GAMEPAD_AXIS_MOTION: _core.EventType  # value = <EventType.GAMEPAD_AXIS_MOTION: 1616>
GAMEPAD_BUTTON_DOWN: _core.EventType  # value = <EventType.GAMEPAD_BUTTON_DOWN: 1617>
GAMEPAD_BUTTON_UP: _core.EventType  # value = <EventType.GAMEPAD_BUTTON_UP: 1618>
GAMEPAD_REMOVED: _core.EventType  # value = <EventType.GAMEPAD_REMOVED: 1620>
GAMEPAD_TOUCHPAD_DOWN: _core.EventType  # value = <EventType.GAMEPAD_TOUCHPAD_DOWN: 1622>
GAMEPAD_TOUCHPAD_MOTION: _core.EventType  # value = <EventType.GAMEPAD_TOUCHPAD_MOTION: 1623>
GAMEPAD_TOUCHPAD_UP: _core.EventType  # value = <EventType.GAMEPAD_TOUCHPAD_UP: 1624>
KEYBOARD_ADDED: _core.EventType  # value = <EventType.KEYBOARD_ADDED: 773>
KEYBOARD_REMOVED: _core.EventType  # value = <EventType.KEYBOARD_REMOVED: 774>
KEY_DOWN: _core.EventType  # value = <EventType.KEY_DOWN: 768>
KEY_UP: _core.EventType  # value = <EventType.KEY_UP: 769>
K_0: _core.Keycode  # value = <Keycode.K_0: 48>
K_1: _core.Keycode  # value = <Keycode.K_1: 49>
K_2: _core.Keycode  # value = <Keycode.K_2: 50>
K_3: _core.Keycode  # value = <Keycode.K_3: 51>
K_4: _core.Keycode  # value = <Keycode.K_4: 52>
K_5: _core.Keycode  # value = <Keycode.K_5: 53>
K_6: _core.Keycode  # value = <Keycode.K_6: 54>
K_7: _core.Keycode  # value = <Keycode.K_7: 55>
K_8: _core.Keycode  # value = <Keycode.K_8: 56>
K_9: _core.Keycode  # value = <Keycode.K_9: 57>
K_AGAIN: _core.Keycode  # value = <Keycode.K_AGAIN: 1073741945>
K_AMPERSAND: _core.Keycode  # value = <Keycode.K_AMPERSAND: 38>
K_ASTERISK: _core.Keycode  # value = <Keycode.K_ASTERISK: 42>
K_AT: _core.Keycode  # value = <Keycode.K_AT: 64>
K_BACKSLASH: _core.Keycode  # value = <Keycode.K_BACKSLASH: 92>
K_BACKSPACE: _core.Keycode  # value = <Keycode.K_BACKSPACE: 8>
K_CAPS: _core.Keycode  # value = <Keycode.K_CAPS: 1073741881>
K_CARET: _core.Keycode  # value = <Keycode.K_CARET: 94>
K_COLON: _core.Keycode  # value = <Keycode.K_COLON: 58>
K_COMMA: _core.Keycode  # value = <Keycode.K_COMMA: 44>
K_COPY: _core.Keycode  # value = <Keycode.K_COPY: 1073741948>
K_CUT: _core.Keycode  # value = <Keycode.K_CUT: 1073741947>
K_DBLQUOTE: _core.Keycode  # value = <Keycode.K_DBLQUOTE: 34>
K_DEL: _core.Keycode  # value = <Keycode.K_DEL: 127>
K_DOLLAR: _core.Keycode  # value = <Keycode.K_DOLLAR: 36>
K_DOWN: _core.Keycode  # value = <Keycode.K_DOWN: 1073741905>
K_END: _core.Keycode  # value = <Keycode.K_END: 1073741901>
K_EQ: _core.Keycode  # value = <Keycode.K_EQ: 61>
K_ESC: _core.Keycode  # value = <Keycode.K_ESC: 27>
K_EXCLAIM: _core.Keycode  # value = <Keycode.K_EXCLAIM: 33>
K_F1: _core.Keycode  # value = <Keycode.K_F1: 1073741882>
K_F10: _core.Keycode  # value = <Keycode.K_F10: 1073741891>
K_F11: _core.Keycode  # value = <Keycode.K_F11: 1073741892>
K_F12: _core.Keycode  # value = <Keycode.K_F12: 1073741893>
K_F2: _core.Keycode  # value = <Keycode.K_F2: 1073741883>
K_F3: _core.Keycode  # value = <Keycode.K_F3: 1073741884>
K_F4: _core.Keycode  # value = <Keycode.K_F4: 1073741885>
K_F5: _core.Keycode  # value = <Keycode.K_F5: 1073741886>
K_F6: _core.Keycode  # value = <Keycode.K_F6: 1073741887>
K_F7: _core.Keycode  # value = <Keycode.K_F7: 1073741888>
K_F8: _core.Keycode  # value = <Keycode.K_F8: 1073741889>
K_F9: _core.Keycode  # value = <Keycode.K_F9: 1073741890>
K_FIND: _core.Keycode  # value = <Keycode.K_FIND: 1073741950>
K_GRAVE: _core.Keycode  # value = <Keycode.K_GRAVE: 96>
K_GT: _core.Keycode  # value = <Keycode.K_GT: 62>
K_HASH: _core.Keycode  # value = <Keycode.K_HASH: 35>
K_HOME: _core.Keycode  # value = <Keycode.K_HOME: 1073741898>
K_INS: _core.Keycode  # value = <Keycode.K_INS: 1073741897>
K_KP_0: _core.Keycode  # value = <Keycode.K_KP_0: 1073741922>
K_KP_1: _core.Keycode  # value = <Keycode.K_KP_1: 1073741913>
K_KP_2: _core.Keycode  # value = <Keycode.K_KP_2: 1073741914>
K_KP_3: _core.Keycode  # value = <Keycode.K_KP_3: 1073741915>
K_KP_4: _core.Keycode  # value = <Keycode.K_KP_4: 1073741916>
K_KP_5: _core.Keycode  # value = <Keycode.K_KP_5: 1073741917>
K_KP_6: _core.Keycode  # value = <Keycode.K_KP_6: 1073741918>
K_KP_7: _core.Keycode  # value = <Keycode.K_KP_7: 1073741919>
K_KP_8: _core.Keycode  # value = <Keycode.K_KP_8: 1073741920>
K_KP_9: _core.Keycode  # value = <Keycode.K_KP_9: 1073741921>
K_KP_DIV: _core.Keycode  # value = <Keycode.K_KP_DIV: 1073741908>
K_KP_ENTER: _core.Keycode  # value = <Keycode.K_KP_ENTER: 1073741912>
K_KP_MINUS: _core.Keycode  # value = <Keycode.K_KP_MINUS: 1073741910>
K_KP_MULT: _core.Keycode  # value = <Keycode.K_KP_MULT: 1073741909>
K_KP_PERIOD: _core.Keycode  # value = <Keycode.K_KP_PERIOD: 1073741923>
K_KP_PLUS: _core.Keycode  # value = <Keycode.K_KP_PLUS: 1073741911>
K_LALT: _core.Keycode  # value = <Keycode.K_LALT: 1073742050>
K_LBRACE: _core.Keycode  # value = <Keycode.K_LBRACE: 123>
K_LBRACKET: _core.Keycode  # value = <Keycode.K_LBRACKET: 91>
K_LCTRL: _core.Keycode  # value = <Keycode.K_LCTRL: 1073742048>
K_LEFT: _core.Keycode  # value = <Keycode.K_LEFT: 1073741904>
K_LGUI: _core.Keycode  # value = <Keycode.K_LGUI: 1073742051>
K_LPAREN: _core.Keycode  # value = <Keycode.K_LPAREN: 40>
K_LSHIFT: _core.Keycode  # value = <Keycode.K_LSHIFT: 1073742049>
K_LT: _core.Keycode  # value = <Keycode.K_LT: 60>
K_MINUS: _core.Keycode  # value = <Keycode.K_MINUS: 45>
K_MUTE: _core.Keycode  # value = <Keycode.K_MUTE: 1073741951>
K_NUMLOCK: _core.Keycode  # value = <Keycode.K_NUMLOCK: 1073741907>
K_PASTE: _core.Keycode  # value = <Keycode.K_PASTE: 1073741949>
K_PAUSE: _core.Keycode  # value = <Keycode.K_PAUSE: 1073741896>
K_PERCENT: _core.Keycode  # value = <Keycode.K_PERCENT: 37>
K_PERIOD: _core.Keycode  # value = <Keycode.K_PERIOD: 46>
K_PGDOWN: _core.Keycode  # value = <Keycode.K_PGDOWN: 1073741902>
K_PGUP: _core.Keycode  # value = <Keycode.K_PGUP: 1073741899>
K_PIPE: _core.Keycode  # value = <Keycode.K_PIPE: 124>
K_PLUS: _core.Keycode  # value = <Keycode.K_PLUS: 43>
K_PRTSCR: _core.Keycode  # value = <Keycode.K_PRTSCR: 1073741894>
K_QUESTION: _core.Keycode  # value = <Keycode.K_QUESTION: 63>
K_RALT: _core.Keycode  # value = <Keycode.K_RALT: 1073742054>
K_RBRACE: _core.Keycode  # value = <Keycode.K_RBRACE: 125>
K_RBRACKET: _core.Keycode  # value = <Keycode.K_RBRACKET: 93>
K_RCTRL: _core.Keycode  # value = <Keycode.K_RCTRL: 1073742052>
K_RETURN: _core.Keycode  # value = <Keycode.K_RETURN: 13>
K_RGUI: _core.Keycode  # value = <Keycode.K_RGUI: 1073742055>
K_RIGHT: _core.Keycode  # value = <Keycode.K_RIGHT: 1073741903>
K_RPAREN: _core.Keycode  # value = <Keycode.K_RPAREN: 41>
K_RSHIFT: _core.Keycode  # value = <Keycode.K_RSHIFT: 1073742053>
K_SCRLK: _core.Keycode  # value = <Keycode.K_SCRLK: 1073741895>
K_SEMICOLON: _core.Keycode  # value = <Keycode.K_SEMICOLON: 59>
K_SGLQUOTE: _core.Keycode  # value = <Keycode.K_SGLQUOTE: 39>
K_SLASH: _core.Keycode  # value = <Keycode.K_SLASH: 47>
K_SPACE: _core.Keycode  # value = <Keycode.K_SPACE: 32>
K_TAB: _core.Keycode  # value = <Keycode.K_TAB: 9>
K_TILDE: _core.Keycode  # value = <Keycode.K_TILDE: 126>
K_UNDERSCORE: _core.Keycode  # value = <Keycode.K_UNDERSCORE: 95>
K_UNDO: _core.Keycode  # value = <Keycode.K_UNDO: 1073741946>
K_UP: _core.Keycode  # value = <Keycode.K_UP: 1073741906>
K_VOLDOWN: _core.Keycode  # value = <Keycode.K_VOLDOWN: 1073741953>
K_VOLUP: _core.Keycode  # value = <Keycode.K_VOLUP: 1073741952>
K_a: _core.Keycode  # value = <Keycode.K_a: 97>
K_b: _core.Keycode  # value = <Keycode.K_b: 98>
K_c: _core.Keycode  # value = <Keycode.K_c: 99>
K_d: _core.Keycode  # value = <Keycode.K_d: 100>
K_e: _core.Keycode  # value = <Keycode.K_e: 101>
K_f: _core.Keycode  # value = <Keycode.K_f: 102>
K_g: _core.Keycode  # value = <Keycode.K_g: 103>
K_h: _core.Keycode  # value = <Keycode.K_h: 104>
K_i: _core.Keycode  # value = <Keycode.K_i: 105>
K_j: _core.Keycode  # value = <Keycode.K_j: 106>
K_k: _core.Keycode  # value = <Keycode.K_k: 107>
K_l: _core.Keycode  # value = <Keycode.K_l: 108>
K_m: _core.Keycode  # value = <Keycode.K_m: 109>
K_n: _core.Keycode  # value = <Keycode.K_n: 110>
K_o: _core.Keycode  # value = <Keycode.K_o: 111>
K_p: _core.Keycode  # value = <Keycode.K_p: 112>
K_q: _core.Keycode  # value = <Keycode.K_q: 113>
K_r: _core.Keycode  # value = <Keycode.K_r: 114>
K_s: _core.Keycode  # value = <Keycode.K_s: 115>
K_t: _core.Keycode  # value = <Keycode.K_t: 116>
K_u: _core.Keycode  # value = <Keycode.K_u: 117>
K_v: _core.Keycode  # value = <Keycode.K_v: 118>
K_w: _core.Keycode  # value = <Keycode.K_w: 119>
K_x: _core.Keycode  # value = <Keycode.K_x: 120>
K_y: _core.Keycode  # value = <Keycode.K_y: 121>
K_z: _core.Keycode  # value = <Keycode.K_z: 122>
MID_LEFT: _core.Anchor  # value = <Anchor.MID_LEFT: 3>
MID_RIGHT: _core.Anchor  # value = <Anchor.MID_RIGHT: 5>
MOUSE_ADDED: _core.EventType  # value = <EventType.MOUSE_ADDED: 1028>
MOUSE_BUTTON_DOWN: _core.EventType  # value = <EventType.MOUSE_BUTTON_DOWN: 1025>
MOUSE_BUTTON_UP: _core.EventType  # value = <EventType.MOUSE_BUTTON_UP: 1026>
MOUSE_MOTION: _core.EventType  # value = <EventType.MOUSE_MOTION: 1024>
MOUSE_REMOVED: _core.EventType  # value = <EventType.MOUSE_REMOVED: 1029>
MOUSE_WHEEL: _core.EventType  # value = <EventType.MOUSE_WHEEL: 1027>
M_LEFT: _core.MouseButton  # value = <MouseButton.M_LEFT: 1>
M_MIDDLE: _core.MouseButton  # value = <MouseButton.M_MIDDLE: 2>
M_RIGHT: _core.MouseButton  # value = <MouseButton.M_RIGHT: 3>
M_SIDE1: _core.MouseButton  # value = <MouseButton.M_SIDE1: 4>
M_SIDE2: _core.MouseButton  # value = <MouseButton.M_SIDE2: 5>
PEN_AXIS: _core.EventType  # value = <EventType.PEN_AXIS: 4871>
PEN_BUTTON_DOWN: _core.EventType  # value = <EventType.PEN_BUTTON_DOWN: 4868>
PEN_BUTTON_UP: _core.EventType  # value = <EventType.PEN_BUTTON_UP: 4869>
PEN_DOWN: _core.EventType  # value = <EventType.PEN_DOWN: 4866>
PEN_MOTION: _core.EventType  # value = <EventType.PEN_MOTION: 4870>
PEN_PROXIMITY_IN: _core.EventType  # value = <EventType.PEN_PROXIMITY_IN: 4864>
PEN_PROXIMITY_OUT: _core.EventType  # value = <EventType.PEN_PROXIMITY_OUT: 4865>
PEN_UP: _core.EventType  # value = <EventType.PEN_UP: 4867>
P_DISTANCE: _core.PenAxis  # value = <PenAxis.P_DISTANCE: 3>
P_PRESSURE: _core.PenAxis  # value = <PenAxis.P_PRESSURE: 0>
P_ROTATION: _core.PenAxis  # value = <PenAxis.P_ROTATION: 4>
P_SLIDER: _core.PenAxis  # value = <PenAxis.P_SLIDER: 5>
P_TANGENTIAL_PRESSURE: _core.PenAxis  # value = <PenAxis.P_TANGENTIAL_PRESSURE: 6>
P_TILT_X: _core.PenAxis  # value = <PenAxis.P_TILT_X: 1>
P_TILT_Y: _core.PenAxis  # value = <PenAxis.P_TILT_Y: 2>
QUIT: _core.EventType  # value = <EventType.QUIT: 256>
S_0: _core.Scancode  # value = <Scancode.S_0: 39>
S_1: _core.Scancode  # value = <Scancode.S_1: 30>
S_2: _core.Scancode  # value = <Scancode.S_2: 31>
S_3: _core.Scancode  # value = <Scancode.S_3: 32>
S_4: _core.Scancode  # value = <Scancode.S_4: 33>
S_5: _core.Scancode  # value = <Scancode.S_5: 34>
S_6: _core.Scancode  # value = <Scancode.S_6: 35>
S_7: _core.Scancode  # value = <Scancode.S_7: 36>
S_8: _core.Scancode  # value = <Scancode.S_8: 37>
S_9: _core.Scancode  # value = <Scancode.S_9: 38>
S_AGAIN: _core.Scancode  # value = <Scancode.S_AGAIN: 121>
S_APOSTROPHE: _core.Scancode  # value = <Scancode.S_APOSTROPHE: 52>
S_BACKSLASH: _core.Scancode  # value = <Scancode.S_BACKSLASH: 49>
S_BACKSPACE: _core.Scancode  # value = <Scancode.S_BACKSPACE: 42>
S_CAPS: _core.Scancode  # value = <Scancode.S_CAPS: 57>
S_COMMA: _core.Scancode  # value = <Scancode.S_COMMA: 54>
S_COPY: _core.Scancode  # value = <Scancode.S_COPY: 124>
S_CUT: _core.Scancode  # value = <Scancode.S_CUT: 123>
S_DEL: _core.Scancode  # value = <Scancode.S_DEL: 76>
S_DOWN: _core.Scancode  # value = <Scancode.S_DOWN: 81>
S_END: _core.Scancode  # value = <Scancode.S_END: 77>
S_EQ: _core.Scancode  # value = <Scancode.S_EQ: 46>
S_ESC: _core.Scancode  # value = <Scancode.S_ESC: 41>
S_F1: _core.Scancode  # value = <Scancode.S_F1: 58>
S_F10: _core.Scancode  # value = <Scancode.S_F10: 67>
S_F11: _core.Scancode  # value = <Scancode.S_F11: 68>
S_F12: _core.Scancode  # value = <Scancode.S_F12: 69>
S_F2: _core.Scancode  # value = <Scancode.S_F2: 59>
S_F3: _core.Scancode  # value = <Scancode.S_F3: 60>
S_F4: _core.Scancode  # value = <Scancode.S_F4: 61>
S_F5: _core.Scancode  # value = <Scancode.S_F5: 62>
S_F6: _core.Scancode  # value = <Scancode.S_F6: 63>
S_F7: _core.Scancode  # value = <Scancode.S_F7: 64>
S_F8: _core.Scancode  # value = <Scancode.S_F8: 65>
S_F9: _core.Scancode  # value = <Scancode.S_F9: 66>
S_FIND: _core.Scancode  # value = <Scancode.S_FIND: 126>
S_GRAVE: _core.Scancode  # value = <Scancode.S_GRAVE: 53>
S_HOME: _core.Scancode  # value = <Scancode.S_HOME: 74>
S_INS: _core.Scancode  # value = <Scancode.S_INS: 73>
S_KP_0: _core.Scancode  # value = <Scancode.S_KP_0: 98>
S_KP_1: _core.Scancode  # value = <Scancode.S_KP_1: 89>
S_KP_2: _core.Scancode  # value = <Scancode.S_KP_2: 90>
S_KP_3: _core.Scancode  # value = <Scancode.S_KP_3: 91>
S_KP_4: _core.Scancode  # value = <Scancode.S_KP_4: 92>
S_KP_5: _core.Scancode  # value = <Scancode.S_KP_5: 93>
S_KP_6: _core.Scancode  # value = <Scancode.S_KP_6: 94>
S_KP_7: _core.Scancode  # value = <Scancode.S_KP_7: 95>
S_KP_8: _core.Scancode  # value = <Scancode.S_KP_8: 96>
S_KP_9: _core.Scancode  # value = <Scancode.S_KP_9: 97>
S_KP_DIV: _core.Scancode  # value = <Scancode.S_KP_DIV: 84>
S_KP_ENTER: _core.Scancode  # value = <Scancode.S_KP_ENTER: 88>
S_KP_MINUS: _core.Scancode  # value = <Scancode.S_KP_MINUS: 86>
S_KP_MULT: _core.Scancode  # value = <Scancode.S_KP_MULT: 85>
S_KP_PERIOD: _core.Scancode  # value = <Scancode.S_KP_PERIOD: 99>
S_KP_PLUS: _core.Scancode  # value = <Scancode.S_KP_PLUS: 87>
S_LALT: _core.Scancode  # value = <Scancode.S_LALT: 226>
S_LBRACKET: _core.Scancode  # value = <Scancode.S_LBRACKET: 47>
S_LCTRL: _core.Scancode  # value = <Scancode.S_LCTRL: 224>
S_LEFT: _core.Scancode  # value = <Scancode.S_LEFT: 80>
S_LGUI: _core.Scancode  # value = <Scancode.S_LGUI: 227>
S_LSHIFT: _core.Scancode  # value = <Scancode.S_LSHIFT: 225>
S_MINUS: _core.Scancode  # value = <Scancode.S_MINUS: 45>
S_MUTE: _core.Scancode  # value = <Scancode.S_MUTE: 127>
S_NUMLOCK: _core.Scancode  # value = <Scancode.S_NUMLOCK: 83>
S_PASTE: _core.Scancode  # value = <Scancode.S_PASTE: 125>
S_PAUSE: _core.Scancode  # value = <Scancode.S_PAUSE: 72>
S_PERIOD: _core.Scancode  # value = <Scancode.S_PERIOD: 55>
S_PGDOWN: _core.Scancode  # value = <Scancode.S_PGDOWN: 78>
S_PGUP: _core.Scancode  # value = <Scancode.S_PGUP: 75>
S_PRTSCR: _core.Scancode  # value = <Scancode.S_PRTSCR: 70>
S_RALT: _core.Scancode  # value = <Scancode.S_RALT: 230>
S_RBRACKET: _core.Scancode  # value = <Scancode.S_RBRACKET: 48>
S_RCTRL: _core.Scancode  # value = <Scancode.S_RCTRL: 228>
S_RETURN: _core.Scancode  # value = <Scancode.S_RETURN: 40>
S_RGUI: _core.Scancode  # value = <Scancode.S_RGUI: 231>
S_RIGHT: _core.Scancode  # value = <Scancode.S_RIGHT: 79>
S_RSHIFT: _core.Scancode  # value = <Scancode.S_RSHIFT: 229>
S_SCRLK: _core.Scancode  # value = <Scancode.S_SCRLK: 71>
S_SEMICOLON: _core.Scancode  # value = <Scancode.S_SEMICOLON: 51>
S_SLASH: _core.Scancode  # value = <Scancode.S_SLASH: 56>
S_SPACE: _core.Scancode  # value = <Scancode.S_SPACE: 44>
S_TAB: _core.Scancode  # value = <Scancode.S_TAB: 43>
S_UNDO: _core.Scancode  # value = <Scancode.S_UNDO: 122>
S_UP: _core.Scancode  # value = <Scancode.S_UP: 82>
S_VOLDOWN: _core.Scancode  # value = <Scancode.S_VOLDOWN: 129>
S_VOLUP: _core.Scancode  # value = <Scancode.S_VOLUP: 128>
S_a: _core.Scancode  # value = <Scancode.S_a: 4>
S_b: _core.Scancode  # value = <Scancode.S_b: 5>
S_c: _core.Scancode  # value = <Scancode.S_c: 6>
S_d: _core.Scancode  # value = <Scancode.S_d: 7>
S_e: _core.Scancode  # value = <Scancode.S_e: 8>
S_f: _core.Scancode  # value = <Scancode.S_f: 9>
S_g: _core.Scancode  # value = <Scancode.S_g: 10>
S_h: _core.Scancode  # value = <Scancode.S_h: 11>
S_i: _core.Scancode  # value = <Scancode.S_i: 12>
S_j: _core.Scancode  # value = <Scancode.S_j: 13>
S_k: _core.Scancode  # value = <Scancode.S_k: 14>
S_l: _core.Scancode  # value = <Scancode.S_l: 15>
S_m: _core.Scancode  # value = <Scancode.S_m: 16>
S_n: _core.Scancode  # value = <Scancode.S_n: 17>
S_o: _core.Scancode  # value = <Scancode.S_o: 18>
S_p: _core.Scancode  # value = <Scancode.S_p: 19>
S_q: _core.Scancode  # value = <Scancode.S_q: 20>
S_r: _core.Scancode  # value = <Scancode.S_r: 21>
S_s: _core.Scancode  # value = <Scancode.S_s: 22>
S_t: _core.Scancode  # value = <Scancode.S_t: 23>
S_u: _core.Scancode  # value = <Scancode.S_u: 24>
S_v: _core.Scancode  # value = <Scancode.S_v: 25>
S_w: _core.Scancode  # value = <Scancode.S_w: 26>
S_x: _core.Scancode  # value = <Scancode.S_x: 27>
S_y: _core.Scancode  # value = <Scancode.S_y: 28>
S_z: _core.Scancode  # value = <Scancode.S_z: 29>
TEXT_EDITING: _core.EventType  # value = <EventType.TEXT_EDITING: 770>
TEXT_INPUT: _core.EventType  # value = <EventType.TEXT_INPUT: 771>
TOP_LEFT: _core.Anchor  # value = <Anchor.TOP_LEFT: 0>
TOP_MID: _core.Anchor  # value = <Anchor.TOP_MID: 1>
TOP_RIGHT: _core.Anchor  # value = <Anchor.TOP_RIGHT: 2>
WINDOW_ENTER_FULLSCREEN: _core.EventType  # value = <EventType.WINDOW_ENTER_FULLSCREEN: 535>
WINDOW_EXPOSED: _core.EventType  # value = <EventType.WINDOW_EXPOSED: 516>
WINDOW_FOCUS_GAINED: _core.EventType  # value = <EventType.WINDOW_FOCUS_GAINED: 526>
WINDOW_FOCUS_LOST: _core.EventType  # value = <EventType.WINDOW_FOCUS_LOST: 527>
WINDOW_HIDDEN: _core.EventType  # value = <EventType.WINDOW_HIDDEN: 515>
WINDOW_LEAVE_FULLSCREEN: _core.EventType  # value = <EventType.WINDOW_LEAVE_FULLSCREEN: 536>
WINDOW_MAXIMIZED: _core.EventType  # value = <EventType.WINDOW_MAXIMIZED: 522>
WINDOW_MINIMIZED: _core.EventType  # value = <EventType.WINDOW_MINIMIZED: 521>
WINDOW_MOUSE_ENTER: _core.EventType  # value = <EventType.WINDOW_MOUSE_ENTER: 524>
WINDOW_MOUSE_LEAVE: _core.EventType  # value = <EventType.WINDOW_MOUSE_LEAVE: 525>
WINDOW_MOVED: _core.EventType  # value = <EventType.WINDOW_MOVED: 517>
WINDOW_OCCLUDED: _core.EventType  # value = <EventType.WINDOW_OCCLUDED: 534>
WINDOW_RESIZED: _core.EventType  # value = <EventType.WINDOW_RESIZED: 518>
WINDOW_RESTORED: _core.EventType  # value = <EventType.WINDOW_RESTORED: 523>
WINDOW_SHOWN: _core.EventType  # value = <EventType.WINDOW_SHOWN: 514>
