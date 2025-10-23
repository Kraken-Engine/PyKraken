#include "Collision.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "TileMap.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <pybind11/native_enum.h>
#include <pybind11/stl.h>

#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>

namespace kn
{
Layer::Layer(const Type type, const bool isVisible, std::string name,
             const std::shared_ptr<Texture>& tileSetTexture)
    : type(type), isVisible(isVisible), name(std::move(name)), m_tileSetTexture(tileSetTexture)
{
}

void Layer::render() const
{
    for (const auto& tile : tiles)
    {
        if (tile.antiDiagFlip)
        {
            m_tileSetTexture->angle = 90.0;
            m_tileSetTexture->flip.h = tile.vFlip;
            m_tileSetTexture->flip.v = !tile.hFlip;
        }
        else
        {
            m_tileSetTexture->angle = tile.angle;
            m_tileSetTexture->flip.h = tile.hFlip;
            m_tileSetTexture->flip.v = tile.vFlip;
        }

        renderer::draw(*m_tileSetTexture, tile.dst, tile.src);
    }
}

void Layer::buildTileGrid(const int mapWidth, const int mapHeight, const int tileWidth,
                          const int tileHeight)
{
    if (type != TILE)
        return;

    if (mapWidth <= 0 || mapHeight <= 0 || tileWidth <= 0 || tileHeight <= 0)
    {
        m_gridWidth = 0;
        m_gridHeight = 0;
        m_tileWidth = 0;
        m_tileHeight = 0;
        m_tileIndices.clear();
        return;
    }

    m_gridWidth = mapWidth;
    m_gridHeight = mapHeight;
    m_tileWidth = tileWidth;
    m_tileHeight = tileHeight;

    const size_t cellCount = static_cast<size_t>(mapWidth) * static_cast<size_t>(mapHeight);
    m_tileIndices.assign(cellCount, -1);

    for (size_t i = 0; i < tiles.size(); ++i)
    {
        const auto& tile = tiles[i];

        const int column =
            static_cast<int>(std::floor(tile.dst.x / static_cast<double>(m_tileWidth)));
        const int row =
            static_cast<int>(std::floor(tile.dst.y / static_cast<double>(m_tileHeight)));

        if (column < 0 || column >= m_gridWidth || row < 0 || row >= m_gridHeight)
            continue;

        const size_t gridIndex = static_cast<size_t>(row) * static_cast<size_t>(m_gridWidth) +
                                 static_cast<size_t>(column);
        m_tileIndices[gridIndex] = static_cast<int>(i);
    }
}

std::vector<Tile> Layer::getFromArea(const Rect& area) const
{
    std::vector<Tile> result;
    result.reserve(4);
    forEachTileInArea(area,
                      [&result, &area](const Tile& tile)
                      {
                          if (!collision::overlap(tile.dst, area))
                              return true;
                          result.push_back(tile);
                          return true;
                      });
    return result;
}

std::optional<Tile> Layer::getTileAt(const int column, const int row) const
{
    if (type != TILE || m_tileIndices.empty() || m_gridWidth <= 0 || m_gridHeight <= 0)
        return std::nullopt;

    if (column < 0 || column >= m_gridWidth || row < 0 || row >= m_gridHeight)
        return std::nullopt;

    const size_t index =
        static_cast<size_t>(row) * static_cast<size_t>(m_gridWidth) + static_cast<size_t>(column);
    const int tileIndex = m_tileIndices[index];
    if (tileIndex < 0)
        return std::nullopt;

    return tiles[static_cast<size_t>(tileIndex)];
}

TileMap::TileMap(const std::string& tmxPath, int borderSize)
{
    pugi::xml_document doc;
    if (!doc.load_file(tmxPath.c_str()))
        throw std::runtime_error("Failed to load TMX file: " + tmxPath);

    const size_t lastSlashPos = tmxPath.find_last_of('/');
    m_dirPath = lastSlashPos != std::string::npos ? tmxPath.substr(0, lastSlashPos + 1) : "";

    const auto map = doc.child("map");

    std::string texturePath = getTexturePath(map);
    if (texturePath.empty())
        throw std::runtime_error("Failed to find tileset image from TMX file: " + tmxPath);

    const auto tileSetTexture = std::make_shared<Texture>(texturePath);

    SDL_Surface* surface = IMG_Load(texturePath.c_str());

    const int mapWidth = std::stoi(map.attribute("width").value());
    const int mapHeight = std::stoi(map.attribute("height").value());
    const int tileWidth = std::stoi(map.attribute("tilewidth").value());
    const int tileHeight = std::stoi(map.attribute("tileheight").value());
    const int tileSetWidth =
        static_cast<int>(tileSetTexture->getSize().x) / (tileWidth + 2 * borderSize);

    for (const auto& child : map.children())
    {
        std::string childName = child.name();
        if (childName == "tileset")
            continue;

        std::string layerName = child.attribute("name").value();
        const auto layerVisibility = std::string(child.attribute("visible").value());
        const bool isVisible = layerVisibility.empty() || layerVisibility != "0";
        std::vector<Tile> tiles;
        std::shared_ptr<Layer> layerPtr = nullptr;

        if (childName == "layer")
        {
            std::string dataContent = child.child("data").child_value();
            std::erase(dataContent, '\n');
            std::erase(dataContent, '\r');

            std::stringstream ss(dataContent);
            std::string value;
            layerPtr =
                std::make_shared<Layer>(Layer(Layer::TILE, isVisible, layerName, tileSetTexture));

            int tileCounter = 0;
            while (std::getline(ss, value, ','))
            {
                if (value.empty() || !std::ranges::all_of(value, ::isdigit))
                {
                    tileCounter++;
                    continue;
                }

                const uint32_t rawId = std::stoul(value);
                constexpr uint32_t FLIP_HORIZONTAL_FLAG = 0x80000000;
                constexpr uint32_t FLIP_VERTICAL_FLAG = 0x40000000;
                constexpr uint32_t FLIP_DIAGONAL_FLAG = 0x20000000;

                const bool horizontalFlip = (rawId & FLIP_HORIZONTAL_FLAG) != 0;
                const bool verticalFlip = (rawId & FLIP_VERTICAL_FLAG) != 0;
                const bool antiDiagonalFlip = (rawId & FLIP_DIAGONAL_FLAG) != 0;

                const int tileId = static_cast<int>(rawId & 0x0FFFFFFF) - 1;
                if (tileId < 0)
                {
                    tileCounter++;
                    continue;
                }

                const int srcX = tileId % tileSetWidth * (tileWidth + 2 * borderSize) + borderSize;
                const int srcY = tileId / tileSetWidth * (tileHeight + 2 * borderSize) + borderSize;
                const int destX = tileCounter % mapWidth * tileWidth;
                const int destY = tileCounter / mapWidth * tileHeight;

                const Rect tileSrcRect = {srcX, srcY, tileWidth, tileHeight};
                const Rect tileDstRect = {destX, destY, tileWidth, tileHeight};

                tiles.push_back({layerPtr, tileSrcRect, tileDstRect,
                                 getFittedRect(surface, tileSrcRect, tileDstRect.getTopLeft()),
                                 horizontalFlip, verticalFlip, antiDiagonalFlip, 0.0});
                tileCounter++;
            }
            layerPtr->tiles = std::move(tiles);
            layerPtr->buildTileGrid(mapWidth, mapHeight, tileWidth, tileHeight);
        }
        else if (childName == "objectgroup")
        {
            layerPtr =
                std::make_shared<Layer>(Layer(Layer::OBJECT, isVisible, layerName, tileSetTexture));

            for (const auto& object : child.children())
            {
                const int width = std::stoi(object.attribute("width").value());
                const int height = std::stoi(object.attribute("height").value());
                const std::string rotation = object.attribute("rotation").value();
                const int angle = rotation.empty() ? 0 : std::stoi(rotation);

                const uint32_t rawId = std::stoul(object.attribute("gid").value());
                constexpr uint32_t FLIP_HORIZONTAL_FLAG = 0x80000000;
                constexpr uint32_t FLIP_VERTICAL_FLAG = 0x40000000;

                const bool horizontalFlip = (rawId & FLIP_HORIZONTAL_FLAG) != 0;
                const bool verticalFlip = (rawId & FLIP_VERTICAL_FLAG) != 0;

                const int tileId = static_cast<int>(rawId & 0x0FFFFFFF) - 1;

                const int srcX = tileId % tileSetWidth * (width + 2 * borderSize) + borderSize;
                const int srcY = tileId / tileSetWidth * (height + 2 * borderSize) + borderSize;
                const float destX = std::stof(object.attribute("x").value());
                const float destY = std::stof(object.attribute("y").value());

                const Rect objectSrcRect{srcX, srcY, tileWidth, tileHeight};
                Rect objectDstRect;
                objectDstRect.setSize({tileWidth, tileHeight});

                switch (angle)
                {
                case 90:
                    objectDstRect.setTopLeft({destX, destY});
                    break;
                case 180:
                    objectDstRect.setTopRight({destX, destY});
                    break;
                case -90:
                    objectDstRect.setBottomRight({destX, destY});
                    break;
                default:
                    objectDstRect.setBottomLeft({destX, destY}); // 0 degrees
                }

                tiles.push_back({layerPtr, objectSrcRect, objectDstRect,
                                 getFittedRect(surface, objectSrcRect, objectDstRect.getTopLeft()),
                                 horizontalFlip, verticalFlip, false, static_cast<double>(angle)});
            }
            layerPtr->tiles = std::move(tiles);
        }
        m_layerVec.push_back(layerPtr);
    }

    SDL_DestroySurface(surface);
}

std::shared_ptr<const Layer> TileMap::getLayer(const std::string& name,
                                               const Layer::Type type) const
{
    for (const auto& layer : m_layerVec)
    {
        if (layer->name == name && layer->type == type)
            return layer;
    }

    throw std::invalid_argument("Layer not found or type mismatch: " + name);
}

const std::vector<std::shared_ptr<const Layer>>& TileMap::getLayers() const { return m_layerVec; }

void TileMap::render() const
{
    if (m_layerVec.empty())
        return;

    for (const auto& layer : m_layerVec)
    {
        if (!layer->isVisible)
            continue;

        layer->render();
    }
}

std::vector<Tile>
TileMap::getTileCollection(const std::vector<std::shared_ptr<const Layer>>& layers)
{
    std::vector<Tile> tiles;
    if (layers.empty())
        return tiles;

    for (const auto& layer : layers)
    {
        const auto& layerTiles = layer->tiles;
        tiles.insert(tiles.end(), layerTiles.begin(), layerTiles.end());
    }

    return tiles;
}

std::string TileMap::getTexturePath(const pugi::xml_node& mapNode) const
{
    const std::string tsxPath = m_dirPath + mapNode.child("tileset").attribute("source").value();

    pugi::xml_document doc;
    if (!doc.load_file(tsxPath.c_str()))
        throw std::runtime_error("Failed to load TSX file: " + tsxPath);

    return m_dirPath + doc.child("tileset").child("image").attribute("source").value();
}

Rect TileMap::getFittedRect(SDL_Surface* surface, const Rect& srcRect, const Vec2& position)
{
    const auto [x, y, w, h] = static_cast<SDL_Rect>(srcRect);
    int top = h;
    int bottom = 0;
    int left = w;
    int right = 0;

    uint8_t alpha;
    for (int dy = y; dy < y + h; dy++)
        for (int dx = x; dx < x + w; dx++)
        {
            SDL_ReadSurfacePixel(surface, dx, dy, nullptr, nullptr, nullptr, &alpha);
            if (alpha == 0)
                continue;

            top = std::min(top, dy - y);
            bottom = std::max(bottom, dy - y);
            left = std::min(left, dx - x);
            right = std::max(right, dx - x);
        }

    return {static_cast<int>(position.x) + left, static_cast<int>(position.y) + top,
            right - left + 1, bottom - top + 1};
}

namespace tile_map
{
void _bind(const py::module_& module)
{
    // Initial Layer binding
    auto layerPy = py::classh<Layer>(module, "Layer", R"doc(
A layer within a tile map.

Layers can be either tile layers or object layers and contain a list of tiles.
    )doc");

    // Tile binding
    py::classh<Tile>(module, "Tile", R"doc(
Represents a single tile instance in a layer.

Contains source and destination rectangles, a collider, flip flags, rotation angle,
and a reference to its owning Layer.
    )doc")
        .def_property_readonly(
            "layer",
            [](const Tile& self) -> std::shared_ptr<const Layer>
            {
                if (auto sp = self.layer.lock())
                    return sp;
                return {};
            },
            R"doc(
Get the owning Layer.

Returns:
    Layer | None: The owning Layer if it still exists; otherwise None.
            )doc")
        .def_readonly("src", &Tile::src, R"doc(
The source rectangle within the tileset texture.
        )doc")
        .def_readonly("dst", &Tile::dst, R"doc(
The destination rectangle on the map.
        )doc")
        .def_readonly("collider", &Tile::collider, R"doc(
The fitted collider rectangle for the tile's opaque area.
        )doc")
        .def_readonly("h_flip", &Tile::hFlip, R"doc(
Whether the tile is flipped horizontally.
        )doc")
        .def_readonly("v_flip", &Tile::vFlip, R"doc(
Whether the tile is flipped vertically.
        )doc")
        .def_readonly("anti_diag_flip", &Tile::antiDiagFlip, R"doc(
Whether the tile is flipped across the anti-diagonal.
        )doc")
        .def_readonly("angle", &Tile::angle, R"doc(
The rotation angle in degrees.
        )doc");

    // Layer bindings cont.
    py::native_enum<Layer::Type>(layerPy, "Type", "enum.IntEnum", R"doc(
The type of a Layer.
    )doc")
        .value("OBJECT", Layer::Type::OBJECT)
        .value("TILE", Layer::Type::TILE)
        .export_values()
        .finalize();

    layerPy
        .def_readonly("type", &Layer::type, R"doc(
The layer type (OBJECT or TILE).
        )doc")
        .def_readonly("is_visible", &Layer::isVisible, R"doc(
Whether the layer is visible.
        )doc")
        .def_readonly("name", &Layer::name, R"doc(
The name of the layer.
        )doc")
        .def_readonly("tiles", &Layer::tiles, R"doc(
The list of Tile instances contained in this layer.
        )doc")
        .def("render", &Layer::render, R"doc(
Render the layer.
        )doc")
        .def("get_from_area", &Layer::getFromArea, py::arg("area"), R"doc(
Get all tiles whose destination rectangles fall within a query rect.

Args:
    area (Rect): The world-space rectangle to test.

Returns:
    list[Tile]: Tiles intersecting the given area.
        )doc")
        .def("get_tile_at", &Layer::getTileAt, py::arg("column"), py::arg("row"), R"doc(
Get the tile located at the specified grid coordinates.

Args:
    column (int): The tile column index.
    row (int): The tile row index.

Returns:
    Tile | None: The tile at the grid coordinate, or None if empty/out of range.
        )doc");

    // TileMap binding
    py::classh<TileMap>(module, "TileMap", R"doc(
Loads and renders TMX tile maps.

Parses a Tiled TMX file, loads the tileset texture, and exposes layers and tiles for rendering and queries.
    )doc")
        .def(py::init<const std::string&, int>(), py::arg("tmx_path"), py::arg("border_size") = 0,
             R"doc(
Create a TileMap by loading a TMX file.

Args:
    tmx_path (str): Path to the TMX file.
    border_size (int): Optional border (in pixels) around each tile in the tileset; defaults to 0.

Raises:
    RuntimeError: If the TMX or TSX files cannot be loaded or parsed.
             )doc")
        .def("get_layer", &TileMap::getLayer, py::arg("name"), py::arg("type") = Layer::TILE,
             R"doc(
Get a layer by name and type.

Args:
    name (str): The layer name.
    type (Layer.Type): The expected layer type (defaults to TILE).

Returns:
    Layer: The matching layer.

Raises:
    ValueError: If no matching layer is found or the type doesn't match.
             )doc")
        .def("get_layers", &TileMap::getLayers, R"doc(
Get all layers in the map.

Returns:
    list[Layer]: A list of all layers.
        )doc")
        .def("render", &TileMap::render, R"doc(
Render all visible layers.
        )doc")

        .def_static("get_tile_collection", &TileMap::getTileCollection, py::arg("layers"),
                    R"doc(
Collect all tiles from the provided layers into a single list.

Args:
    layers (Sequence[Layer]): The layers to collect tiles from.

Returns:
    list[Tile]: A flat list of tiles from the given layers.
                    )doc");
}
} // namespace tile_map
} // namespace kn
