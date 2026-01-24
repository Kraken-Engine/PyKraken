#include <pybind11/native_enum.h>
#include <pybind11/stl_bind.h>

#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "Camera.hpp"
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
        throw std::runtime_error("Failed to load TMX map from path: " + tmxPath);

    if (tmxMap.getTilesets().size() >= static_cast<size_t>(std::numeric_limits<uint8_t>::max()))
        throw std::runtime_error("Too many tilesets in TMX map: " + tmxPath);

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
                TileLayer::Tile tile{};
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
            tileSet.m_tileIndex[tile.m_id] = tile.m_id + 1;
        }

        m_tileSets.push_back(std::move(tileSet));
    }

    // Populate tileset index for each tile in tile layers to avoid per-tile
    // tileset lookups during rendering.
    for (auto& layerPtr : m_layers)
    {
        if (layerPtr->getType() != tmx::Layer::Type::Tile)
            continue;

        auto* tileLayer = static_cast<TileLayer*>(layerPtr.get());
        for (auto& tile : tileLayer->m_tiles)
        {
            const uint32_t gid = tile.m_id;
            if (gid == 0)
                continue;
            for (size_t tsIdx = 0; tsIdx < m_tileSets.size(); ++tsIdx)
            {
                if (m_tileSets[tsIdx].hasTile(gid))
                {
                    tile.m_tilesetIdx = static_cast<uint8_t>(tsIdx);
                    break;
                }
            }
        }
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

struct FlipInfo
{
    float rotation;
    bool h;
    bool v;
};

// index by (flags & 0b111)
static constexpr FlipInfo kFlipLUT[8] = {
    {0.0f, false, false},            // 0: none
    {0.0f, true, false},             // 1: H
    {0.0f, false, true},             // 2: V
    {0.0f, true, true},              // 3: H|V
    {+float(M_PI_2), false, true},   // 4: D
    {+float(M_PI_2), true, false},   // 5: D|H
    {-float(M_PI_2), false, false},  // 6: D|V
    {-float(M_PI_2), false, true},   // 7: D|H|V
};

void TileLayer::render()
{
    if (!visible)
        return;

    const auto mapW = static_cast<int>(m_map->getMapSize().x);
    const auto mapH = static_cast<int>(m_map->getMapSize().y);
    const auto tileW = static_cast<int>(m_map->getTileSize().x);
    const auto tileH = static_cast<int>(m_map->getTileSize().y);

    // Compute visible tile range from the active camera and clamp to map bounds
    const Vec2 camPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();
    const double camLeft = camPos.x;
    const double camTop = camPos.y;
    const double camRight = camLeft + targetRes.x;
    const double camBottom = camTop + targetRes.y;

    auto camMinX = static_cast<int>(std::floor((camLeft - offset.x) / tileW));
    auto camMinY = static_cast<int>(std::floor((camTop - offset.y) / tileH));
    auto camMaxX = static_cast<int>(std::floor((camRight - offset.x) / tileW));
    auto camMaxY = static_cast<int>(std::floor((camBottom - offset.y) / tileH));

    camMinX = std::max(0, std::min(mapW - 1, camMinX));
    camMinY = std::max(0, std::min(mapH - 1, camMinY));
    camMaxX = std::max(0, std::min(mapW - 1, camMaxX));
    camMaxY = std::max(0, std::min(mapH - 1, camMaxY));

    if (camMinX > camMaxX || camMinY > camMaxY)
        return;

    int startX, endX, stepX;
    int startY, endY, stepY;

    switch (m_map->getRenderOrder())
    {
    case tmx::RenderOrder::RightDown:
        startX = camMinX;
        endX = camMaxX;
        stepX = 1;
        startY = camMinY;
        endY = camMaxY;
        stepY = 1;
        break;
    case tmx::RenderOrder::RightUp:
        startX = camMinX;
        endX = camMaxX;
        stepX = 1;
        startY = camMaxY;
        endY = camMinY;
        stepY = -1;
        break;
    case tmx::RenderOrder::LeftDown:
        startX = camMaxX;
        endX = camMinX;
        stepX = -1;
        startY = camMinY;
        endY = camMaxY;
        stepY = 1;
        break;
    case tmx::RenderOrder::LeftUp:
        startX = camMaxX;
        endX = camMinX;
        stepX = -1;
        startY = camMaxY;
        endY = camMinY;
        stepY = -1;
        break;
    default:
        throw std::runtime_error("Unsupported render order in tile layer rendering");
    }

    Transform renderTransform{};
    renderTransform.pos = offset;

    const auto layerAlpha = static_cast<float>(m_opacity);

    const int endYExclusive = endY + stepY;
    const int endXExclusive = endX + stepX;
    const auto dx = static_cast<double>(stepX * tileW);

    for (int y = startY; y != endYExclusive; y += stepY)
    {
        const auto rowBase = static_cast<size_t>(y * mapW);
        auto px = offset.x + static_cast<double>(startX * tileW);
        const auto py = offset.y + static_cast<double>(y * tileH);

        for (int x = startX; x != endXExclusive; x += stepX, px += dx)
        {
            const TileLayer::Tile& tile = m_tiles[rowBase + static_cast<size_t>(x)];
            const uint32_t gid = tile.getID();
            if (gid == 0)
                continue;

            const TileSet& foundSet = m_map->getTileSets()[tile.getTilesetIndex()];
            const TileSet::Tile* setTile = foundSet.getTile(gid);
            const auto setTexture = foundSet.getTexture();

            setTexture->setAlpha(layerAlpha);
            renderTransform.pos = {px, py};

            const FlipInfo flipInfo = kFlipLUT[tile.getFlipFlags() & 0x7];
            renderTransform.angle = flipInfo.rotation;
            setTexture->flip.h = flipInfo.h;
            setTexture->flip.v = flipInfo.v;

            renderer::draw(setTexture, renderTransform, setTile->getClipRect());
        }
    }
}

std::vector<TileLayer::TileResult> TileLayer::getFromArea(const Rect& area) const
{
    const double tileW = m_map->getTileSize().x;
    const double tileH = m_map->getTileSize().y;
    const auto mapW = static_cast<int>(m_map->getMapSize().x);
    const auto mapH = static_cast<int>(m_map->getMapSize().y);

    // Early reject if the query area doesn't intersect this layer's bounds
    {
        const double layerLeft = offset.x;
        const double layerTop = offset.y;
        const double layerRight = layerLeft + static_cast<double>(mapW) * tileW;
        const double layerBottom = layerTop + static_cast<double>(mapH) * tileH;
        if (area.getRight() < layerLeft || area.getLeft() > layerRight ||
            area.getBottom() < layerTop || area.getTop() > layerBottom)
        {
            return {};
        }
    }

    // Calculate the grid range (clamped to map boundaries)
    const auto startX =
        std::max(0, static_cast<int>(std::floor((area.getLeft() - offset.x) / tileW)));
    const auto startY =
        std::max(0, static_cast<int>(std::floor((area.getTop() - offset.y) / tileH)));
    const auto endX =
        std::min(mapW - 1, static_cast<int>(std::floor((area.getRight() - offset.x) / tileW)));
    const auto endY =
        std::min(mapH - 1, static_cast<int>(std::floor((area.getBottom() - offset.y) / tileH)));

    if (startX > endX || startY > endY)
        return {};

    std::vector<TileLayer::TileResult> foundTiles;
    foundTiles.reserve(static_cast<size_t>((endX - startX + 1) * (endY - startY + 1)));

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
    py::native_enum<tmx::Orientation>(subTilemap, "MapOrientation", "enum.IntEnum", R"doc(
TMX map orientation values.
    )doc")
        .value("ORTHOGONAL", tmx::Orientation::Orthogonal, "Orthogonal grid orientation")
        .value("ISOMETRIC", tmx::Orientation::Isometric, "Isometric orientation")
        .value("STAGGERED", tmx::Orientation::Staggered, "Staggered orientation")
        .value("HEXAGONAL", tmx::Orientation::Hexagonal, "Hexagonal orientation")
        .value("NONE", tmx::Orientation::None, "No orientation specified")
        .finalize();

    py::native_enum<tmx::RenderOrder>(subTilemap, "MapRenderOrder", "enum.IntEnum", R"doc(
Tile render order for TMX maps.
    )doc")
        .value("RIGHT_DOWN", tmx::RenderOrder::RightDown, "Render right then down")
        .value("RIGHT_UP", tmx::RenderOrder::RightUp, "Render right then up")
        .value("LEFT_DOWN", tmx::RenderOrder::LeftDown, "Render left then down")
        .value("LEFT_UP", tmx::RenderOrder::LeftUp, "Render left then up")
        .value("NONE", tmx::RenderOrder::None, "No render order specified")
        .finalize();

    py::native_enum<tmx::StaggerAxis>(subTilemap, "MapStaggerAxis", "enum.IntEnum", R"doc(
Stagger axis for staggered/hex maps.
    )doc")
        .value("X", tmx::StaggerAxis::X, "Stagger along the X axis")
        .value("Y", tmx::StaggerAxis::Y, "Stagger along the Y axis")
        .value("NONE", tmx::StaggerAxis::None, "No stagger axis")
        .finalize();

    py::native_enum<tmx::StaggerIndex>(subTilemap, "MapStaggerIndex", "enum.IntEnum", R"doc(
Stagger index for staggered/hex maps.
    )doc")
        .value("EVEN", tmx::StaggerIndex::Even, "Even rows/columns are staggered")
        .value("ODD", tmx::StaggerIndex::Odd, "Odd rows/columns are staggered")
        .value("NONE", tmx::StaggerIndex::None, "No stagger index")
        .finalize();

    py::native_enum<tmx::Layer::Type>(subTilemap, "LayerType", "enum.IntEnum", R"doc(
TMX layer type values.
    )doc")
        .value("TILE", tmx::Layer::Type::Tile, "Tile layer")
        .value("OBJECT", tmx::Layer::Type::Object, "Object layer")
        .value("IMAGE", tmx::Layer::Type::Image, "Image layer")
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
    terrains (TerrainList): List of terrain definitions.
    tiles (TileSetTileList): List of tile metadata.
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
    tileSetTileClass
        .def_property_readonly("id", &TileSet::Tile::getID, R"doc(
Local tile id within the tileset.
    )doc")
        .def_property_readonly("terrain_indices", &TileSet::Tile::getTerrainIndices, R"doc(
Terrain indices for each corner of the tile.
    )doc")
        .def_property_readonly("probability", &TileSet::Tile::getProbability, R"doc(
Probability used for weighted/random tile placement.
    )doc")
        .def_property_readonly("clip_rect", &TileSet::Tile::getClipRect, R"doc(
Source rectangle of the tile within the tileset texture.
    )doc");
    py::bind_vector<std::vector<TileSet::Tile>>(tileSetClass, "TileSetTileList");

    py::classh<TileSet::Terrain>(tileSetClass, "Terrain", R"doc(
Terrain describes a named terrain type defined in a tileset.

Attributes:
    name (str): Terrain name.
    tile_id (int): Representative tile id for the terrain.
    )doc")
        .def_property_readonly("name", &TileSet::Terrain::getName, R"doc(
Terrain name.
    )doc")
        .def_property_readonly("tile_id", &TileSet::Terrain::getTileID, R"doc(
Representative tile id for the terrain.
    )doc");
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

        .def_property_readonly("first_gid", &TileSet::getFirstGID, R"doc(
    First global tile id (GID) in this tileset.
        )doc")
        .def_property_readonly("last_gid", &TileSet::getLastGID, R"doc(
    Last global tile id (GID) in this tileset.
        )doc")
        .def_property_readonly("name", &TileSet::getName, R"doc(
    Tileset name.
        )doc")
        .def_property_readonly("tile_size", &TileSet::getTileSize, R"doc(
    Size of tiles in pixels.
        )doc")
        .def_property_readonly("spacing", &TileSet::getSpacing, R"doc(
    Pixel spacing between tiles in the source image.
        )doc")
        .def_property_readonly("margin", &TileSet::getMargin, R"doc(
    Pixel margin around the source image.
        )doc")
        .def_property_readonly("tile_count", &TileSet::getTileCount, R"doc(
    Total number of tiles in the tileset.
        )doc")
        .def_property_readonly("columns", &TileSet::getColumns, R"doc(
    Number of tile columns in the source image.
        )doc")
        .def_property_readonly("tile_offset", &TileSet::getTileOffset, R"doc(
    Per-tile offset applied when rendering.
        )doc")
        .def_property_readonly("terrains", &TileSet::getTerrains, R"doc(
    TerrainList of terrain definitions.
        )doc")
        .def_property_readonly("tiles", &TileSet::getTiles, R"doc(
    TileSetTileList of tile metadata entries.
        )doc")
        .def_property_readonly("texture", &TileSet::getTexture, R"doc(
    Source texture for the tileset.
        )doc");
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
        .def_readwrite("visible", &Layer::visible, R"doc(
Whether the layer is visible.
    )doc")
        .def_readwrite("offset", &Layer::offset, R"doc(
Per-layer drawing offset.
    )doc")

        .def_property("opacity", &Layer::getOpacity, &Layer::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")

        .def_property_readonly("name", &Layer::getName, R"doc(
Layer name.
    )doc")
        .def_property_readonly("type", &Layer::getType, R"doc(
Layer type enum.
    )doc")

        .def("render", &Layer::render, R"doc(
Draw the layer to the current renderer.
        )doc");
    py::bind_vector<std::vector<std::shared_ptr<Layer>>>(subTilemap, "LayerList");

    // ----- TileLayer -----
    auto tileLayerClass = py::classh<TileLayer, Layer>(subTilemap, "TileLayer", R"doc(
TileLayer represents a grid of tiles within the map.

Attributes:
    opacity (float): Layer opacity (0.0-1.0).
    tiles (TileLayerTileList): List of `Tile` entries for the layer grid.

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
    tileset_index (int): Index of the tileset this tile belongs to.
    )doc")
        .def_property_readonly("id", &TileLayer::Tile::getID, R"doc(
Global tile id (GID).
    )doc")
        .def_property_readonly("flip_flags", &TileLayer::Tile::getFlipFlags, R"doc(
Tile flip/rotation flags.
    )doc")
        .def_property_readonly("tileset_index", &TileLayer::Tile::getTilesetIndex, R"doc(
Index of the tileset used by this tile.
    )doc");
    py::bind_vector<std::vector<TileLayer::Tile>>(tileLayerClass, "TileLayerTileList");

    py::classh<TileLayer::TileResult>(tileLayerClass, "TileResult", R"doc(
TileResult bundles a `Tile` with its world-space `Rect`.

Attributes:
    tile (Tile): The tile entry.
    rect (Rect): The world-space rectangle covered by the tile.
    )doc")
        .def_readonly("tile", &TileLayer::TileResult::tile, R"doc(
The tile entry.
    )doc")
        .def_readonly("rect", &TileLayer::TileResult::rect, R"doc(
World-space rectangle covered by the tile.
    )doc");
    py::bind_vector<std::vector<TileLayer::TileResult>>(tileLayerClass, "TileResultList");

    tileLayerClass
        .def_property("opacity", &TileLayer::getOpacity, &TileLayer::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")
        .def_property_readonly("tiles", &TileLayer::getTiles, R"doc(
TileLayerTileList of tiles in the layer grid.
    )doc")

        .def("get_from_area", &TileLayer::getFromArea, py::arg("area"), R"doc(
Return tiles intersecting a Rect area.

Args:
    area (Rect): World-space area to query.

Returns:
    TileResultList: List of TileResult entries for tiles intersecting the area.
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
        .def_readwrite("font_family", &TextProperties::fontFamily, R"doc(
Font family name.
    )doc")
        .def_readwrite("pixel_size", &TextProperties::pixelSize, R"doc(
Font size in pixels.
    )doc")
        .def_readwrite("wrap", &TextProperties::wrap, R"doc(
Whether text wrapping is enabled.
    )doc")
        .def_readwrite("color", &TextProperties::color, R"doc(
Text color.
    )doc")
        .def_readwrite("bold", &TextProperties::bold, R"doc(
Bold style flag.
    )doc")
        .def_readwrite("italic", &TextProperties::italic, R"doc(
Italic style flag.
    )doc")
        .def_readwrite("underline", &TextProperties::underline, R"doc(
Underline style flag.
    )doc")
        .def_readwrite("strikethrough", &TextProperties::strikethrough, R"doc(
Strikethrough style flag.
    )doc")
        .def_readwrite("kerning", &TextProperties::kerning, R"doc(
Kerning enabled flag.
    )doc")
        .def_readwrite("align", &TextProperties::align, R"doc(
Horizontal text alignment.
    )doc")
        .def_readwrite("text", &TextProperties::text, R"doc(
Text content.
    )doc");

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
    vertices (Vec2List): Vertex list for polygon/polyline shapes.
    text (TextProperties): Text properties when shape is text.
    )doc");

    py::native_enum<tmx::Object::Shape>(mapObjectClass, "ShapeType", "enum.IntEnum", R"doc(
TMX object shape types.
    )doc")
        .value("RECTANGLE", tmx::Object::Shape::Rectangle, "Rectangle shape")
        .value("ELLIPSE", tmx::Object::Shape::Ellipse, "Ellipse shape")
        .value("POINT", tmx::Object::Shape::Point, "Point shape")
        .value("POLYGON", tmx::Object::Shape::Polygon, "Polygon shape")
        .value("POLYLINE", tmx::Object::Shape::Polyline, "Polyline shape")
        .value("TEXT", tmx::Object::Shape::Text, "Text object")
        .finalize();

    mapObjectClass
        .def_readwrite("transform", &MapObject::transform, R"doc(
Transform component for the object.
    )doc")
        .def_readwrite("visible", &MapObject::visible, R"doc(
Visibility flag.
    )doc")

        .def_property_readonly("uid", &MapObject::getUID, R"doc(
Unique object identifier.
    )doc")
        .def_property_readonly("name", &MapObject::getName, R"doc(
Object name.
    )doc")
        .def_property_readonly("type", &MapObject::getType, R"doc(
Object type string.
    )doc")
        .def_property_readonly("rect", &MapObject::getRect, R"doc(
Object bounding rectangle.
    )doc")
        .def_property_readonly("tile_id", &MapObject::getTileID, R"doc(
Associated tile id when the object is a tile.
    )doc")
        .def_property_readonly("shape_type", &MapObject::getShapeType, R"doc(
Shape type enum for the object.
    )doc")
        .def_property_readonly("vertices", &MapObject::getVertices, R"doc(
Vec2List of vertices for polygon/polyline shapes.
    )doc")
        .def_property_readonly("text", &MapObject::getTextProperties, R"doc(
Text properties if the object is text.
    )doc");
    py::bind_vector<std::vector<MapObject>>(subTilemap, "MapObjectList");

    // ----- ObjectGroup -----
    auto objGroupClass = py::classh<ObjectGroup, Layer>(subTilemap, "ObjectGroup", R"doc(
ObjectGroup is a layer containing placed MapObjects.

Attributes:
    color (Color): Tint color applied to non-tile objects.
    opacity (float): Layer opacity.
    draw_order (DrawOrder): Drawing order for objects.
    objects (MapObjectList): List of contained MapObject instances.

Methods:
    render: Draw the object group.
    )doc");

    py::native_enum<tmx::ObjectGroup::DrawOrder>(objGroupClass, "DrawOrder", "enum.IntEnum", R"doc(
Object drawing order for object layers.
    )doc")
        .value("INDEX", tmx::ObjectGroup::DrawOrder::Index, "Draw by object index")
        .value("TOP_DOWN", tmx::ObjectGroup::DrawOrder::TopDown, "Draw top-down by Y")
        .finalize();

    objGroupClass
        .def_readwrite("color", &ObjectGroup::color, R"doc(
Tint color for non-tile objects.
    )doc")

        .def_property("opacity", &ObjectGroup::getOpacity, &ObjectGroup::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")

        .def_property_readonly("draw_order", &ObjectGroup::getDrawOrder, R"doc(
Drawing order for objects in the group.
    )doc")
        .def_property_readonly("objects", &ObjectGroup::getObjects, R"doc(
MapObjectList of objects in the group.
    )doc")

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
        .def_property("opacity", &ImageLayer::getOpacity, &ImageLayer::setOpacity, R"doc(
Layer opacity from 0.0 to 1.0.
    )doc")

        .def_property_readonly("texture", &ImageLayer::getTexture, R"doc(
Texture used by the image layer.
    )doc")

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
    tile_sets (TileSetList): List of TileSet objects.
    layers (LayerList): List of Layer instances.

Methods:
    load: Load a TMX file from path.
    render: Render all layers.
    )doc")
        .def(py::init<>())

        .def_readwrite("background_color", &Map::backgroundColor, R"doc(
Map background color.
    )doc")

        .def("load", &Map::load, py::arg("tmx_path"), R"doc(
Load a TMX file from path.

Args:
    tmx_path (str): Path to the TMX file to load.
        )doc")
        .def("render", &Map::render, R"doc(
Render all layers.
        )doc")

        .def_property_readonly("orientation", &Map::getOrientation, R"doc(
Map orientation enum.
    )doc")
        .def_property_readonly("render_order", &Map::getRenderOrder, R"doc(
Tile render order enum.
    )doc")
        .def_property_readonly("map_size", &Map::getMapSize, R"doc(
Map dimensions in tiles.
    )doc")
        .def_property_readonly("tile_size", &Map::getTileSize, R"doc(
Size of tiles in pixels.
    )doc")
        .def_property_readonly("bounds", &Map::getBounds, R"doc(
Map bounds in pixels.
    )doc")
        .def_property_readonly("hex_side_length", &Map::getHexSideLength, R"doc(
Hex side length for hex maps.
    )doc")
        .def_property_readonly("stagger_axis", &Map::getStaggerAxis, R"doc(
Stagger axis enum for staggered/hex maps.
    )doc")
        .def_property_readonly("stagger_index", &Map::getStaggerIndex, R"doc(
Stagger index enum for staggered/hex maps.
    )doc")
        .def_property_readonly("tile_sets", &Map::getTileSets, R"doc(
TileSetList of tilesets used by the map.
    )doc")
        .def_property_readonly("layers", &Map::getLayers, R"doc(
LayerList of layers in the map.
    )doc");
}
}  // namespace tilemap
}  // namespace kn
