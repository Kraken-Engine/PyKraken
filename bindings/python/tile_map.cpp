#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <algorithm>
#include <cmath>
#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "bindings/python/bindings.hpp"
#include "bindings/python/opaque_types.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/tilemap/TileMap.hpp"

namespace kn::tilemap
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subTilemap = module.def_submodule("tilemap", "Tile map handling module");

    // ----- Enums -----
    nb::enum_<tmx::Orientation>(subTilemap, "MapOrientation", R"doc(
TMX map orientation values.
    )doc")
        .value("ORTHOGONAL", tmx::Orientation::Orthogonal, "Orthogonal grid orientation")
        .value("ISOMETRIC", tmx::Orientation::Isometric, "Isometric orientation")
        .value("STAGGERED", tmx::Orientation::Staggered, "Staggered orientation")
        .value("HEXAGONAL", tmx::Orientation::Hexagonal, "Hexagonal orientation")
        .value("NONE", tmx::Orientation::None, "No orientation specified");

    nb::enum_<tmx::RenderOrder>(subTilemap, "MapRenderOrder", R"doc(
Tile render order for TMX maps.
    )doc")
        .value("RIGHT_DOWN", tmx::RenderOrder::RightDown, "Render right then down")
        .value("RIGHT_UP", tmx::RenderOrder::RightUp, "Render right then up")
        .value("LEFT_DOWN", tmx::RenderOrder::LeftDown, "Render left then down")
        .value("LEFT_UP", tmx::RenderOrder::LeftUp, "Render left then up")
        .value("NONE", tmx::RenderOrder::None, "No render order specified");

    nb::enum_<tmx::StaggerAxis>(subTilemap, "MapStaggerAxis", R"doc(
Stagger axis for staggered/hex maps.
    )doc")
        .value("X", tmx::StaggerAxis::X, "Stagger along the X axis")
        .value("Y", tmx::StaggerAxis::Y, "Stagger along the Y axis")
        .value("NONE", tmx::StaggerAxis::None, "No stagger axis");

    nb::enum_<tmx::StaggerIndex>(subTilemap, "MapStaggerIndex", R"doc(
Stagger index for staggered/hex maps.
    )doc")
        .value("EVEN", tmx::StaggerIndex::Even, "Even rows/columns are staggered")
        .value("ODD", tmx::StaggerIndex::Odd, "Odd rows/columns are staggered")
        .value("NONE", tmx::StaggerIndex::None, "No stagger index");

    nb::enum_<tmx::Layer::Type>(subTilemap, "LayerType", R"doc(
TMX layer type values.
    )doc")
        .value("TILE", tmx::Layer::Type::Tile, "Tile layer")
        .value("OBJECT", tmx::Layer::Type::Object, "Object layer")
        .value("IMAGE", tmx::Layer::Type::Image, "Image layer");

    // ----- TileSet -----
    auto tileSetClass = nb::class_<TileSet>(subTilemap, "TileSet", R"doc(
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
    )doc");

    auto tileSetTileClass = nb::class_<TileSet::Tile>(tileSetClass, "Tile", R"doc(
Tile represents a single tile entry within a TileSet.

Attributes:
    id (int): Local tile id.
    terrain_indices (list): Terrain indices for the tile.
    probability (float): Chance for auto-tiling/probability maps.
    clip_rect (Rect): Source rectangle in the tileset texture.
    )doc");

    nb::class_<std::array<int, 4>>(tileSetTileClass, "TerrainIndices")
        .def("__len__", [](const std::array<int, 4>&) { return 4; })
        .def(
            "__iter__",
            [](const std::array<int, 4>& arr)
            {
                return nb::make_iterator(
                    nb::type<std::array<int, 4>>(), "iterator", arr.begin(), arr.end()
                );
            },
            nb::keep_alive<0, 1>()
        )
        .def(
            "__getitem__",
            [](const std::array<int, 4>& arr, const size_t i)
            {
                if (i >= arr.size())
                    throw nb::index_error("Index out of range");
                return arr[i];
            }
        )
        .def(
            "__repr__",
            [](const std::array<int, 4>& arr)
            {
                std::string repr = "TerrainIndices(";
                for (size_t i = 0; i < arr.size(); ++i)
                {
                    repr += std::to_string(arr[i]);
                    if (i < arr.size() - 1)
                        repr += ", ";
                }
                repr += ")";
                return repr;
            }
        )
        .def(
            "__str__",
            [](const std::array<int, 4>& arr)
            {
                std::string str = "[";
                for (size_t i = 0; i < arr.size(); ++i)
                {
                    str += std::to_string(arr[i]);
                    if (i < arr.size() - 1)
                        str += ", ";
                }
                str += "]";
                return str;
            }
        );

    tileSetTileClass
        .def_prop_ro("id", &TileSet::Tile::getID, R"doc(
Local tile id within the tileset.
    )doc")
        .def_prop_ro(
            "terrain_indices", &TileSet::Tile::getTerrainIndices, nb::rv_policy::reference_internal,
            R"doc(TerrainIndices for each corner of the tile.)doc"
        )
        .def_prop_ro("probability", &TileSet::Tile::getProbability, R"doc(
Probability used for weighted/random tile placement.
    )doc")
        .def_prop_ro("clip_area", &TileSet::Tile::getClipArea, R"doc(
Source rectangle of the tile within the tileset texture.
    )doc");
    nb::bind_vector<std::vector<TileSet::Tile>>(tileSetClass, "TileSetTileList");

    nb::class_<TileSet::Terrain>(tileSetClass, "Terrain", R"doc(
Terrain describes a named terrain type defined in a tileset.

Attributes:
    name (str): Terrain name.
    tile_id (int): Representative tile id for the terrain.
    )doc")
        .def_prop_ro("name", &TileSet::Terrain::getName, R"doc(
Terrain name.
    )doc")
        .def_prop_ro("tile_id", &TileSet::Terrain::getTileID, R"doc(
Representative tile id for the terrain.
    )doc");
    nb::bind_vector<std::vector<TileSet::Terrain>>(tileSetClass, "TerrainList");

    tileSetClass
        .def("has_tile", &TileSet::hasTile, "id"_a, R"doc(
Check whether a global tile id belongs to this tileset.

Args:
    id (int): Global tile id (GID).

Returns:
    bool: True if the tileset contains the tile id, False otherwise.
        )doc")
        .def(
            "get_tile", &TileSet::getTile, "id"_a, nb::rv_policy::reference_internal,
            R"doc(
Retrieve tile metadata for a given id.

Args:
    id (int): Global tile id (GID).

Returns:
    Tile: The tile metadata, or None if not found.
        )doc"
        )

        .def_prop_ro(
            "first_gid", &TileSet::getFirstGID,
            R"doc(First global tile id (GID) in this tileset.)doc"
        )
        .def_prop_ro(
            "last_gid", &TileSet::getLastGID, R"doc(Last global tile id (GID) in this tileset.)doc"
        )
        .def_prop_ro("name", &TileSet::getName, R"doc(Tileset name.)doc")
        .def_prop_ro("tile_size", &TileSet::getTileSize, R"doc(Size of tiles in pixels.)doc")
        .def_prop_ro(
            "spacing", &TileSet::getSpacing,
            R"doc(Pixel spacing between tiles in the source image.)doc"
        )
        .def_prop_ro(
            "margin", &TileSet::getMargin, R"doc(Pixel margin around the source image.)doc"
        )
        .def_prop_ro(
            "tile_count", &TileSet::getTileCount, R"doc(Total number of tiles in the tileset.)doc"
        )
        .def_prop_ro(
            "columns", &TileSet::getColumns, R"doc(Number of tile columns in the source image.)doc"
        )
        .def_prop_ro(
            "tile_offset", &TileSet::getTileOffset,
            R"doc(Per-tile offset applied when rendering.)doc"
        )
        .def_prop_ro(
            "terrains", &TileSet::getTerrains, nb::rv_policy::reference_internal,
            R"doc(TerrainList of terrain definitions.)doc"
        )
        .def_prop_ro(
            "tiles", &TileSet::getTiles, nb::rv_policy::reference_internal,
            R"doc(TileSetTileList of tile metadata entries.)doc"
        )
        .def_prop_ro("texture", &TileSet::getTexture, R"doc(Source texture for the tileset.)doc");
    nb::bind_vector<std::vector<TileSet>>(subTilemap, "TileSetList");

    // ----- Layer -----
    nb::class_<Layer>(subTilemap, "Layer", R"doc(
Layer is the base class for all tilemap layers.

Attributes:
    visible (bool): Whether the layer is visible.
    offset (Vec2): Per-layer drawing offset.
    opacity (float): Layer opacity (0.0-1.0).
    name (str): Layer name.
    type (LayerType): Layer type enum.

Methods:
    draw: Draw the layer to the current renderer.
    )doc")
        .def_rw("visible", &Layer::visible, R"doc(
Whether the layer is visible.
    )doc")
        .def_rw("offset", &Layer::offset, R"doc(
Per-layer drawing offset.
    )doc")

        .def_prop_rw("opacity", &Layer::getOpacity, &Layer::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")

        .def_prop_ro("name", &Layer::getName, R"doc(Layer name.)doc")
        .def_prop_ro("type", &Layer::getType, R"doc(Layer type enum.)doc")

        .def("draw", &Layer::draw, "angle"_a = 0.0, "pivot"_a = Vec2{0.5, 0.5}, R"doc(
Draw the layer to the current renderer.

Args:
    angle (float, optional): Rotation angle in degrees. Defaults to 0.0.
    pivot (Vec2, optional): Rotation pivot as normalized coordinates relative to the map size. Defaults to (0.5, 0.5).
        )doc");
    nb::bind_vector<std::vector<std::shared_ptr<Layer>>>(subTilemap, "LayerList");

    // ----- TileLayer -----
    auto tileLayerClass = nb::class_<TileLayer, Layer>(subTilemap, "TileLayer", R"doc(
TileLayer represents a grid of tiles within the map.

Attributes:
    opacity (float): Layer opacity (0.0-1.0).
    tiles (TileLayerTileList): List of `Tile` entries for the layer grid.

Methods:
    get_from_area: Return tiles intersecting a Rect area.
    get_from_point: Return the tile at a given world position.
    draw: Draw the tile layer.
    )doc");

    nb::class_<TileLayer::Tile>(tileLayerClass, "Tile", R"doc(
Tile represents an instance of a tile in a TileLayer.

Attributes:
    id (int): Global tile id (GID).
    flip_flags (int): Flags describing tile flips/rotations.
    tileset_index (int): Index of the tileset this tile belongs to.
    )doc")
        .def_prop_ro("id", &TileLayer::Tile::getID, R"doc(Global tile id (GID).)doc")
        .def_prop_ro(
            "flip_flags", &TileLayer::Tile::getFlipFlags, R"doc(Tile flip/rotation flags.)doc"
        )
        .def_prop_ro(
            "tileset_index", &TileLayer::Tile::getTilesetIndex,
            R"doc(Index of the tileset used by this tile.)doc"
        );
    nb::bind_vector<std::vector<TileLayer::Tile>>(tileLayerClass, "TileLayerTileList");

    nb::class_<TileLayer::TileResult>(
        tileLayerClass, "TileResult", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY), R"doc(
TileResult bundles a `Tile` with its world-space `Rect`.

Attributes:
    tile (Tile): The tile entry.
    rect (Rect): The world-space rectangle covered by the tile.
    )doc"
    )
        .def_ro("tile", &TileLayer::TileResult::tile, R"doc(
The tile entry.
    )doc")
        .def_ro("rect", &TileLayer::TileResult::rect, R"doc(
World-space rectangle covered by the tile.
    )doc");

    tileLayerClass
        .def_prop_rw(
            "opacity", &TileLayer::getOpacity, &TileLayer::setOpacity,
            R"doc(Layer opacity from 0.0 to 1.0.)doc"
        )
        .def_prop_ro(
            "tiles", &TileLayer::getTiles, nb::rv_policy::reference_internal,
            R"doc(TileLayerTileList of tiles in the layer grid.)doc"
        )

        .def("get_from_area", &TileLayer::getFromArea, "area"_a, R"doc(
Return tiles intersecting a Rect area.

Args:
    area (Rect): World-space area to query.

Returns:
    list[TileLayer.TileResult]: List of TileResult entries for tiles intersecting the area.
        )doc")
        .def("get_from_point", &TileLayer::getFromPoint, "position"_a, R"doc(
Return the tile at a given world position.

Args:
    position (Vec2): World-space position to query.

Returns:
    Optional[TileLayer.TileResult]: TileResult entry if a tile exists at the position, None otherwise.
        )doc")
        .def("draw", &TileLayer::draw, "angle"_a = 0.0, "pivot"_a = Vec2{0.5, 0.5}, R"doc(
Draw the tile layer.

Args:
    angle (float, optional): Rotation angle in degrees. Defaults to 0.0.
    pivot (Vec2, optional): Rotation pivot as normalized coordinates relative to the map size. Defaults to (0.5, 0.5).
        )doc");

    // ----- MapObject -----
    nb::class_<TextProperties>(subTilemap, "TextProperties", R"doc(
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
    align (TextAlign): Horizontal alignment.
    text (str): The text content.
    )doc")
        .def_rw("font_family", &TextProperties::fontFamily, R"doc(
Font family name.
    )doc")
        .def_rw("pixel_size", &TextProperties::pixelSize, R"doc(
Font size in pixels.
    )doc")
        .def_rw("wrap", &TextProperties::wrap, R"doc(
Whether text wrapping is enabled.
    )doc")
        .def_rw("color", &TextProperties::color, R"doc(
Text color.
    )doc")
        .def_rw("bold", &TextProperties::bold, R"doc(
Bold style flag.
    )doc")
        .def_rw("italic", &TextProperties::italic, R"doc(
Italic style flag.
    )doc")
        .def_rw("underline", &TextProperties::underline, R"doc(
Underline style flag.
    )doc")
        .def_rw("strikethrough", &TextProperties::strikethrough, R"doc(
Strikethrough style flag.
    )doc")
        .def_rw("kerning", &TextProperties::kerning, R"doc(
Kerning enabled flag.
    )doc")
        .def_rw("align", &TextProperties::align, R"doc(
Horizontal text alignment.
    )doc")
        .def_rw("text", &TextProperties::text, R"doc(
Text content.
    )doc");

    auto mapObjectClass = nb::class_<MapObject>(subTilemap, "MapObject", R"doc(
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
    )doc");

    nb::enum_<tmx::Object::Shape>(mapObjectClass, "ShapeType", R"doc(
TMX object shape types.
    )doc")
        .value("RECTANGLE", tmx::Object::Shape::Rectangle, "Rectangle shape")
        .value("ELLIPSE", tmx::Object::Shape::Ellipse, "Ellipse shape")
        .value("POINT", tmx::Object::Shape::Point, "Point shape")
        .value("POLYGON", tmx::Object::Shape::Polygon, "Polygon shape")
        .value("POLYLINE", tmx::Object::Shape::Polyline, "Polyline shape")
        .value("TEXT", tmx::Object::Shape::Text, "Text object");

    mapObjectClass
        .def_rw("transform", &MapObject::transform, R"doc(
Transform component for the object.
    )doc")
        .def_rw("is_visible", &MapObject::visible, R"doc(
Visibility flag.
    )doc")

        .def_prop_ro("uid", &MapObject::getUID, R"doc(
Unique object identifier.
    )doc")
        .def_prop_ro("name", &MapObject::getName, R"doc(
Object name.
    )doc")
        .def_prop_ro("type", &MapObject::getType, R"doc(
Object type string.
    )doc")
        .def_prop_ro("rect", &MapObject::getRect, R"doc(
Object bounding rectangle.
    )doc")
        .def_prop_ro("tile_id", &MapObject::getTileID, R"doc(
Associated tile id when the object is a tile.
    )doc")
        .def_prop_ro("shape_type", &MapObject::getShapeType, R"doc(
Shape type enum for the object.
    )doc")
        .def_prop_ro("vertices", &MapObject::getVertices, R"doc(
List of vertices for polygon/polyline shapes.
    )doc")
        .def_prop_ro("text", &MapObject::getTextProperties, R"doc(
Text properties if the object is text.
    )doc");
    nb::bind_vector<std::vector<MapObject>>(subTilemap, "MapObjectList");

    // ----- ObjectGroup -----
    auto objGroupClass = nb::class_<ObjectGroup, Layer>(subTilemap, "ObjectGroup", R"doc(
ObjectGroup is a layer containing placed MapObjects.

Attributes:
    color (Color): Tint color applied to non-tile objects.
    opacity (float): Layer opacity.
    draw_order (DrawOrder): Drawing order for objects.
    objects (MapObjectList): List of contained MapObject instances.

Methods:
    draw: Draw the object group.
    )doc");

    nb::enum_<tmx::ObjectGroup::DrawOrder>(objGroupClass, "DrawOrder", R"doc(
Object drawing order for object layers.
    )doc")
        .value("INDEX", tmx::ObjectGroup::DrawOrder::Index, "Draw by object index")
        .value("TOP_DOWN", tmx::ObjectGroup::DrawOrder::TopDown, "Draw top-down by Y");

    objGroupClass
        .def_rw("color", &ObjectGroup::color, R"doc(Tint color for non-tile objects.)doc")

        .def_prop_rw("opacity", &ObjectGroup::getOpacity, &ObjectGroup::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")

        .def_prop_ro("draw_order", &ObjectGroup::getDrawOrder, R"doc(
Drawing order for objects in the group.
    )doc")
        .def_prop_ro(
            "objects", &ObjectGroup::getObjects, nb::rv_policy::reference_internal,
            R"doc(MapObjectList of objects in the group.)doc"
        )

        .def("draw", &ObjectGroup::draw, "angle"_a = 0.0, "pivot"_a = Vec2{0.5, 0.5}, R"doc(
Draw the object group.

Args:
    angle (float, optional): Rotation angle in degrees. Defaults to 0.0.
    pivot (Vec2, optional): Rotation pivot as normalized coordinates relative to the map size. Defaults to (0.5, 0.5).
        )doc");

    // ----- ImageLayer -----
    nb::class_<ImageLayer, Layer>(subTilemap, "ImageLayer", R"doc(
ImageLayer displays a single image as a layer.

Attributes:
    opacity (float): Layer opacity.
    texture (Texture): The layer image texture.

Methods:
    draw: Draw the image layer.
    )doc")
        .def_prop_rw("opacity", &ImageLayer::getOpacity, &ImageLayer::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")

        .def_prop_ro("texture", &ImageLayer::getTexture, R"doc(
Texture used by the image layer.
    )doc")

        .def("draw", &ImageLayer::draw, "angle"_a = 0.0, "pivot"_a = Vec2{0.5, 0.5}, R"doc(
Draw the image layer.

Args:
    angle (float, optional): Rotation angle in degrees. Defaults to 0.0.
    pivot (Vec2, optional): Rotation pivot as normalized coordinates relative to the map size. Defaults to (0.5, 0.5).
        )doc");

    // ----- Map -----
    nb::class_<Map>(subTilemap, "Map", R"doc(
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
    )doc")
        .def(nb::init<const std::filesystem::path&>(), "tmx_path"_a = "", R"doc(
Create a Map with the option to load an initial TMX file from the given path.

Args:
    tmx_path (str | os.PathLike[str], optional): Path to the TMX file to load during construction.
        )doc")

        .def_rw("background_color", &Map::backgroundColor, R"doc(Map background color.)doc")

        .def_prop_ro("orientation", &Map::getOrientation, R"doc(Map orientation enum.)doc")
        .def_prop_ro("render_order", &Map::getRenderOrder, R"doc(Tile render order enum.)doc")
        .def_prop_ro("map_size", &Map::getMapSize, R"doc(Map dimensions in tiles.)doc")
        .def_prop_ro("tile_size", &Map::getTileSize, R"doc(Size of tiles in pixels.)doc")
        .def_prop_ro("bounds", &Map::getBounds, R"doc(Map bounds in pixels.)doc")
        .def_prop_ro(
            "hex_side_length", &Map::getHexSideLength, R"doc(Hex side length for hex maps.)doc"
        )
        .def_prop_ro(
            "stagger_axis", &Map::getStaggerAxis,
            R"doc(Stagger axis enum for staggered/hex maps.)doc"
        )
        .def_prop_ro(
            "stagger_index", &Map::getStaggerIndex,
            R"doc(Stagger index enum for staggered/hex maps.)doc"
        )
        .def_prop_ro(
            "tile_sets", &Map::getTileSets, R"doc(TileSetList of tilesets used by the map.)doc"
        )
        .def_prop_ro(
            "all_layers", &Map::getAllLayers, nb::rv_policy::reference_internal,
            R"doc(LayerList of layers in the map.)doc"
        )
        .def_prop_ro(
            "tile_layers", &Map::getTileLayers, nb::rv_policy::reference_internal,
            R"doc(List of tile layers in the map.)doc"
        )
        .def_prop_ro(
            "object_groups", &Map::getObjectGroups, nb::rv_policy::reference_internal,
            R"doc(List of object group layers in the map.)doc"
        )
        .def_prop_ro(
            "image_layers", &Map::getImageLayers, nb::rv_policy::reference_internal,
            R"doc(List of image layers in the map.)doc"
        )

        .def("load", &Map::load, "tmx_path"_a, R"doc(
Load a TMX file from path.

Args:
    tmx_path (str | os.PathLike[str]): Path to the TMX file to load.
        )doc")
        .def(
            "draw", &Map::draw, "angle"_a = 0.0, "pivot"_a = Vec2{0.5, 0.5},
            R"doc(
Draw all layers.

Args:
    angle (float, optional): Rotation angle in degrees to apply to the entire map. Defaults to 0.0.
    pivot (Vec2, optional): Pivot point for rotation, as normalized coordinates relative to the map size. Defaults to (0.5, 0.5).
            )doc"
        )
        .def("get_layer", &Map::getLayer, "name"_a, nb::rv_policy::reference_internal, R"doc(
Get a layer by its name. Will return None if not found.

Args:
    name (str): Name of the layer to retrieve.
        )doc");
}
}  // namespace kn::tilemap
