#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>
#include <pugixml/pugixml.hpp>
#include <pybind11/pybind11.h>
#include <string>
#include <vector>

#include "Rect.hpp"

namespace py = pybind11;

namespace kn
{
class Texture;
class TileLayer;

namespace tile_map
{
void _bind(const py::module_& module);
}

struct Tile
{
    std::weak_ptr<const TileLayer> layer;

    Rect src;
    Rect dst;
    Rect collider;

    bool hFlip;
    bool vFlip;
    bool antiDiagFlip;
    double angle;
};

class TileLayer
{
  public:
    enum Type
    {
        OBJECT,
        TILE,
    } const type;

    const bool isVisible;
    const std::string name;
    std::vector<Tile> tiles;

    TileLayer(Type type, bool isVisible, std::string name,
          const std::shared_ptr<Texture>& tileSetTexture);
    ~TileLayer() = default;

    void render() const;
    [[nodiscard]] std::vector<Tile> getFromArea(const Rect& area) const;
    [[nodiscard]] std::optional<Tile> getTileAt(int column, int row) const;

  private:
    friend class TileMap;
    void buildTileGrid(int mapWidth, int mapHeight, int tileWidth, int tileHeight);
    template <typename Fn> bool forEachTileInArea(const Rect& area, Fn&& fn) const;

    std::shared_ptr<Texture> m_tileSetTexture;
    int m_gridWidth = 0;
    int m_gridHeight = 0;
    int m_tileWidth = 0;
    int m_tileHeight = 0;
    std::vector<int> m_tileIndices;
};

class TileMap
{
  public:
    explicit TileMap(const std::string& tmxPath, int borderSize = 0);
    ~TileMap() = default;

    [[nodiscard]] std::shared_ptr<const TileLayer> getLayer(const std::string& name,
                                                        TileLayer::Type type = TileLayer::TILE) const;
    [[nodiscard]] const std::vector<std::shared_ptr<const TileLayer>>& getLayers() const;

    void render() const;

    static std::vector<Tile>
    getTileCollection(const std::vector<std::shared_ptr<const TileLayer>>& layers);

  private:
    std::string m_dirPath;
    std::vector<std::shared_ptr<const TileLayer>> m_layerVec;

    [[nodiscard]] std::string getTexturePath(const pugi::xml_node& mapNode) const;
    static Rect getFittedRect(SDL_Surface* surface, const Rect& srcRect, const Vec2& position);
};

template <typename Fn> bool TileLayer::forEachTileInArea(const Rect& area, Fn&& fn) const
{
    if (type != TILE || m_tileIndices.empty() || m_gridWidth <= 0 || m_gridHeight <= 0 ||
        m_tileWidth <= 0 || m_tileHeight <= 0)
        return true;

    if (area.w <= 0 || area.h <= 0)
        return true;

    const double minX = area.x;
    const double minY = area.y;
    const double maxX = area.x + area.w;
    const double maxY = area.y + area.h;

    const int rawStartCol = static_cast<int>(std::floor(minX / static_cast<double>(m_tileWidth)));
    const int rawEndCol = static_cast<int>(std::ceil(maxX / static_cast<double>(m_tileWidth))) - 1;
    const int rawStartRow = static_cast<int>(std::floor(minY / static_cast<double>(m_tileHeight)));
    const int rawEndRow = static_cast<int>(std::ceil(maxY / static_cast<double>(m_tileHeight))) - 1;

    if (rawEndCol < 0 || rawEndRow < 0 || rawStartCol >= m_gridWidth || rawStartRow >= m_gridHeight)
        return true;

    const int startCol = std::clamp(rawStartCol, 0, m_gridWidth - 1);
    const int endCol = std::clamp(rawEndCol, 0, m_gridWidth - 1);
    const int startRow = std::clamp(rawStartRow, 0, m_gridHeight - 1);
    const int endRow = std::clamp(rawEndRow, 0, m_gridHeight - 1);

    if (startCol > endCol || startRow > endRow)
        return true;

    for (int row = startRow; row <= endRow; ++row)
    {
        const size_t rowOff = static_cast<size_t>(row) * static_cast<size_t>(m_gridWidth);
        for (int col = startCol; col <= endCol; ++col)
        {
            const int tileIndex = m_tileIndices[rowOff + static_cast<size_t>(col)];
            if (tileIndex < 0)
                continue;

            if (!fn(tiles[static_cast<size_t>(tileIndex)]))
                return false;
        }
    }

    return true;
}
} // namespace kn
