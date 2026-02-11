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
    """
    
    ImageLayer displays a single image as a layer.
    
    Attributes:
        opacity (float): Layer opacity.
        texture (Texture): The layer image texture.
    
    Methods:
        draw: Draw the image layer.
        
    """
    def draw(self) -> None:
        """
        Draw the image layer.
        """
    @property
    def opacity(self) -> float:
        """
        Layer opacity from 0.0 to 1.0.
        """
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def texture(self) -> pykraken._core.Texture:
        """
        Texture used by the image layer.
        """
class Layer:
    """
    
    Layer is the base class for all tilemap layers.
    
    Attributes:
        visible (bool): Whether the layer is visible.
        offset (Vec2): Per-layer drawing offset.
        opacity (float): Layer opacity (0.0-1.0).
        name (str): Layer name.
        type (LayerType): Layer type enum.
    
    Methods:
        draw: Draw the layer to the current renderer.
        
    """
    def draw(self) -> None:
        """
        Draw the layer to the current renderer.
        """
    @property
    def name(self) -> str:
        """
        Layer name.
        """
    @property
    def offset(self) -> pykraken._core.Vec2:
        """
        Per-layer drawing offset.
        """
    @offset.setter
    def offset(self, arg0: pykraken._core.Vec2) -> None:
        ...
    @property
    def opacity(self) -> float:
        """
        Layer opacity from 0.0 to 1.0.
        """
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def type(self) -> LayerType:
        """
        Layer type enum.
        """
    @property
    def visible(self) -> bool:
        """
        Whether the layer is visible.
        """
    @visible.setter
    def visible(self, arg0: bool) -> None:
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
    """
    
    TMX layer type values.
        
    """
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
    """
    
    A TMX map with access to its layers and tilesets.
    
    Attributes:
        background_color (Color): Map background color.
        orientation (MapOrientation): Map orientation enum.
        render_order (MapRenderOrder): Tile render order enum.
        map_size (Vec2): Tile grid dimensions.
        tile_size (Vec2): Size of individual tiles.
        bounds (Rect): Map bounds in pixels.
        hex_side_length (float): Hex side length for hex maps.
        stagger_axis (MapStaggerAxis): Stagger axis enum for staggered/hex maps.
        stagger_index (MapStaggerIndex): Stagger index enum.
        tile_sets (TileSetList): List of TileSet objects.
        all_layers (LayerList): List of Layer instances.
        tile_layers (List[TileLayer]): List of tile layers.
        object_groups (List[ObjectGroup]): List of object groups.
        image_layers (List[ImageLayer]): List of image layers.
    
    Methods:
        load: Load a TMX file from path.
        draw: Draw all layers.
        get_layer: Get a layer by name.
        
    """
    def __init__(self) -> None:
        ...
    def draw(self) -> None:
        """
        Draw all layers.
        """
    def get_layer(self, name: str) -> Layer:
        """
        Get a layer by its name. Will return None if not found.
        
        Args:
            name (str): Name of the layer to retrieve.
        """
    def load(self, tmx_path: str) -> None:
        """
        Load a TMX file from path.
        
        Args:
            tmx_path (str): Path to the TMX file to load.
        """
    @property
    def all_layers(self) -> list[Layer]:
        """
        LayerList of layers in the map.
        """
    @property
    def background_color(self) -> pykraken._core.Color:
        """
        Map background color.
        """
    @background_color.setter
    def background_color(self, arg0: pykraken._core.Color) -> None:
        ...
    @property
    def bounds(self) -> pykraken._core.Rect:
        """
        Map bounds in pixels.
        """
    @property
    def hex_side_length(self) -> float:
        """
        Hex side length for hex maps.
        """
    @property
    def image_layers(self) -> list[ImageLayer]:
        """
        List of image layers in the map.
        """
    @property
    def map_size(self) -> pykraken._core.Vec2:
        """
        Map dimensions in tiles.
        """
    @property
    def object_groups(self) -> list[ObjectGroup]:
        """
        List of object group layers in the map.
        """
    @property
    def orientation(self) -> MapOrientation:
        """
        Map orientation enum.
        """
    @property
    def render_order(self) -> MapRenderOrder:
        """
        Tile render order enum.
        """
    @property
    def stagger_axis(self) -> MapStaggerAxis:
        """
        Stagger axis enum for staggered/hex maps.
        """
    @property
    def stagger_index(self) -> MapStaggerIndex:
        """
        Stagger index enum for staggered/hex maps.
        """
    @property
    def tile_layers(self) -> list[TileLayer]:
        """
        List of tile layers in the map.
        """
    @property
    def tile_sets(self) -> list[TileSet]:
        """
        TileSetList of tilesets used by the map.
        """
    @property
    def tile_size(self) -> pykraken._core.Vec2:
        """
        Size of tiles in pixels.
        """
