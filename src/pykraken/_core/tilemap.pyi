"""
Tile map handling module
"""
from __future__ import annotations
import collections.abc
import enum
import pykraken._core
import typing
__all__: list[str] = ['ImageLayer', 'Layer', 'LayerList', 'LayerType', 'Map', 'MapObject', 'MapObjectList', 'MapOrientation', 'MapRenderOrder', 'MapStaggerAxis', 'MapStaggerIndex', 'ObjectGroup', 'TextProperties', 'TileLayer', 'TileSet', 'TileSetList']
class ImageLayer(Layer):
    def render(self) -> None:
        ...
    @property
    def opacity(self) -> float:
        ...
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def texture(self) -> pykraken._core.Texture:
        ...
class Layer:
    offset: pykraken._core.Vec2
    visible: bool
    def render(self) -> None:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def opacity(self) -> float:
        ...
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def type(self) -> LayerType:
        ...
class LayerList:
    __hash__: typing.ClassVar[None] = None
    def __bool__(self: collections.abc.Sequence[Layer]) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self: collections.abc.Sequence[Layer], x: Layer) -> bool:
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self: collections.abc.Sequence[Layer], arg0: typing.SupportsInt) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self: collections.abc.Sequence[Layer], arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    def __eq__(self: collections.abc.Sequence[Layer], arg0: collections.abc.Sequence[Layer]) -> bool:
        ...
    @typing.overload
    def __getitem__(self: collections.abc.Sequence[Layer], s: slice) -> list[Layer]:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self: collections.abc.Sequence[Layer], arg0: typing.SupportsInt) -> Layer:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: collections.abc.Sequence[Layer]) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self: collections.abc.Sequence[Layer]) -> collections.abc.Iterator[Layer]:
        ...
    def __len__(self: collections.abc.Sequence[Layer]) -> int:
        ...
    def __ne__(self: collections.abc.Sequence[Layer], arg0: collections.abc.Sequence[Layer]) -> bool:
        ...
    def __repr__(self: collections.abc.Sequence[Layer]) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self: collections.abc.Sequence[Layer], arg0: typing.SupportsInt, arg1: Layer) -> None:
        ...
    @typing.overload
    def __setitem__(self: collections.abc.Sequence[Layer], arg0: slice, arg1: collections.abc.Sequence[Layer]) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self: collections.abc.Sequence[Layer], x: Layer) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self: collections.abc.Sequence[Layer]) -> None:
        """
        Clear the contents
        """
    def count(self: collections.abc.Sequence[Layer], x: Layer) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self: collections.abc.Sequence[Layer], L: collections.abc.Sequence[Layer]) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self: collections.abc.Sequence[Layer], L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self: collections.abc.Sequence[Layer], i: typing.SupportsInt, x: Layer) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self: collections.abc.Sequence[Layer]) -> Layer:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self: collections.abc.Sequence[Layer], i: typing.SupportsInt) -> Layer:
        """
        Remove and return the item at index ``i``
        """
    def remove(self: collections.abc.Sequence[Layer], x: Layer) -> None:
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
class LayerType(enum.IntEnum):
    IMAGE: typing.ClassVar[LayerType]  # value = <LayerType.IMAGE: 2>
    OBJECT: typing.ClassVar[LayerType]  # value = <LayerType.OBJECT: 1>
    TILE: typing.ClassVar[LayerType]  # value = <LayerType.TILE: 0>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Map:
    background_color: pykraken._core.Color
    def __init__(self) -> None:
        ...
    def load_from_tmx(self, tmx_path: str) -> None:
        ...
    def render(self) -> None:
        ...
    @property
    def bounds(self) -> pykraken._core.Rect:
        ...
    @property
    def hex_side_length(self) -> float:
        ...
    @property
    def layers(self) -> list[Layer]:
        ...
    @property
    def map_size(self) -> pykraken._core.Vec2:
        ...
    @property
    def orientation(self) -> MapOrientation:
        ...
    @property
    def render_order(self) -> MapRenderOrder:
        ...
    @property
    def stagger_axis(self) -> MapStaggerAxis:
        ...
    @property
    def stagger_index(self) -> MapStaggerIndex:
        ...
    @property
    def tile_sets(self) -> list[TileSet]:
        ...
    @property
    def tile_size(self) -> pykraken._core.Vec2:
        ...
