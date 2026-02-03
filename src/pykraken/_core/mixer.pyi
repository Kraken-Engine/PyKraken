"""

        Sound mixer and audio management system.

        The mixer manages a pool of 64 mixer tracks/voices for playing
        :class:`Sample` (short polyphonic sounds) and :class:`Stream`
        (long music files). It handles automatic track acquisition and
        priority-based sound stealing when the track pool is exhausted.
    
"""
from __future__ import annotations
import enum
import typing
__all__: list[str] = ['Audio', 'AudioPriority', 'Sample', 'Stream', 'get_master_volume', 'load_sample', 'load_stream', 'set_master_volume']
class Audio:
    """
    
            Abstract base class for all audio resources.
    
            Common interface for local volume and playback status. Local volume
            is multiplied by the mixer's master volume. Both default to 1.0.
    
            Attributes:
                volume (float): Local volume multiplier (0.0 to 1.0). Defaults to 1.0.
                playing (bool): (Read-only) Whether the audio is currently playing on any track.
    
            Methods:
                play(fade_in=0.0): Start audio playback.
                stop(fade_out=0.0): Stop all instances of this audio resource.
        
    """
    def play(self, fade_in: typing.SupportsFloat = 0.0) -> None:
        """
                    Start audio playback.
        
                    Args:
                        fade_in (float): Fade in duration in seconds. Defaults to 0.0.
        """
    def stop(self, fade_out: typing.SupportsFloat = 0.0) -> None:
        """
                    Stop all playing instances of this audio.
        
                    Args:
                        fade_out (float): Fade out duration in seconds. Defaults to 0.0.
        """
    @property
    def playing(self) -> bool:
        """
        True if currently playing.
        """
    @property
    def volume(self) -> float:
        """
        Volume scalar (0.0 to 1.0).
        """
    @volume.setter
    def volume(self, arg1: typing.SupportsFloat) -> None:
        ...
class AudioPriority(enum.IntEnum):
    """
    
            Priority levels used for track acquisition.
    
            Used to determine which sounds to interrupt ('steal') when the 64-track
            limit is reached. Higher priority sounds are more protected from being stolen.
        
    """
    MUSIC: typing.ClassVar[AudioPriority]  # value = <AudioPriority.MUSIC: 0>
    SFX: typing.ClassVar[AudioPriority]  # value = <AudioPriority.SFX: 6>
    UI: typing.ClassVar[AudioPriority]  # value = <AudioPriority.UI: 2>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Sample(Audio):
    """
    
            A sound effect sample loaded entirely into memory.
    
            Samples support polyphony (multiple simultaneous instances). If tracks
            are full, samples attempt to steal tracks from lower-priority or older sounds.
    
            Attributes:
                priority (AudioPriority): Acquisition priority level. Defaults to SFX.
                can_steal (bool): Whether this sound can interrupt others to acquire a
                    track. Defaults to True.
                max_polyphony (int): Maximum simultaneous instances of this specific
                    sample (Range 1-32). Defaults to 1.
        
    """
    @property
    def can_steal(self) -> bool:
        """
        Whether can interrupt others to acquire a track.
        """
    @can_steal.setter
    def can_steal(self, arg0: bool) -> None:
        ...
    @property
    def max_polyphony(self) -> int:
        """
        Max simultaneous instances of sample (1-32).
        """
    @max_polyphony.setter
    def max_polyphony(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def priority(self) -> AudioPriority:
        """
        Acquisition priority level.
        """
    @priority.setter
    def priority(self, arg0: AudioPriority) -> None:
        ...
class Stream(Audio):
    """
    
            A streaming audio resource intended for long music files.
    
            Streams occupy exactly one track while active. They are protected and
            will not be stolen by incoming :class:`Sample` requests.
    
            Attributes:
                playback_pos (float): (Read-only) Current playback position in seconds.
                looping (bool): Whether the stream should loop when it reaches the end.
    
            Methods:
                pause(): Pause playback, preserving position.
                resume(fade_in=0.0): Resume playback from a paused state.
                seek(seconds): Jump to a specific time in the audio file.
        
    """
    def pause(self) -> None:
        """
                    Pause playback. Releases the hardware track but preserves position.
        """
    def resume(self, fade_in: typing.SupportsFloat = 0.0) -> None:
        """
                    Resume playback from a paused state.
        
                    Args:
                        fade_in (float): Duration in seconds to fade back in. Defaults to 0.0.
        """
    def seek(self, seconds: typing.SupportsFloat) -> None:
        """
                    Jump to a specific time in the audio file.
        
                    Args:
                        seconds (float): Target position in seconds from the start.
        """
    @property
    def looping(self) -> bool:
        """
        Whether the stream should loop when it reaches the end.
        """
    @looping.setter
    def looping(self, arg1: bool) -> None:
        ...
    @property
    def playback_pos(self) -> float:
        """
        Current position in seconds. 0.0 if stopped/never played, paused position if paused.
        """
def get_master_volume() -> float:
    """
            Get the current global mixer gain.
    
            Returns:
                float: Master volume (0.0 to 1.0).
    """
def load_sample(path: str, predecode: bool = True) -> Sample:
    """
            Load an audio sample (SFX) from disk.
    
            Args:
                path (str): File path to load.
                predecode (bool): Whether to decode into memory now. Defaults to True.
    
            Returns:
                Sample: The loaded audio object.
    """
def load_stream(path: str, predecode: bool = False) -> Stream:
    """
            Load an audio stream (Music) from disk.
    
            Args:
                path (str): File path to load.
                predecode (bool): Whether to decode into memory now. Defaults to False.
    
            Returns:
                Stream: The loaded audio object.
    """
def set_master_volume(volume: typing.SupportsFloat) -> None:
    """
            Set the global mixer gain.
    
            This affects all playing samples and streams. Individual audio volume
            is multiplied by this value. Default is 1.0.
    
            Args:
                volume (float): Master volume scalar (0.0 to 1.0).
    """