class MapObject:
    """
    
    MapObject represents a placed object on an object layer.
    
    Attributes:
        transform (Transform): Transformation component for the object.
        visible (bool): Visibility flag.
        uid (int): Unique identifier.
        name (str): Object name.
        type (str): Object type string.
        rect (Rect): Bounding rectangle.
        tile_id (int): Associated tile id if the object is a tile.
        shape_type (ShapeType): The shape enum for the object.
        vertices (list[Vec2]): Vertex list for polygon/polyline shapes.
        text (TextProperties): Text properties when shape is text.
        
    """
    class ShapeType(enum.IntEnum):
        """
        
        TMX object shape types.
            
        """
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
    @property
    def is_visible(self) -> bool:
        """
        Visibility flag.
        """
    @is_visible.setter
    def is_visible(self, arg0: bool) -> None:
        ...
    @property
    def name(self) -> str:
        """
        Object name.
        """
    @property
    def rect(self) -> pykraken._core.Rect:
        """
        Object bounding rectangle.
        """
    @property
    def shape_type(self) -> MapObject.ShapeType:
        """
        Shape type enum for the object.
        """
    @property
    def text(self) -> TextProperties:
        """
        Text properties if the object is text.
        """
    @property
    def tile_id(self) -> int:
        """
        Associated tile id when the object is a tile.
        """
    @property
    def transform(self) -> pykraken._core.Transform:
        """
        Transform component for the object.
        """
    @transform.setter
    def transform(self, arg0: pykraken._core.Transform) -> None:
        ...
    @property
    def type(self) -> str:
        """
        Object type string.
        """
    @property
    def uid(self) -> int:
        """
        Unique object identifier.
        """
    @property
    def vertices(self) -> list[pykraken._core.Vec2]:
        """
        List of vertices for polygon/polyline shapes.
        """
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
    """
    
    TMX map orientation values.
        
    """
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
    """
    
    Tile render order for TMX maps.
        
    """
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
    """
    
    Stagger axis for staggered/hex maps.
        
    """
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
    """
    
    Stagger index for staggered/hex maps.
        
    """
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
    """
    
    ObjectGroup is a layer containing placed MapObjects.
    
    Attributes:
        color (Color): Tint color applied to non-tile objects.
        opacity (float): Layer opacity.
        draw_order (DrawOrder): Drawing order for objects.
        objects (MapObjectList): List of contained MapObject instances.
    
    Methods:
        draw: Draw the object group.
        
    """
    class DrawOrder(enum.IntEnum):
        """
        
        Object drawing order for object layers.
            
        """
        INDEX: typing.ClassVar[ObjectGroup.DrawOrder]  # value = <DrawOrder.INDEX: 0>
        TOP_DOWN: typing.ClassVar[ObjectGroup.DrawOrder]  # value = <DrawOrder.TOP_DOWN: 1>
        @classmethod
        def __new__(cls, value):
            ...
        def __format__(self, format_spec):
            """
            Convert to a string according to format_spec.
            """
    def draw(self) -> None:
        """
        Draw the object group.
        """
    @property
    def color(self) -> pykraken._core.Color:
        """
        Tint color for non-tile objects.
        """
    @color.setter
    def color(self, arg0: pykraken._core.Color) -> None:
        ...
    @property
    def draw_order(self) -> ObjectGroup.DrawOrder:
        """
        Drawing order for objects in the group.
        """
    @property
    def objects(self) -> list[MapObject]:
        """
        MapObjectList of objects in the group.
        """
    @property
    def opacity(self) -> float:
        """
        Layer opacity from 0.0 to 1.0.
        """
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
class TextProperties:
    """
    
    TextProperties holds styling for text objects on the map.
    
    Attributes:
        font_family (str): Name of the font family.
        pixel_size (int): Font size in pixels.
        wrap (bool): Whether wrapping is enabled.
        color (Color): Text color.
        bold (bool): Bold style flag.
        italic (bool): Italic style flag.
        underline (bool): Underline flag.
        strikethrough (bool): Strikethrough flag.
        kerning (bool): Kerning enabled flag.
        align (Align): Horizontal alignment.
        text (str): The text content.
        
    """
    @property
    def align(self) -> pykraken._core.Align:
        """
        Horizontal text alignment.
        """
    @align.setter
    def align(self, arg0: pykraken._core.Align) -> None:
        ...
    @property
    def bold(self) -> bool:
        """
        Bold style flag.
        """
    @bold.setter
    def bold(self, arg0: bool) -> None:
        ...
    @property
    def color(self) -> pykraken._core.Color:
        """
        Text color.
        """
    @color.setter
    def color(self, arg0: pykraken._core.Color) -> None:
        ...
    @property
    def font_family(self) -> str:
        """
        Font family name.
        """
    @font_family.setter
    def font_family(self, arg0: str) -> None:
        ...
    @property
    def italic(self) -> bool:
        """
        Italic style flag.
        """
    @italic.setter
    def italic(self, arg0: bool) -> None:
        ...
    @property
    def kerning(self) -> bool:
        """
        Kerning enabled flag.
        """
    @kerning.setter
    def kerning(self, arg0: bool) -> None:
        ...
    @property
    def pixel_size(self) -> int:
        """
        Font size in pixels.
        """
    @pixel_size.setter
    def pixel_size(self, arg0: typing.SupportsInt) -> None:
        ...
    @property
    def strikethrough(self) -> bool:
        """
        Strikethrough style flag.
        """
    @strikethrough.setter
    def strikethrough(self, arg0: bool) -> None:
        ...
    @property
    def text(self) -> str:
        """
        Text content.
        """
    @text.setter
    def text(self, arg0: str) -> None:
        ...
    @property
    def underline(self) -> bool:
        """
        Underline style flag.
        """
    @underline.setter
    def underline(self, arg0: bool) -> None:
        ...
    @property
    def wrap(self) -> bool:
        """
        Whether text wrapping is enabled.
        """
    @wrap.setter
    def wrap(self, arg0: bool) -> None:
        ...
