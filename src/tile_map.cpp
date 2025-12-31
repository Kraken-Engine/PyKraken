#include <pybind11/native_enum.h>
// #include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "Draw.hpp"
#include "Line.hpp"
#include "Polygon.hpp"
#include "Renderer.hpp"
#include "TileMap.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

namespace kn
{
namespace tilemap
{
void Map::load(const std::string& tmxPath)
{
    tmx::Map tmxMap;
    if (!tmxMap.load(tmxPath))
    {
        throw std::runtime_error("Failed to load TMX map from path: " + tmxPath);
    }

    m_orient = tmxMap.getOrientation();
    m_renderOrder = tmxMap.getRenderOrder();
    m_staggerAxis = tmxMap.getStaggerAxis();
    m_staggerIndex = tmxMap.getStaggerIndex();

    const auto& mapSize = tmxMap.getTileCount();
    m_mapSize = {mapSize.x, mapSize.y};

    const auto& tileSize = tmxMap.getTileSize();
    m_tileSize = {tileSize.x, tileSize.y};

    const auto rect = tmxMap.getBounds();
    m_bounds = {rect.left, rect.top, rect.width, rect.height};

    const auto& color = tmxMap.getBackgroundColour();
    backgroundColor = {color.r, color.g, color.b, color.a};

    m_hexSideLength = static_cast<double>(tmxMap.getHexSideLength());

    // Load layers
    for (const auto& tmxLayer : tmxMap.getLayers())
    {
        std::shared_ptr<Layer> layer = nullptr;
        switch (tmxLayer->getType())
        {
        case tmx::Layer::Type::Tile:
        {
            const auto& tmxTileLayer = tmxLayer->getLayerAs<tmx::TileLayer>();
            const auto& tileOffset = tmxTileLayer.getOffset();

            auto tileLayer = std::make_shared<TileLayer>();
            tileLayer->m_type = tmx::Layer::Type::Tile;
            tileLayer->m_name = tmxTileLayer.getName();
            tileLayer->offset = {tileOffset.x, tileOffset.y};
            tileLayer->visible = tmxTileLayer.getVisible();

            const auto& tmxTiles = tmxTileLayer.getTiles();
            tileLayer->m_tiles.reserve(tmxTiles.size());
            for (const auto& tmxTile : tmxTiles)
            {
                TileLayer::Tile tile;
                tile.m_id = tmxTile.ID;
                tile.m_flipFlags = tmxTile.flipFlags;
                tileLayer->m_tiles.push_back(std::move(tile));
            }

            tileLayer->setOpacity(tmxTileLayer.getOpacity());

            layer = tileLayer;
            break;
        }
        case tmx::Layer::Type::Object:
        {
            const auto& tmxObjLayer = tmxLayer->getLayerAs<tmx::ObjectGroup>();
            const auto& tileOffset = tmxObjLayer.getOffset();
            const auto& objColor = tmxObjLayer.getColour();

            auto objGroup = std::make_shared<ObjectGroup>();
            objGroup->m_type = tmx::Layer::Type::Object;
            objGroup->m_name = tmxObjLayer.getName();
            objGroup->offset = {tileOffset.x, tileOffset.y};
            objGroup->visible = tmxObjLayer.getVisible();

            objGroup->color = {objColor.r, objColor.g, objColor.b, objColor.a};
            objGroup->m_drawOrder = tmxObjLayer.getDrawOrder();

            const auto& tmxObjects = tmxObjLayer.getObjects();
            objGroup->m_objects.reserve(tmxObjects.size());
            for (const auto& tmxObject : tmxObjects)
            {
                MapObject mapObj;
                mapObj.m_uid = tmxObject.getUID();
                mapObj.m_name = tmxObject.getName();
                mapObj.m_type = tmxObject.getType();
                const auto& pos = tmxObject.getPosition();
                mapObj.transform.pos = {pos.x, pos.y};
                const auto& aabb = tmxObject.getAABB();
                mapObj.m_rect = {aabb.left, aabb.top, aabb.width, aabb.height};
                mapObj.m_tileId = tmxObject.getTileID();
                mapObj.m_shape = tmxObject.getShape();
                mapObj.transform.angle = math::toRadians(
                    static_cast<double>(tmxObject.getRotation())
                );
                mapObj.visible = tmxObject.visible();

                const auto& points = tmxObject.getPoints();
                mapObj.m_vertices.reserve(points.size());
                for (const auto& point : points)
                {
                    mapObj.m_vertices.emplace_back(point.x, point.y);
                }

                const auto& tmxText = tmxObject.getText();
                mapObj.m_text = TextProperties{
                    tmxText.fontFamily,
                    tmxText.pixelSize,
                    tmxText.wrap,
                    {tmxText.colour.r, tmxText.colour.g, tmxText.colour.b, tmxText.colour.a},
                    tmxText.bold,
                    tmxText.italic,
                    tmxText.underline,
                    tmxText.strikethough,
                    tmxText.kerning,
                    static_cast<Align>(tmxText.hAlign),
                    tmxText.content
                };

                objGroup->m_objects.push_back(std::move(mapObj));
            }

            if (objGroup->m_drawOrder == tmx::ObjectGroup::DrawOrder::TopDown)
            {
                using SortEntry = std::pair<size_t, double>;

                std::vector<SortEntry> sortEntries;
                sortEntries.reserve(objGroup->m_objects.size());
                for (size_t i = 0; i < objGroup->m_objects.size(); ++i)
                    sortEntries.emplace_back(i, objGroup->m_objects[i].getRect().getBottom());

                std::sort(
                    sortEntries.begin(), sortEntries.end(),
                    [](const SortEntry& a, const SortEntry& b) { return a.second < b.second; }
                );

                std::vector<MapObject> sortedObjects;
                sortedObjects.reserve(objGroup->m_objects.size());
                for (const auto& entry : sortEntries)
                    sortedObjects.push_back(std::move(objGroup->m_objects[entry.first]));

                objGroup->m_objects = std::move(sortedObjects);
            }

            objGroup->setOpacity(tmxObjLayer.getOpacity());

            layer = objGroup;
            break;
        }
        case tmx::Layer::Type::Image:
        {
            const auto& tmxImgLayer = tmxLayer->getLayerAs<tmx::ImageLayer>();
            const auto& tileOffset = tmxImgLayer.getOffset();

            auto imgLayer = std::make_shared<ImageLayer>();
            imgLayer->m_type = tmx::Layer::Type::Image;
            imgLayer->m_name = tmxImgLayer.getName();
            imgLayer->offset = {tileOffset.x, tileOffset.y};
            imgLayer->visible = tmxImgLayer.getVisible();

            imgLayer->m_texture = std::make_shared<Texture>(tmxImgLayer.getImagePath());
            imgLayer->transform.pos = {tileOffset.x, tileOffset.y};
            imgLayer->setOpacity(tmxImgLayer.getOpacity());

            layer = imgLayer;
            break;
        }
        }

        if (layer)
        {
            layer->m_map = this;
            m_layers.push_back(std::move(layer));
        }
    }

    // Load tilesets
    for (const auto& tmxTileset : tmxMap.getTilesets())
    {
        const auto& tsTileSize = tmxTileset.getTileSize();
        const auto& tsTileOffset = tmxTileset.getTileOffset();
        const auto& tsTerrainTypes = tmxTileset.getTerrainTypes();
        const auto& tsTiles = tmxTileset.getTiles();

        TileSet tileSet;
        tileSet.m_firstGID = tmxTileset.getFirstGID();
        tileSet.m_lastGID = tmxTileset.getLastGID();
        tileSet.m_name = tmxTileset.getName();
        tileSet.m_tileSize = {tsTileSize.x, tsTileSize.y};
        tileSet.m_spacing = tmxTileset.getSpacing();
        tileSet.m_margin = tmxTileset.getMargin();
        tileSet.m_tileCount = tmxTileset.getTileCount();
        tileSet.m_columns = tmxTileset.getColumnCount();
        tileSet.m_tileOffset = {tsTileOffset.x, tsTileOffset.y};
        tileSet.m_texture = std::make_shared<Texture>(tmxTileset.getImagePath());

        tileSet.m_terrains.reserve(tsTerrainTypes.size());
        for (const auto& tmxTerrain : tsTerrainTypes)
        {
            TileSet::Terrain terrain{tmxTerrain.name, tmxTerrain.tileID};
            tileSet.m_terrains.push_back(std::move(terrain));
        }
        tileSet.m_tiles.reserve(tsTiles.size());
        for (const auto& tmxTile : tsTiles)
        {
            // clang-format off
            TileSet::Tile tile{
                tmxTile.ID,
                tmxTile.terrainIndices,
                tmxTile.probability,
                {
                    tmxTile.imagePosition.x,
                    tmxTile.imagePosition.y,
                    tmxTile.imageSize.x,
                    tmxTile.imageSize.y
                }
            };
            tileSet.m_tiles.push_back(std::move(tile));
            // clang-format on
        }

        tileSet.m_tileIndex.assign(tileSet.m_tileCount, 0);
        for (const auto& tile : tileSet.m_tiles)
        {
            tileSet.m_tileIndex[tile.m_ID] = tile.m_ID + 1;
        }

        m_tileSets.push_back(std::move(tileSet));
    }
}

tmx::Orientation Map::getOrientation() const
{
    return m_orient;
}

void Map::render()
{
    for (const auto& layer : m_layers)
    {
        layer->render();
    }
}

tmx::RenderOrder Map::getRenderOrder() const
{
    return m_renderOrder;
}

Vec2 Map::getMapSize() const
{
    return m_mapSize;
}

Vec2 Map::getTileSize() const
{
    return m_tileSize;
}

Rect Map::getBounds() const
{
    return m_bounds;
}

double Map::getHexSideLength() const
{
    return m_hexSideLength;
}

tmx::StaggerAxis Map::getStaggerAxis() const
{
    return m_staggerAxis;
}

tmx::StaggerIndex Map::getStaggerIndex() const
{
    return m_staggerIndex;
}

const std::vector<TileSet>& Map::getTileSets() const
{
    return m_tileSets;
}

const std::vector<std::shared_ptr<Layer>>& Map::getLayers() const
{
    return m_layers;
}

uint32_t TileSet::getFirstGID() const
{
    return m_firstGID;
}

uint32_t TileSet::getLastGID() const
{
    return m_firstGID + m_tileCount - 1;
}

std::string TileSet::getName() const
{
    return m_name;
}

Vec2 TileSet::getTileSize() const
{
    return m_tileSize;
}

uint32_t TileSet::getSpacing() const
{
    return m_spacing;
}

uint32_t TileSet::getMargin() const
{
    return m_margin;
}

uint32_t TileSet::getTileCount() const
{
    return m_tileCount;
}

uint32_t TileSet::getColumns() const
{
    return m_columns;
}

Vec2 TileSet::getTileOffset() const
{
    return m_tileOffset;
}

const std::vector<TileSet::Terrain>& TileSet::getTerrains() const
{
    return m_terrains;
}

const std::vector<TileSet::Tile>& TileSet::getTiles() const
{
    return m_tiles;
}

bool TileSet::hasTile(const uint32_t id) const
{
    return id >= m_firstGID && id <= m_lastGID;
}

const TileSet::Tile* TileSet::getTile(uint32_t id) const
{
    if (!hasTile(id))
        return nullptr;

    const uint32_t local = id - m_firstGID;
    if (local >= m_tileIndex.size())
        return nullptr;

    const uint32_t idx = m_tileIndex[local];
    return idx ? &m_tiles[idx - 1] : nullptr;
}

std::shared_ptr<Texture> TileSet::getTexture() const
{
    return m_texture;
}

std::string Layer::getName() const
{
    return m_name;
}

tmx::Layer::Type Layer::getType() const
{
    return m_type;
}

const std::vector<TileLayer::Tile>& TileLayer::getTiles() const
{
    return m_tiles;
}

void TileLayer::setOpacity(const double value)
{
    m_opacity = value;
}

double TileLayer::getOpacity() const
{
    return m_opacity;
}

void TileLayer::render()
{
    if (!visible)
        return;

    // Only orthogonal maps supported for now
    if (m_map->getOrientation() != tmx::Orientation::Orthogonal)
        return;

    // Only right-down render order supported for now
    if (m_map->getRenderOrder() != tmx::RenderOrder::RightDown)
        return;

    const auto mapW = static_cast<int>(m_map->getMapSize().x);
    const auto mapH = static_cast<int>(m_map->getMapSize().y);
    const auto tileW = static_cast<int>(m_map->getTileSize().x);
    const auto tileH = static_cast<int>(m_map->getTileSize().y);

    Transform renderTransform{};

    for (size_t y = 0; y < mapH; ++y)
    {
        for (size_t x = 0; x < mapW; ++x)
        {
            const size_t index = y * static_cast<size_t>(mapW) + x;
            if (index >= m_tiles.size())
                continue;

            const TileLayer::Tile& tile = m_tiles[index];
            const uint32_t gid = tile.getID();
            if (gid == 0)
                continue;

            const TileSet* foundSet = nullptr;
            for (const auto& ts : m_map->getTileSets())
            {
                if (ts.hasTile(gid))
                {
                    foundSet = &ts;
                    break;
                }
            }

            if (!foundSet)
                continue;

            const TileSet::Tile* setTile = foundSet->getTile(gid);
            const auto setTexture = foundSet->getTexture();
            if (!setTile || !setTexture)
                continue;

            setTexture->setAlpha(static_cast<float>(m_opacity));

            renderTransform.pos = offset + Vec2{x * tileW, y * tileH};

            const uint8_t flipFlags = tile.getFlipFlags();
            const bool flipH = flipFlags & tmx::TileLayer::FlipFlag::Horizontal;
            const bool flipV = flipFlags & tmx::TileLayer::FlipFlag::Vertical;
            const bool flipD = flipFlags & tmx::TileLayer::FlipFlag::Diagonal;
            double rotation = 0.0;
            bool h = false;
            bool v = false;

            if (!flipD)
            {
                h = flipH;
                v = flipV;
            }
            else
            {
                if (flipH && flipV)
                {
                    rotation = -M_PI_2;
                    h = false;
                    v = true;
                }
                else if (flipH)
                {
                    rotation = M_PI_2;
                }
                else if (flipV)
                {
                    rotation = -M_PI_2;
                }
                else
                {
                    rotation = M_PI_2;
                    h = false;
                    v = true;
                }
            }
            renderTransform.angle = rotation;
            setTexture->flip.h = h;
            setTexture->flip.v = v;

            renderer::draw(setTexture, renderTransform, setTile->getClipRect());
        }
    }
}

std::vector<TileLayer::TileResult> TileLayer::getFromArea(const Rect& area) const
{
    std::vector<TileLayer::TileResult> foundTiles;

    const double tileW = m_map->getTileSize().x;
    const double tileH = m_map->getTileSize().y;
    const auto mapW = static_cast<int>(m_map->getMapSize().x);
    const auto mapH = static_cast<int>(m_map->getMapSize().y);

    // Calculate the grid range (clamped to map boundaries)
    const int startX =
        std::max(0, static_cast<int>(std::floor((area.getLeft() - offset.x) / tileW)));
    const int startY =
        std::max(0, static_cast<int>(std::floor((area.getTop() - offset.y) / tileH)));
    const int endX =
        std::min(mapW - 1, static_cast<int>(std::floor((area.getRight() - offset.x) / tileW)));
    const int endY =
        std::min(mapH - 1, static_cast<int>(std::floor((area.getBottom() - offset.y) / tileH)));

    // Reserve space to avoid frequent reallocations
    foundTiles.reserve((endX - startX + 1) * (endY - startY + 1));

    for (int y = startY; y <= endY; ++y)
        for (int x = startX; x <= endX; ++x)
        {
            const auto index = static_cast<size_t>(y * mapW + x);
            if (index < m_tiles.size() && m_tiles[index].getID() != 0)
            {
                const TileLayer::TileResult result{
                    m_tiles[index],
                    {
                        offset.x + x * tileW,
                        offset.y + y * tileH,
                        tileW,
                        tileH,
                    }
                };
                foundTiles.push_back(std::move(result));
            }
        }

    return foundTiles;
}

std::optional<TileLayer::TileResult> TileLayer::getFromPoint(const Vec2& position) const
{
    // Adjust position by the layer's offset
    const double localX = position.x - offset.x;
    const double localY = position.y - offset.y;

    const double tileW = m_map->getTileSize().x;
    const double tileH = m_map->getTileSize().y;
    const double mapW = m_map->getMapSize().x;
    const double mapH = m_map->getMapSize().y;

    // Convert world coordinates to grid coordinates
    const auto x = static_cast<int>(std::floor(localX / tileW));
    const auto y = static_cast<int>(std::floor(localY / tileH));

    // Bounds check
    if (x < 0 || x >= mapW || y < 0 || y >= mapH)
    {
        return std::nullopt;
    }

    const auto index = static_cast<size_t>(y * mapW + x);
    if (index >= m_tiles.size())
    {
        return std::nullopt;
    }

    const TileResult result{
        m_tiles[index],
        {
            offset.x + x * tileW,
            offset.y + y * tileH,
            tileW,
            tileH,
        }
    };

    return result;
}

uint32_t MapObject::getUID() const
{
    return m_uid;
}

std::string MapObject::getName() const
{
    return m_name;
}

std::string MapObject::getType() const
{
    return m_type;
}

Rect MapObject::getRect() const
{
    return m_rect;
}

uint32_t MapObject::getTileID() const
{
    return m_tileId;
}

tmx::Object::Shape MapObject::getShapeType() const
{
    return m_shape;
}

const std::vector<Vec2>& MapObject::getVertices() const
{
    return m_vertices;
}

const TextProperties& MapObject::getTextProperties() const
{
    return m_text;
}

tmx::ObjectGroup::DrawOrder ObjectGroup::getDrawOrder() const
{
    return m_drawOrder;
}

const std::vector<MapObject>& ObjectGroup::getObjects() const
{
    return m_objects;
}

void ObjectGroup::setOpacity(const double value)
{
    m_opacity = value;
}

double ObjectGroup::getOpacity() const
{
    return m_opacity;
}

void ObjectGroup::render()
{
    if (!visible)
        return;

    for (const auto& obj : m_objects)
    {
        if (!obj.visible)
            continue;

        if (obj.getTileID() != 0)
        {
            const uint32_t gid = obj.getTileID();

            const TileSet* foundTS = nullptr;
            for (const auto& ts : m_map->getTileSets())
            {
                if (ts.hasTile(gid))
                {
                    foundTS = &ts;
                    break;
                }
            }

            if (!foundTS)
                continue;

            const auto* tile = foundTS->getTile(gid);
            auto setTexture = foundTS->getTexture();
            if (!tile || !setTexture)
                continue;

            setTexture->setAlpha(static_cast<float>(m_opacity));
            if (tile && setTexture)
            {
                Transform renderTransform = obj.transform;
                renderTransform.pos += offset;
                renderer::draw(setTexture, renderTransform, tile->getClipRect());
            }

            continue;
        }

        const Vec2 renderOffset = offset + obj.transform.pos;
        Color drawColor = color;
        drawColor.a = static_cast<uint8_t>(static_cast<double>(drawColor.a) * m_opacity);

        switch (obj.getShapeType())
        {
        case tmx::Object::Shape::Rectangle:
        {
            Rect rect = obj.getRect();
            // Only add offset since position is already included in rect
            rect.setTopLeft(rect.getTopLeft() + offset);
            draw::rect(rect, drawColor);
            break;
        }

        case tmx::Object::Shape::Ellipse:
        {
            Rect rect = obj.getRect();
            rect.setTopLeft(rect.getTopLeft() + offset);
            draw::ellipse(rect, drawColor, true);
            break;
        }

        case tmx::Object::Shape::Point:
            if (const auto& verts = obj.getVertices(); !verts.empty())
                draw::point(verts[0] + renderOffset, drawColor);
            break;

        case tmx::Object::Shape::Polygon:
        {
            Polygon polygon{obj.getVertices()};
            polygon.translate(renderOffset);
            draw::polygon(polygon, drawColor, true);
            break;
        }

        case tmx::Object::Shape::Polyline:
        {
            const auto& verts = obj.getVertices();
            if (verts.size() < 2)
                break;

            for (size_t i = 1; i < verts.size(); ++i)
            {
                const Vec2 start = renderOffset + verts[i - 1];
                const Vec2 end = renderOffset + verts[i];
                draw::line(Line{start, end}, drawColor);
            }
            break;
        }

        default:
            break;
        }
    }
}

std::shared_ptr<Texture> ImageLayer::getTexture() const
{
    return m_texture;
}

void ImageLayer::render()
{
    if (!visible)
        return;

    renderer::draw(m_texture, transform);
}

void ImageLayer::setOpacity(const double value)
{
    m_opacity = value;
    m_texture->setAlpha(static_cast<float>(value));
}

double ImageLayer::getOpacity() const
{
    return m_opacity;
}

void _bind(py::module_& module)
{
    auto subTilemap = module.def_submodule("tilemap", "Tile map handling module");

    // ----- Enums -----
    py::native_enum<tmx::Orientation>(subTilemap, "MapOrientation", "enum.IntEnum")
        .value("ORTHOGONAL", tmx::Orientation::Orthogonal)
        .value("ISOMETRIC", tmx::Orientation::Isometric)
        .value("STAGGERED", tmx::Orientation::Staggered)
        .value("HEXAGONAL", tmx::Orientation::Hexagonal)
        .value("NONE", tmx::Orientation::None)
        .finalize();

    py::native_enum<tmx::RenderOrder>(subTilemap, "MapRenderOrder", "enum.IntEnum")
        .value("RIGHT_DOWN", tmx::RenderOrder::RightDown)
        .value("RIGHT_UP", tmx::RenderOrder::RightUp)
        .value("LEFT_DOWN", tmx::RenderOrder::LeftDown)
        .value("LEFT_UP", tmx::RenderOrder::LeftUp)
        .value("NONE", tmx::RenderOrder::None)
        .finalize();

    py::native_enum<tmx::StaggerAxis>(subTilemap, "MapStaggerAxis", "enum.IntEnum")
        .value("X", tmx::StaggerAxis::X)
        .value("Y", tmx::StaggerAxis::Y)
        .value("NONE", tmx::StaggerAxis::None)
        .finalize();

    py::native_enum<tmx::StaggerIndex>(subTilemap, "MapStaggerIndex", "enum.IntEnum")
        .value("EVEN", tmx::StaggerIndex::Even)
        .value("ODD", tmx::StaggerIndex::Odd)
        .value("NONE", tmx::StaggerIndex::None)
        .finalize();

    py::native_enum<tmx::Layer::Type>(subTilemap, "LayerType", "enum.IntEnum")
        .value("TILE", tmx::Layer::Type::Tile)
        .value("OBJECT", tmx::Layer::Type::Object)
        .value("IMAGE", tmx::Layer::Type::Image)
        .finalize();

    // ----- TileSet -----
    auto tileSetClass = py::classh<TileSet>(subTilemap, "TileSet", R"doc(
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
    )doc");

    auto tileSetTileClass = py::classh<TileSet::Tile>(tileSetClass, "Tile", R"doc(
Tile represents a single tile entry within a TileSet.

Attributes:
    id (int): Local tile id.
    terrain_indices (list): Terrain indices for the tile.
    probability (float): Chance for auto-tiling/probability maps.
    clip_rect (Rect): Source rectangle in the tileset texture.
    )doc");

    py::class_<std::array<int, 4>>(tileSetTileClass, "TerrainIndices")
        .def("__len__", [](const std::array<int, 4>&) { return 4; })
        .def(
            "__iter__", [](const std::array<int, 4>& arr)
            { return py::make_iterator(arr.begin(), arr.end()); }, py::keep_alive<0, 1>()
        )
        .def(
            "__getitem__",
            [](const std::array<int, 4>& arr, const size_t i)
            {
                if (i >= arr.size())
                    throw py::index_error("Index out of range");
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
    tileSetTileClass.def_property_readonly("id", &TileSet::Tile::getID)
        .def_property_readonly("terrain_indices", &TileSet::Tile::getTerrainIndices)
        .def_property_readonly("probability", &TileSet::Tile::getProbability)
        .def_property_readonly("clip_rect", &TileSet::Tile::getClipRect);
    py::bind_vector<std::vector<TileSet::Tile>>(tileSetClass, "TileSetTileList");

    py::classh<TileSet::Terrain>(tileSetClass, "Terrain", R"doc(
Terrain describes a named terrain type defined in a tileset.

Attributes:
    name (str): Terrain name.
    tile_id (int): Representative tile id for the terrain.
    )doc")
        .def_property_readonly("name", &TileSet::Terrain::getName)
        .def_property_readonly("tile_id", &TileSet::Terrain::getTileID);
    py::bind_vector<std::vector<TileSet::Terrain>>(tileSetClass, "TerrainList");

    tileSetClass
        .def("has_tile", &TileSet::hasTile, py::arg("id"), R"doc(
Check whether a global tile id belongs to this tileset.

Args:
    id (int): Global tile id (GID).

Returns:
    bool: True if the tileset contains the tile id, False otherwise.
        )doc")
        .def(
            "get_tile", &TileSet::getTile, py::arg("id"),
            py::return_value_policy::reference_internal, R"doc(
Retrieve tile metadata for a given id.

Args:
    id (int): Global tile id (GID).

Returns:
    Tile: The tile metadata, or None if not found.
        )doc"
        )

        .def_property_readonly("first_gid", &TileSet::getFirstGID)
        .def_property_readonly("last_gid", &TileSet::getLastGID)
        .def_property_readonly("name", &TileSet::getName)
        .def_property_readonly("tile_size", &TileSet::getTileSize)
        .def_property_readonly("spacing", &TileSet::getSpacing)
        .def_property_readonly("margin", &TileSet::getMargin)
        .def_property_readonly("tile_count", &TileSet::getTileCount)
        .def_property_readonly("columns", &TileSet::getColumns)
        .def_property_readonly("tile_offset", &TileSet::getTileOffset)
        .def_property_readonly("terrains", &TileSet::getTerrains)
        .def_property_readonly("tiles", &TileSet::getTiles)
        .def_property_readonly("texture", &TileSet::getTexture);
    py::bind_vector<std::vector<TileSet>>(subTilemap, "TileSetList");

    // ----- Layer -----
    py::classh<Layer>(subTilemap, "Layer", R"doc(
Layer is the base class for all tilemap layers.

Attributes:
    visible (bool): Whether the layer is visible.
    offset (Vec2): Per-layer drawing offset.
    opacity (float): Layer opacity (0.0-1.0).
    name (str): Layer name.
    type (LayerType): Layer type enum.

Methods:
    render: Draw the layer to the current renderer.
    )doc")
        .def_readwrite("visible", &Layer::visible)
        .def_readwrite("offset", &Layer::offset)

        .def_property("opacity", &Layer::getOpacity, &Layer::setOpacity)

        .def_property_readonly("name", &Layer::getName)
        .def_property_readonly("type", &Layer::getType)

        .def("render", &Layer::render, R"doc(
Draw the layer to the current renderer.
        )doc");
    py::bind_vector<std::vector<std::shared_ptr<Layer>>>(subTilemap, "LayerList");

    // ----- TileLayer -----
    auto tileLayerClass = py::classh<TileLayer, Layer>(subTilemap, "TileLayer", R"doc(
TileLayer represents a grid of tiles within the map.

Attributes:
    opacity (float): Layer opacity (0.0-1.0).
    tiles (list): List of `Tile` entries for the layer grid.

Methods:
    get_from_area: Return tiles intersecting a Rect area.
    get_from_point: Return the tile at a given world position.
    render: Draw the tile layer.
    )doc");

    py::classh<TileLayer::Tile>(tileLayerClass, "Tile", R"doc(
Tile represents an instance of a tile in a TileLayer.

Attributes:
    id (int): Global tile id (GID).
    flip_flags (int): Flags describing tile flips/rotations.
    )doc")
        .def_property_readonly("id", &TileLayer::Tile::getID)
        .def_property_readonly("flip_flags", &TileLayer::Tile::getFlipFlags);
    py::bind_vector<std::vector<TileLayer::Tile>>(tileLayerClass, "TileLayerTileList");

    py::classh<TileLayer::TileResult>(tileLayerClass, "TileResult", R"doc(
TileResult bundles a `Tile` with its world-space `Rect`.

Attributes:
    tile (Tile): The tile entry.
    rect (Rect): The world-space rectangle covered by the tile.
    )doc")
        .def_readonly("tile", &TileLayer::TileResult::tile)
        .def_readonly("rect", &TileLayer::TileResult::rect);
    py::bind_vector<std::vector<TileLayer::TileResult>>(tileLayerClass, "TileResultList");

    tileLayerClass.def_property("opacity", &TileLayer::getOpacity, &TileLayer::setOpacity)
        .def_property_readonly("tiles", &TileLayer::getTiles)

        .def("get_from_area", &TileLayer::getFromArea, py::arg("area"), R"doc(
Return tiles intersecting a Rect area.

Args:
    area (Rect): World-space area to query.

Returns:
    list[TileLayer.TileResult]: List of TileResult entries for tiles intersecting the area.
        )doc")
        .def(
            "get_from_point",
            [](const TileLayer& self, const Vec2& position) -> py::object
            {
                const auto result = self.getFromPoint(position);
                return result.has_value() ? py::cast(result.value()) : py::none();
            },
            py::arg("position"), R"doc(
Return the tile at a given world position.

Args:
    position (Vec2): World-space position to query.

Returns:
    Optional[TileLayer.TileResult]: TileResult entry if a tile exists at the position, None otherwise.
        )doc"
        )
        .def("render", &TileLayer::render, R"doc(
Draw the tile layer.
        )doc");

    // ----- MapObject -----
    py::classh<TextProperties>(subTilemap, "TextProperties", R"doc(
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
    )doc")
        .def_readwrite("font_family", &TextProperties::fontFamily)
        .def_readwrite("pixel_size", &TextProperties::pixelSize)
        .def_readwrite("wrap", &TextProperties::wrap)
        .def_readwrite("color", &TextProperties::color)
        .def_readwrite("bold", &TextProperties::bold)
        .def_readwrite("italic", &TextProperties::italic)
        .def_readwrite("underline", &TextProperties::underline)
        .def_readwrite("strikethrough", &TextProperties::strikethrough)
        .def_readwrite("kerning", &TextProperties::kerning)
        .def_readwrite("align", &TextProperties::align)
        .def_readwrite("text", &TextProperties::text);

    auto mapObjectClass = py::classh<MapObject>(subTilemap, "MapObject", R"doc(
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
    )doc");

    py::native_enum<tmx::Object::Shape>(mapObjectClass, "ShapeType", "enum.IntEnum")
        .value("RECTANGLE", tmx::Object::Shape::Rectangle)
        .value("ELLIPSE", tmx::Object::Shape::Ellipse)
        .value("POINT", tmx::Object::Shape::Point)
        .value("POLYGON", tmx::Object::Shape::Polygon)
        .value("POLYLINE", tmx::Object::Shape::Polyline)
        .value("TEXT", tmx::Object::Shape::Text)
        .finalize();

    mapObjectClass.def_readwrite("transform", &MapObject::transform)
        .def_readwrite("visible", &MapObject::visible)

        .def_property_readonly("uid", &MapObject::getUID)
        .def_property_readonly("name", &MapObject::getName)
        .def_property_readonly("type", &MapObject::getType)
        .def_property_readonly("rect", &MapObject::getRect)
        .def_property_readonly("tile_id", &MapObject::getTileID)
        .def_property_readonly("shape_type", &MapObject::getShapeType)
        .def_property_readonly("vertices", &MapObject::getVertices)
        .def_property_readonly("text", &MapObject::getTextProperties);
    py::bind_vector<std::vector<MapObject>>(subTilemap, "MapObjectList");

    // ----- ObjectGroup -----
    auto objGroupClass = py::classh<ObjectGroup, Layer>(subTilemap, "ObjectGroup", R"doc(
ObjectGroup is a layer containing placed MapObjects.

Attributes:
    color (Color): Tint color applied to non-tile objects.
    opacity (float): Layer opacity.
    draw_order (DrawOrder): Drawing order for objects.
    objects (list): List of contained MapObject instances.

Methods:
    render: Draw the object group.
    )doc");

    py::native_enum<tmx::ObjectGroup::DrawOrder>(objGroupClass, "DrawOrder", "enum.IntEnum")
        .value("INDEX", tmx::ObjectGroup::DrawOrder::Index)
        .value("TOP_DOWN", tmx::ObjectGroup::DrawOrder::TopDown)
        .finalize();

    objGroupClass
        .def_readwrite("color", &ObjectGroup::color)

        .def_property("opacity", &ObjectGroup::getOpacity, &ObjectGroup::setOpacity)

        .def_property_readonly("draw_order", &ObjectGroup::getDrawOrder)
        .def_property_readonly("objects", &ObjectGroup::getObjects)

        .def("render", &ObjectGroup::render, R"doc(
Draw the object group.
        )doc");

    // ----- ImageLayer -----
    py::classh<ImageLayer, Layer>(subTilemap, "ImageLayer", R"doc(
ImageLayer displays a single image as a layer.

Attributes:
    opacity (float): Layer opacity.
    texture (Texture): The layer image texture.

Methods:
    render: Draw the image layer.
    )doc")
        .def_property("opacity", &ImageLayer::getOpacity, &ImageLayer::setOpacity)

        .def_property_readonly("texture", &ImageLayer::getTexture)

        .def("render", &ImageLayer::render, R"doc(
Draw the image layer.
        )doc");

    // ----- Map -----
    py::classh<Map>(subTilemap, "Map", R"doc(
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
    )doc")
        .def(py::init<>())

        .def_readwrite("background_color", &Map::backgroundColor)

        .def("load", &Map::load, py::arg("tmx_path"), R"doc(
Load a TMX file from path.

Args:
    tmx_path (str): Path to the TMX file to load.
        )doc")
        .def("render", &Map::render, R"doc(
Render all layers.
        )doc")

        .def_property_readonly("orientation", &Map::getOrientation)
        .def_property_readonly("render_order", &Map::getRenderOrder)
        .def_property_readonly("map_size", &Map::getMapSize)
        .def_property_readonly("tile_size", &Map::getTileSize)
        .def_property_readonly("bounds", &Map::getBounds)
        .def_property_readonly("hex_side_length", &Map::getHexSideLength)
        .def_property_readonly("stagger_axis", &Map::getStaggerAxis)
        .def_property_readonly("stagger_index", &Map::getStaggerIndex)
        .def_property_readonly("tile_sets", &Map::getTileSets)
        .def_property_readonly("layers", &Map::getLayers);
}
}  // namespace tilemap
}  // namespace kn
