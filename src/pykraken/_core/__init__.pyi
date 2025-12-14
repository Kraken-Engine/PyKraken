from __future__ import annotations
import collections.abc
import enum
import typing
from . import collision
from . import color
from . import draw
from . import ease
from . import event
from . import gamepad
from . import input
from . import key
from . import line
from . import log
from . import math
from . import mouse
from . import rect
from . import renderer
from . import time
from . import transform
from . import viewport
from . import window
__all__: list[str] = ['AUDIO_DEVICE_ADDED', 'AUDIO_DEVICE_FORMAT_CHANGED', 'AUDIO_DEVICE_REMOVED', 'Align', 'Anchor', 'Animation', 'AnimationController', 'Audio', 'AudioStream', 'CAMERA_DEVICE_ADDED', 'CAMERA_DEVICE_APPROVED', 'CAMERA_DEVICE_DENIED', 'CAMERA_DEVICE_REMOVED', 'CLIPBOARD_UPDATE', 'C_BACK', 'C_DPAD_DOWN', 'C_DPAD_LEFT', 'C_DPAD_RIGHT', 'C_DPAD_UP', 'C_EAST', 'C_GUIDE', 'C_LSHOULDER', 'C_LSTICK', 'C_LTRIGGER', 'C_LX', 'C_LY', 'C_NORTH', 'C_PS3', 'C_PS4', 'C_PS5', 'C_RSHOULDER', 'C_RSTICK', 'C_RTRIGGER', 'C_RX', 'C_RY', 'C_SOUTH', 'C_STANDARD', 'C_START', 'C_SWITCH_JOYCON_LEFT', 'C_SWITCH_JOYCON_PAIR', 'C_SWITCH_JOYCON_RIGHT', 'C_SWITCH_PRO', 'C_WEST', 'C_XBOX_360', 'C_XBOX_ONE', 'Camera', 'Circle', 'Color', 'DID_ENTER_BACKGROUND', 'DID_ENTER_FOREGROUND', 'DISPLAY_ADDED', 'DISPLAY_CONTENT_SCALE_CHANGED', 'DISPLAY_CURRENT_MODE_CHANGED', 'DISPLAY_DESKTOP_MODE_CHANGED', 'DISPLAY_MOVED', 'DISPLAY_ORIENTATION', 'DISPLAY_REMOVED', 'DROP_BEGIN', 'DROP_COMPLETE', 'DROP_FILE', 'DROP_POSITION', 'DROP_TEXT', 'EasingAnimation', 'Event', 'EventType', 'FINGER_CANCELED', 'FINGER_DOWN', 'FINGER_MOTION', 'FINGER_UP', 'Font', 'FontHint', 'Frame', 'GAMEPAD_ADDED', 'GAMEPAD_AXIS_MOTION', 'GAMEPAD_BUTTON_DOWN', 'GAMEPAD_BUTTON_UP', 'GAMEPAD_REMAPPED', 'GAMEPAD_REMOVED', 'GAMEPAD_SENSOR_UPDATE', 'GAMEPAD_STEAM_HANDLE_UPDATED', 'GAMEPAD_TOUCHPAD_DOWN', 'GAMEPAD_TOUCHPAD_MOTION', 'GAMEPAD_TOUCHPAD_UP', 'GAMEPAD_UPDATE_COMPLETE', 'GamepadAxis', 'GamepadButton', 'GamepadType', 'InputAction', 'KEYBOARD_ADDED', 'KEYBOARD_REMOVED', 'KEYMAP_CHANGED', 'KEY_DOWN', 'KEY_UP', 'K_0', 'K_1', 'K_2', 'K_3', 'K_4', 'K_5', 'K_6', 'K_7', 'K_8', 'K_9', 'K_AGAIN', 'K_AMPERSAND', 'K_APPLICATION', 'K_ASTERISK', 'K_AT', 'K_BACKSLASH', 'K_BACKSPACE', 'K_CALL', 'K_CAPS', 'K_CARET', 'K_CHANNEL_DEC', 'K_CHANNEL_INC', 'K_COLON', 'K_COMMA', 'K_COPY', 'K_CUT', 'K_DBLQUOTE', 'K_DEL', 'K_DOLLAR', 'K_DOWN', 'K_END', 'K_ENDCALL', 'K_EQ', 'K_ESC', 'K_EXCLAIM', 'K_EXECUTE', 'K_F1', 'K_F10', 'K_F11', 'K_F12', 'K_F13', 'K_F14', 'K_F15', 'K_F2', 'K_F3', 'K_F4', 'K_F5', 'K_F6', 'K_F7', 'K_F8', 'K_F9', 'K_FIND', 'K_GRAVE', 'K_GT', 'K_HASH', 'K_HELP', 'K_HOME', 'K_INS', 'K_KP_0', 'K_KP_1', 'K_KP_2', 'K_KP_3', 'K_KP_4', 'K_KP_5', 'K_KP_6', 'K_KP_7', 'K_KP_8', 'K_KP_9', 'K_KP_DIV', 'K_KP_ENTER', 'K_KP_MINUS', 'K_KP_MULT', 'K_KP_PERIOD', 'K_KP_PLUS', 'K_LALT', 'K_LBRACE', 'K_LBRACKET', 'K_LCTRL', 'K_LEFT', 'K_LGUI', 'K_LPAREN', 'K_LSHIFT', 'K_LT', 'K_MEDIA_EJECT', 'K_MEDIA_FF', 'K_MEDIA_NEXT', 'K_MEDIA_PAUSE', 'K_MEDIA_PLAY', 'K_MEDIA_PLAY_PAUSE', 'K_MEDIA_PREV', 'K_MEDIA_REC', 'K_MEDIA_REWIND', 'K_MEDIA_SELECT', 'K_MEDIA_STOP', 'K_MENU', 'K_MINUS', 'K_MODE', 'K_MUTE', 'K_NUMLOCK', 'K_PASTE', 'K_PAUSE', 'K_PERCENT', 'K_PERIOD', 'K_PGDOWN', 'K_PGUP', 'K_PIPE', 'K_PLUS', 'K_POWER', 'K_PRTSCR', 'K_QUESTION', 'K_RALT', 'K_RBRACE', 'K_RBRACKET', 'K_RCTRL', 'K_RETURN', 'K_RGUI', 'K_RIGHT', 'K_RPAREN', 'K_RSHIFT', 'K_SCRLK', 'K_SELECT', 'K_SEMICOLON', 'K_SGLQUOTE', 'K_SLASH', 'K_SLEEP', 'K_SOFTLEFT', 'K_SOFTRIGHT', 'K_SPACE', 'K_STOP', 'K_TAB', 'K_TILDE', 'K_UNDERSCORE', 'K_UNDO', 'K_UNKNOWN', 'K_UP', 'K_VOLDOWN', 'K_VOLUP', 'K_WAKE', 'K_a', 'K_b', 'K_c', 'K_d', 'K_e', 'K_f', 'K_g', 'K_h', 'K_i', 'K_j', 'K_k', 'K_l', 'K_m', 'K_n', 'K_o', 'K_p', 'K_q', 'K_r', 'K_s', 'K_t', 'K_u', 'K_v', 'K_w', 'K_x', 'K_y', 'K_z', 'Keycode', 'LOCALE_CHANGED', 'LOW_MEMORY', 'Line', 'MOUSE_ADDED', 'MOUSE_BUTTON_DOWN', 'MOUSE_BUTTON_UP', 'MOUSE_MOTION', 'MOUSE_REMOVED', 'MOUSE_WHEEL', 'M_LEFT', 'M_MIDDLE', 'M_RIGHT', 'M_SIDE1', 'M_SIDE2', 'Mask', 'MouseButton', 'PEN_AXIS', 'PEN_BUTTON_DOWN', 'PEN_BUTTON_UP', 'PEN_DOWN', 'PEN_MOTION', 'PEN_PROXIMITY_IN', 'PEN_PROXIMITY_OUT', 'PEN_UP', 'P_DISTANCE', 'P_PRESSURE', 'P_ROTATION', 'P_SLIDER', 'P_TANGENTIAL_PRESSURE', 'P_TILT_X', 'P_TILT_Y', 'PenAxis', 'PixelArray', 'PolarCoordinate', 'Polygon', 'QUIT', 'RENDER_DEVICE_LOST', 'RENDER_DEVICE_RESET', 'RENDER_TARGETS_RESET', 'Rect', 'SENSOR_UPDATE', 'SYSTEM_THEME_CHANGED', 'S_0', 'S_1', 'S_2', 'S_3', 'S_4', 'S_5', 'S_6', 'S_7', 'S_8', 'S_9', 'S_AGAIN', 'S_APOSTROPHE', 'S_APPLICATION', 'S_BACKSLASH', 'S_BACKSPACE', 'S_CALL', 'S_CAPS', 'S_CHANNEL_DEC', 'S_CHANNEL_INC', 'S_COMMA', 'S_COPY', 'S_CUT', 'S_DEL', 'S_DOWN', 'S_END', 'S_ENDCALL', 'S_EQ', 'S_ESC', 'S_EXECUTE', 'S_F1', 'S_F10', 'S_F11', 'S_F12', 'S_F13', 'S_F14', 'S_F15', 'S_F2', 'S_F3', 'S_F4', 'S_F5', 'S_F6', 'S_F7', 'S_F8', 'S_F9', 'S_FIND', 'S_GRAVE', 'S_HELP', 'S_HOME', 'S_INS', 'S_KP_0', 'S_KP_1', 'S_KP_2', 'S_KP_3', 'S_KP_4', 'S_KP_5', 'S_KP_6', 'S_KP_7', 'S_KP_8', 'S_KP_9', 'S_KP_DIV', 'S_KP_ENTER', 'S_KP_MINUS', 'S_KP_MULT', 'S_KP_PERIOD', 'S_KP_PLUS', 'S_LALT', 'S_LBRACKET', 'S_LCTRL', 'S_LEFT', 'S_LGUI', 'S_LSHIFT', 'S_MEDIA_EJECT', 'S_MEDIA_FAST_FORWARD', 'S_MEDIA_NEXT', 'S_MEDIA_PAUSE', 'S_MEDIA_PLAY', 'S_MEDIA_PLAY_PAUSE', 'S_MEDIA_PREV', 'S_MEDIA_REC', 'S_MEDIA_REWIND', 'S_MEDIA_SELECT', 'S_MEDIA_STOP', 'S_MENU', 'S_MINUS', 'S_MODE', 'S_MUTE', 'S_NUMLOCK', 'S_PASTE', 'S_PAUSE', 'S_PERIOD', 'S_PGDOWN', 'S_PGUP', 'S_POWER', 'S_PRTSCR', 'S_RALT', 'S_RBRACKET', 'S_RCTRL', 'S_RETURN', 'S_RGUI', 'S_RIGHT', 'S_RSHIFT', 'S_SCRLK', 'S_SELECT', 'S_SEMICOLON', 'S_SLASH', 'S_SLEEP', 'S_SOFTLEFT', 'S_SOFTRIGHT', 'S_SPACE', 'S_STOP', 'S_TAB', 'S_UNDO', 'S_UP', 'S_VOLDOWN', 'S_VOLUP', 'S_WAKE', 'S_a', 'S_b', 'S_c', 'S_d', 'S_e', 'S_f', 'S_g', 'S_h', 'S_i', 'S_j', 'S_k', 'S_l', 'S_m', 'S_n', 'S_o', 'S_p', 'S_q', 'S_r', 'S_s', 'S_t', 'S_u', 'S_v', 'S_w', 'S_x', 'S_y', 'S_z', 'Scancode', 'ScrollMode', 'ShaderState', 'SheetStrip', 'TERMINATING', 'TEXT_EDITING', 'TEXT_EDITING_CANDIDATES', 'TEXT_INPUT', 'Text', 'Texture', 'Tile', 'TileLayer', 'TileMap', 'Timer', 'Vec2', 'ViewportMode', 'WILL_ENTER_BACKGROUND', 'WILL_ENTER_FOREGROUND', 'WINDOW_CLOSE_REQUESTED', 'WINDOW_DESTROYED', 'WINDOW_DISPLAY_CHANGED', 'WINDOW_DISPLAY_SCALE_CHANGED', 'WINDOW_ENTER_FULLSCREEN', 'WINDOW_EXPOSED', 'WINDOW_FOCUS_GAINED', 'WINDOW_FOCUS_LOST', 'WINDOW_HDR_STATE_CHANGED', 'WINDOW_HIDDEN', 'WINDOW_HIT_TEST', 'WINDOW_ICCPROF_CHANGED', 'WINDOW_LEAVE_FULLSCREEN', 'WINDOW_MAXIMIZED', 'WINDOW_MINIMIZED', 'WINDOW_MOUSE_ENTER', 'WINDOW_MOUSE_LEAVE', 'WINDOW_MOVED', 'WINDOW_OCCLUDED', 'WINDOW_RESIZED', 'WINDOW_RESTORED', 'WINDOW_SAFE_AREA_CHANGED', 'WINDOW_SHOWN', 'collision', 'color', 'draw', 'ease', 'event', 'gamepad', 'init', 'input', 'key', 'line', 'log', 'math', 'mouse', 'quit', 'rect', 'renderer', 'time', 'transform', 'viewport', 'window']
class Align(enum.IntEnum):
    CENTER: typing.ClassVar[Align]  # value = <Align.CENTER: 1>
    LEFT: typing.ClassVar[Align]  # value = <Align.LEFT: 0>
    RIGHT: typing.ClassVar[Align]  # value = <Align.RIGHT: 2>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Anchor(enum.IntEnum):
    BOTTOM_LEFT: typing.ClassVar[Anchor]  # value = <Anchor.BOTTOM_LEFT: 6>
    BOTTOM_MID: typing.ClassVar[Anchor]  # value = <Anchor.BOTTOM_MID: 7>
    BOTTOM_RIGHT: typing.ClassVar[Anchor]  # value = <Anchor.BOTTOM_RIGHT: 8>
    CENTER: typing.ClassVar[Anchor]  # value = <Anchor.CENTER: 4>
    MID_LEFT: typing.ClassVar[Anchor]  # value = <Anchor.MID_LEFT: 3>
    MID_RIGHT: typing.ClassVar[Anchor]  # value = <Anchor.MID_RIGHT: 5>
    TOP_LEFT: typing.ClassVar[Anchor]  # value = <Anchor.TOP_LEFT: 0>
    TOP_MID: typing.ClassVar[Anchor]  # value = <Anchor.TOP_MID: 1>
    TOP_RIGHT: typing.ClassVar[Anchor]  # value = <Anchor.TOP_RIGHT: 2>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Animation:
    """
    
    A complete animation sequence with frames and playback settings.
    
    Contains a sequence of frames and the frames per second (FPS) rate for playback timing.
            
    """
    @property
    def fps(self) -> int:
        """
        The frames per second rate for animation playback.
        """
    @property
    def frames(self) -> list[Frame]:
        """
        The list of frames in the animation sequence.
        """
