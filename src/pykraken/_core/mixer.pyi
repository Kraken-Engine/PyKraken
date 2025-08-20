from __future__ import annotations
import typing
__all__ = ['Audio']
class Audio:
    def __init__(self, path: str, volume: typing.SupportsFloat = 1.0) -> None:
        """
        Create an Audio object from a file path with optional volume
        """
    def pause(self, id: typing.SupportsInt) -> None:
        """
        Pause a specific voice by ID
        """
    def play(self, fadeInMs: typing.SupportsInt = 0, loop: bool = False) -> int:
        """
        Play the audio with optional fade-in time and loop setting. Returns a voice ID.
        """
    def resume(self, id: typing.SupportsInt) -> None:
        """
        Resume a specific voice by ID
        """
    def stop(self, id: typing.SupportsInt, fadeOutMs: typing.SupportsInt = 0) -> None:
        """
        Stop a specific voice by ID with optional fade-out time
        """
    def stopAll(self, fadeOutMs: typing.SupportsInt = 0) -> None:
        """
        Stop all voices with optional fade-out time
        """
    def update(self) -> None:
        """
        Update the audio system - cleans up finished voices
        """
    @property
    def volume(self) -> float:
        ...
    @volume.setter
    def volume(self, arg1: typing.SupportsFloat) -> None:
        ...