class TileLayer(Layer):
    """
    
    TileLayer represents a grid of tiles within the map.
    
    Attributes:
        opacity (float): Layer opacity (0.0-1.0).
        tiles (TileLayerTileList): List of `Tile` entries for the layer grid.
    
    Methods:
        get_from_area: Return tiles intersecting a Rect area.
        get_from_point: Return the tile at a given world position.
        draw: Draw the tile layer.
        
    """
    class Tile:
        """
        
        Tile represents an instance of a tile in a TileLayer.
        
        Attributes:
            id (int): Global tile id (GID).
            flip_flags (int): Flags describing tile flips/rotations.
            tileset_index (int): Index of the tileset this tile belongs to.
            
        """
        @property
        def flip_flags(self) -> int:
            """
            Tile flip/rotation flags.
            """
        @property
        def id(self) -> int:
            """
            Global tile id (GID).
            """
        @property
        def tileset_index(self) -> int:
            """
            Index of the tileset used by this tile.
            """
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
    class TileResult:
        """
        
        TileResult bundles a `Tile` with its world-space `Rect`.
        
        Attributes:
            tile (Tile): The tile entry.
            rect (Rect): The world-space rectangle covered by the tile.
            
        """
        @property
        def rect(self) -> pykraken._core.Rect:
            """
            World-space rectangle covered by the tile.
            """
        @property
        def tile(self) -> TileLayer.Tile:
            """
            The tile entry.
            """
    def draw(self) -> None:
        """
        Draw the tile layer.
        """
    def get_from_area(self, area: pykraken._core.Rect) -> list[TileLayer.TileResult]:
        """
        Return tiles intersecting a Rect area.
        
        Args:
            area (Rect): World-space area to query.
        
        Returns:
            list[TileLayer.TileResult]: List of TileResult entries for tiles intersecting the area.
        """
    def get_from_point(self, position: pykraken._core.Vec2) -> typing.Any:
        """
        Return the tile at a given world position.
        
        Args:
            position (Vec2): World-space position to query.
        
        Returns:
            Optional[TileLayer.TileResult]: TileResult entry if a tile exists at the position, None otherwise.
        """
    @property
    def opacity(self) -> float:
        """
        Layer opacity from 0.0 to 1.0.
        """
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def tiles(self) -> list[TileLayer.Tile]:
        """
        TileLayerTileList of tiles in the layer grid.
        """
