"""
Input event handling
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['new_custom', 'poll', 'push', 'schedule', 'start_text_input', 'stop_text_input', 'unschedule']
def new_custom() -> pykraken._core.Event:
    """
    Create a new custom event type.
    
    Returns:
        Event: The newly registered custom Event.
    
    Raises:
        RuntimeError: If registration fails.
    """
def poll() -> list[pykraken._core.Event]:
    """
    Poll for all pending user input events.
    
    This clears input states and returns a list of events that occurred since the last call.
    
    Returns:
        list[Event]: A list of input event objects.
    """
def push(event: pykraken._core.Event) -> None:
    """
    Push a custom event to the event queue.
    
    Args:
        event (Event): The custom event to push to the queue.
    
    Raises:
        ValueError: If the event is not a custom event type.
        RuntimeError: If the event could not be queued.
    """
def schedule(event: pykraken._core.Event, delay_ms: typing.SupportsInt, repeat: bool = False) -> None:
    """
    Schedule a custom event to be pushed after a delay. Will overwrite any existing timer for the same event.
    
    Args:
        event (Event): The custom event to schedule.
        delay_ms (int): Delay in milliseconds before the event is pushed.
        repeat (bool, optional): If True, the event will be pushed repeatedly at the
            specified interval. If False, the event is pushed only once. Defaults to False.
    
    Raises:
        ValueError: If the event is not a custom event type.
        RuntimeError: If the timer could not be created.
    """
def start_text_input() -> None:
    """
    Start text input for TEXT_INPUT and TEXT_EDITING events.
    
    Raises:
        RuntimeError: If text input could not be started.
    """
def stop_text_input() -> None:
    """
    Stop text input for TEXT_INPUT and TEXT_EDITING events.
    
    Raises:
        RuntimeError: If text input could not be stopped.
    """
def unschedule(event: pykraken._core.Event) -> None:
    """
    Cancel a scheduled event timer.
    
    Args:
        event (Event): The custom event whose timer should be cancelled.
    """