class MapObject:
    class ShapeType(enum.IntEnum):
        ELLIPSE: typing.ClassVar[MapObject.ShapeType]  # value = <ShapeType.ELLIPSE: 1>
        POINT: typing.ClassVar[MapObject.ShapeType]  # value = <ShapeType.POINT: 2>
        POLYGON: typing.ClassVar[MapObject.ShapeType]  # value = <ShapeType.POLYGON: 3>
        POLYLINE: typing.ClassVar[MapObject.ShapeType]  # value = <ShapeType.POLYLINE: 4>
        RECTANGLE: typing.ClassVar[MapObject.ShapeType]  # value = <ShapeType.RECTANGLE: 0>
        TEXT: typing.ClassVar[MapObject.ShapeType]  # value = <ShapeType.TEXT: 5>
        @classmethod
        def __new__(cls, value):
            ...
        def __format__(self, format_spec):
            """
            Convert to a string according to format_spec.
            """
    transform: pykraken._core.Transform
    visible: bool
    @property
    def name(self) -> str:
        ...
    @property
    def rect(self) -> pykraken._core.Rect:
        ...
    @property
    def shape_type(self) -> MapObject.ShapeType:
        ...
    @property
    def text(self) -> TextProperties:
        ...
    @property
    def tile_id(self) -> int:
        ...
    @property
    def type(self) -> str:
        ...
    @property
    def uid(self) -> int:
        ...
    @property
    def vertices(self) -> list[pykraken._core.Vec2]:
        ...
class MapObjectList:
    def __bool__(self: collections.abc.Sequence[MapObject]) -> bool:
        """
        Check whether the list is nonempty
        """
    @typing.overload
    def __delitem__(self: collections.abc.Sequence[MapObject], arg0: typing.SupportsInt) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self: collections.abc.Sequence[MapObject], arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    @typing.overload
    def __getitem__(self: collections.abc.Sequence[MapObject], s: slice) -> list[MapObject]:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self: collections.abc.Sequence[MapObject], arg0: typing.SupportsInt) -> MapObject:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: collections.abc.Sequence[MapObject]) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self: collections.abc.Sequence[MapObject]) -> collections.abc.Iterator[MapObject]:
        ...
    def __len__(self: collections.abc.Sequence[MapObject]) -> int:
        ...
    @typing.overload
    def __setitem__(self: collections.abc.Sequence[MapObject], arg0: typing.SupportsInt, arg1: MapObject) -> None:
        ...
    @typing.overload
    def __setitem__(self: collections.abc.Sequence[MapObject], arg0: slice, arg1: collections.abc.Sequence[MapObject]) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self: collections.abc.Sequence[MapObject], x: MapObject) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self: collections.abc.Sequence[MapObject]) -> None:
        """
        Clear the contents
        """
    @typing.overload
    def extend(self: collections.abc.Sequence[MapObject], L: collections.abc.Sequence[MapObject]) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self: collections.abc.Sequence[MapObject], L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self: collections.abc.Sequence[MapObject], i: typing.SupportsInt, x: MapObject) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self: collections.abc.Sequence[MapObject]) -> MapObject:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self: collections.abc.Sequence[MapObject], i: typing.SupportsInt) -> MapObject:
        """
        Remove and return the item at index ``i``
        """
