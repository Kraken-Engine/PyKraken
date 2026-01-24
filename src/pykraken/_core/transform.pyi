"""

Submodule for Transform-related functionality.
        
"""
from __future__ import annotations
import collections.abc
import pykraken._core
import typing
__all__: list[str] = ['TransformList', 'compose', 'compose_chain']
class TransformList:
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    @typing.overload
    def __delitem__(self, arg0: typing.SupportsInt) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> TransformList:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: typing.SupportsInt) -> pykraken._core.Transform:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: TransformList) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self) -> collections.abc.Iterator[pykraken._core.Transform]:
        ...
    def __len__(self) -> int:
        ...
    @typing.overload
    def __setitem__(self, arg0: typing.SupportsInt, arg1: pykraken._core.Transform) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: TransformList) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: pykraken._core.Transform) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    @typing.overload
    def extend(self, L: TransformList) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: typing.SupportsInt, x: pykraken._core.Transform) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> pykraken._core.Transform:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: typing.SupportsInt) -> pykraken._core.Transform:
        """
        Remove and return the item at index ``i``
        """
def compose(*args) -> pykraken._core.Transform:
    """
    Compose multiple Transform objects in order and return the resulting Transform in world space.
    The first transform is treated as already in world space; each subsequent transform is local to the previous.
    
    Args:
        *transforms: Two or more Transform objects to compose.
    
    Returns:
        Transform: The composed Transform in world space.
    """
def compose_chain(*args) -> TransformList:
    """
    Returns a list of cumulative world-space transforms excluding the initial input.
    
    Args:
        *transforms: Two or more Transform objects to compose.
    
    Returns:
        TransformList: The composed Transforms for inputs 2..N in world space.
    """