class AnimationController:
    """
    
    Manages and controls sprite animations with multiple animation sequences.
    
    The AnimationController handles loading animations from sprite sheets or image folders,
    managing playback state, and providing frame-by-frame animation control.
        
    """
    def __init__(self) -> None:
        ...
    def is_finished(self) -> bool:
        """
        Check if the animation completed a full loop during the last update.
        
        Returns True if the animation looped back to the beginning during the most recent
        frame update. This method is const and can be called multiple times per frame
        with consistent results.
        
        Returns:
            bool: True if the animation completed a loop during the last update.
        """
    def load_sprite_sheet(self, file_path: str, frame_size: Vec2, strips: collections.abc.Sequence[SheetStrip]) -> None:
        """
        Load one or more animations from a sprite sheet texture.
        
        Divides the sprite sheet into horizontal strips, where each strip represents a different
        animation. Each strip is divided into equal-sized frames based on the specified frame size.
        Frames are read left-to-right within each strip, and strips are read top-to-bottom.
        
        Args:
            file_path (str): Path to the sprite sheet image file.
            frame_size (Vec2): Size of each frame as (width, height).
            strips (list[SheetStrip]): List of strip definitions, each containing:
                - name (str): Unique identifier for the animation
                - frame_count (int): Number of frames in this strip/animation
                - fps (int): Frames per second for playback timing
        
        Raises:
            ValueError: If frame size is not positive, no strips provided, frame count is not
                       positive, or frame count exceeds sprite sheet width.
            RuntimeError: If sprite sheet dimensions are not divisible by frame dimensions,
                         duplicate animation names exist, or strip index exceeds sprite sheet height.
        """
    def pause(self) -> None:
        """
        Pause the animation playback.
        
        Stops animation frame advancement while preserving the current frame position.
        """
    def play(self, name: str) -> None:
        """
        Play an animation from the beginning.
        
        Switches to the specified animation, rewinds it to frame 0, and starts playback.
        
        Args:
            name (str): The name of the animation to play.
        
        Raises:
            ValueError: If the specified animation name is not found.
        """
    def play_from(self, frame_index: typing.SupportsInt) -> None:
        """
        Start playing the current animation from a specific frame.
        
        Sets the animation to the specified frame index and resumes playback. Useful for
        starting animations mid-sequence or implementing custom animation logic.
        
        Args:
            frame_index (int): The frame index to start from (0-based).
        
        Raises:
            IndexError: If the frame index is out of range for the current animation.
        """
    def resume(self) -> None:
        """
        Resume paused animation playback.
        
        Resumes animation frame advancement if the playback speed is greater than 0.
        Does nothing if the animation is already playing or playback speed is 0.
        """
    def rewind(self) -> None:
        """
        Reset the animation to the beginning.
        
        Sets the animation back to frame 0 and resets loop detection state.
        """
    def set(self, name: str) -> None:
        """
        Set the current active animation by name without affecting playback state.
        
        Switches to the specified animation while preserving the current frame index and 
        playback state (paused/playing). Useful for seamless animation transitions.
        
        Args:
            name (str): The name of the animation to activate.
        
        Raises:
            ValueError: If the specified animation name is not found.
        """
    @property
    def current_animation_name(self) -> str:
        """
        The name of the currently active animation.
        
        Returns:
            str: The name of the current animation, or empty string if none is set.
        """
    @property
    def current_frame(self) -> Frame:
        """
        The current animation frame being displayed.
        
        Returns:
            Frame: The current frame with texture and rectangle data.
        
        Raises:
            RuntimeError: If no animation is currently set or the animation has no frames.
        """
    @property
    def frame_index(self) -> int:
        """
        The current frame index in the animation sequence.
        
        Returns the integer frame index (0-based) of the currently displayed frame.
        
        Returns:
            int: The current frame index.
        """
    @property
    def looping(self) -> bool:
        """
        Whether the animation should loop when it reaches the end.
        
        Returns:
            bool: True if the animation is set to loop, False otherwise.
        """
    @looping.setter
    def looping(self, arg1: bool) -> None:
        ...
    @property
    def playback_speed(self) -> float:
        """
        The playback speed multiplier for animation timing.
        
        A value of 1.0 represents normal speed, 2.0 is double speed, 0.5 is half speed.
        Setting to 0 will pause the animation.
        
        Returns:
            float: The current playback speed multiplier.
        """
    @playback_speed.setter
    def playback_speed(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def progress(self) -> float:
        """
        The normalized progress through the current animation.
        
        Returns a value between 0.0 (start) and 1.0 (end) representing how far through
        the animation sequence the playback has progressed. Useful for UI progress bars
        or triggering events at specific points in the animation.
        
        Returns:
            float: The animation progress as a value between 0.0 and 1.0.
        """
class Audio:
    """
    
    A decoded audio object that supports multiple simultaneous playbacks.
    
    Audio objects decode the entire file into memory for low-latency playback. They support
    multiple concurrent playbacks of the same sound. Use this for short sound effects that may need to overlap.
        
    """
    def __init__(self, file_path: str, volume: typing.SupportsFloat = 1.0) -> None:
        """
        Create an Audio object from a file path with optional volume.
        
        Args:
            file_path (str): Path to the audio file to load.
            volume (float, optional): Initial volume level (0.0 to 1.0+). Defaults to 1.0.
        
        Raises:
            RuntimeError: If the audio file cannot be loaded or decoded.
        """
    def play(self, fade_in_ms: typing.SupportsInt = 0, loop: bool = False) -> None:
        """
        Play the audio with optional fade-in time and loop setting.
        
        Creates a new voice for playback, allowing multiple simultaneous plays of the same audio.
        Each play instance is independent and can have different fade and loop settings.
        
        Args:
            fade_in_ms (int, optional): Fade-in duration in milliseconds. Defaults to 0.
            loop (bool, optional): Whether to loop the audio continuously. Defaults to False.
        
        Raises:
            RuntimeError: If audio playback initialization fails.
        """
    def stop(self, fade_out_ms: typing.SupportsInt = 0) -> None:
        """
        Stop all active playbacks of this audio.
        
        Stops all currently playing voices associated with this Audio object. If a fade-out
        time is specified, all voices will fade out over that duration before stopping.
        
        Args:
            fade_out_ms (int, optional): Fade-out duration in milliseconds. Defaults to 0.
        """
    @property
    def volume(self) -> float:
        """
        The volume level for new and existing playbacks.
        
        Setting this property affects all currently playing voices and sets the default
        volume for future playbacks. Volume can exceed 1.0 for amplification.
        
        Type:
            float: Volume level (0.0 = silent, 1.0 = original volume, >1.0 = amplified).
        """
    @volume.setter
    def volume(self, arg1: typing.SupportsFloat) -> None:
        ...
class AudioStream:
    """
    
    A streaming audio object for single-instance playback of large audio files.
    
    AudioStream objects stream audio data from disk during playback, using minimal memory.
    They support only one playback instance at a time, making them ideal for background
    music, long audio tracks, or when memory usage is a concern.
        
    """
    def __init__(self, file_path: str, volume: typing.SupportsFloat = 1.0) -> None:
        """
        Create an AudioStream object from a file path with optional volume.
        
        Args:
            file_path (str): Path to the audio file to stream.
            volume (float, optional): Initial volume level (0.0 to 1.0+). Defaults to 1.0.
        
        Raises:
            RuntimeError: If the audio file cannot be opened for streaming.
        """
    def pause(self) -> None:
        """
        Pause the audio stream playback.
        
        The stream position is preserved and can be resumed with resume().
        """
    def play(self, fade_in_ms: typing.SupportsInt = 0, loop: bool = False, start_time_seconds: typing.SupportsFloat = 0.0) -> None:
        """
        Play the audio stream with optional fade-in time, loop setting, and start position.
        
        Starts playback from the specified time position. If the stream is already
        playing, it will restart from the specified position.
        
        Args:
            fade_in_ms (int, optional): Fade-in duration in milliseconds. Defaults to 0.
            loop (bool, optional): Whether to loop the audio continuously. Defaults to False.
            start_time_seconds (float, optional): Time position in seconds to start playback from. Defaults to 0.0.
        """
    def resume(self) -> None:
        """
        Resume paused audio stream playback.
        
        Continues playback from the current stream position.
        """
    def rewind(self) -> None:
        """
        Rewind the audio stream to the beginning.
        
        Sets the playback position back to the start of the audio file. Does not affect
        the current play state (playing/paused).
        """
    def seek(self, time_seconds: typing.SupportsFloat) -> None:
        """
        Seek to a specific time position in the audio stream.
        
        Sets the playback position to the specified time in seconds. Does not affect
        the current play state (playing/paused).
        
        Args:
            time_seconds (float): The time position in seconds to seek to.
        """
    def set_looping(self, loop: bool) -> None:
        """
        Set whether the audio stream loops continuously.
        
        Args:
            loop (bool): True to enable looping, False to disable.
        """
    def stop(self, fade_out_ms: typing.SupportsInt = 0) -> None:
        """
        Stop the audio stream playback.
        
        Args:
            fade_out_ms (int, optional): Fade-out duration in milliseconds. If 0, stops immediately.
                                      If > 0, fades out over the specified duration. Defaults to 0.
        """
    @property
    def current_time(self) -> float:
        """
        The current playback time position in seconds.
        """
    @property
    def volume(self) -> float:
        """
        The volume level of the audio stream.
        
        Volume can exceed 1.0 for amplification.
        
        Type:
            float: Volume level (0.0 = silent, 1.0 = original volume, >1.0 = amplified).
        """
    @volume.setter
    def volume(self, arg1: typing.SupportsFloat) -> None:
        ...
class Camera:
    """
    
    Represents a 2D camera used for rendering.
    
    Controls the viewport's translation, allowing you to move the view of the world.
        
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Create a camera at the default position (0, 0).
        
        Returns:
            Camera: A new camera instance.
        """
    @typing.overload
    def __init__(self, pos: Vec2) -> None:
        """
        Create a camera at the given position.
        
        Args:
            pos (Vec2): The camera's initial position.
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> None:
        """
        Create a camera at the given position.
        
        Args:
            x (float): The x-coordinate of the camera's initial position.
            y (float): The y-coordinate of the camera's initial position.
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
        ...
    def __getitem__(self, index: typing.SupportsInt) -> float:
        ...
    @typing.overload
    def __init__(self, pos: Vec2, radius: typing.SupportsFloat) -> None:
        """
        Create a circle at a given position and radius.
        
        Args:
            pos (Vec2): Center position of the circle.
            radius (float): Radius of the circle.
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Sequence) -> None:
        """
        Create a circle from a nested sequence: ([x, y], radius).
        """
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, other: Circle) -> bool:
        ...
    def as_rect(self) -> Rect:
        """
        Return the smallest rectangle that fully contains the circle.
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
    def radius(self, arg0: typing.SupportsFloat) -> None:
        ...
class Color:
    """
    
    Represents an RGBA color.
    
    Each channel (r, g, b, a) is an 8-bit unsigned integer.
        
    """
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, other: Color) -> bool:
        ...
    def __getitem__(self, index: typing.SupportsInt) -> int:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Create a Color with default values (0, 0, 0, 255).
        """
    @typing.overload
    def __init__(self, r: typing.SupportsInt, g: typing.SupportsInt, b: typing.SupportsInt, a: typing.SupportsInt = 255) -> None:
        """
        Create a Color from RGBA components.
        
        Args:
            r (int): Red value [0-255].
            g (int): Green value [0-255].
            b (int): Blue value [0-255].
            a (int, optional): Alpha value [0-255]. Defaults to 255.
        """
    @typing.overload
    def __init__(self, hex: str) -> None:
        """
        Create a Color from a hex string.
        
        Args:
            hex (str): Hex color string (with or without '#' prefix).
        """
    @typing.overload
    def __init__(self, sequence: collections.abc.Sequence) -> None:
        """
        Create a Color from a sequence of RGB(A) integers.
        
        Args:
            sequence (list or tuple): A sequence of 3 or 4 integers [0-255].
                - 3 values: RGB (alpha defaults to 255)
                - 4 values: RGBA
        """
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __mul__(self, scalar: typing.SupportsFloat) -> Color:
        ...
    def __ne__(self, other: Color) -> bool:
        ...
    def __neg__(self) -> Color:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, scalar: typing.SupportsFloat) -> Color:
        ...
    def __setitem__(self, index: typing.SupportsInt, value: typing.SupportsInt) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __truediv__(self, scalar: typing.SupportsFloat) -> Color:
        ...
    def copy(self) -> Color:
        """
        Create a copy of the color.
        
        Returns:
            Color: A new Color object with the same RGBA values.
        """
    @property
    def a(self) -> int:
        """
        Alpha (transparency) channel value.
        
        Type: int
        Range: 0-255 (8-bit unsigned integer)
        Note: 0 = fully transparent, 255 = fully opaque
        """
    @a.setter
    def a(self, arg0: typing.SupportsInt) -> None:
        ...
    @property
    def b(self) -> int:
        """
        Blue channel value.
        
        Type: int
        Range: 0-255 (8-bit unsigned integer)
        """
    @b.setter
    def b(self, arg0: typing.SupportsInt) -> None:
        ...
    @property
    def g(self) -> int:
        """
        Green channel value.
        
        Type: int
        Range: 0-255 (8-bit unsigned integer)
        """
    @g.setter
    def g(self, arg0: typing.SupportsInt) -> None:
        ...
    @property
    def hex(self) -> str:
        """
        Get or set the color as a hex string.
        
        When getting, returns an 8-digit hex string in the format "#RRGGBBAA".
        When setting, accepts various hex formats (see from_hex for details).
        
        Example:
            color.hex = "#FF00FF"     # Set to magenta
            print(color.hex)          # Returns "#FF00FFFF"
        """
    @hex.setter
    def hex(self, arg1: str) -> None:
        ...
    @property
    def hsv(self) -> tuple[float, float, float, float]:
        """
        Get or set the color as an HSV tuple.
        
        When getting, returns a tuple of (hue, saturation, value, alpha).
        When setting, accepts a tuple of 3 or 4 values.
        
        Values:
            hue (float): Hue angle in degrees (0-360)
            saturation (float): Saturation level (0-1)
            value (float): Brightness/value level (0-1)
            alpha (float): Alpha transparency (0-1), optional
        
        Example:
            color.hsv = (120, 1.0, 1.0)        # Pure green
            color.hsv = (240, 0.5, 0.8, 0.9)   # Light blue with transparency
            h, s, v, a = color.hsv              # Get HSV values
        """
    @hsv.setter
    def hsv(self, arg1: collections.abc.Sequence) -> None:
        ...
    @property
    def r(self) -> int:
        """
        Red channel value.
        
        Type: int
        Range: 0-255 (8-bit unsigned integer)
        """
    @r.setter
    def r(self, arg0: typing.SupportsInt) -> None:
        ...
class EasingAnimation:
    """
    
    A class for animating values over time using easing functions.
    
    This class supports pausing, resuming, reversing, and checking progress.
        
    """
    def __init__(self, ease_func: collections.abc.Callable[[typing.SupportsFloat], float], duration: typing.SupportsFloat) -> None:
        """
        Create an EasingAnimation.
        
        Args:
            ease_func (Callable): Easing function that maps [0, 1] → [0, 1].
            duration (float): Time in seconds for full animation.
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
    def step(self) -> Vec2:
        """
        Advance the animation and get its current position.
        
        Returns:
            Vec2: Interpolated position.
        """
    @property
    def end_pos(self) -> Vec2:
        """
        The ending position of the animation.
        """
    @end_pos.setter
    def end_pos(self, arg0: Vec2) -> None:
        ...
    @property
    def is_done(self) -> bool:
        """
        Check whether the animation has finished.
        """
    @property
    def start_pos(self) -> Vec2:
        """
        The starting position of the animation.
        """
    @start_pos.setter
    def start_pos(self, arg0: Vec2) -> None:
        ...
class Event:
    """
    
    Represents a single input event such as keyboard, mouse, or gamepad activity.
    
    Attributes:
        type (int): Event type. Additional fields are accessed dynamically.
            
    """
    def __getattr__(self, arg0: str) -> typing.Any:
        ...
    @property
    def type(self) -> int:
        """
        The event type (e.g., KEY_DOWN, MOUSE_BUTTON_UP).
        """