class MapOrientation(enum.IntEnum):
    HEXAGONAL: typing.ClassVar[MapOrientation]  # value = <MapOrientation.HEXAGONAL: 3>
    ISOMETRIC: typing.ClassVar[MapOrientation]  # value = <MapOrientation.ISOMETRIC: 1>
    NONE: typing.ClassVar[MapOrientation]  # value = <MapOrientation.NONE: 4>
    ORTHOGONAL: typing.ClassVar[MapOrientation]  # value = <MapOrientation.ORTHOGONAL: 0>
    STAGGERED: typing.ClassVar[MapOrientation]  # value = <MapOrientation.STAGGERED: 2>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class MapRenderOrder(enum.IntEnum):
    LEFT_DOWN: typing.ClassVar[MapRenderOrder]  # value = <MapRenderOrder.LEFT_DOWN: 2>
    LEFT_UP: typing.ClassVar[MapRenderOrder]  # value = <MapRenderOrder.LEFT_UP: 3>
    NONE: typing.ClassVar[MapRenderOrder]  # value = <MapRenderOrder.NONE: 4>
    RIGHT_DOWN: typing.ClassVar[MapRenderOrder]  # value = <MapRenderOrder.RIGHT_DOWN: 0>
    RIGHT_UP: typing.ClassVar[MapRenderOrder]  # value = <MapRenderOrder.RIGHT_UP: 1>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class MapStaggerAxis(enum.IntEnum):
    NONE: typing.ClassVar[MapStaggerAxis]  # value = <MapStaggerAxis.NONE: 2>
    X: typing.ClassVar[MapStaggerAxis]  # value = <MapStaggerAxis.X: 0>
    Y: typing.ClassVar[MapStaggerAxis]  # value = <MapStaggerAxis.Y: 1>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class MapStaggerIndex(enum.IntEnum):
    EVEN: typing.ClassVar[MapStaggerIndex]  # value = <MapStaggerIndex.EVEN: 0>
    NONE: typing.ClassVar[MapStaggerIndex]  # value = <MapStaggerIndex.NONE: 2>
    ODD: typing.ClassVar[MapStaggerIndex]  # value = <MapStaggerIndex.ODD: 1>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class ObjectGroup(Layer):
    class DrawOrder(enum.IntEnum):
        INDEX: typing.ClassVar[ObjectGroup.DrawOrder]  # value = <DrawOrder.INDEX: 0>
        TOP_DOWN: typing.ClassVar[ObjectGroup.DrawOrder]  # value = <DrawOrder.TOP_DOWN: 1>
        @classmethod
        def __new__(cls, value):
            ...
        def __format__(self, format_spec):
            """
            Convert to a string according to format_spec.
            """
    color: pykraken._core.Color
    def render(self) -> None:
        ...
    @property
    def draw_order(self) -> ObjectGroup.DrawOrder:
        ...
    @property
    def objects(self) -> list[MapObject]:
        ...
    @property
    def opacity(self) -> float:
        ...
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
class TextProperties:
    align: pykraken._core.Align
    bold: bool
    color: pykraken._core.Color
    font_family: str
    italic: bool
    kerning: bool
    strikethrough: bool
    text: str
    underline: bool
    wrap: bool
    @property
    def pixel_size(self) -> int:
        ...
    @pixel_size.setter
    def pixel_size(self, arg0: typing.SupportsInt) -> None:
        ...
class TileLayer(Layer):
    class Tile:
        @property
        def flip_flags(self) -> int:
            ...
        @property
        def id(self) -> int:
            ...
    class TileLayerTileList:
        def __bool__(self: collections.abc.Sequence[TileLayer.Tile]) -> bool:
            """
            Check whether the list is nonempty
            """
        @typing.overload
        def __delitem__(self: collections.abc.Sequence[TileLayer.Tile], arg0: typing.SupportsInt) -> None:
            """
            Delete the list elements at index ``i``
            """
        @typing.overload
        def __delitem__(self: collections.abc.Sequence[TileLayer.Tile], arg0: slice) -> None:
            """
            Delete list elements using a slice object
            """
        @typing.overload
        def __getitem__(self: collections.abc.Sequence[TileLayer.Tile], s: slice) -> list[TileLayer.Tile]:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self: collections.abc.Sequence[TileLayer.Tile], arg0: typing.SupportsInt) -> TileLayer.Tile:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: collections.abc.Sequence[TileLayer.Tile]) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self: collections.abc.Sequence[TileLayer.Tile]) -> collections.abc.Iterator[TileLayer.Tile]:
            ...
        def __len__(self: collections.abc.Sequence[TileLayer.Tile]) -> int:
            ...
        @typing.overload
        def __setitem__(self: collections.abc.Sequence[TileLayer.Tile], arg0: typing.SupportsInt, arg1: TileLayer.Tile) -> None:
            ...
        @typing.overload
        def __setitem__(self: collections.abc.Sequence[TileLayer.Tile], arg0: slice, arg1: collections.abc.Sequence[TileLayer.Tile]) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self: collections.abc.Sequence[TileLayer.Tile], x: TileLayer.Tile) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self: collections.abc.Sequence[TileLayer.Tile]) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self: collections.abc.Sequence[TileLayer.Tile], L: collections.abc.Sequence[TileLayer.Tile]) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self: collections.abc.Sequence[TileLayer.Tile], L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self: collections.abc.Sequence[TileLayer.Tile], i: typing.SupportsInt, x: TileLayer.Tile) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self: collections.abc.Sequence[TileLayer.Tile]) -> TileLayer.Tile:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self: collections.abc.Sequence[TileLayer.Tile], i: typing.SupportsInt) -> TileLayer.Tile:
            """
            Remove and return the item at index ``i``
            """
    def render(self) -> None:
        ...
    @property
    def opacity(self) -> float:
        ...
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def tiles(self) -> list[TileLayer.Tile]:
        ...
