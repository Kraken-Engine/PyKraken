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
        Event: The newly registered custom Event. If registration fails, the Event has type 0.
    """
def poll() -> list[pykraken._core.Event]:
    """
    Poll for all pending user input events.
    
    This clears input states and returns a list of events that occurred since the last call.
    
    Returns:
        list[Event]: A list of input event objects.
    """
def push(event: pykraken._core.Event) -> bool:
    """
    Push a custom event to the event queue.
    
    Args:
        event (Event): The custom event to push to the queue.
    
    Returns:
        bool: True if the event was queued, False otherwise.
    """
def schedule(event: pykraken._core.Event, delay_ms: typing.SupportsInt, repeat: bool = False) -> bool:
    """
    Schedule a custom event to be pushed after a delay. Will overwrite any existing timer for the same event.
    
    Args:
        event (Event): The custom event to schedule.
        delay_ms (int): Delay in milliseconds before the event is pushed.
        repeat (bool, optional): If True, the event will be pushed repeatedly at the
            specified interval. If False, the event is pushed only once. Defaults to False.
    
    Returns:
        bool: True if the timer was scheduled, False on invalid types or timer creation failure.
    """
def start_text_input() -> bool:
    """
    Start text input for TEXT_INPUT and TEXT_EDITING events.
    
    Returns:
        bool: True if text input was started successfully, False otherwise.
    """
def stop_text_input() -> bool:
    """
    Stop text input for TEXT_INPUT and TEXT_EDITING events.
    
    Returns:
        bool: True if text input was stopped successfully, False otherwise.
    """
def unschedule(event: pykraken._core.Event) -> bool:
    """
    Cancel a scheduled event timer.
    
    Args:
        event (Event): The custom event whose timer should be cancelled.
    
    Returns:
        bool: True if a timer was found and removed, False otherwise.
    """
