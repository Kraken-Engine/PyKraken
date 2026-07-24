
#include <algorithm>
#include <cmath>
#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/TileLayer.hpp>

#include "kraken/core/Log.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/tilemap/TileMap.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

namespace
{
constexpr int TILEMAP_ELLIPSE_SEGMENTS = 24;

kn::Vec2 rotatePoint(const kn::Vec2& point, const kn::Vec2& pivot, const double angle)
{
    if (angle == 0.0)
        return point;

    kn::Vec2 rotated = point - pivot;
    rotated.rotate(angle);
    return pivot + rotated;
}

kn::Vec2 getMapPivotWorld(const kn::tilemap::Map* map, const kn::Vec2& pivot)
{
    if (!map)
        return {};

    const kn::Rect bounds = map->getBounds();
    return bounds.getTopLeft() + bounds.getSize() * pivot;
}

std::vector<kn::Vec2> rotatePoints(
    const std::vector<kn::Vec2>& points, const kn::Vec2& pivot, const double angle
)
{
    std::vector<kn::Vec2> rotatedPoints;
    rotatedPoints.reserve(points.size());

    for (const auto& point : points)
        rotatedPoints.push_back(rotatePoint(point, pivot, angle));

    return rotatedPoints;
}

std::vector<kn::Vec2> rotateRectCorners(
    const kn::Rect& rect, const kn::Vec2& pivot, const double angle
)
{
    const auto corners = rect.getCorners();
    std::vector<kn::Vec2> rotatedCorners;
    rotatedCorners.reserve(corners.size());

    for (const auto& corner : corners)
        rotatedCorners.push_back(rotatePoint(corner, pivot, angle));

    return rotatedCorners;
}

std::vector<kn::Vec2> rotateEllipsePoints(
    const kn::Rect& rect, const kn::Vec2& pivot, const double angle
)
{
    std::vector<kn::Vec2> points;
    points.reserve(TILEMAP_ELLIPSE_SEGMENTS);

    const kn::Vec2 center = rect.getCenter();
    const kn::Vec2 radius = rect.getSize() * 0.5;

    for (int i = 0; i < TILEMAP_ELLIPSE_SEGMENTS; ++i)
    {
        const double t = (2.0 * M_PI * static_cast<double>(i)) /
                         static_cast<double>(TILEMAP_ELLIPSE_SEGMENTS);
        const kn::Vec2 point{center.x + std::cos(t) * radius.x, center.y + std::sin(t) * radius.y};
        points.push_back(rotatePoint(point, pivot, angle));
    }

    return points;
}
}  // namespace