class TileSet:
    class Terrain:
        @property
        def name(self) -> str:
            ...
        @property
        def tile_id(self) -> int:
            ...
    class TerrainList:
        def __bool__(self: collections.abc.Sequence[TileSet.Terrain]) -> bool:
            """
            Check whether the list is nonempty
            """
        @typing.overload
        def __delitem__(self: collections.abc.Sequence[TileSet.Terrain], arg0: typing.SupportsInt) -> None:
            """
            Delete the list elements at index ``i``
            """
        @typing.overload
        def __delitem__(self: collections.abc.Sequence[TileSet.Terrain], arg0: slice) -> None:
            """
            Delete list elements using a slice object
            """
        @typing.overload
        def __getitem__(self: collections.abc.Sequence[TileSet.Terrain], s: slice) -> list[TileSet.Terrain]:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self: collections.abc.Sequence[TileSet.Terrain], arg0: typing.SupportsInt) -> TileSet.Terrain:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: collections.abc.Sequence[TileSet.Terrain]) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self: collections.abc.Sequence[TileSet.Terrain]) -> collections.abc.Iterator[TileSet.Terrain]:
            ...
        def __len__(self: collections.abc.Sequence[TileSet.Terrain]) -> int:
            ...
        @typing.overload
        def __setitem__(self: collections.abc.Sequence[TileSet.Terrain], arg0: typing.SupportsInt, arg1: TileSet.Terrain) -> None:
            ...
        @typing.overload
        def __setitem__(self: collections.abc.Sequence[TileSet.Terrain], arg0: slice, arg1: collections.abc.Sequence[TileSet.Terrain]) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self: collections.abc.Sequence[TileSet.Terrain], x: TileSet.Terrain) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self: collections.abc.Sequence[TileSet.Terrain]) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self: collections.abc.Sequence[TileSet.Terrain], L: collections.abc.Sequence[TileSet.Terrain]) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self: collections.abc.Sequence[TileSet.Terrain], L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self: collections.abc.Sequence[TileSet.Terrain], i: typing.SupportsInt, x: TileSet.Terrain) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self: collections.abc.Sequence[TileSet.Terrain]) -> TileSet.Terrain:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self: collections.abc.Sequence[TileSet.Terrain], i: typing.SupportsInt) -> TileSet.Terrain:
            """
            Remove and return the item at index ``i``
            """
    class Tile:
        @property
        def clip_rect(self) -> pykraken._core.Rect:
            ...
        @property
        def id(self) -> int:
            ...
        @property
        def probability(self) -> int:
            ...
        @property
        def terrain_indices(self) -> typing.Annotated[list[int], "FixedSize(4)"]:
            ...
    class TileSetTileList:
        def __bool__(self: collections.abc.Sequence[TileSet.Tile]) -> bool:
            """
            Check whether the list is nonempty
            """
        @typing.overload
        def __delitem__(self: collections.abc.Sequence[TileSet.Tile], arg0: typing.SupportsInt) -> None:
            """
            Delete the list elements at index ``i``
            """
        @typing.overload
        def __delitem__(self: collections.abc.Sequence[TileSet.Tile], arg0: slice) -> None:
            """
            Delete list elements using a slice object
            """
        @typing.overload
        def __getitem__(self: collections.abc.Sequence[TileSet.Tile], s: slice) -> list[TileSet.Tile]:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self: collections.abc.Sequence[TileSet.Tile], arg0: typing.SupportsInt) -> TileSet.Tile:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: collections.abc.Sequence[TileSet.Tile]) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self: collections.abc.Sequence[TileSet.Tile]) -> collections.abc.Iterator[TileSet.Tile]:
            ...
        def __len__(self: collections.abc.Sequence[TileSet.Tile]) -> int:
            ...
        @typing.overload
        def __setitem__(self: collections.abc.Sequence[TileSet.Tile], arg0: typing.SupportsInt, arg1: TileSet.Tile) -> None:
            ...
        @typing.overload
        def __setitem__(self: collections.abc.Sequence[TileSet.Tile], arg0: slice, arg1: collections.abc.Sequence[TileSet.Tile]) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self: collections.abc.Sequence[TileSet.Tile], x: TileSet.Tile) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self: collections.abc.Sequence[TileSet.Tile]) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self: collections.abc.Sequence[TileSet.Tile], L: collections.abc.Sequence[TileSet.Tile]) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self: collections.abc.Sequence[TileSet.Tile], L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self: collections.abc.Sequence[TileSet.Tile], i: typing.SupportsInt, x: TileSet.Tile) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self: collections.abc.Sequence[TileSet.Tile]) -> TileSet.Tile:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self: collections.abc.Sequence[TileSet.Tile], i: typing.SupportsInt) -> TileSet.Tile:
            """
            Remove and return the item at index ``i``
            """
    def get_tile(self, id: typing.SupportsInt) -> TileSet.Tile:
        ...
    def has_tile(self, id: typing.SupportsInt) -> bool:
        ...
    @property
    def columns(self) -> int:
        ...
    @property
    def first_gid(self) -> int:
        ...
    @property
    def last_gid(self) -> int:
        ...
    @property
    def margin(self) -> int:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def spacing(self) -> int:
        ...
    @property
    def terrains(self) -> list[TileSet.Terrain]:
        ...
    @property
    def texture(self) -> pykraken._core.Texture:
        ...
    @property
    def tile_count(self) -> int:
        ...
    @property
    def tile_offset(self) -> pykraken._core.Vec2:
        ...
    @property
    def tile_size(self) -> pykraken._core.Vec2:
        ...
    @property
    def tiles(self) -> list[TileSet.Tile]:
        ...
