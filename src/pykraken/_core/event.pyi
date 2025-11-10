"""
Input event handling
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['cancel_scheduled', 'new_custom', 'poll', 'push', 'schedule']
def cancel_scheduled(event: pykraken._core.Event) -> None:
    """
    Cancel a scheduled event timer.
    
    Args:
        event (Event): The custom event whose timer should be cancelled.
    
    Raises:
        RuntimeError: If attempting to cancel a non-custom event type.
    """
def new_custom() -> pykraken._core.Event:
    """
    Create a new custom event type.
    
    Returns:
        Event: A new Event object with a unique custom event type.
    
    Raises:
        RuntimeError: If registering a custom event type fails.
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
        RuntimeError: If attempting to push a non-custom event type.
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
        RuntimeError: If attempting to schedule a non-custom event type, or if timer
            creation fails.
    """