namespace kn
{
namespace tilemap
{
Map::Map(const std::filesystem::path& tmxPath)
{
    if (!tmxPath.empty())
        load(tmxPath);
}

void Map::load(const std::filesystem::path& tmxPath)
{
    tmx::Map tmxMap;
    if (!tmxMap.load(tmxPath.string()))
        throw std::runtime_error("Failed to load TMX map from path: " + tmxPath.string());

    if (tmxMap.getTilesets().size() >= static_cast<size_t>(std::numeric_limits<uint8_t>::max()))
        throw std::runtime_error("Too many tilesets in TMX map: " + tmxPath.string());

    m_orient = tmxMap.getOrientation();
    m_renderOrder = tmxMap.getRenderOrder();
    m_staggerAxis = tmxMap.getStaggerAxis();
    m_staggerIndex = tmxMap.getStaggerIndex();

    if (m_renderOrder == tmx::RenderOrder::None)
        kn::log::warn(
            "TMX map ({}) has no render order specified, defaulting to right-down rendering.",
            tmxPath.string()
        );

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
            const auto mapWidth = static_cast<int>(m_mapSize.x);
            const auto mapHeight = static_cast<int>(m_mapSize.y);

            auto tileLayer = std::make_shared<TileLayer>();
            tileLayer->m_type = tmx::Layer::Type::Tile;
            tileLayer->m_name = tmxTileLayer.getName();
            tileLayer->offset = {tileOffset.x, tileOffset.y};
            tileLayer->visible = tmxTileLayer.getVisible();

            const auto& tmxTiles = tmxTileLayer.getTiles();
            const auto totalTileCount = static_cast<size_t>(std::max(0, mapWidth)) *
                                        static_cast<size_t>(std::max(0, mapHeight));
            tileLayer->m_tiles.assign(totalTileCount, TileLayer::Tile{});

            if (!tmxTiles.empty())
            {
                const auto copyCount = std::min(tmxTiles.size(), tileLayer->m_tiles.size());
                for (size_t i = 0; i < copyCount; ++i)
                {
                    tileLayer->m_tiles[i].m_id = tmxTiles[i].ID;
                    tileLayer->m_tiles[i].m_flipFlags = tmxTiles[i].flipFlags;
                }
            }
            else
            {
                const auto& tmxChunks = tmxTileLayer.getChunks();
                for (const auto& chunk : tmxChunks)
                {
                    const int chunkW = chunk.size.x;
                    const int chunkH = chunk.size.y;
                    const int chunkX = chunk.position.x;
                    const int chunkY = chunk.position.y;

                    if (chunkW <= 0 || chunkH <= 0)
                        continue;

                    for (int cy = 0; cy < chunkH; ++cy)
                    {
                        const int mapY = chunkY + cy;
                        if (mapY < 0 || mapY >= mapHeight)
                            continue;

                        for (int cx = 0; cx < chunkW; ++cx)
                        {
                            const int mapX = chunkX + cx;
                            if (mapX < 0 || mapX >= mapWidth)
                                continue;

                            const size_t chunkIndex = static_cast<size_t>(cy * chunkW + cx);
                            if (chunkIndex >= chunk.tiles.size())
                                continue;

                            const size_t mapIndex = static_cast<size_t>(mapY * mapWidth + mapX);
                            if (mapIndex >= tileLayer->m_tiles.size())
                                continue;

                            tileLayer->m_tiles[mapIndex].m_id = chunk.tiles[chunkIndex].ID;
                            tileLayer->m_tiles[mapIndex].m_flipFlags =
                                chunk.tiles[chunkIndex].flipFlags;
                        }
                    }
                }
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
                    static_cast<TextAlign>(tmxText.hAlign),
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

            PixelArray pa(tmxImgLayer.getImagePath());
            if (tmxImgLayer.hasTransparency())
            {
                const tmx::Colour& c = tmxImgLayer.getTransparencyColour();
                pa.setColorKey({c.r, c.g, c.b, c.a});
            }
            imgLayer->m_texture = std::make_shared<Texture>(pa);

            imgLayer->transform.pos = {tileOffset.x, tileOffset.y};
            imgLayer->setOpacity(tmxImgLayer.getOpacity());

            layer = imgLayer;
            break;
        }
        case tmx::Layer::Type::Group:
            break;
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
        // TMXLite is a goober DO NOT use .getLastGID
        tileSet.m_lastGID = tileSet.m_firstGID;
        tileSet.m_name = tmxTileset.getName();
        tileSet.m_tileSize = {tsTileSize.x, tsTileSize.y};
        tileSet.m_spacing = tmxTileset.getSpacing();
        tileSet.m_margin = tmxTileset.getMargin();
        tileSet.m_tileCount = tmxTileset.getTileCount();
        tileSet.m_columns = tmxTileset.getColumnCount();
        tileSet.m_tileOffset = {tsTileOffset.x, tsTileOffset.y};

        PixelArray pa(tmxTileset.getImagePath());
        if (tmxTileset.hasTransparency())
        {
            const tmx::Colour& c = tmxTileset.getTransparencyColour();
            pa.setColorKey({c.r, c.g, c.b, c.a});
        }
        tileSet.m_texture = std::make_shared<Texture>(pa);

        uint32_t maxExplicitLocalID = 0;
        for (const auto& tmxTile : tsTiles)
            maxExplicitLocalID = std::max(maxExplicitLocalID, tmxTile.ID);

        uint32_t resolvedColumns = tileSet.m_columns;
        uint32_t resolvedTileCount = tileSet.m_tileCount;

        const int texW = tileSet.m_texture ? tileSet.m_texture->getWidth() : 0;
        const int texH = tileSet.m_texture ? tileSet.m_texture->getHeight() : 0;

        const int tileW = static_cast<int>(tileSet.m_tileSize.x);
        const int tileH = static_cast<int>(tileSet.m_tileSize.y);
        const int spacing = static_cast<int>(tileSet.m_spacing);
        const int margin = static_cast<int>(tileSet.m_margin);

        if (resolvedColumns == 0 && tileW > 0)
        {
            const int usableW = std::max(0, texW - (2 * margin));
            const int strideW = tileW + spacing;
            if (strideW > 0)
                resolvedColumns = static_cast<uint32_t>((usableW + spacing) / strideW);
        }

        if (resolvedTileCount == 0 && tileW > 0 && tileH > 0)
        {
            const int usableW = std::max(0, texW - (2 * margin));
            const int usableH = std::max(0, texH - (2 * margin));
            const int strideW = tileW + spacing;
            const int strideH = tileH + spacing;

            uint32_t colsFromImage = 0;
            uint32_t rowsFromImage = 0;

            if (strideW > 0)
                colsFromImage = static_cast<uint32_t>((usableW + spacing) / strideW);
            if (strideH > 0)
                rowsFromImage = static_cast<uint32_t>((usableH + spacing) / strideH);

            if (resolvedColumns == 0)
                resolvedColumns = colsFromImage;

            if (resolvedColumns > 0)
                resolvedTileCount = resolvedColumns * rowsFromImage;
        }

        if (!tsTiles.empty())
            resolvedTileCount = std::max(resolvedTileCount, maxExplicitLocalID + 1);

        if (resolvedColumns == 0 && resolvedTileCount > 0)
            resolvedColumns = resolvedTileCount;

        tileSet.m_columns = resolvedColumns;
        tileSet.m_tileCount = resolvedTileCount;
        tileSet.m_lastGID = (tileSet.m_tileCount > 0)
                                ? (tileSet.m_firstGID + tileSet.m_tileCount - 1)
                                : tileSet.m_firstGID;

        tileSet.m_terrains.reserve(tsTerrainTypes.size());
        for (const auto& tmxTerrain : tsTerrainTypes)
        {
            TileSet::Terrain terrain{tmxTerrain.name, tmxTerrain.tileID};
            tileSet.m_terrains.push_back(std::move(terrain));
        }

        tileSet.m_tiles.reserve(tileSet.m_tileCount);
        for (uint32_t localID = 0; localID < tileSet.m_tileCount; ++localID)
        {
            const uint32_t column = (tileSet.m_columns > 0) ? (localID % tileSet.m_columns) : 0;
            const uint32_t row = (tileSet.m_columns > 0) ? (localID / tileSet.m_columns) : 0;

            const uint32_t clipX = static_cast<uint32_t>(margin + column * (tileW + spacing));
            const uint32_t clipY = static_cast<uint32_t>(margin + row * (tileH + spacing));

            TileSet::Tile tile{
                localID,
                {},
                100,
                {static_cast<double>(clipX), static_cast<double>(clipY), tileSet.m_tileSize.x,
                 tileSet.m_tileSize.y}
            };
            tileSet.m_tiles.push_back(std::move(tile));
        }

        for (const auto& tmxTile : tsTiles)
        {
            if (tmxTile.ID >= tileSet.m_tiles.size())
                continue;

            auto& tile = tileSet.m_tiles[tmxTile.ID];
            tile.m_terrainIndices = tmxTile.terrainIndices;
            tile.m_probability = tmxTile.probability;

            if (tmxTile.imageSize.x > 0 && tmxTile.imageSize.y > 0)
            {
                tile.m_clipArea =
                    {static_cast<double>(tmxTile.imagePosition.x),
                     static_cast<double>(tmxTile.imagePosition.y),
                     static_cast<double>(tmxTile.imageSize.x),
                     static_cast<double>(tmxTile.imageSize.y)};
            }
        }

        tileSet.m_tileIndex.assign(tileSet.m_tileCount, 0);
        for (size_t i = 0; i < tileSet.m_tiles.size(); ++i)
        {
            const auto localID = tileSet.m_tiles[i].m_id;
            if (localID < tileSet.m_tileIndex.size())
                tileSet.m_tileIndex[localID] = static_cast<uint32_t>(i + 1);
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

void Map::draw(const double angle, const Vec2& pivot)
{
    for (const auto& layer : m_layers)
        layer->draw(angle, pivot);
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

const std::vector<std::shared_ptr<Layer>>& Map::getAllLayers() const
{
    return m_layers;
}

std::shared_ptr<Layer> Map::getLayer(const std::string& name) const
{
    for (const auto& layer : m_layers)
    {
        if (layer->getName() == name)
            return layer;
    }
    return nullptr;
}

std::vector<std::shared_ptr<TileLayer>> Map::getTileLayers() const
{
    std::vector<std::shared_ptr<TileLayer>> layers;
    for (const auto& layer : m_layers)
    {
        if (layer->getType() == tmx::Layer::Type::Tile)
        {
            layers.push_back(std::static_pointer_cast<TileLayer>(layer));
        }
    }
    return layers;
}

std::vector<std::shared_ptr<ObjectGroup>> Map::getObjectGroups() const
{
    std::vector<std::shared_ptr<ObjectGroup>> layers;
    for (const auto& layer : m_layers)
    {
        if (layer->getType() == tmx::Layer::Type::Object)
        {
            layers.push_back(std::static_pointer_cast<ObjectGroup>(layer));
        }
    }
    return layers;
}

std::vector<std::shared_ptr<ImageLayer>> Map::getImageLayers() const
{
    std::vector<std::shared_ptr<ImageLayer>> layers;
    for (const auto& layer : m_layers)
        if (layer->getType() == tmx::Layer::Type::Image)
            layers.push_back(std::static_pointer_cast<ImageLayer>(layer));

    return layers;
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
    if (m_tileCount == 0)
        return false;
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
    if (idx == 0)
        return nullptr;

    const size_t tileVectorIndex = static_cast<size_t>(idx - 1);
    if (tileVectorIndex >= m_tiles.size())
        return nullptr;

    return &m_tiles[tileVectorIndex];
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

struct HexTransformInfo
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

// tmxlite exposes hex flags in the same bitfield, where the low bit (0x1)
// represents Tiled's extra 120-degree rotation flag.

static HexTransformInfo decodeHexTransform(const uint8_t rawFlipFlags)
{
    static constexpr uint8_t kHexRotate120Flag = 0x1;

    const bool h = (rawFlipFlags & tmx::TileLayer::FlipFlag::Horizontal) != 0;
    const bool v = (rawFlipFlags & tmx::TileLayer::FlipFlag::Vertical) != 0;
    const bool rotate60 = (rawFlipFlags & tmx::TileLayer::FlipFlag::Diagonal) != 0;
    const bool rotate120 = (rawFlipFlags & kHexRotate120Flag) != 0;

    float rotation = 0.0f;
    if (rotate60)
        rotation += float(M_PI) / 3.0f;
    if (rotate120)
        rotation += (2.0f * float(M_PI)) / 3.0f;

    return {rotation, h, v};
}

void TileLayer::draw(const double angle, const Vec2& pivot)
{
    if (!visible)
        return;

    const auto mapW = static_cast<int>(m_map->getMapSize().x);
    const auto mapH = static_cast<int>(m_map->getMapSize().y);
    const auto tileW = static_cast<int>(m_map->getTileSize().x);
    const auto tileH = static_cast<int>(m_map->getTileSize().y);

    if (mapW <= 0 || mapH <= 0 || tileW <= 0 || tileH <= 0)
        return;

    const auto expectedTileCount = static_cast<size_t>(mapW) * static_cast<size_t>(mapH);
    if (m_tiles.size() < expectedTileCount)
        return;

    const auto orient = m_map->getOrientation();

    int camMinX = 0;
    int camMinY = 0;
    int camMaxX = mapW - 1;
    int camMaxY = mapH - 1;

    const bool rotateLayer = angle != 0.0;
    const Vec2 pivotWorld = rotateLayer ? getMapPivotWorld(m_map, pivot) : Vec2{};

    if (orient == tmx::Orientation::Orthogonal && !rotateLayer)
    {
        // Compute a conservative world-space camera coverage from all screen corners so
        // culling remains correct even when the active camera is rotated.
        const Rect rendSize{renderer::getCurrentResolution()};
        const std::array<Vec2, 4> worldCorners = {
            camera::screenToWorld(rendSize.getTopLeft()),
            camera::screenToWorld(rendSize.getTopRight()),
            camera::screenToWorld(rendSize.getBottomLeft()),
            camera::screenToWorld(rendSize.getBottomRight()),
        };

        auto [camLeft, camTop] = worldCorners[0];
        auto [camRight, camBottom] = worldCorners[0];
        for (const Vec2& corner : worldCorners)
        {
            camLeft = std::min(camLeft, corner.x);
            camTop = std::min(camTop, corner.y);
            camRight = std::max(camRight, corner.x);
            camBottom = std::max(camBottom, corner.y);
        }

        // Expand by one tile to hide precision edge artifacts while rotating.
        camMinX = static_cast<int>(std::floor((camLeft - offset.x) / tileW)) - 1;
        camMinY = static_cast<int>(std::floor((camTop - offset.y) / tileH)) - 1;
        camMaxX = static_cast<int>(std::floor((camRight - offset.x) / tileW)) + 1;
        camMaxY = static_cast<int>(std::floor((camBottom - offset.y) / tileH)) + 1;

        camMinX = std::max(0, std::min(mapW - 1, camMinX));
        camMinY = std::max(0, std::min(mapH - 1, camMinY));
        camMaxX = std::max(0, std::min(mapW - 1, camMaxX));
        camMaxY = std::max(0, std::min(mapH - 1, camMaxY));
    }

    if (camMinX > camMaxX || camMinY > camMaxY)
        return;

    int startX, endX, stepX;
    int startY, endY, stepY;

    switch (m_map->getRenderOrder())
    {
    case tmx::RenderOrder::RightDown:
    case tmx::RenderOrder::None:
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
    }

    Transform renderTransform{};
    renderTransform.pos = offset;

    const auto layerAlpha = static_cast<float>(m_opacity);
    const auto halfTileW = static_cast<double>(tileW) * 0.5;
    const auto halfTileH = static_cast<double>(tileH) * 0.5;
    const auto isoOriginX = offset.x + static_cast<double>(mapH - 1) * halfTileW;
    const auto isoOriginY = offset.y;

    const auto staggerAxis = m_map->getStaggerAxis();
    const auto staggerIndex = m_map->getStaggerIndex();
    const double hexSideLength = (orient == tmx::Orientation::Hexagonal) ? m_map->getHexSideLength()
                                                                         : 0.0;

    const double sideLengthX = (staggerAxis == tmx::StaggerAxis::X) ? hexSideLength : 0.0;
    const double sideLengthY = (staggerAxis == tmx::StaggerAxis::Y) ? hexSideLength : 0.0;
    const double columnWidth = (static_cast<double>(tileW) + sideLengthX) * 0.5;
    const double rowHeight = (static_cast<double>(tileH) + sideLengthY) * 0.5;
    const double stepXStaggerY = static_cast<double>(tileW) + sideLengthX;
    const double stepYStaggerX = static_cast<double>(tileH) + sideLengthY;

    const auto isStaggeredCoord = [staggerIndex](const int v)
    {
        const bool isEven = (v & 1) == 0;
        return (staggerIndex == tmx::StaggerIndex::Even) ? isEven : !isEven;
    };

    const int endYExclusive = endY + stepY;
    const int endXExclusive = endX + stepX;

    for (int y = startY; y != endYExclusive; y += stepY)
    {
        const auto rowBase = static_cast<size_t>(y * mapW);

        for (int x = startX; x != endXExclusive; x += stepX)
        {
            const TileLayer::Tile& tile = m_tiles[rowBase + static_cast<size_t>(x)];
            const uint32_t gid = tile.getID();
            if (gid == 0)
                continue;

            const uint8_t tilesetIndex = tile.getTilesetIndex();
            const auto& tileSets = m_map->getTileSets();
            if (tilesetIndex >= tileSets.size())
                continue;

            const TileSet& foundSet = tileSets[tilesetIndex];
            const TileSet::Tile* setTile = foundSet.getTile(gid);
            const auto setTexture = foundSet.getTexture();
            if (!setTile || !setTexture)
                continue;

            setTexture->setAlpha(layerAlpha);
            if (orient == tmx::Orientation::Isometric)
            {
                renderTransform.pos =
                    {isoOriginX + (static_cast<double>(x) - static_cast<double>(y)) * halfTileW,
                     isoOriginY + (static_cast<double>(x) + static_cast<double>(y)) * halfTileH};
            }
            else if (orient == tmx::Orientation::Staggered || orient == tmx::Orientation::Hexagonal)
            {
                if (staggerAxis == tmx::StaggerAxis::Y)
                {
                    renderTransform.pos =
                        {offset.x + static_cast<double>(x) * stepXStaggerY +
                             (isStaggeredCoord(y) ? columnWidth : 0.0),
                         offset.y + static_cast<double>(y) * rowHeight};
                }
                else if (staggerAxis == tmx::StaggerAxis::X)
                {
                    renderTransform.pos =
                        {offset.x + static_cast<double>(x) * columnWidth,
                         offset.y + static_cast<double>(y) * stepYStaggerX +
                             (isStaggeredCoord(x) ? rowHeight : 0.0)};
                }
                else
                {
                    renderTransform.pos =
                        {offset.x + static_cast<double>(x * tileW),
                         offset.y + static_cast<double>(y * tileH)};
                }
            }
            else
            {
                renderTransform.pos =
                    {offset.x + static_cast<double>(x * tileW),
                     offset.y + static_cast<double>(y * tileH)};
            }

            const uint8_t rawFlipFlags = tile.getFlipFlags();
            if (orient == tmx::Orientation::Hexagonal)
            {
                const HexTransformInfo hexInfo = decodeHexTransform(rawFlipFlags);
                renderTransform.angle = hexInfo.rotation;
                setTexture->flip.h = hexInfo.h;
                setTexture->flip.v = hexInfo.v;
            }
            else
            {
                // Normalize to LUT bits H=0x1, V=0x2, D=0x4.
                const uint8_t lutFlipFlags =
                    ((rawFlipFlags & tmx::TileLayer::FlipFlag::Horizontal) ? 0x1 : 0x0) |
                    ((rawFlipFlags & tmx::TileLayer::FlipFlag::Vertical) ? 0x2 : 0x0) |
                    ((rawFlipFlags & tmx::TileLayer::FlipFlag::Diagonal) ? 0x4 : 0x0);

                const FlipInfo flipInfo = kFlipLUT[lutFlipFlags];
                renderTransform.angle = flipInfo.rotation;
                setTexture->flip.h = flipInfo.h;
                setTexture->flip.v = flipInfo.v;
            }

            if (rotateLayer)
            {
                renderTransform.pos = rotatePoint(renderTransform.pos, pivotWorld, angle);
                renderTransform.angle += angle;
            }

            setTexture->setClipArea(setTile->getClipArea());
            renderer::draw(*setTexture, renderTransform);
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
            return {};
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
    const auto [localX, localY] = position - offset;
    const auto [tileW, tileH] = m_map->getTileSize();
    const auto [mapW, mapH] = m_map->getMapSize();

    // Convert world coordinates to grid coordinates
    const auto x = static_cast<int>(std::floor(localX / tileW));
    const auto y = static_cast<int>(std::floor(localY / tileH));

    // Bounds check
    if (x < 0 || x >= mapW || y < 0 || y >= mapH)
        return std::nullopt;

    const auto index = static_cast<size_t>(y * mapW + x);
    if (index >= m_tiles.size())
        return std::nullopt;

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

std::vector<Vec2> MapObject::getVertices() const
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

void ObjectGroup::draw(const double angle, const Vec2& pivot)
{
    if (!visible)
        return;

    const bool rotateLayer = angle != 0.0;
    const Vec2 pivotWorld = rotateLayer ? getMapPivotWorld(m_map, pivot) : Vec2{};

    if (!rotateLayer)
    {
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
                    setTexture->setClipArea(tile->getClipArea());
                    renderer::draw(*setTexture, renderTransform);
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
                polygon.move(renderOffset);
                draw::polygon(polygon, drawColor);
                break;
            }

            case tmx::Object::Shape::Polyline:
            {
                const auto& verts = obj.getVertices();
                if (verts.size() < 2)
                    break;

                for (size_t i = 1; i < verts.size(); ++i)
                    draw::line({renderOffset + verts[i - 1], renderOffset + verts[i]}, drawColor);

                break;
            }

            default:
                break;
            }
        }

        return;
    }

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
                renderTransform.pos = rotatePoint(renderTransform.pos, pivotWorld, angle);
                renderTransform.angle += angle;
                setTexture->setClipArea(tile->getClipArea());
                renderer::draw(*setTexture, renderTransform);
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
            rect.setTopLeft(rect.getTopLeft() + offset);
            draw::polygon(Polygon{rotateRectCorners(rect, pivotWorld, angle)}, drawColor);
            break;
        }

        case tmx::Object::Shape::Ellipse:
        {
            Rect rect = obj.getRect();
            rect.setTopLeft(rect.getTopLeft() + offset);
            draw::polygon(Polygon{rotateEllipsePoints(rect, pivotWorld, angle)}, drawColor);
            break;
        }

        case tmx::Object::Shape::Point:
            if (const auto& verts = obj.getVertices(); !verts.empty())
                draw::point(rotatePoint(verts[0] + renderOffset, pivotWorld, angle), drawColor);
            break;

        case tmx::Object::Shape::Polygon:
        {
            std::vector<Vec2> points;
            points.reserve(obj.getVertices().size());
            for (const auto& vert : obj.getVertices())
                points.push_back(vert + renderOffset);
            points = rotatePoints(points, pivotWorld, angle);
            draw::polygon(Polygon{points}, drawColor);
            break;
        }

        case tmx::Object::Shape::Polyline:
        {
            const auto& verts = obj.getVertices();
            if (verts.size() < 2)
                break;

            std::vector<Vec2> points;
            points.reserve(verts.size());
            for (const auto& vert : verts)
                points.push_back(vert + renderOffset);

            points = rotatePoints(points, pivotWorld, angle);

            draw::polyline(points, drawColor);

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

void ImageLayer::draw(const double angle, const Vec2& pivot)
{
    if (!visible)
        return;

    Transform renderTransform = transform;
    renderTransform.pos += offset;

    if (angle != 0.0)
    {
        const Vec2 pivotWorld = getMapPivotWorld(m_map, pivot);
        renderTransform.pos = rotatePoint(renderTransform.pos, pivotWorld, angle);
        renderTransform.angle += angle;
    }

    renderer::draw(*m_texture, renderTransform);
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

}  // namespace tilemap
}  // namespace kn