class TileSetList:
    def __bool__(self: collections.abc.Sequence[TileSet]) -> bool:
        """
        Check whether the list is nonempty
        """
    @typing.overload
    def __delitem__(self: collections.abc.Sequence[TileSet], arg0: typing.SupportsInt) -> None:
        """
        Delete the list elements at index ``i``
        """
    @typing.overload
    def __delitem__(self: collections.abc.Sequence[TileSet], arg0: slice) -> None:
        """
        Delete list elements using a slice object
        """
    @typing.overload
    def __getitem__(self: collections.abc.Sequence[TileSet], s: slice) -> list[TileSet]:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self: collections.abc.Sequence[TileSet], arg0: typing.SupportsInt) -> TileSet:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: collections.abc.Sequence[TileSet]) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self: collections.abc.Sequence[TileSet]) -> collections.abc.Iterator[TileSet]:
        ...
    def __len__(self: collections.abc.Sequence[TileSet]) -> int:
        ...
    @typing.overload
    def __setitem__(self: collections.abc.Sequence[TileSet], arg0: typing.SupportsInt, arg1: TileSet) -> None:
        ...
    @typing.overload
    def __setitem__(self: collections.abc.Sequence[TileSet], arg0: slice, arg1: collections.abc.Sequence[TileSet]) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self: collections.abc.Sequence[TileSet], x: TileSet) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self: collections.abc.Sequence[TileSet]) -> None:
        """
        Clear the contents
        """
    @typing.overload
    def extend(self: collections.abc.Sequence[TileSet], L: collections.abc.Sequence[TileSet]) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self: collections.abc.Sequence[TileSet], L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self: collections.abc.Sequence[TileSet], i: typing.SupportsInt, x: TileSet) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self: collections.abc.Sequence[TileSet]) -> TileSet:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self: collections.abc.Sequence[TileSet], i: typing.SupportsInt) -> TileSet:
        """
        Remove and return the item at index ``i``
        """