class TileSet:
    """
    
    TileSet represents a collection of tiles and associated metadata.
    
    Attributes:
        first_gid (int): First global tile ID in the tileset.
        last_gid (int): Last global tile ID in the tileset.
        name (str): Name of the tileset.
        tile_size (Vec2): Size of individual tiles.
        spacing (int): Pixel spacing between tiles in the source image.
        margin (int): Margin in the source image.
        tile_count (int): Total number of tiles.
        columns (int): Number of tile columns in the source image.
        tile_offset (Vec2): Offset applied to tiles.
        terrains (TerrainList): List of terrain definitions.
        tiles (TileSetTileList): List of tile metadata.
        texture (Texture): Source texture for this tileset.
    
    Methods:
        has_tile: Check whether a global tile id belongs to this tileset.
        get_tile: Retrieve tile metadata for a given id.
        
    """
    class Terrain:
        """
        
        Terrain describes a named terrain type defined in a tileset.
        
        Attributes:
            name (str): Terrain name.
            tile_id (int): Representative tile id for the terrain.
            
        """
        @property
        def name(self) -> str:
            """
            Terrain name.
            """
        @property
        def tile_id(self) -> int:
            """
            Representative tile id for the terrain.
            """
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
        """
        
        Tile represents a single tile entry within a TileSet.
        
        Attributes:
            id (int): Local tile id.
            terrain_indices (list): Terrain indices for the tile.
            probability (float): Chance for auto-tiling/probability maps.
            clip_rect (Rect): Source rectangle in the tileset texture.
            
        """
        class TerrainIndices:
            def __getitem__(self: typing.Annotated[collections.abc.Sequence[typing.SupportsInt], "FixedSize(4)"], arg0: typing.SupportsInt) -> int:
                ...
            def __iter__(self: typing.Annotated[collections.abc.Sequence[typing.SupportsInt], "FixedSize(4)"]) -> collections.abc.Iterator[int]:
                ...
            def __len__(self: typing.Annotated[collections.abc.Sequence[typing.SupportsInt], "FixedSize(4)"]) -> int:
                ...
            def __repr__(self: typing.Annotated[collections.abc.Sequence[typing.SupportsInt], "FixedSize(4)"]) -> str:
                ...
            def __str__(self: typing.Annotated[collections.abc.Sequence[typing.SupportsInt], "FixedSize(4)"]) -> str:
                ...
        @property
        def clip_area(self) -> pykraken._core.Rect:
            """
            Source rectangle of the tile within the tileset texture.
            """
        @property
        def id(self) -> int:
            """
            Local tile id within the tileset.
            """
        @property
        def probability(self) -> int:
            """
            Probability used for weighted/random tile placement.
            """
        @property
        def terrain_indices(self) -> typing.Annotated[list[int], "FixedSize(4)"]:
            """
            TerrainIndices for each corner of the tile.
            """
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
        """
        Retrieve tile metadata for a given id.
        
        Args:
            id (int): Global tile id (GID).
        
        Returns:
            Tile: The tile metadata, or None if not found.
        """
    def has_tile(self, id: typing.SupportsInt) -> bool:
        """
        Check whether a global tile id belongs to this tileset.
        
        Args:
            id (int): Global tile id (GID).
        
        Returns:
            bool: True if the tileset contains the tile id, False otherwise.
        """
    @property
    def columns(self) -> int:
        """
        Number of tile columns in the source image.
        """
    @property
    def first_gid(self) -> int:
        """
        First global tile id (GID) in this tileset.
        """
    @property
    def last_gid(self) -> int:
        """
        Last global tile id (GID) in this tileset.
        """
    @property
    def margin(self) -> int:
        """
        Pixel margin around the source image.
        """
    @property
    def name(self) -> str:
        """
        Tileset name.
        """
    @property
    def spacing(self) -> int:
        """
        Pixel spacing between tiles in the source image.
        """
    @property
    def terrains(self) -> list[TileSet.Terrain]:
        """
        TerrainList of terrain definitions.
        """
    @property
    def texture(self) -> pykraken._core.Texture:
        """
        Source texture for the tileset.
        """
    @property
    def tile_count(self) -> int:
        """
        Total number of tiles in the tileset.
        """
    @property
    def tile_offset(self) -> pykraken._core.Vec2:
        """
        Per-tile offset applied when rendering.
        """
    @property
    def tile_size(self) -> pykraken._core.Vec2:
        """
        Size of tiles in pixels.
        """
    @property
    def tiles(self) -> list[TileSet.Tile]:
        """
        TileSetTileList of tile metadata entries.
        """
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
