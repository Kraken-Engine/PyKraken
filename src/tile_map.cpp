#include <pybind11/native_enum.h>

#include "TileMap.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

namespace kn
{
void TileMap::loadFromTMX(const std::string& tmxPath)
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

        tileSet.m_terrainTypes.reserve(tsTerrainTypes.size());
        for (const auto& tmxTerrain : tsTerrainTypes)
        {
            TileSet::Terrain terrain{tmxTerrain.name, tmxTerrain.tileID};
            tileSet.m_terrainTypes.push_back(std::move(terrain));
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

        // Move last in case other operations need it
        tileSet.m_tileIndex = std::move(tileSet.m_tileIndex);

        m_tileSets.push_back(std::move(tileSet));
    }
}

tmx::Orientation TileMap::getOrientation() const
{
    return m_orient;
}

tmx::RenderOrder TileMap::getRenderOrder() const
{
    return m_renderOrder;
}

Vec2 TileMap::getMapSize() const
{
    return m_mapSize;
}

Vec2 TileMap::getTileSize() const
{
    return m_tileSize;
}

Rect TileMap::getBounds() const
{
    return m_bounds;
}

double TileMap::getHexSideLength() const
{
    return m_hexSideLength;
}

tmx::StaggerAxis TileMap::getStaggerAxis() const
{
    return m_staggerAxis;
}

tmx::StaggerIndex TileMap::getStaggerIndex() const
{
    return m_staggerIndex;
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

const std::vector<TileSet::Terrain>& TileSet::getTerrainTypes() const
{
    return m_terrainTypes;
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

    id -= m_firstGID;
    id = m_tileIndex[id];
    return id ? &m_tiles[id - 1] : nullptr;
}

std::shared_ptr<Texture> TileSet::getTexture() const
{
    return m_texture;
}

namespace tile_map
{
void _bind(const py::module_& module)
{
    py::native_enum<tmx::Orientation>(module, "TileMapOrientation", "enum.IntEnum")
        .value("ORTHOGONAL", tmx::Orientation::Orthogonal)
        .value("ISOMETRIC", tmx::Orientation::Isometric)
        .value("STAGGERED", tmx::Orientation::Staggered)
        .value("HEXAGONAL", tmx::Orientation::Hexagonal)
        .value("NONE", tmx::Orientation::None)
        .finalize();

    py::native_enum<tmx::RenderOrder>(module, "TileMapRenderOrder", "enum.IntEnum")
        .value("RIGHT_DOWN", tmx::RenderOrder::RightDown)
        .value("RIGHT_UP", tmx::RenderOrder::RightUp)
        .value("LEFT_DOWN", tmx::RenderOrder::LeftDown)
        .value("LEFT_UP", tmx::RenderOrder::LeftUp)
        .value("NONE", tmx::RenderOrder::None)
        .finalize();

    py::native_enum<tmx::StaggerAxis>(module, "TileMapStaggerAxis", "enum.IntEnum")
        .value("X", tmx::StaggerAxis::X)
        .value("Y", tmx::StaggerAxis::Y)
        .value("NONE", tmx::StaggerAxis::None)
        .finalize();

    py::native_enum<tmx::StaggerIndex>(module, "TileMapStaggerIndex", "enum.IntEnum")
        .value("EVEN", tmx::StaggerIndex::Even)
        .value("ODD", tmx::StaggerIndex::Odd)
        .value("NONE", tmx::StaggerIndex::None)
        .finalize();

    py::classh<TileSet>(module, "TileSet")
        .def("has_tile", &TileSet::hasTile, py::arg("id"))
        .def("get_tile", &TileSet::getTile, py::arg("id"))

        .def_property_readonly("first_gid", &TileSet::getFirstGID)
        .def_property_readonly("last_gid", &TileSet::getLastGID)
        .def_property_readonly("name", &TileSet::getName)
        .def_property_readonly("tile_size", &TileSet::getTileSize)
        .def_property_readonly("spacing", &TileSet::getSpacing)
        .def_property_readonly("margin", &TileSet::getMargin)
        .def_property_readonly("tile_count", &TileSet::getTileCount)
        .def_property_readonly("columns", &TileSet::getColumns)
        .def_property_readonly("tile_offset", &TileSet::getTileOffset)
        .def_property_readonly("terrain_types", &TileSet::getTerrainTypes)
        .def_property_readonly("tiles", &TileSet::getTiles)
        .def_property_readonly("texture", &TileSet::getTexture);

    py::classh<TileMap>(module, "TileMap")
        .def(py::init<>())

        .def_readwrite("background_color", &TileMap::backgroundColor)

        .def("load_from_tmx", &TileMap::loadFromTMX, py::arg("tmx_path"))

        .def_property_readonly("orientation", &TileMap::getOrientation)
        .def_property_readonly("render_order", &TileMap::getRenderOrder)
        .def_property_readonly("map_size", &TileMap::getMapSize)
        .def_property_readonly("tile_size", &TileMap::getTileSize)
        .def_property_readonly("bounds", &TileMap::getBounds)
        .def_property_readonly("hex_side_length", &TileMap::getHexSideLength)
        .def_property_readonly("stagger_axis", &TileMap::getStaggerAxis)
        .def_property_readonly("stagger_index", &TileMap::getStaggerIndex);
}
}  // namespace tile_map
}  // namespace kn