class EventType(enum.IntEnum):
    AUDIO_DEVICE_ADDED: typing.ClassVar[EventType]  # value = <EventType.AUDIO_DEVICE_ADDED: 4352>
    AUDIO_DEVICE_FORMAT_CHANGED: typing.ClassVar[EventType]  # value = <EventType.AUDIO_DEVICE_FORMAT_CHANGED: 4354>
    AUDIO_DEVICE_REMOVED: typing.ClassVar[EventType]  # value = <EventType.AUDIO_DEVICE_REMOVED: 4353>
    CAMERA_DEVICE_ADDED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_DEVICE_ADDED: 5120>
    CAMERA_DEVICE_APPROVED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_DEVICE_APPROVED: 5122>
    CAMERA_DEVICE_DENIED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_DEVICE_DENIED: 5123>
    CAMERA_DEVICE_REMOVED: typing.ClassVar[EventType]  # value = <EventType.CAMERA_DEVICE_REMOVED: 5121>
    CLIPBOARD_UPDATE: typing.ClassVar[EventType]  # value = <EventType.CLIPBOARD_UPDATE: 2304>
    DID_ENTER_BACKGROUND: typing.ClassVar[EventType]  # value = <EventType.DID_ENTER_BACKGROUND: 260>
    DID_ENTER_FOREGROUND: typing.ClassVar[EventType]  # value = <EventType.DID_ENTER_FOREGROUND: 262>
    DISPLAY_ADDED: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_ADDED: 338>
    DISPLAY_CONTENT_SCALE_CHANGED: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_CONTENT_SCALE_CHANGED: 343>
    DISPLAY_CURRENT_MODE_CHANGED: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_CURRENT_MODE_CHANGED: 342>
    DISPLAY_DESKTOP_MODE_CHANGED: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_DESKTOP_MODE_CHANGED: 341>
    DISPLAY_MOVED: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_MOVED: 340>
    DISPLAY_ORIENTATION: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_ORIENTATION: 337>
    DISPLAY_REMOVED: typing.ClassVar[EventType]  # value = <EventType.DISPLAY_REMOVED: 339>
    DROP_BEGIN: typing.ClassVar[EventType]  # value = <EventType.DROP_BEGIN: 4098>
    DROP_COMPLETE: typing.ClassVar[EventType]  # value = <EventType.DROP_COMPLETE: 4099>
    DROP_FILE: typing.ClassVar[EventType]  # value = <EventType.DROP_FILE: 4096>
    DROP_POSITION: typing.ClassVar[EventType]  # value = <EventType.DROP_POSITION: 4100>
    DROP_TEXT: typing.ClassVar[EventType]  # value = <EventType.DROP_TEXT: 4097>
    FINGER_CANCELED: typing.ClassVar[EventType]  # value = <EventType.FINGER_CANCELED: 1795>
    FINGER_DOWN: typing.ClassVar[EventType]  # value = <EventType.FINGER_DOWN: 1792>
    FINGER_MOTION: typing.ClassVar[EventType]  # value = <EventType.FINGER_MOTION: 1794>
    FINGER_UP: typing.ClassVar[EventType]  # value = <EventType.FINGER_UP: 1793>
    GAMEPAD_ADDED: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_ADDED: 1619>
    GAMEPAD_AXIS_MOTION: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_AXIS_MOTION: 1616>
    GAMEPAD_BUTTON_DOWN: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_BUTTON_DOWN: 1617>
    GAMEPAD_BUTTON_UP: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_BUTTON_UP: 1618>
    GAMEPAD_REMAPPED: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_REMAPPED: 1621>
    GAMEPAD_REMOVED: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_REMOVED: 1620>
    GAMEPAD_SENSOR_UPDATE: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_SENSOR_UPDATE: 1625>
    GAMEPAD_STEAM_HANDLE_UPDATED: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_STEAM_HANDLE_UPDATED: 1627>
    GAMEPAD_TOUCHPAD_DOWN: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_TOUCHPAD_DOWN: 1622>
    GAMEPAD_TOUCHPAD_MOTION: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_TOUCHPAD_MOTION: 1623>
    GAMEPAD_TOUCHPAD_UP: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_TOUCHPAD_UP: 1624>
    GAMEPAD_UPDATE_COMPLETE: typing.ClassVar[EventType]  # value = <EventType.GAMEPAD_UPDATE_COMPLETE: 1626>
    KEYBOARD_ADDED: typing.ClassVar[EventType]  # value = <EventType.KEYBOARD_ADDED: 773>
    KEYBOARD_REMOVED: typing.ClassVar[EventType]  # value = <EventType.KEYBOARD_REMOVED: 774>
    KEYMAP_CHANGED: typing.ClassVar[EventType]  # value = <EventType.KEYMAP_CHANGED: 772>
    KEY_DOWN: typing.ClassVar[EventType]  # value = <EventType.KEY_DOWN: 768>
    KEY_UP: typing.ClassVar[EventType]  # value = <EventType.KEY_UP: 769>
    LOCALE_CHANGED: typing.ClassVar[EventType]  # value = <EventType.LOCALE_CHANGED: 263>
    LOW_MEMORY: typing.ClassVar[EventType]  # value = <EventType.LOW_MEMORY: 258>
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
    RENDER_DEVICE_LOST: typing.ClassVar[EventType]  # value = <EventType.RENDER_DEVICE_LOST: 8194>
    RENDER_DEVICE_RESET: typing.ClassVar[EventType]  # value = <EventType.RENDER_DEVICE_RESET: 8193>
    RENDER_TARGETS_RESET: typing.ClassVar[EventType]  # value = <EventType.RENDER_TARGETS_RESET: 8192>
    SENSOR_UPDATE: typing.ClassVar[EventType]  # value = <EventType.SENSOR_UPDATE: 4608>
    SYSTEM_THEME_CHANGED: typing.ClassVar[EventType]  # value = <EventType.SYSTEM_THEME_CHANGED: 264>
    TERMINATING: typing.ClassVar[EventType]  # value = <EventType.TERMINATING: 257>
    TEXT_EDITING: typing.ClassVar[EventType]  # value = <EventType.TEXT_EDITING: 770>
    TEXT_EDITING_CANDIDATES: typing.ClassVar[EventType]  # value = <EventType.TEXT_EDITING_CANDIDATES: 775>
    TEXT_INPUT: typing.ClassVar[EventType]  # value = <EventType.TEXT_INPUT: 771>
    WILL_ENTER_BACKGROUND: typing.ClassVar[EventType]  # value = <EventType.WILL_ENTER_BACKGROUND: 259>
    WILL_ENTER_FOREGROUND: typing.ClassVar[EventType]  # value = <EventType.WILL_ENTER_FOREGROUND: 261>
    WINDOW_CLOSE_REQUESTED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_CLOSE_REQUESTED: 528>
    WINDOW_DESTROYED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_DESTROYED: 537>
    WINDOW_DISPLAY_CHANGED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_DISPLAY_CHANGED: 531>
    WINDOW_DISPLAY_SCALE_CHANGED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_DISPLAY_SCALE_CHANGED: 532>
    WINDOW_ENTER_FULLSCREEN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_ENTER_FULLSCREEN: 535>
    WINDOW_EXPOSED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_EXPOSED: 516>
    WINDOW_FOCUS_GAINED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_FOCUS_GAINED: 526>
    WINDOW_FOCUS_LOST: typing.ClassVar[EventType]  # value = <EventType.WINDOW_FOCUS_LOST: 527>
    WINDOW_HDR_STATE_CHANGED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_HDR_STATE_CHANGED: 538>
    WINDOW_HIDDEN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_HIDDEN: 515>
    WINDOW_HIT_TEST: typing.ClassVar[EventType]  # value = <EventType.WINDOW_HIT_TEST: 529>
    WINDOW_ICCPROF_CHANGED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_ICCPROF_CHANGED: 530>
    WINDOW_LEAVE_FULLSCREEN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_LEAVE_FULLSCREEN: 536>
    WINDOW_MAXIMIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MAXIMIZED: 522>
    WINDOW_MINIMIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MINIMIZED: 521>
    WINDOW_MOUSE_ENTER: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MOUSE_ENTER: 524>
    WINDOW_MOUSE_LEAVE: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MOUSE_LEAVE: 525>
    WINDOW_MOVED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_MOVED: 517>
    WINDOW_OCCLUDED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_OCCLUDED: 534>
    WINDOW_RESIZED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_RESIZED: 518>
    WINDOW_RESTORED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_RESTORED: 523>
    WINDOW_SAFE_AREA_CHANGED: typing.ClassVar[EventType]  # value = <EventType.WINDOW_SAFE_AREA_CHANGED: 533>
    WINDOW_SHOWN: typing.ClassVar[EventType]  # value = <EventType.WINDOW_SHOWN: 514>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Font:
    """
    
    A font typeface for rendering text.
    
    This class wraps an SDL_ttf font and manages font properties like size,
    style, and alignment. You can load fonts from a file path or use one of
    the built-in typefaces:
    
    - "kraken-clean": A clean sans-serif font bundled with the engine.
    - "kraken-retro": A pixel/retro font bundled with the engine. Point size is
                      rounded to the nearest multiple of 8 for crisp rendering.
    
    Note:
        A window/renderer must be created before using fonts. Typically you should
        call kn.window.create(...) first, which initializes the font engine.
        
    """
    def __init__(self, file_dir: str, pt_size: typing.SupportsInt) -> None:
        """
        Create a Font.
        
        Args:
            file_dir (str): Path to a .ttf font file, or one of the built-in names
                            "kraken-clean" or "kraken-retro".
            pt_size (int): The point size. Values below 8 are clamped to 8. For
                           "kraken-retro", the size is rounded to the nearest multiple
                           of 8 to preserve pixel alignment.
        
        Raises:
            RuntimeError: If the font fails to load.
        """
    @property
    def alignment(self) -> Align:
        """
        Get or set the text alignment for wrapped text.
        
        Valid values: Align.LEFT, Align.CENTER, Align.RIGHT
        """
    @alignment.setter
    def alignment(self, arg1: Align) -> None:
        ...
    @property
    def ascent(self) -> int:
        """
        Get the pixel ascent of the font.
        
        Returns:
            int: The font ascent in pixels.
        """
    @property
    def bold(self) -> bool:
        """
        Get or set whether bold text style is enabled.
        """
    @bold.setter
    def bold(self, arg1: bool) -> None:
        ...
    @property
    def descent(self) -> int:
        """
        Get the pixel descent of the font.
        
        Returns:
            int: The font descent in pixels.
        """
    @property
    def height(self) -> int:
        """
        Get the maximum pixel height of all glyphs in the font.
        
        Returns:
            int: The font height in pixels.
        """
    @property
    def hinting(self) -> FontHint:
        """
        Get or set the font hinting mode.
        
        Valid values: FontHinting.NORMAL, FontHinting.MONO, FontHinting.LIGHT,
                      FontHinting.LIGHT_SUBPIXEL, FontHinting.NONE
        """
    @hinting.setter
    def hinting(self, arg1: FontHint) -> None:
        ...
    @property
    def italic(self) -> bool:
        """
        Get or set whether italic text style is enabled.
        """
    @italic.setter
    def italic(self, arg1: bool) -> None:
        ...
    @property
    def kerning(self) -> bool:
        """
        Get or set whether kerning is enabled.
        """
    @kerning.setter
    def kerning(self, arg1: bool) -> None:
        ...
    @property
    def line_spacing(self) -> int:
        """
        Get or set the spacing between lines of text in pixels.
        """
    @line_spacing.setter
    def line_spacing(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def outline(self) -> int:
        """
        Get or set the outline width in pixels (0 for no outline).
        """
    @outline.setter
    def outline(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def pt_size(self) -> int:
        """
        Get or set the point size of the font. Values below 8 are clamped to 8.
        """
    @pt_size.setter
    def pt_size(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def strikethrough(self) -> bool:
        """
        Get or set whether strikethrough text style is enabled.
        """
    @strikethrough.setter
    def strikethrough(self, arg1: bool) -> None:
        ...
    @property
    def underline(self) -> bool:
        """
        Get or set whether underline text style is enabled.
        """
    @underline.setter
    def underline(self, arg1: bool) -> None:
        ...
class FontHint(enum.IntEnum):
    """
    
    Font hinting modes for controlling how fonts are rendered.
    
    Hinting is the process of fitting font outlines to the pixel grid to improve
    readability at small sizes.
        
    """
    LIGHT: typing.ClassVar[FontHint]  # value = <FontHint.LIGHT: 2>
    LIGHT_SUBPIXEL: typing.ClassVar[FontHint]  # value = <FontHint.LIGHT_SUBPIXEL: 3>
    MONO: typing.ClassVar[FontHint]  # value = <FontHint.MONO: 1>
    NONE: typing.ClassVar[FontHint]  # value = <FontHint.NONE: 4>
    NORMAL: typing.ClassVar[FontHint]  # value = <FontHint.NORMAL: 0>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Frame:
    """
    
    A single animation frame containing texture and rectangle data.
    
    Represents one frame of an animation with its associated texture and the rectangle
    defining which portion of the texture to display.
            
    """
    @property
    def src(self) -> Rect:
        """
        The rectangle defining the frame bounds within the texture.
        """
    @property
    def tex(self) -> Texture:
        """
        The texture containing the frame image.
        """
class GamepadAxis(enum.IntEnum):
    C_LTRIGGER: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_LTRIGGER: 4>
    C_LX: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_LX: 0>
    C_LY: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_LY: 1>
    C_RTRIGGER: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_RTRIGGER: 5>
    C_RX: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_RX: 2>
    C_RY: typing.ClassVar[GamepadAxis]  # value = <GamepadAxis.C_RY: 3>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class GamepadButton(enum.IntEnum):
    C_BACK: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_BACK: 4>
    C_DPAD_DOWN: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPAD_DOWN: 12>
    C_DPAD_LEFT: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPAD_LEFT: 13>
    C_DPAD_RIGHT: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPAD_RIGHT: 14>
    C_DPAD_UP: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_DPAD_UP: 11>
    C_EAST: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_EAST: 1>
    C_GUIDE: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_GUIDE: 5>
    C_LSHOULDER: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_LSHOULDER: 9>
    C_LSTICK: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_LSTICK: 7>
    C_NORTH: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_NORTH: 3>
    C_RSHOULDER: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_RSHOULDER: 10>
    C_RSTICK: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_RSTICK: 8>
    C_SOUTH: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_SOUTH: 0>
    C_START: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_START: 6>
    C_WEST: typing.ClassVar[GamepadButton]  # value = <GamepadButton.C_WEST: 2>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class GamepadType(enum.IntEnum):
    C_PS3: typing.ClassVar[GamepadType]  # value = <GamepadType.C_PS3: 4>
    C_PS4: typing.ClassVar[GamepadType]  # value = <GamepadType.C_PS4: 5>
    C_PS5: typing.ClassVar[GamepadType]  # value = <GamepadType.C_PS5: 6>
    C_STANDARD: typing.ClassVar[GamepadType]  # value = <GamepadType.C_STANDARD: 1>
    C_SWITCH_JOYCON_LEFT: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCH_JOYCON_LEFT: 8>
    C_SWITCH_JOYCON_PAIR: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCH_JOYCON_PAIR: 10>
    C_SWITCH_JOYCON_RIGHT: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCH_JOYCON_RIGHT: 9>
    C_SWITCH_PRO: typing.ClassVar[GamepadType]  # value = <GamepadType.C_SWITCH_PRO: 7>
    C_XBOX_360: typing.ClassVar[GamepadType]  # value = <GamepadType.C_XBOX_360: 2>
    C_XBOX_ONE: typing.ClassVar[GamepadType]  # value = <GamepadType.C_XBOX_ONE: 3>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
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
    def __init__(self, gamepad_button: GamepadButton, slot: typing.SupportsInt = 0) -> None:
        """
        Create an input action from a gamepad button.
        
        Args:
            gamepad_button (GamepadButton): Gamepad button code.
            slot (int, optional): Gamepad slot (default is 0).
        """
    @typing.overload
    def __init__(self, gamepad_axis: GamepadAxis, is_positive: bool, slot: typing.SupportsInt = 0) -> None:
        """
        Create an input action from a gamepad axis direction.
        
        Args:
            gamepad_axis (GamepadAxis): Gamepad axis code.
            is_positive (bool): True for positive direction, False for negative.
            slot (int, optional): Gamepad slot (default is 0).
        """
class Keycode(enum.IntEnum):
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
    K_APPLICATION: typing.ClassVar[Keycode]  # value = <Keycode.K_APPLICATION: 1073741925>
    K_ASTERISK: typing.ClassVar[Keycode]  # value = <Keycode.K_ASTERISK: 42>
    K_AT: typing.ClassVar[Keycode]  # value = <Keycode.K_AT: 64>
    K_BACKSLASH: typing.ClassVar[Keycode]  # value = <Keycode.K_BACKSLASH: 92>
    K_BACKSPACE: typing.ClassVar[Keycode]  # value = <Keycode.K_BACKSPACE: 8>
    K_CALL: typing.ClassVar[Keycode]  # value = <Keycode.K_CALL: 1073742113>
    K_CAPS: typing.ClassVar[Keycode]  # value = <Keycode.K_CAPS: 1073741881>
    K_CARET: typing.ClassVar[Keycode]  # value = <Keycode.K_CARET: 94>
    K_CHANNEL_DEC: typing.ClassVar[Keycode]  # value = <Keycode.K_CHANNEL_DEC: 1073742085>
    K_CHANNEL_INC: typing.ClassVar[Keycode]  # value = <Keycode.K_CHANNEL_INC: 1073742084>
    K_COLON: typing.ClassVar[Keycode]  # value = <Keycode.K_COLON: 58>
    K_COMMA: typing.ClassVar[Keycode]  # value = <Keycode.K_COMMA: 44>
    K_COPY: typing.ClassVar[Keycode]  # value = <Keycode.K_COPY: 1073741948>
    K_CUT: typing.ClassVar[Keycode]  # value = <Keycode.K_CUT: 1073741947>
    K_DBLQUOTE: typing.ClassVar[Keycode]  # value = <Keycode.K_DBLQUOTE: 34>
    K_DEL: typing.ClassVar[Keycode]  # value = <Keycode.K_DEL: 127>
    K_DOLLAR: typing.ClassVar[Keycode]  # value = <Keycode.K_DOLLAR: 36>
    K_DOWN: typing.ClassVar[Keycode]  # value = <Keycode.K_DOWN: 1073741905>
    K_END: typing.ClassVar[Keycode]  # value = <Keycode.K_END: 1073741901>
    K_ENDCALL: typing.ClassVar[Keycode]  # value = <Keycode.K_ENDCALL: 1073742114>
    K_EQ: typing.ClassVar[Keycode]  # value = <Keycode.K_EQ: 61>
    K_ESC: typing.ClassVar[Keycode]  # value = <Keycode.K_ESC: 27>
    K_EXCLAIM: typing.ClassVar[Keycode]  # value = <Keycode.K_EXCLAIM: 33>
    K_EXECUTE: typing.ClassVar[Keycode]  # value = <Keycode.K_EXECUTE: 1073741940>
    K_F1: typing.ClassVar[Keycode]  # value = <Keycode.K_F1: 1073741882>
    K_F10: typing.ClassVar[Keycode]  # value = <Keycode.K_F10: 1073741891>
    K_F11: typing.ClassVar[Keycode]  # value = <Keycode.K_F11: 1073741892>
    K_F12: typing.ClassVar[Keycode]  # value = <Keycode.K_F12: 1073741893>
    K_F13: typing.ClassVar[Keycode]  # value = <Keycode.K_F13: 1073741928>
    K_F14: typing.ClassVar[Keycode]  # value = <Keycode.K_F14: 1073741929>
    K_F15: typing.ClassVar[Keycode]  # value = <Keycode.K_F15: 1073741930>
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
    K_HELP: typing.ClassVar[Keycode]  # value = <Keycode.K_HELP: 1073741941>
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
    K_MEDIA_EJECT: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_EJECT: 1073742094>
    K_MEDIA_FF: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_FF: 1073742089>
    K_MEDIA_NEXT: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_NEXT: 1073742091>
    K_MEDIA_PAUSE: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_PAUSE: 1073742087>
    K_MEDIA_PLAY: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_PLAY: 1073742086>
    K_MEDIA_PLAY_PAUSE: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_PLAY_PAUSE: 1073742095>
    K_MEDIA_PREV: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_PREV: 1073742092>
    K_MEDIA_REC: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_REC: 1073742088>
    K_MEDIA_REWIND: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_REWIND: 1073742090>
    K_MEDIA_SELECT: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_SELECT: 1073742096>
    K_MEDIA_STOP: typing.ClassVar[Keycode]  # value = <Keycode.K_MEDIA_STOP: 1073742093>
    K_MENU: typing.ClassVar[Keycode]  # value = <Keycode.K_MENU: 1073741942>
    K_MINUS: typing.ClassVar[Keycode]  # value = <Keycode.K_MINUS: 45>
    K_MODE: typing.ClassVar[Keycode]  # value = <Keycode.K_MODE: 1073742081>
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
    K_POWER: typing.ClassVar[Keycode]  # value = <Keycode.K_POWER: 1073741926>
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
    K_SELECT: typing.ClassVar[Keycode]  # value = <Keycode.K_SELECT: 1073741943>
    K_SEMICOLON: typing.ClassVar[Keycode]  # value = <Keycode.K_SEMICOLON: 59>
    K_SGLQUOTE: typing.ClassVar[Keycode]  # value = <Keycode.K_SGLQUOTE: 39>
    K_SLASH: typing.ClassVar[Keycode]  # value = <Keycode.K_SLASH: 47>
    K_SLEEP: typing.ClassVar[Keycode]  # value = <Keycode.K_SLEEP: 1073742082>
    K_SOFTLEFT: typing.ClassVar[Keycode]  # value = <Keycode.K_SOFTLEFT: 1073742111>
    K_SOFTRIGHT: typing.ClassVar[Keycode]  # value = <Keycode.K_SOFTRIGHT: 1073742112>
    K_SPACE: typing.ClassVar[Keycode]  # value = <Keycode.K_SPACE: 32>
    K_STOP: typing.ClassVar[Keycode]  # value = <Keycode.K_STOP: 1073741944>
    K_TAB: typing.ClassVar[Keycode]  # value = <Keycode.K_TAB: 9>
    K_TILDE: typing.ClassVar[Keycode]  # value = <Keycode.K_TILDE: 126>
    K_UNDERSCORE: typing.ClassVar[Keycode]  # value = <Keycode.K_UNDERSCORE: 95>
    K_UNDO: typing.ClassVar[Keycode]  # value = <Keycode.K_UNDO: 1073741946>
    K_UNKNOWN: typing.ClassVar[Keycode]  # value = <Keycode.K_UNKNOWN: 0>
    K_UP: typing.ClassVar[Keycode]  # value = <Keycode.K_UP: 1073741906>
    K_VOLDOWN: typing.ClassVar[Keycode]  # value = <Keycode.K_VOLDOWN: 1073741953>
    K_VOLUP: typing.ClassVar[Keycode]  # value = <Keycode.K_VOLUP: 1073741952>
    K_WAKE: typing.ClassVar[Keycode]  # value = <Keycode.K_WAKE: 1073742083>
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
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Line:
    """
    
    A 2D line segment defined by two points: A and B.
    You can access or modify points using `.a`, `.b`, or directly via `.ax`, `.ay`, `.bx`, `.by`.
        
    """
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, other: Line) -> bool:
        ...
    def __getitem__(self, index: typing.SupportsInt) -> float:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Create a default line with all values set to 0.
        """
    @typing.overload
    def __init__(self, ax: typing.SupportsFloat, ay: typing.SupportsFloat, bx: typing.SupportsFloat, by: typing.SupportsFloat) -> None:
        """
        Create a line from two coordinate points.
        
        Args:
            ax (float): X-coordinate of point A.
            ay (float): Y-coordinate of point A.
            bx (float): X-coordinate of point B.
            by (float): Y-coordinate of point B.
        """
    @typing.overload
    def __init__(self, ax: typing.SupportsFloat, ay: typing.SupportsFloat, b: Vec2) -> None:
        """
        Create a line from A coordinates and a Vec2 B point.
        
        Args:
            ax (float): X-coordinate of point A.
            ay (float): Y-coordinate of point A.
            b (Vec2): Point B.
        """
    @typing.overload
    def __init__(self, a: Vec2, bx: typing.SupportsFloat, by: typing.SupportsFloat) -> None:
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
    def __init__(self, arg0: collections.abc.Sequence) -> None:
        """
        Create a line from two 2-element sequences: [[ax, ay], [bx, by]].
        
        Raises:
            ValueError: If either point is not a 2-element sequence.
        """
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, other: Line) -> bool:
        ...
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
    def a(self) -> Vec2:
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
    def ax(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def ay(self) -> float:
        """
        Y-coordinate of point A.
        """
    @ay.setter
    def ay(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def b(self) -> Vec2:
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
    def bx(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def by(self) -> float:
        """
        Y-coordinate of point B.
        """
    @by.setter
    def by(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def length(self) -> float:
        """
        The Euclidean length of the line segment.
        """
class Mask:
    """
    
    A collision mask for pixel-perfect collision detection.
    
    A Mask represents a 2D bitmap, typically used for precise collision detection based on 
    non-transparent pixels.
        
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Create an empty mask with size (0, 0).
        """
    @typing.overload
    def __init__(self, size: Vec2, filled: bool = False) -> None:
        """
        Create a mask with specified size.
        
        Args:
            size (Vec2): The size of the mask as (width, height).
            filled (bool): Whether to fill the mask with solid pixels. Defaults to False.
        """
    @typing.overload
    def __init__(self, pixel_array: PixelArray, threshold: typing.SupportsInt = 1) -> None:
        """
        Create a mask from a pixel array based on alpha threshold.
        
        Args:
            pixel_array (PixelArray): The source pixel array to create the mask from.
            threshold (int): Alpha threshold value (0-255). Pixels with alpha >= threshold are solid.
        
        Raises:
            RuntimeError: If the pixel array is invalid.
        """
    def add(self, other: Mask, offset: typing.Any = None) -> None:
        """
        Add another mask to this mask with an offset.
        
        Performs a bitwise OR operation between the masks.
        
        Args:
            other (Mask): The mask to add.
            offset (Vec2): Position offset for the other mask. Defaults to (0, 0).
        """
    def clear(self) -> None:
        """
        Clear the entire mask, setting all pixels to transparent.
        """
    def collide_mask(self, other: Mask, offset: typing.Any = None) -> bool:
        """
        Check collision between this mask and another mask with an offset.
        
        Args:
            other (Mask): The other mask to test collision with.
            offset (Vec2): Position offset between the masks. Defaults to (0, 0).
        
        Returns:
            bool: True if the masks collide, False otherwise.
        """
    def copy(self) -> Mask:
        """
        Create a copy of this mask.
        
        Returns:
            Mask: A new Mask with the same dimensions and pixel data.
        """
    def fill(self) -> None:
        """
        Fill the entire mask with solid pixels.
        """
    def get_at(self, pos: Vec2) -> bool:
        """
        Get the pixel value at a specific position.
        
        Args:
            pos (Vec2): The position to check.
        
        Returns:
            bool: True if the pixel is solid (above threshold), False otherwise.
        """
    def get_bounding_rect(self) -> Rect:
        """
        Get the bounding rectangle that contains all solid pixels.
        
        Returns:
            Rect: The smallest rectangle containing all solid pixels. 
                  Returns empty rect if mask has no solid pixels.
        """
    def get_center_of_mass(self) -> Vec2:
        """
        Calculate the center of mass of all solid pixels.
        
        Returns:
            Vec2: The center of mass position. Returns (0, 0) if mask is empty.
        """
    def get_collision_points(self, other: Mask, offset: typing.Any = None) -> list[Vec2]:
        """
        Get all points where this mask collides with another mask.
        
        Args:
            other (Mask): The other mask to test collision with.
            offset (Vec2): Position offset between the masks. Defaults to (0, 0).
        
        Returns:
            list[Vec2]: A list of collision points.
        """
    def get_count(self) -> int:
        """
        Get the number of solid pixels in the mask.
        
        Returns:
            int: The count of solid pixels.
        """
    def get_outline(self) -> list[Vec2]:
        """
        Get the outline points of the mask.
        
        Returns a list of points that form the outline of all solid regions.
        
        Returns:
            list[Vec2]: A list of outline points.
        """
    def get_overlap_area(self, other: Mask, offset: typing.Any = None) -> int:
        """
        Get the number of overlapping pixels between this mask and another.
        
        Args:
            other (Mask): The other mask to check overlap with.
            offset (Vec2): Position offset between the masks. Defaults to (0, 0).
        
        Returns:
            int: The number of overlapping solid pixels.
        """
    def get_overlap_mask(self, other: Mask, offset: typing.Any = None) -> Mask:
        """
        Get a mask representing the overlapping area between this mask and another.
        
        Args:
            other (Mask): The other mask to check overlap with.
            offset (Vec2): Position offset between the masks. Defaults to (0, 0).
        
        Returns:
            Mask: A new mask containing only the overlapping pixels.
        """
    def get_pixel_array(self, color: typing.Any = None) -> PixelArray:
        """
        Convert the mask to a pixel array with the specified color.
        
        Solid pixels become the specified color, transparent pixels become transparent.
        
        Args:
            color (Color): The color to use for solid pixels. Defaults to white (255, 255, 255, 255).
        
        Returns:
            PixelArray: A new pixel array representation of the mask.
        
        Raises:
            RuntimeError: If pixel array creation fails.
        """
    def get_rect(self) -> Rect:
        """
        Get the bounding rectangle of the mask starting at (0, 0).
        """
    def invert(self) -> None:
        """
        Invert all pixels in the mask.
        
        Solid pixels become transparent and transparent pixels become solid.
        """
    def is_empty(self) -> bool:
        """
        Check if the mask contains no solid pixels.
        
        Returns:
            bool: True if the mask is empty, False otherwise.
        """
    def set_at(self, pos: Vec2, value: bool) -> None:
        """
        Set the pixel value at a specific position.
        
        Args:
            pos (Vec2): The position to set.
            value (bool): The pixel value (True for solid, False for transparent).
        """
    def subtract(self, other: Mask, offset: typing.Any = None) -> None:
        """
        Subtract another mask from this mask with an offset.
        
        Removes pixels where the other mask has solid pixels.
        
        Args:
            other (Mask): The mask to subtract.
            offset (Vec2): Position offset for the other mask. Defaults to (0, 0).
        """
    @property
    def height(self) -> int:
        """
        The height of the mask in pixels.
        """
    @property
    def size(self) -> Vec2:
        """
        The size of the mask as a Vec2.
        """
    @property
    def width(self) -> int:
        """
        The width of the mask in pixels.
        """
class MouseButton(enum.IntEnum):
    M_LEFT: typing.ClassVar[MouseButton]  # value = <MouseButton.M_LEFT: 1>
    M_MIDDLE: typing.ClassVar[MouseButton]  # value = <MouseButton.M_MIDDLE: 2>
    M_RIGHT: typing.ClassVar[MouseButton]  # value = <MouseButton.M_RIGHT: 3>
    M_SIDE1: typing.ClassVar[MouseButton]  # value = <MouseButton.M_SIDE1: 4>
    M_SIDE2: typing.ClassVar[MouseButton]  # value = <MouseButton.M_SIDE2: 5>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class PenAxis(enum.IntEnum):
    P_DISTANCE: typing.ClassVar[PenAxis]  # value = <PenAxis.P_DISTANCE: 3>
    P_PRESSURE: typing.ClassVar[PenAxis]  # value = <PenAxis.P_PRESSURE: 0>
    P_ROTATION: typing.ClassVar[PenAxis]  # value = <PenAxis.P_ROTATION: 4>
    P_SLIDER: typing.ClassVar[PenAxis]  # value = <PenAxis.P_SLIDER: 5>
    P_TANGENTIAL_PRESSURE: typing.ClassVar[PenAxis]  # value = <PenAxis.P_TANGENTIAL_PRESSURE: 6>
    P_TILT_X: typing.ClassVar[PenAxis]  # value = <PenAxis.P_TILT_X: 1>
    P_TILT_Y: typing.ClassVar[PenAxis]  # value = <PenAxis.P_TILT_Y: 2>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class PixelArray:
    """
    
    Represents a 2D pixel buffer for image manipulation and blitting operations.
    
    A PixelArray is a 2D array of pixels that can be manipulated, drawn on, and used as a source
    for texture creation or blitting to other PixelArrays. Supports pixel-level operations,
    color key transparency, and alpha blending.
        
    """
    @typing.overload
    def __init__(self, size: Vec2) -> None:
        """
        Create a new PixelArray with the specified dimensions.
        
        Args:
            size (Vec2): The size of the pixel array as (width, height).
        
        Raises:
            RuntimeError: If pixel array creation fails.
        """
    @typing.overload
    def __init__(self, file_path: str) -> None:
        """
        Create a PixelArray by loading an image from a file.
        
        Args:
            file_path (str): Path to the image file to load.
        
        Raises:
            RuntimeError: If the file cannot be loaded or doesn't exist.
        """
    @typing.overload
    def blit(self, pixel_array: PixelArray, pos: Vec2, anchor: Anchor = Anchor.CENTER, src: typing.Any = None) -> None:
        """
        Blit (copy) another pixel array onto this pixel array at the specified position with anchor alignment.
        
        Args:
            pixel_array (PixelArray): The source pixel array to blit from.
            pos (Vec2): The position to blit to.
            anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
            src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.
        
        Raises:
            RuntimeError: If the blit operation fails.
        """
    @typing.overload
    def blit(self, pixel_array: PixelArray, dst: Rect, src: typing.Any = None) -> None:
        """
        Blit (copy) another pixel array onto this pixel array with specified destination and source rectangles.
        
        Args:
            pixel_array (PixelArray): The source pixel array to blit from.
            dst (Rect): The destination rectangle on this pixel array.
            src (Rect, optional): The source rectangle to blit from. Defaults to entire source pixel array.
        
        Raises:
            RuntimeError: If the blit operation fails.
        """
    def copy(self) -> PixelArray:
        """
        Create a copy of this pixel array.
        
        Returns:
            PixelArray: A new PixelArray that is an exact copy of this one.
        
        Raises:
            RuntimeError: If pixel array copying fails.
        """
    def fill(self, color: Color) -> None:
        """
        Fill the entire pixel array with a solid color.
        
        Args:
            color (Color): The color to fill the pixel array with.
        """
    def get_at(self, coord: Vec2) -> Color:
        """
        Get the color of a pixel at the specified coordinates.
        
        Args:
            coord (Vec2): The coordinates of the pixel as (x, y).
        
        Returns:
            Color: The color of the pixel at the specified coordinates.
        
        Raises:
            IndexError: If coordinates are outside the pixel array bounds.
        """
    def get_rect(self) -> Rect:
        """
        Get a rectangle representing the pixel array bounds.
        
        Returns:
            Rect: A rectangle with position (0, 0) and the pixel array's dimensions.
        """
    def scroll(self, dx: typing.SupportsInt, dy: typing.SupportsInt, scroll_mode: ScrollMode) -> None:
        """
        Scroll the pixel array's contents by the specified offset.
        
        Args:
            dx (int): Horizontal scroll offset in pixels.
            dy (int): Vertical scroll offset in pixels.
            scroll_mode (ScrollMode, optional): Behavior for pixels scrolled off the edge.
                - REPEAT: Wrap pixels around to the opposite edge.
                - ERASE: Fill scrolled areas with transparent pixels.
                - SMEAR: Extend edge pixels into scrolled areas.
        """
    def set_at(self, coord: Vec2, color: Color) -> None:
        """
        Set the color of a pixel at the specified coordinates.
        
        Args:
            coord (Vec2): The coordinates of the pixel as (x, y).
            color (Color): The color to set the pixel to.
        
        Raises:
            IndexError: If coordinates are outside the pixel array bounds.
        """
    @property
    def alpha_mod(self) -> int:
        """
        The alpha modulation value for the pixel array.
        
        Controls the overall transparency of the pixel array. Values range from 0 (fully transparent)
        to 255 (fully opaque).
        
        Returns:
            int: The current alpha modulation value [0-255].
        
        Raises:
            RuntimeError: If getting the alpha value fails.
        """
    @alpha_mod.setter
    def alpha_mod(self, arg1: typing.SupportsInt) -> None:
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
        The height of the pixel array.
        
        Returns:
            int: The pixel array height.
        """
    @property
    def size(self) -> Vec2:
        """
        The size of the pixel array as a Vec2.
        
        Returns:
            Vec2: The pixel array size as (width, height).
        """
    @property
    def width(self) -> int:
        """
        The width of the pixel array.
        
        Returns:
            int: The pixel array width.
        """
class PolarCoordinate:
    """
    
    PolarCoordinate models a polar coordinate pair.
    
    Attributes:
        angle (float): Angle in radians.
        radius (float): Distance from origin.
    
    Methods:
        to_cartesian: Convert the coordinate to a Vec2.
            
    """
    def __eq__(self, arg0: PolarCoordinate) -> bool:
        ...
    def __getitem__(self, index: typing.SupportsInt) -> float:
        ...
    def __hash__(self) -> int:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Initialize a PolarCoordinate with zero angle and radius.
        """
    @typing.overload
    def __init__(self, angle: typing.SupportsFloat, radius: typing.SupportsFloat) -> None:
        """
        Initialize a PolarCoordinate from explicit values.
        
        Args:
            angle (float): Angle in radians.
            radius (float): Distance from the origin.
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Sequence) -> None:
        """
        Initialize a PolarCoordinate from a two-item sequence.
        
        Args:
            sequence (Sequence[float]): Iterable containing [angle, radius].
        
        Raises:
            RuntimeError: If the sequence does not contain exactly two elements.
        """
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: PolarCoordinate) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setitem__(self, index: typing.SupportsInt, value: typing.SupportsFloat) -> None:
        ...
    def __str__(self) -> str:
        ...
    def to_cartesian(self) -> Vec2:
        """
        Convert this PolarCoordinate to a Vec2.
        
        Returns:
            Vec2: Cartesian representation of this coordinate.
        """
    @property
    def angle(self) -> float:
        """
        The angle component in radians.
        """
    @angle.setter
    def angle(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def radius(self) -> float:
        """
        The radius component (distance from origin).
        """
    @radius.setter
    def radius(self, arg0: typing.SupportsFloat) -> None:
        ...
class Polygon:
    """
    
    Represents a polygon shape defined by a sequence of points.
    
    A polygon is a closed shape made up of connected line segments. The points define
    the vertices of the polygon in order. Supports various geometric operations.
        
    """
    def __getitem__(self, index: typing.SupportsInt) -> Vec2:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Create an empty polygon with no points.
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[Vec2]) -> None:
        """
        Create a polygon from a vector of Vec2 points.
        
        Args:
            points (list[Vec2]): List of Vec2 points defining the polygon vertices.
        """
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def copy(self) -> Polygon:
        """
        Return a copy of the polygon.
        
        Returns:
            Polygon: A new polygon with the same points.
        """
    def rotate(self, angle: typing.SupportsFloat, pivot: Vec2) -> None:
        """
        Rotate the polygon around a pivot point.
        
        Args:
            angle (float): The rotation angle in radians.
            pivot (Vec2): The point to rotate around.
        """
    @typing.overload
    def scale(self, factor: typing.SupportsFloat, pivot: Vec2) -> None:
        """
        Scale the polygon uniformly from a pivot point.
        
        Args:
            factor (float): The scaling factor.
            pivot (Vec2): The point to scale from.
        """
    @typing.overload
    def scale(self, factor: Vec2, pivot: Vec2) -> None:
        """
        Scale the polygon non-uniformly from a pivot point.
        
        Args:
            factor (Vec2): The scaling factors for x and y.
            pivot (Vec2): The point to scale from.
        """
    def translate(self, offset: Vec2) -> None:
        """
        Move the polygon by an offset.
        
        Args:
            offset (Vec2): The offset to move by.
        """
    @property
    def area(self) -> float:
        """
        Get the area of the polygon.
        
        Returns:
            float: The area enclosed by the polygon.
        """
    @property
    def bounds(self) -> Rect:
        """
        Get the bounding rectangle of the polygon.
        
        Returns:
            Rect: The smallest rectangle that contains the polygon.
        """
    @property
    def centroid(self) -> Vec2:
        """
        Get the centroid of the polygon.
        
        Returns:
            Vec2: The center point of the polygon.
        """
    @property
    def perimeter(self) -> float:
        """
        Get the perimeter of the polygon.
        
        Returns:
            float: The total distance around the polygon.
        """
    @property
    def points(self) -> list[Vec2]:
        """
        The list of Vec2 points that define the polygon vertices.
        """
    @points.setter
    def points(self, arg0: collections.abc.Sequence[Vec2]) -> None:
        ...
