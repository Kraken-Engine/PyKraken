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
        render: Draw the image layer.
        
    """
    def render(self) -> None:
        """
        Draw the image layer.
        """
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
    """
    
    Layer is the base class for all tilemap layers.
    
    Attributes:
        visible (bool): Whether the layer is visible.
        offset (Vec2): Per-layer drawing offset.
        opacity (float): Layer opacity (0.0-1.0).
        name (str): Layer name.
        type (LayerType): Layer type enum.
    
    Methods:
        render: Draw the layer to the current renderer.
        
    """
    offset: pykraken._core.Vec2
    visible: bool
    def render(self) -> None:
        """
        Draw the layer to the current renderer.
        """
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
    def __bool__(self) -> bool:
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: Layer) -> bool:
        """
        Return true the container contains ``x``
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
    def __eq__(self, arg0: LayerList) -> bool:
        ...
    @typing.overload
    def __getitem__(self, s: slice) -> LayerList:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: typing.SupportsInt) -> Layer:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: LayerList) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self) -> collections.abc.Iterator[Layer]:
        ...
    def __len__(self) -> int:
        ...
    def __ne__(self, arg0: LayerList) -> bool:
        ...
    def __repr__(self) -> str:
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: typing.SupportsInt, arg1: Layer) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: LayerList) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: Layer) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    def count(self, x: Layer) -> int:
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: LayerList) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: typing.SupportsInt, x: Layer) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Layer:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: typing.SupportsInt) -> Layer:
        """
        Remove and return the item at index ``i``
        """
    def remove(self, x: Layer) -> None:
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
    """
    
    Map represents a loaded TMX map and provides access to its layers and tilesets.
    
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
        tile_sets (list): List of TileSet objects.
        layers (list): List of Layer instances.
    
    Methods:
        load: Load a TMX file from path.
        render: Render all layers.
        
    """
    background_color: pykraken._core.Color
    def __init__(self) -> None:
        ...
    def load(self, tmx_path: str) -> None:
        """
        Load a TMX file from path.
        
        Args:
            tmx_path (str): Path to the TMX file to load.
        """
    def render(self) -> None:
        """
        Render all layers.
        """
    @property
    def bounds(self) -> pykraken._core.Rect:
        ...
    @property
    def hex_side_length(self) -> float:
        ...
    @property
    def layers(self) -> LayerList:
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
    def tile_sets(self) -> TileSetList:
        ...
    @property
    def tile_size(self) -> pykraken._core.Vec2:
        ...
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
        vertices (list): Vertex list for polygon/polyline shapes.
        text (TextProperties): Text properties when shape is text.
        
    """
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
    def vertices(self) -> pykraken._core.Vec2List:
        ...
class MapObjectList:
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
    def __getitem__(self, s: slice) -> MapObjectList:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: typing.SupportsInt) -> MapObject:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: MapObjectList) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self) -> collections.abc.Iterator[MapObject]:
        ...
    def __len__(self) -> int:
        ...
    @typing.overload
    def __setitem__(self, arg0: typing.SupportsInt, arg1: MapObject) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: MapObjectList) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: MapObject) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    @typing.overload
    def extend(self, L: MapObjectList) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: typing.SupportsInt, x: MapObject) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> MapObject:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: typing.SupportsInt) -> MapObject:
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
    """
    
    ObjectGroup is a layer containing placed MapObjects.
    
    Attributes:
        color (Color): Tint color applied to non-tile objects.
        opacity (float): Layer opacity.
        draw_order (DrawOrder): Drawing order for objects.
        objects (list): List of contained MapObject instances.
    
    Methods:
        render: Draw the object group.
        
    """
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
        """
        Draw the object group.
        """
    @property
    def draw_order(self) -> ObjectGroup.DrawOrder:
        ...
    @property
    def objects(self) -> MapObjectList:
        ...
    @property
    def opacity(self) -> float:
        ...
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
    """
    
    TileLayer represents a grid of tiles within the map.
    
    Attributes:
        opacity (float): Layer opacity (0.0-1.0).
        tiles (list): List of `Tile` entries for the layer grid.
    
    Methods:
        get_from_area: Return tiles intersecting a Rect area.
        get_from_point: Return the tile at a given world position.
        render: Draw the tile layer.
        
    """
    class Tile:
        """
        
        Tile represents an instance of a tile in a TileLayer.
        
        Attributes:
            id (int): Global tile id (GID).
            flip_flags (int): Flags describing tile flips/rotations.
            
        """
        @property
        def flip_flags(self) -> int:
            ...
        @property
        def id(self) -> int:
            ...
    class TileLayerTileList:
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
        def __getitem__(self, s: slice) -> TileLayer.TileLayerTileList:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self, arg0: typing.SupportsInt) -> TileLayer.Tile:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: TileLayer.TileLayerTileList) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self) -> collections.abc.Iterator[TileLayer.Tile]:
            ...
        def __len__(self) -> int:
            ...
        @typing.overload
        def __setitem__(self, arg0: typing.SupportsInt, arg1: TileLayer.Tile) -> None:
            ...
        @typing.overload
        def __setitem__(self, arg0: slice, arg1: TileLayer.TileLayerTileList) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self, x: TileLayer.Tile) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self, L: TileLayer.TileLayerTileList) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self, L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self, i: typing.SupportsInt, x: TileLayer.Tile) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self) -> TileLayer.Tile:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self, i: typing.SupportsInt) -> TileLayer.Tile:
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
            ...
        @property
        def tile(self) -> TileLayer.Tile:
            ...
    class TileResultList:
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
        def __getitem__(self, s: slice) -> TileLayer.TileResultList:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self, arg0: typing.SupportsInt) -> TileLayer.TileResult:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: TileLayer.TileResultList) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self) -> collections.abc.Iterator[TileLayer.TileResult]:
            ...
        def __len__(self) -> int:
            ...
        @typing.overload
        def __setitem__(self, arg0: typing.SupportsInt, arg1: TileLayer.TileResult) -> None:
            ...
        @typing.overload
        def __setitem__(self, arg0: slice, arg1: TileLayer.TileResultList) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self, x: TileLayer.TileResult) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self, L: TileLayer.TileResultList) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self, L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self, i: typing.SupportsInt, x: TileLayer.TileResult) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self) -> TileLayer.TileResult:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self, i: typing.SupportsInt) -> TileLayer.TileResult:
            """
            Remove and return the item at index ``i``
            """
    def get_from_area(self, area: pykraken._core.Rect) -> TileLayer.TileResultList:
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
    def render(self) -> None:
        """
        Draw the tile layer.
        """
    @property
    def opacity(self) -> float:
        ...
    @opacity.setter
    def opacity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def tiles(self) -> TileLayer.TileLayerTileList:
        ...
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
        terrains (list): List of terrain definitions.
        tiles (list): List of tile metadata.
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
            ...
        @property
        def tile_id(self) -> int:
            ...
    class TerrainList:
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
        def __getitem__(self, s: slice) -> TileSet.TerrainList:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self, arg0: typing.SupportsInt) -> TileSet.Terrain:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: TileSet.TerrainList) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self) -> collections.abc.Iterator[TileSet.Terrain]:
            ...
        def __len__(self) -> int:
            ...
        @typing.overload
        def __setitem__(self, arg0: typing.SupportsInt, arg1: TileSet.Terrain) -> None:
            ...
        @typing.overload
        def __setitem__(self, arg0: slice, arg1: TileSet.TerrainList) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self, x: TileSet.Terrain) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self, L: TileSet.TerrainList) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self, L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self, i: typing.SupportsInt, x: TileSet.Terrain) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self) -> TileSet.Terrain:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self, i: typing.SupportsInt) -> TileSet.Terrain:
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
            def __getitem__(self, arg0: typing.SupportsInt) -> int:
                ...
            def __iter__(self) -> collections.abc.Iterator[int]:
                ...
            def __len__(self) -> int:
                ...
            def __repr__(self) -> str:
                ...
            def __str__(self) -> str:
                ...
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
        def terrain_indices(self) -> TileSet.Tile.TerrainIndices:
            ...
    class TileSetTileList:
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
        def __getitem__(self, s: slice) -> TileSet.TileSetTileList:
            """
            Retrieve list elements using a slice object
            """
        @typing.overload
        def __getitem__(self, arg0: typing.SupportsInt) -> TileSet.Tile:
            ...
        @typing.overload
        def __init__(self) -> None:
            ...
        @typing.overload
        def __init__(self, arg0: TileSet.TileSetTileList) -> None:
            """
            Copy constructor
            """
        @typing.overload
        def __init__(self, arg0: collections.abc.Iterable) -> None:
            ...
        def __iter__(self) -> collections.abc.Iterator[TileSet.Tile]:
            ...
        def __len__(self) -> int:
            ...
        @typing.overload
        def __setitem__(self, arg0: typing.SupportsInt, arg1: TileSet.Tile) -> None:
            ...
        @typing.overload
        def __setitem__(self, arg0: slice, arg1: TileSet.TileSetTileList) -> None:
            """
            Assign list elements using a slice object
            """
        def append(self, x: TileSet.Tile) -> None:
            """
            Add an item to the end of the list
            """
        def clear(self) -> None:
            """
            Clear the contents
            """
        @typing.overload
        def extend(self, L: TileSet.TileSetTileList) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        @typing.overload
        def extend(self, L: collections.abc.Iterable) -> None:
            """
            Extend the list by appending all the items in the given list
            """
        def insert(self, i: typing.SupportsInt, x: TileSet.Tile) -> None:
            """
            Insert an item at a given position.
            """
        @typing.overload
        def pop(self) -> TileSet.Tile:
            """
            Remove and return the last item
            """
        @typing.overload
        def pop(self, i: typing.SupportsInt) -> TileSet.Tile:
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
    def terrains(self) -> TileSet.TerrainList:
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
    def tiles(self) -> TileSet.TileSetTileList:
        ...
class TileSetList:
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
    def __getitem__(self, s: slice) -> TileSetList:
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, arg0: typing.SupportsInt) -> TileSet:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, arg0: TileSetList) -> None:
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: collections.abc.Iterable) -> None:
        ...
    def __iter__(self) -> collections.abc.Iterator[TileSet]:
        ...
    def __len__(self) -> int:
        ...
    @typing.overload
    def __setitem__(self, arg0: typing.SupportsInt, arg1: TileSet) -> None:
        ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: TileSetList) -> None:
        """
        Assign list elements using a slice object
        """
    def append(self, x: TileSet) -> None:
        """
        Add an item to the end of the list
        """
    def clear(self) -> None:
        """
        Clear the contents
        """
    @typing.overload
    def extend(self, L: TileSetList) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: collections.abc.Iterable) -> None:
        """
        Extend the list by appending all the items in the given list
        """
    def insert(self, i: typing.SupportsInt, x: TileSet) -> None:
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> TileSet:
        """
        Remove and return the last item
        """
    @typing.overload
    def pop(self, i: typing.SupportsInt) -> TileSet:
        """
        Remove and return the item at index ``i``
        """