class Rect:
    """
    
    Represents a rectangle with position and size.
    
    A Rect is defined by its top-left corner position (x, y) and dimensions (w, h).
    Supports various geometric operations, collision detection, and positioning methods.
            
    """
    __hash__: typing.ClassVar[None] = None
    def __bool__(self) -> bool:
        ...
    def __eq__(self, other: Rect) -> bool:
        ...
    def __getitem__(self, index: typing.SupportsInt) -> float:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Create a Rect with default values (0, 0, 0, 0).
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, w: typing.SupportsFloat, h: typing.SupportsFloat) -> None:
        """
        Create a Rect with specified position and dimensions.
        
        Args:
            x (float): The x coordinate of the top-left corner.
            y (float): The y coordinate of the top-left corner.
            w (float): The width of the rectangle.
            h (float): The height of the rectangle.
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, size: Vec2) -> None:
        """
        Create a Rect with specified position and size vector.
        
        Args:
            x (float): The x coordinate of the top-left corner.
            y (float): The y coordinate of the top-left corner.
            size (Vec2): The size as a Vec2 (width, height).
        """
    @typing.overload
    def __init__(self, pos: Vec2, w: typing.SupportsFloat, h: typing.SupportsFloat) -> None:
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
    def __init__(self, arg0: collections.abc.Sequence) -> None:
        """
        Create a Rect from a sequence of four elements.
        
        Args:
            sequence: A sequence (list, tuple) containing [x, y, w, h].
        
        Raises:
            RuntimeError: If sequence doesn't contain exactly 4 elements.
        """
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, other: Rect) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __str__(self) -> str:
        ...
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
    def scale_by(self, factor: typing.SupportsFloat) -> None:
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
    def bottom(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def bottom_left(self) -> Vec2:
        """
        The position of the bottom-left corner as (x, y).
        """
    @bottom_left.setter
    def bottom_left(self, arg1: Vec2) -> None:
        ...
    @property
    def bottom_mid(self) -> Vec2:
        """
        The position of the bottom-middle point as (x, y).
        """
    @bottom_mid.setter
    def bottom_mid(self, arg1: Vec2) -> None:
        ...
    @property
    def bottom_right(self) -> Vec2:
        """
        The position of the bottom-right corner as (x, y).
        """
    @bottom_right.setter
    def bottom_right(self, arg1: Vec2) -> None:
        ...
    @property
    def center(self) -> Vec2:
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
    def h(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def left(self) -> float:
        """
        The x coordinate of the left edge.
        """
    @left.setter
    def left(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def mid_left(self) -> Vec2:
        """
        The position of the middle-left point as (x, y).
        """
    @mid_left.setter
    def mid_left(self, arg1: Vec2) -> None:
        ...
    @property
    def mid_right(self) -> Vec2:
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
    def right(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def size(self) -> Vec2:
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
    def top(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def top_left(self) -> Vec2:
        """
        The position of the top-left corner as (x, y).
        """
    @top_left.setter
    def top_left(self, arg1: Vec2) -> None:
        ...
    @property
    def top_mid(self) -> Vec2:
        """
        The position of the top-middle point as (x, y).
        """
    @top_mid.setter
    def top_mid(self, arg1: Vec2) -> None:
        ...
    @property
    def top_right(self) -> Vec2:
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
    def w(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def x(self) -> float:
        """
        The x coordinate of the top-left corner.
        """
    @x.setter
    def x(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        """
        The y coordinate of the top-left corner.
        """
    @y.setter
    def y(self, arg0: typing.SupportsFloat) -> None:
        ...
class Scancode(enum.IntEnum):
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
    S_APPLICATION: typing.ClassVar[Scancode]  # value = <Scancode.S_APPLICATION: 101>
    S_BACKSLASH: typing.ClassVar[Scancode]  # value = <Scancode.S_BACKSLASH: 49>
    S_BACKSPACE: typing.ClassVar[Scancode]  # value = <Scancode.S_BACKSPACE: 42>
    S_CALL: typing.ClassVar[Scancode]  # value = <Scancode.S_CALL: 289>
    S_CAPS: typing.ClassVar[Scancode]  # value = <Scancode.S_CAPS: 57>
    S_CHANNEL_DEC: typing.ClassVar[Scancode]  # value = <Scancode.S_CHANNEL_DEC: 261>
    S_CHANNEL_INC: typing.ClassVar[Scancode]  # value = <Scancode.S_CHANNEL_INC: 260>
    S_COMMA: typing.ClassVar[Scancode]  # value = <Scancode.S_COMMA: 54>
    S_COPY: typing.ClassVar[Scancode]  # value = <Scancode.S_COPY: 124>
    S_CUT: typing.ClassVar[Scancode]  # value = <Scancode.S_CUT: 123>
    S_DEL: typing.ClassVar[Scancode]  # value = <Scancode.S_DEL: 76>
    S_DOWN: typing.ClassVar[Scancode]  # value = <Scancode.S_DOWN: 81>
    S_END: typing.ClassVar[Scancode]  # value = <Scancode.S_END: 77>
    S_ENDCALL: typing.ClassVar[Scancode]  # value = <Scancode.S_ENDCALL: 290>
    S_EQ: typing.ClassVar[Scancode]  # value = <Scancode.S_EQ: 46>
    S_ESC: typing.ClassVar[Scancode]  # value = <Scancode.S_ESC: 41>
    S_EXECUTE: typing.ClassVar[Scancode]  # value = <Scancode.S_EXECUTE: 116>
    S_F1: typing.ClassVar[Scancode]  # value = <Scancode.S_F1: 58>
    S_F10: typing.ClassVar[Scancode]  # value = <Scancode.S_F10: 67>
    S_F11: typing.ClassVar[Scancode]  # value = <Scancode.S_F11: 68>
    S_F12: typing.ClassVar[Scancode]  # value = <Scancode.S_F12: 69>
    S_F13: typing.ClassVar[Scancode]  # value = <Scancode.S_F13: 104>
    S_F14: typing.ClassVar[Scancode]  # value = <Scancode.S_F14: 105>
    S_F15: typing.ClassVar[Scancode]  # value = <Scancode.S_F15: 106>
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
    S_HELP: typing.ClassVar[Scancode]  # value = <Scancode.S_HELP: 117>
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
    S_MEDIA_EJECT: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_EJECT: 270>
    S_MEDIA_FAST_FORWARD: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_FAST_FORWARD: 265>
    S_MEDIA_NEXT: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_NEXT: 267>
    S_MEDIA_PAUSE: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_PAUSE: 263>
    S_MEDIA_PLAY: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_PLAY: 262>
    S_MEDIA_PLAY_PAUSE: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_PLAY_PAUSE: 271>
    S_MEDIA_PREV: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_PREV: 268>
    S_MEDIA_REC: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_REC: 264>
    S_MEDIA_REWIND: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_REWIND: 266>
    S_MEDIA_SELECT: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_SELECT: 272>
    S_MEDIA_STOP: typing.ClassVar[Scancode]  # value = <Scancode.S_MEDIA_STOP: 269>
    S_MENU: typing.ClassVar[Scancode]  # value = <Scancode.S_MENU: 118>
    S_MINUS: typing.ClassVar[Scancode]  # value = <Scancode.S_MINUS: 45>
    S_MODE: typing.ClassVar[Scancode]  # value = <Scancode.S_MODE: 257>
    S_MUTE: typing.ClassVar[Scancode]  # value = <Scancode.S_MUTE: 127>
    S_NUMLOCK: typing.ClassVar[Scancode]  # value = <Scancode.S_NUMLOCK: 83>
    S_PASTE: typing.ClassVar[Scancode]  # value = <Scancode.S_PASTE: 125>
    S_PAUSE: typing.ClassVar[Scancode]  # value = <Scancode.S_PAUSE: 72>
    S_PERIOD: typing.ClassVar[Scancode]  # value = <Scancode.S_PERIOD: 55>
    S_PGDOWN: typing.ClassVar[Scancode]  # value = <Scancode.S_PGDOWN: 78>
    S_PGUP: typing.ClassVar[Scancode]  # value = <Scancode.S_PGUP: 75>
    S_POWER: typing.ClassVar[Scancode]  # value = <Scancode.S_POWER: 102>
    S_PRTSCR: typing.ClassVar[Scancode]  # value = <Scancode.S_PRTSCR: 70>
    S_RALT: typing.ClassVar[Scancode]  # value = <Scancode.S_RALT: 230>
    S_RBRACKET: typing.ClassVar[Scancode]  # value = <Scancode.S_RBRACKET: 48>
    S_RCTRL: typing.ClassVar[Scancode]  # value = <Scancode.S_RCTRL: 228>
    S_RETURN: typing.ClassVar[Scancode]  # value = <Scancode.S_RETURN: 40>
    S_RGUI: typing.ClassVar[Scancode]  # value = <Scancode.S_RGUI: 231>
    S_RIGHT: typing.ClassVar[Scancode]  # value = <Scancode.S_RIGHT: 79>
    S_RSHIFT: typing.ClassVar[Scancode]  # value = <Scancode.S_RSHIFT: 229>
    S_SCRLK: typing.ClassVar[Scancode]  # value = <Scancode.S_SCRLK: 71>
    S_SELECT: typing.ClassVar[Scancode]  # value = <Scancode.S_SELECT: 119>
    S_SEMICOLON: typing.ClassVar[Scancode]  # value = <Scancode.S_SEMICOLON: 51>
    S_SLASH: typing.ClassVar[Scancode]  # value = <Scancode.S_SLASH: 56>
    S_SLEEP: typing.ClassVar[Scancode]  # value = <Scancode.S_SLEEP: 258>
    S_SOFTLEFT: typing.ClassVar[Scancode]  # value = <Scancode.S_SOFTLEFT: 287>
    S_SOFTRIGHT: typing.ClassVar[Scancode]  # value = <Scancode.S_SOFTRIGHT: 288>
    S_SPACE: typing.ClassVar[Scancode]  # value = <Scancode.S_SPACE: 44>
    S_STOP: typing.ClassVar[Scancode]  # value = <Scancode.S_STOP: 120>
    S_TAB: typing.ClassVar[Scancode]  # value = <Scancode.S_TAB: 43>
    S_UNDO: typing.ClassVar[Scancode]  # value = <Scancode.S_UNDO: 122>
    S_UP: typing.ClassVar[Scancode]  # value = <Scancode.S_UP: 82>
    S_VOLDOWN: typing.ClassVar[Scancode]  # value = <Scancode.S_VOLDOWN: 129>
    S_VOLUP: typing.ClassVar[Scancode]  # value = <Scancode.S_VOLUP: 128>
    S_WAKE: typing.ClassVar[Scancode]  # value = <Scancode.S_WAKE: 259>
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
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class ScrollMode(enum.IntEnum):
    ERASE: typing.ClassVar[ScrollMode]  # value = <ScrollMode.ERASE: 1>
    REPEAT: typing.ClassVar[ScrollMode]  # value = <ScrollMode.REPEAT: 2>
    SMEAR: typing.ClassVar[ScrollMode]  # value = <ScrollMode.SMEAR: 0>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class ShaderState:
    """
    Encapsulates a GPU shader and its associated render state.
    """
    def __init__(self, fragment_file_path: str, uniform_buffer_count: typing.SupportsInt = 0) -> None:
        """
        Create a ShaderState from the specified fragment shader file.
        
        Args:
            fragment_file_path (str): Path to the fragment shader file.
            uniform_buffer_count (int, optional): Number of uniform buffers used by the shader. Default is 0.
        """
    def bind(self) -> None:
        """
        Binds this shader state to the current render pass, making it active for subsequent draw calls.
        """
    def set_uniform(self, binding: typing.SupportsInt, data: collections.abc.Buffer) -> None:
        """
        Set uniform data for the fragment shader at the specified binding point.
        
        Args:
            binding (int): Uniform buffer binding index.
            data (buffer): Buffer or bytes object containing the uniform data to upload.
        """
    def unbind(self) -> None:
        """
        Unbinds the current shader state, reverting to the default render state.
        """
class SheetStrip:
    """
    
    A descriptor for one horizontal strip (row) in a sprite sheet.
    
    Defines a single animation within a sprite sheet by specifying the animation name,
    the number of frames to extract from the strip, and the playback speed in frames
    per second (FPS).
        
    """
    def __init__(self, name: str, frame_count: typing.SupportsInt, fps: typing.SupportsInt) -> None:
        """
        Create a sprite sheet strip definition.
        
        Args:
            name (str): Unique identifier for this animation.
            frame_count (int): Number of frames to extract from this strip/row.
            fps (int): Frames per second for playback timing.
        """
    @property
    def fps(self) -> int:
        """
        The playback speed in frames per second.
        
        Determines how fast the animation plays. Higher values result in faster playback.
        
        Type:
            int: The frames per second for this animation.
        """
    @fps.setter
    def fps(self, arg0: typing.SupportsInt) -> None:
        ...
    @property
    def frame_count(self) -> int:
        """
        The number of frames in this animation strip.
        
        Specifies how many frames to extract from the horizontal strip in the sprite sheet,
        reading from left to right.
        
        Type:
            int: The number of frames (must be positive).
        """
    @frame_count.setter
    def frame_count(self, arg0: typing.SupportsInt) -> None:
        ...
    @property
    def name(self) -> str:
        """
        The unique name identifier for this animation strip.
        
        Type:
            str: The animation name used to reference this strip.
        """
    @name.setter
    def name(self, arg0: str) -> None:
        ...
class Text:
    """
    
    A text object for rendering text to the active renderer.
    
    This class handles the rendered text instance. You must provide a Font object
    when creating a Text instance.
    
    Note:
        A window/renderer must be created before using text. Typically you should
        call kn.window.create(...) first, which initializes the text engine.
        
    """
    def __init__(self, font: Font) -> None:
        """
        Create a Text object.
        
        Args:
            font (Font): The font to use for rendering this text.
        
        Raises:
            RuntimeError: If text creation fails.
        """
    def draw(self, pos: typing.Any = None, anchor: Anchor = Anchor.TOP_LEFT) -> None:
        """
        Draw the text to the renderer at the specified position with alignment.
        
        Args:
            pos (Vec2 | None): The position in pixels. Defaults to (0, 0).
            anchor (Anchor): The anchor point for alignment. Defaults to TopLeft.
        """
    def get_rect(self) -> Rect:
        """
        Get the bounding rectangle of the current text.
        
        Returns:
            Rect: A rectangle with x=0, y=0, and width/height of the text.
        """
    def set_font(self, font: Font) -> None:
        """
        Set the font to use for rendering this text.
        
        Args:
            font (Font): The font to use.
        """
    @property
    def color(self) -> Color:
        """
        Get or set the color of the rendered text.
        """
    @color.setter
    def color(self, arg1: Color) -> None:
        ...
    @property
    def height(self) -> int:
        """
        Get the height in pixels of the current text.
        
        Returns:
            int: The text height.
        """
    @property
    def size(self) -> Vec2:
        """
        Get the size (width, height) of the current text as a Vec2.
        
        Returns:
            Vec2: The text dimensions.
        """
    @property
    def text(self) -> str:
        """
        Get or set the text string to be rendered.
        """
    @text.setter
    def text(self, arg1: str) -> None:
        ...
    @property
    def width(self) -> int:
        """
        Get the width in pixels of the current text.
        
        Returns:
            int: The text width.
        """
    @property
    def wrap_width(self) -> int:
        """
        Get or set the wrap width in pixels for text wrapping.
        
        Set to 0 to disable wrapping. Negative values are clamped to 0.
        """
    @wrap_width.setter
    def wrap_width(self, arg1: typing.SupportsInt) -> None:
        ...
class Texture:
    """
    
    Represents a hardware-accelerated image that can be efficiently rendered.
    
    Textures are optimized for fast rendering operations and support various effects
    like rotation, flipping, tinting, alpha blending, and different blend modes.
    They can be created from image files or pixel arrays.
        
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
    def __init__(self, file_path: str) -> None:
        """
        Create a Texture by loading an image from a file.
        
        Args:
            file_path (str): Path to the image file to load.
        
        Raises:
            ValueError: If file_path is empty.
            RuntimeError: If the file cannot be loaded or texture creation fails.
        """
    @typing.overload
    def __init__(self, pixel_array: PixelArray) -> None:
        """
        Create a Texture from an existing PixelArray.
        
        Args:
            pixel_array (PixelArray): The pixel array to convert to a texture.
        
        Raises:
            RuntimeError: If texture creation from pixel array fails.
        """
    def get_rect(self) -> Rect:
        """
        Get a rectangle representing the texture bounds.
        
        Returns:
            Rect: A rectangle with position (0, 0) and the texture's dimensions.
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
    @property
    def alpha(self) -> float:
        """
        Get or set the alpha modulation of the texture as a float between `0.0` and `1.0`.
        """
    @alpha.setter
    def alpha(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def angle(self) -> float:
        """
        The rotation angle in radians for rendering.
        
        When the texture is drawn, it will be rotated by this angle about its center.
        """
    @angle.setter
    def angle(self, arg0: typing.SupportsFloat) -> None:
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
    @property
    def size(self) -> Vec2:
        """
        Get the size of the texture.
        
        Returns:
            Vec2: The texture size as (width, height).
        """
    @property
    def tint(self) -> Color:
        """
        Get or set the color tint applied to the texture during rendering.
        """
    @tint.setter
    def tint(self, arg1: Color) -> None:
        ...
class Tile:
    """
    
    Represents a single tile instance in a layer.
        
    """
    @property
    def angle(self) -> float:
        """
        The rotation angle in radians.
        """
    @property
    def anti_diag_flip(self) -> bool:
        """
        Whether the tile is flipped across the anti-diagonal.
        """
    @property
    def collider(self) -> Rect:
        """
        The fitted collider rectangle for the tile's opaque area.
        """
    @property
    def dst(self) -> Rect:
        """
        The destination rectangle on the map.
        """
    @property
    def h_flip(self) -> bool:
        """
        Whether the tile is flipped horizontally.
        """
    @property
    def layer(self) -> TileLayer:
        """
        Get the owning Layer.
        
        Returns:
            Layer | None: The owning Layer if it still exists; otherwise None.
        """
    @property
    def src(self) -> Rect:
        """
        The source rectangle within the tileset texture.
        """
    @property
    def v_flip(self) -> bool:
        """
        Whether the tile is flipped vertically.
        """
class TileLayer:
    """
    
    A layer of a tile map.
    
    Layers can be either tile layers or object layers and contain a list of tiles.
        
    """
    class Type(enum.IntEnum):
        """
        
        The type of a Layer.
            
        """
        OBJECT: typing.ClassVar[TileLayer.Type]  # value = <Type.OBJECT: 0>
        TILE: typing.ClassVar[TileLayer.Type]  # value = <Type.TILE: 1>
        @classmethod
        def __new__(cls, value):
            ...
        def __format__(self, format_spec):
            """
            Convert to a string according to format_spec.
            """
    OBJECT: typing.ClassVar[TileLayer.Type]  # value = <Type.OBJECT: 0>
    TILE: typing.ClassVar[TileLayer.Type]  # value = <Type.TILE: 1>
    def get_from_area(self, area: Rect) -> list[Tile]:
        """
        Get all tiles whose destination rectangles fall within a query rect.
        
        Args:
            area (Rect): The world-space rectangle to test.
        
        Returns:
            list[Tile]: Tiles intersecting the given area.
        """
    def get_tile_at(self, column: typing.SupportsInt, row: typing.SupportsInt) -> pykraken._core.Tile | None:
        """
        Get the tile located at the specified grid coordinates.
        
        Args:
            column (int): The tile column index.
            row (int): The tile row index.
        
        Returns:
            Tile | None: The tile at the grid coordinate, or None if empty/out of range.
        """
    def render(self) -> None:
        """
        Render the layer.
        """
    @property
    def is_visible(self) -> bool:
        """
        Whether the layer is visible.
        """
    @property
    def name(self) -> str:
        """
        The name of the layer.
        """
    @property
    def tiles(self) -> list[Tile]:
        """
        The list of Tile instances contained in this layer.
        """
    @property
    def type(self) -> TileLayer.Type:
        """
        The layer type (OBJECT or TILE).
        """
class TileMap:
    """
    
    Loads and renders TMX tile maps.
    
    Parses a Tiled TMX file, loads the tileset texture, and exposes layers and tiles for rendering and queries.
        
    """
    @staticmethod
    def get_tile_collection(layers: collections.abc.Sequence[TileLayer]) -> list[Tile]:
        """
        Collect all tiles from the provided layers into a single list.
        
        Args:
            layers (Sequence[Layer]): The layers to collect tiles from.
        
        Returns:
            list[Tile]: A flat list of tiles from the given layers.
        """
    def __init__(self, tmx_path: str, border_size: typing.SupportsInt = 0) -> None:
        """
        Create a TileMap by loading a TMX file.
        
        Args:
            tmx_path (str): Path to the TMX file.
            border_size (int): Optional border (in pixels) around each tile in the tileset; defaults to 0.
        
        Raises:
            RuntimeError: If the TMX or TSX files cannot be loaded or parsed.
        """
    def get_layer(self, name: str, type: TileLayer.Type = TileLayer.Type.TILE) -> TileLayer:
        """
        Get a layer by name and type.
        
        Args:
            name (str): The layer name.
            type (Layer.Type): The expected layer type (defaults to TILE).
        
        Returns:
            Layer: The matching layer.
        
        Raises:
            ValueError: If no matching layer is found or the type doesn't match.
        """
    def get_layers(self) -> list[TileLayer]:
        """
        Get all layers in the map.
        
        Returns:
            list[Layer]: A list of all layers.
        """
    def render(self) -> None:
        """
        Render all visible layers.
        """
class Timer:
    """
    
    A timer for tracking countdown durations with pause/resume functionality.
    
    The Timer class provides a simple countdown timer that can be started, paused,
    and resumed. It's useful for implementing time-based game mechanics like
    cooldowns, temporary effects, or timed events.
        
    """
    def __init__(self, duration: typing.SupportsFloat) -> None:
        """
        Create a new Timer instance with the specified duration.
        
        Args:
            duration (float): The countdown duration in seconds. Must be greater than 0.
        
        Raises:
            RuntimeError: If duration is less than or equal to 0.
        """
    def pause(self) -> None:
        """
        Pause the timer countdown.
        
        The timer will stop counting down but retain its current state. Use resume()
        to continue the countdown from where it was paused. Has no effect if the
        timer is not started or already paused.
        """
    def reset(self) -> None:
        """
        Reset the timer to its initial state.
        
        Stops the timer and resets it back to its initial, unstarted state.
        The timer can be started again with `start()` after being reset.
        """
    def resume(self) -> None:
        """
        Resume a paused timer countdown.
        
        Continues the countdown from where it was paused. Has no effect if the
        timer is not started or not currently paused.
        """
    def start(self) -> None:
        """
        Start or restart the timer countdown.
        
        This begins the countdown from the full duration. If the timer was previously
        started, this will reset it back to the beginning.
        """
    @property
    def done(self) -> bool:
        """
        bool: True if the timer has finished counting down, False otherwise.
        
        A timer is considered done when the elapsed time since start (excluding
        paused time) equals or exceeds the specified duration.
        """
    @property
    def elapsed_time(self) -> float:
        """
        float: The time elapsed since the timer was started, in seconds.
        
        Returns 0.0 if the timer hasn't been started. This includes time spent
        while paused, giving you the total wall-clock time since start().
        """
    @property
    def progress(self) -> float:
        """
        float: The completion progress of the timer as a value between 0.0 and 1.0.
        
        Returns 0.0 if the timer hasn't been started, and 1.0 when the timer
        is complete. Useful for progress bars and interpolated animations.
        """
    @property
    def time_remaining(self) -> float:
        """
        float: The remaining time in seconds before the timer completes.
        
        Returns the full duration if the timer hasn't been started, or 0.0 if
        the timer has already finished.
        """
class Vec2:
    """
    
    Vec2 represents a 2D vector.
    
    Attributes:
        x (float): Horizontal component.
        y (float): Vertical component.
    
    Methods:
        copy: Return a duplicated Vec2.
        is_zero: Test whether components are near zero.
        rotate: Rotate the vector in place.
        to_polar: Convert the vector to a PolarCoordinate.
        scale_to_length: Scale the vector to a specific length.
        project: Project onto another Vec2.
        reject: Remove the projection onto another Vec2.
        reflect: Reflect across another Vec2.
        normalize: Normalize the vector in place.
        distance_to: Measure distance to another Vec2.
        distance_squared_to: Measure squared distance to another Vec2.
            
    """
    def __add__(self, other: Vec2) -> Vec2:
        ...
    def __bool__(self) -> bool:
        ...
    def __eq__(self, other: Vec2) -> bool:
        ...
    def __ge__(self, other: Vec2) -> bool:
        ...
    def __getitem__(self, index: typing.SupportsInt) -> float:
        ...
    def __gt__(self, other: Vec2) -> bool:
        ...
    def __hash__(self) -> int:
        ...
    def __iadd__(self, other: Vec2) -> Vec2:
        ...
    def __imul__(self, scalar: typing.SupportsFloat) -> Vec2:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Initialize a Vec2 with zeroed components.
        """
    @typing.overload
    def __init__(self, value: typing.SupportsFloat) -> None:
        """
        Initialize a Vec2 with identical x and y values.
        
        Args:
            value (float): Value assigned to both components.
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> None:
        """
        Initialize a Vec2 with explicit component values.
        
        Args:
            x (float): Horizontal component.
            y (float): Vertical component.
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Sequence) -> None:
        """
        Initialize a Vec2 from a two-item sequence.
        
        Args:
            sequence (Sequence[float]): Iterable containing [x, y].
        
        Raises:
            RuntimeError: If the sequence does not contain exactly two elements.
        """
    def __isub__(self, other: Vec2) -> Vec2:
        ...
    def __iter__(self) -> collections.abc.Iterator:
        ...
    def __itruediv__(self, scalar: typing.SupportsFloat) -> Vec2:
        ...
    def __le__(self, other: Vec2) -> bool:
        ...
    def __len__(self) -> int:
        ...
    def __lt__(self, other: Vec2) -> bool:
        ...
    def __mul__(self, scalar: typing.SupportsFloat) -> Vec2:
        ...
    def __ne__(self, other: Vec2) -> bool:
        ...
    def __neg__(self) -> Vec2:
        ...
    def __radd__(self, other: Vec2) -> Vec2:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, scalar: typing.SupportsFloat) -> Vec2:
        ...
    def __rsub__(self, other: Vec2) -> Vec2:
        ...
    def __setitem__(self, index: typing.SupportsInt, value: typing.SupportsFloat) -> None:
        ...
    def __str__(self) -> str:
        ...
    def __sub__(self, other: Vec2) -> Vec2:
        ...
    def __truediv__(self, scalar: typing.SupportsFloat) -> Vec2:
        ...
    def copy(self) -> Vec2:
        """
        Return a copy of this Vec2.
        
        Returns:
            Vec2: A duplicated vector with the same components.
        """
    def distance_squared_to(self, other: Vec2) -> float:
        """
        Compute the squared distance to another Vec2.
        
        Args:
            other (Vec2): Comparison vector.
        
        Returns:
            float: Squared distance between the vectors.
        """
    def distance_to(self, other: Vec2) -> float:
        """
        Compute the Euclidean distance to another Vec2.
        
        Args:
            other (Vec2): Comparison vector.
        
        Returns:
            float: Distance between the vectors.
        """
    def is_zero(self, tolerance: typing.SupportsFloat = 1e-08) -> bool:
        """
        Determine whether this Vec2 is effectively zero.
        
        Args:
            tolerance (float): Largest allowed absolute component magnitude.
        
        Returns:
            bool: True if both components are within the tolerance.
        """
    def normalize(self) -> None:
        """
        Normalize this Vec2 in place.
        """
    def project(self, other: Vec2) -> Vec2:
        """
        Project this Vec2 onto another Vec2.
        
        Args:
            other (Vec2): The vector to project onto.
        
        Returns:
            Vec2: Projection of this vector onto the other vector.
        """
    def reflect(self, other: Vec2) -> Vec2:
        """
        Reflect this Vec2 across another Vec2.
        
        Args:
            other (Vec2): The vector used as the reflection normal.
        
        Returns:
            Vec2: Reflected vector.
        """
    def reject(self, other: Vec2) -> Vec2:
        """
        Compute the rejection of this Vec2 from another Vec2.
        
        Args:
            other (Vec2): The vector defining the projection axis.
        
        Returns:
            Vec2: Component of this vector orthogonal to the other vector.
        """
    def rotate(self, radians: typing.SupportsFloat) -> None:
        """
        Rotate this Vec2 in place.
        
        Args:
            radians (float): Rotation angle in radians.
        """
    def scale_to_length(self, length: typing.SupportsFloat) -> None:
        """
        Scale this Vec2 to a specific magnitude.
        
        Args:
            length (float): Target vector length.
        """
    def to_polar(self) -> PolarCoordinate:
        """
        Convert this Vec2 to polar coordinates.
        
        Returns:
            PolarCoordinate: Polar representation with angle and length.
        """
    @property
    def angle(self) -> float:
        """
        Return the vector angle in radians.
        
        Returns:
            float: Angle measured from the positive x-axis.
        """
    @property
    def length(self) -> float:
        """
        Return the magnitude of this Vec2.
        
        Returns:
            float: Euclidean length of the vector.
        """
    @property
    def length_squared(self) -> float:
        """
        Return the squared magnitude of this Vec2.
        
        Returns:
            float: Squared Euclidean length.
        """
    @property
    def x(self) -> float:
        """
        The x component of the vector.
        """
    @x.setter
    def x(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def xx(self) -> Vec2:
        """
        Return a Vec2 with both components set to x.
        
        Returns:
            Vec2: Vector composed of (x, x).
        """
    @property
    def xy(self) -> Vec2:
        """
        Access or assign the (x, y) components as a Vec2.
        
        Returns:
            Vec2: Current (x, y) components.
        """
    @xy.setter
    def xy(self, arg1: typing.SupportsFloat, arg2: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        """
        The y component of the vector.
        """
    @y.setter
    def y(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def yx(self) -> Vec2:
        """
        Access or assign the (y, x) components as a Vec2.
        
        Returns:
            Vec2: Current (y, x) components.
        """
    @yx.setter
    def yx(self, arg1: typing.SupportsFloat, arg2: typing.SupportsFloat) -> None:
        ...
    @property
    def yy(self) -> Vec2:
        """
        Return a Vec2 with both components set to y.
        
        Returns:
            Vec2: Vector composed of (y, y).
        """
class ViewportMode(enum.IntEnum):
    HORIZONTAL: typing.ClassVar[ViewportMode]  # value = <ViewportMode.HORIZONTAL: 1>
    VERTICAL: typing.ClassVar[ViewportMode]  # value = <ViewportMode.VERTICAL: 0>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
def init() -> None:
    """
    Initialize the Kraken engine subsystems.
    
    Raises:
        RuntimeError: If SDL initialization fails.
    """
def quit() -> None:
    """
    Tear down the Kraken engine subsystems.
    """
AUDIO_DEVICE_ADDED: EventType  # value = <EventType.AUDIO_DEVICE_ADDED: 4352>
AUDIO_DEVICE_FORMAT_CHANGED: EventType  # value = <EventType.AUDIO_DEVICE_FORMAT_CHANGED: 4354>
AUDIO_DEVICE_REMOVED: EventType  # value = <EventType.AUDIO_DEVICE_REMOVED: 4353>
CAMERA_DEVICE_ADDED: EventType  # value = <EventType.CAMERA_DEVICE_ADDED: 5120>
CAMERA_DEVICE_APPROVED: EventType  # value = <EventType.CAMERA_DEVICE_APPROVED: 5122>
CAMERA_DEVICE_DENIED: EventType  # value = <EventType.CAMERA_DEVICE_DENIED: 5123>
CAMERA_DEVICE_REMOVED: EventType  # value = <EventType.CAMERA_DEVICE_REMOVED: 5121>
CLIPBOARD_UPDATE: EventType  # value = <EventType.CLIPBOARD_UPDATE: 2304>
C_BACK: GamepadButton  # value = <GamepadButton.C_BACK: 4>
C_DPAD_DOWN: GamepadButton  # value = <GamepadButton.C_DPAD_DOWN: 12>
C_DPAD_LEFT: GamepadButton  # value = <GamepadButton.C_DPAD_LEFT: 13>
C_DPAD_RIGHT: GamepadButton  # value = <GamepadButton.C_DPAD_RIGHT: 14>
C_DPAD_UP: GamepadButton  # value = <GamepadButton.C_DPAD_UP: 11>
C_EAST: GamepadButton  # value = <GamepadButton.C_EAST: 1>
C_GUIDE: GamepadButton  # value = <GamepadButton.C_GUIDE: 5>
C_LSHOULDER: GamepadButton  # value = <GamepadButton.C_LSHOULDER: 9>
C_LSTICK: GamepadButton  # value = <GamepadButton.C_LSTICK: 7>
C_LTRIGGER: GamepadAxis  # value = <GamepadAxis.C_LTRIGGER: 4>
C_LX: GamepadAxis  # value = <GamepadAxis.C_LX: 0>
C_LY: GamepadAxis  # value = <GamepadAxis.C_LY: 1>
C_NORTH: GamepadButton  # value = <GamepadButton.C_NORTH: 3>
C_PS3: GamepadType  # value = <GamepadType.C_PS3: 4>
C_PS4: GamepadType  # value = <GamepadType.C_PS4: 5>
C_PS5: GamepadType  # value = <GamepadType.C_PS5: 6>
C_RSHOULDER: GamepadButton  # value = <GamepadButton.C_RSHOULDER: 10>
C_RSTICK: GamepadButton  # value = <GamepadButton.C_RSTICK: 8>
C_RTRIGGER: GamepadAxis  # value = <GamepadAxis.C_RTRIGGER: 5>
C_RX: GamepadAxis  # value = <GamepadAxis.C_RX: 2>
C_RY: GamepadAxis  # value = <GamepadAxis.C_RY: 3>
C_SOUTH: GamepadButton  # value = <GamepadButton.C_SOUTH: 0>
C_STANDARD: GamepadType  # value = <GamepadType.C_STANDARD: 1>
C_START: GamepadButton  # value = <GamepadButton.C_START: 6>
C_SWITCH_JOYCON_LEFT: GamepadType  # value = <GamepadType.C_SWITCH_JOYCON_LEFT: 8>
C_SWITCH_JOYCON_PAIR: GamepadType  # value = <GamepadType.C_SWITCH_JOYCON_PAIR: 10>
C_SWITCH_JOYCON_RIGHT: GamepadType  # value = <GamepadType.C_SWITCH_JOYCON_RIGHT: 9>
C_SWITCH_PRO: GamepadType  # value = <GamepadType.C_SWITCH_PRO: 7>
C_WEST: GamepadButton  # value = <GamepadButton.C_WEST: 2>
C_XBOX_360: GamepadType  # value = <GamepadType.C_XBOX_360: 2>
C_XBOX_ONE: GamepadType  # value = <GamepadType.C_XBOX_ONE: 3>
DID_ENTER_BACKGROUND: EventType  # value = <EventType.DID_ENTER_BACKGROUND: 260>
DID_ENTER_FOREGROUND: EventType  # value = <EventType.DID_ENTER_FOREGROUND: 262>
DISPLAY_ADDED: EventType  # value = <EventType.DISPLAY_ADDED: 338>
DISPLAY_CONTENT_SCALE_CHANGED: EventType  # value = <EventType.DISPLAY_CONTENT_SCALE_CHANGED: 343>
DISPLAY_CURRENT_MODE_CHANGED: EventType  # value = <EventType.DISPLAY_CURRENT_MODE_CHANGED: 342>
DISPLAY_DESKTOP_MODE_CHANGED: EventType  # value = <EventType.DISPLAY_DESKTOP_MODE_CHANGED: 341>
DISPLAY_MOVED: EventType  # value = <EventType.DISPLAY_MOVED: 340>
DISPLAY_ORIENTATION: EventType  # value = <EventType.DISPLAY_ORIENTATION: 337>
DISPLAY_REMOVED: EventType  # value = <EventType.DISPLAY_REMOVED: 339>
DROP_BEGIN: EventType  # value = <EventType.DROP_BEGIN: 4098>
DROP_COMPLETE: EventType  # value = <EventType.DROP_COMPLETE: 4099>
DROP_FILE: EventType  # value = <EventType.DROP_FILE: 4096>
DROP_POSITION: EventType  # value = <EventType.DROP_POSITION: 4100>
DROP_TEXT: EventType  # value = <EventType.DROP_TEXT: 4097>
FINGER_CANCELED: EventType  # value = <EventType.FINGER_CANCELED: 1795>
FINGER_DOWN: EventType  # value = <EventType.FINGER_DOWN: 1792>
FINGER_MOTION: EventType  # value = <EventType.FINGER_MOTION: 1794>
FINGER_UP: EventType  # value = <EventType.FINGER_UP: 1793>
GAMEPAD_ADDED: EventType  # value = <EventType.GAMEPAD_ADDED: 1619>
GAMEPAD_AXIS_MOTION: EventType  # value = <EventType.GAMEPAD_AXIS_MOTION: 1616>
GAMEPAD_BUTTON_DOWN: EventType  # value = <EventType.GAMEPAD_BUTTON_DOWN: 1617>
GAMEPAD_BUTTON_UP: EventType  # value = <EventType.GAMEPAD_BUTTON_UP: 1618>
GAMEPAD_REMAPPED: EventType  # value = <EventType.GAMEPAD_REMAPPED: 1621>
GAMEPAD_REMOVED: EventType  # value = <EventType.GAMEPAD_REMOVED: 1620>
GAMEPAD_SENSOR_UPDATE: EventType  # value = <EventType.GAMEPAD_SENSOR_UPDATE: 1625>
GAMEPAD_STEAM_HANDLE_UPDATED: EventType  # value = <EventType.GAMEPAD_STEAM_HANDLE_UPDATED: 1627>
GAMEPAD_TOUCHPAD_DOWN: EventType  # value = <EventType.GAMEPAD_TOUCHPAD_DOWN: 1622>
GAMEPAD_TOUCHPAD_MOTION: EventType  # value = <EventType.GAMEPAD_TOUCHPAD_MOTION: 1623>
GAMEPAD_TOUCHPAD_UP: EventType  # value = <EventType.GAMEPAD_TOUCHPAD_UP: 1624>
GAMEPAD_UPDATE_COMPLETE: EventType  # value = <EventType.GAMEPAD_UPDATE_COMPLETE: 1626>
KEYBOARD_ADDED: EventType  # value = <EventType.KEYBOARD_ADDED: 773>
KEYBOARD_REMOVED: EventType  # value = <EventType.KEYBOARD_REMOVED: 774>
KEYMAP_CHANGED: EventType  # value = <EventType.KEYMAP_CHANGED: 772>
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
K_APPLICATION: Keycode  # value = <Keycode.K_APPLICATION: 1073741925>
K_ASTERISK: Keycode  # value = <Keycode.K_ASTERISK: 42>
K_AT: Keycode  # value = <Keycode.K_AT: 64>
K_BACKSLASH: Keycode  # value = <Keycode.K_BACKSLASH: 92>
K_BACKSPACE: Keycode  # value = <Keycode.K_BACKSPACE: 8>
K_CALL: Keycode  # value = <Keycode.K_CALL: 1073742113>
K_CAPS: Keycode  # value = <Keycode.K_CAPS: 1073741881>
K_CARET: Keycode  # value = <Keycode.K_CARET: 94>
K_CHANNEL_DEC: Keycode  # value = <Keycode.K_CHANNEL_DEC: 1073742085>
K_CHANNEL_INC: Keycode  # value = <Keycode.K_CHANNEL_INC: 1073742084>
K_COLON: Keycode  # value = <Keycode.K_COLON: 58>
K_COMMA: Keycode  # value = <Keycode.K_COMMA: 44>
K_COPY: Keycode  # value = <Keycode.K_COPY: 1073741948>
K_CUT: Keycode  # value = <Keycode.K_CUT: 1073741947>
K_DBLQUOTE: Keycode  # value = <Keycode.K_DBLQUOTE: 34>
K_DEL: Keycode  # value = <Keycode.K_DEL: 127>
K_DOLLAR: Keycode  # value = <Keycode.K_DOLLAR: 36>
K_DOWN: Keycode  # value = <Keycode.K_DOWN: 1073741905>
K_END: Keycode  # value = <Keycode.K_END: 1073741901>
K_ENDCALL: Keycode  # value = <Keycode.K_ENDCALL: 1073742114>
K_EQ: Keycode  # value = <Keycode.K_EQ: 61>
K_ESC: Keycode  # value = <Keycode.K_ESC: 27>
K_EXCLAIM: Keycode  # value = <Keycode.K_EXCLAIM: 33>
K_EXECUTE: Keycode  # value = <Keycode.K_EXECUTE: 1073741940>
K_F1: Keycode  # value = <Keycode.K_F1: 1073741882>
K_F10: Keycode  # value = <Keycode.K_F10: 1073741891>
K_F11: Keycode  # value = <Keycode.K_F11: 1073741892>
K_F12: Keycode  # value = <Keycode.K_F12: 1073741893>
K_F13: Keycode  # value = <Keycode.K_F13: 1073741928>
K_F14: Keycode  # value = <Keycode.K_F14: 1073741929>
K_F15: Keycode  # value = <Keycode.K_F15: 1073741930>
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
K_HELP: Keycode  # value = <Keycode.K_HELP: 1073741941>
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
K_MEDIA_EJECT: Keycode  # value = <Keycode.K_MEDIA_EJECT: 1073742094>
K_MEDIA_FF: Keycode  # value = <Keycode.K_MEDIA_FF: 1073742089>
K_MEDIA_NEXT: Keycode  # value = <Keycode.K_MEDIA_NEXT: 1073742091>
K_MEDIA_PAUSE: Keycode  # value = <Keycode.K_MEDIA_PAUSE: 1073742087>
K_MEDIA_PLAY: Keycode  # value = <Keycode.K_MEDIA_PLAY: 1073742086>
K_MEDIA_PLAY_PAUSE: Keycode  # value = <Keycode.K_MEDIA_PLAY_PAUSE: 1073742095>
K_MEDIA_PREV: Keycode  # value = <Keycode.K_MEDIA_PREV: 1073742092>
K_MEDIA_REC: Keycode  # value = <Keycode.K_MEDIA_REC: 1073742088>
K_MEDIA_REWIND: Keycode  # value = <Keycode.K_MEDIA_REWIND: 1073742090>
K_MEDIA_SELECT: Keycode  # value = <Keycode.K_MEDIA_SELECT: 1073742096>
K_MEDIA_STOP: Keycode  # value = <Keycode.K_MEDIA_STOP: 1073742093>
K_MENU: Keycode  # value = <Keycode.K_MENU: 1073741942>
K_MINUS: Keycode  # value = <Keycode.K_MINUS: 45>
K_MODE: Keycode  # value = <Keycode.K_MODE: 1073742081>
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
K_POWER: Keycode  # value = <Keycode.K_POWER: 1073741926>
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
K_SELECT: Keycode  # value = <Keycode.K_SELECT: 1073741943>
K_SEMICOLON: Keycode  # value = <Keycode.K_SEMICOLON: 59>
K_SGLQUOTE: Keycode  # value = <Keycode.K_SGLQUOTE: 39>
K_SLASH: Keycode  # value = <Keycode.K_SLASH: 47>
K_SLEEP: Keycode  # value = <Keycode.K_SLEEP: 1073742082>
K_SOFTLEFT: Keycode  # value = <Keycode.K_SOFTLEFT: 1073742111>
K_SOFTRIGHT: Keycode  # value = <Keycode.K_SOFTRIGHT: 1073742112>
K_SPACE: Keycode  # value = <Keycode.K_SPACE: 32>
K_STOP: Keycode  # value = <Keycode.K_STOP: 1073741944>
K_TAB: Keycode  # value = <Keycode.K_TAB: 9>
K_TILDE: Keycode  # value = <Keycode.K_TILDE: 126>
K_UNDERSCORE: Keycode  # value = <Keycode.K_UNDERSCORE: 95>
K_UNDO: Keycode  # value = <Keycode.K_UNDO: 1073741946>
K_UNKNOWN: Keycode  # value = <Keycode.K_UNKNOWN: 0>
K_UP: Keycode  # value = <Keycode.K_UP: 1073741906>
K_VOLDOWN: Keycode  # value = <Keycode.K_VOLDOWN: 1073741953>
K_VOLUP: Keycode  # value = <Keycode.K_VOLUP: 1073741952>
K_WAKE: Keycode  # value = <Keycode.K_WAKE: 1073742083>
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
LOCALE_CHANGED: EventType  # value = <EventType.LOCALE_CHANGED: 263>
LOW_MEMORY: EventType  # value = <EventType.LOW_MEMORY: 258>
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
P_DISTANCE: PenAxis  # value = <PenAxis.P_DISTANCE: 3>
P_PRESSURE: PenAxis  # value = <PenAxis.P_PRESSURE: 0>
P_ROTATION: PenAxis  # value = <PenAxis.P_ROTATION: 4>
P_SLIDER: PenAxis  # value = <PenAxis.P_SLIDER: 5>
P_TANGENTIAL_PRESSURE: PenAxis  # value = <PenAxis.P_TANGENTIAL_PRESSURE: 6>
P_TILT_X: PenAxis  # value = <PenAxis.P_TILT_X: 1>
P_TILT_Y: PenAxis  # value = <PenAxis.P_TILT_Y: 2>
QUIT: EventType  # value = <EventType.QUIT: 256>
RENDER_DEVICE_LOST: EventType  # value = <EventType.RENDER_DEVICE_LOST: 8194>
RENDER_DEVICE_RESET: EventType  # value = <EventType.RENDER_DEVICE_RESET: 8193>
RENDER_TARGETS_RESET: EventType  # value = <EventType.RENDER_TARGETS_RESET: 8192>
SENSOR_UPDATE: EventType  # value = <EventType.SENSOR_UPDATE: 4608>
SYSTEM_THEME_CHANGED: EventType  # value = <EventType.SYSTEM_THEME_CHANGED: 264>
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
S_APPLICATION: Scancode  # value = <Scancode.S_APPLICATION: 101>
S_BACKSLASH: Scancode  # value = <Scancode.S_BACKSLASH: 49>
S_BACKSPACE: Scancode  # value = <Scancode.S_BACKSPACE: 42>
S_CALL: Scancode  # value = <Scancode.S_CALL: 289>
S_CAPS: Scancode  # value = <Scancode.S_CAPS: 57>
S_CHANNEL_DEC: Scancode  # value = <Scancode.S_CHANNEL_DEC: 261>
S_CHANNEL_INC: Scancode  # value = <Scancode.S_CHANNEL_INC: 260>
S_COMMA: Scancode  # value = <Scancode.S_COMMA: 54>
S_COPY: Scancode  # value = <Scancode.S_COPY: 124>
S_CUT: Scancode  # value = <Scancode.S_CUT: 123>
S_DEL: Scancode  # value = <Scancode.S_DEL: 76>
S_DOWN: Scancode  # value = <Scancode.S_DOWN: 81>
S_END: Scancode  # value = <Scancode.S_END: 77>
S_ENDCALL: Scancode  # value = <Scancode.S_ENDCALL: 290>
S_EQ: Scancode  # value = <Scancode.S_EQ: 46>
S_ESC: Scancode  # value = <Scancode.S_ESC: 41>
S_EXECUTE: Scancode  # value = <Scancode.S_EXECUTE: 116>
S_F1: Scancode  # value = <Scancode.S_F1: 58>
S_F10: Scancode  # value = <Scancode.S_F10: 67>
S_F11: Scancode  # value = <Scancode.S_F11: 68>
S_F12: Scancode  # value = <Scancode.S_F12: 69>
S_F13: Scancode  # value = <Scancode.S_F13: 104>
S_F14: Scancode  # value = <Scancode.S_F14: 105>
S_F15: Scancode  # value = <Scancode.S_F15: 106>
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
S_HELP: Scancode  # value = <Scancode.S_HELP: 117>
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
S_MEDIA_EJECT: Scancode  # value = <Scancode.S_MEDIA_EJECT: 270>
S_MEDIA_FAST_FORWARD: Scancode  # value = <Scancode.S_MEDIA_FAST_FORWARD: 265>
S_MEDIA_NEXT: Scancode  # value = <Scancode.S_MEDIA_NEXT: 267>
S_MEDIA_PAUSE: Scancode  # value = <Scancode.S_MEDIA_PAUSE: 263>
S_MEDIA_PLAY: Scancode  # value = <Scancode.S_MEDIA_PLAY: 262>
S_MEDIA_PLAY_PAUSE: Scancode  # value = <Scancode.S_MEDIA_PLAY_PAUSE: 271>
S_MEDIA_PREV: Scancode  # value = <Scancode.S_MEDIA_PREV: 268>
S_MEDIA_REC: Scancode  # value = <Scancode.S_MEDIA_REC: 264>
S_MEDIA_REWIND: Scancode  # value = <Scancode.S_MEDIA_REWIND: 266>
S_MEDIA_SELECT: Scancode  # value = <Scancode.S_MEDIA_SELECT: 272>
S_MEDIA_STOP: Scancode  # value = <Scancode.S_MEDIA_STOP: 269>
S_MENU: Scancode  # value = <Scancode.S_MENU: 118>
S_MINUS: Scancode  # value = <Scancode.S_MINUS: 45>
S_MODE: Scancode  # value = <Scancode.S_MODE: 257>
S_MUTE: Scancode  # value = <Scancode.S_MUTE: 127>
S_NUMLOCK: Scancode  # value = <Scancode.S_NUMLOCK: 83>
S_PASTE: Scancode  # value = <Scancode.S_PASTE: 125>
S_PAUSE: Scancode  # value = <Scancode.S_PAUSE: 72>
S_PERIOD: Scancode  # value = <Scancode.S_PERIOD: 55>
S_PGDOWN: Scancode  # value = <Scancode.S_PGDOWN: 78>
S_PGUP: Scancode  # value = <Scancode.S_PGUP: 75>
S_POWER: Scancode  # value = <Scancode.S_POWER: 102>
S_PRTSCR: Scancode  # value = <Scancode.S_PRTSCR: 70>
S_RALT: Scancode  # value = <Scancode.S_RALT: 230>
S_RBRACKET: Scancode  # value = <Scancode.S_RBRACKET: 48>
S_RCTRL: Scancode  # value = <Scancode.S_RCTRL: 228>
S_RETURN: Scancode  # value = <Scancode.S_RETURN: 40>
S_RGUI: Scancode  # value = <Scancode.S_RGUI: 231>
S_RIGHT: Scancode  # value = <Scancode.S_RIGHT: 79>
S_RSHIFT: Scancode  # value = <Scancode.S_RSHIFT: 229>
S_SCRLK: Scancode  # value = <Scancode.S_SCRLK: 71>
S_SELECT: Scancode  # value = <Scancode.S_SELECT: 119>
S_SEMICOLON: Scancode  # value = <Scancode.S_SEMICOLON: 51>
S_SLASH: Scancode  # value = <Scancode.S_SLASH: 56>
S_SLEEP: Scancode  # value = <Scancode.S_SLEEP: 258>
S_SOFTLEFT: Scancode  # value = <Scancode.S_SOFTLEFT: 287>
S_SOFTRIGHT: Scancode  # value = <Scancode.S_SOFTRIGHT: 288>
S_SPACE: Scancode  # value = <Scancode.S_SPACE: 44>
S_STOP: Scancode  # value = <Scancode.S_STOP: 120>
S_TAB: Scancode  # value = <Scancode.S_TAB: 43>
S_UNDO: Scancode  # value = <Scancode.S_UNDO: 122>
S_UP: Scancode  # value = <Scancode.S_UP: 82>
S_VOLDOWN: Scancode  # value = <Scancode.S_VOLDOWN: 129>
S_VOLUP: Scancode  # value = <Scancode.S_VOLUP: 128>
S_WAKE: Scancode  # value = <Scancode.S_WAKE: 259>
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
TERMINATING: EventType  # value = <EventType.TERMINATING: 257>
TEXT_EDITING: EventType  # value = <EventType.TEXT_EDITING: 770>
TEXT_EDITING_CANDIDATES: EventType  # value = <EventType.TEXT_EDITING_CANDIDATES: 775>
TEXT_INPUT: EventType  # value = <EventType.TEXT_INPUT: 771>
WILL_ENTER_BACKGROUND: EventType  # value = <EventType.WILL_ENTER_BACKGROUND: 259>
WILL_ENTER_FOREGROUND: EventType  # value = <EventType.WILL_ENTER_FOREGROUND: 261>
WINDOW_CLOSE_REQUESTED: EventType  # value = <EventType.WINDOW_CLOSE_REQUESTED: 528>
WINDOW_DESTROYED: EventType  # value = <EventType.WINDOW_DESTROYED: 537>
WINDOW_DISPLAY_CHANGED: EventType  # value = <EventType.WINDOW_DISPLAY_CHANGED: 531>
WINDOW_DISPLAY_SCALE_CHANGED: EventType  # value = <EventType.WINDOW_DISPLAY_SCALE_CHANGED: 532>
WINDOW_ENTER_FULLSCREEN: EventType  # value = <EventType.WINDOW_ENTER_FULLSCREEN: 535>
WINDOW_EXPOSED: EventType  # value = <EventType.WINDOW_EXPOSED: 516>
WINDOW_FOCUS_GAINED: EventType  # value = <EventType.WINDOW_FOCUS_GAINED: 526>
WINDOW_FOCUS_LOST: EventType  # value = <EventType.WINDOW_FOCUS_LOST: 527>
WINDOW_HDR_STATE_CHANGED: EventType  # value = <EventType.WINDOW_HDR_STATE_CHANGED: 538>
WINDOW_HIDDEN: EventType  # value = <EventType.WINDOW_HIDDEN: 515>
WINDOW_HIT_TEST: EventType  # value = <EventType.WINDOW_HIT_TEST: 529>
WINDOW_ICCPROF_CHANGED: EventType  # value = <EventType.WINDOW_ICCPROF_CHANGED: 530>
WINDOW_LEAVE_FULLSCREEN: EventType  # value = <EventType.WINDOW_LEAVE_FULLSCREEN: 536>
WINDOW_MAXIMIZED: EventType  # value = <EventType.WINDOW_MAXIMIZED: 522>
WINDOW_MINIMIZED: EventType  # value = <EventType.WINDOW_MINIMIZED: 521>
WINDOW_MOUSE_ENTER: EventType  # value = <EventType.WINDOW_MOUSE_ENTER: 524>
WINDOW_MOUSE_LEAVE: EventType  # value = <EventType.WINDOW_MOUSE_LEAVE: 525>
WINDOW_MOVED: EventType  # value = <EventType.WINDOW_MOVED: 517>
WINDOW_OCCLUDED: EventType  # value = <EventType.WINDOW_OCCLUDED: 534>
WINDOW_RESIZED: EventType  # value = <EventType.WINDOW_RESIZED: 518>
WINDOW_RESTORED: EventType  # value = <EventType.WINDOW_RESTORED: 523>
WINDOW_SAFE_AREA_CHANGED: EventType  # value = <EventType.WINDOW_SAFE_AREA_CHANGED: 533>
WINDOW_SHOWN: EventType  # value = <EventType.WINDOW_SHOWN: 514>
