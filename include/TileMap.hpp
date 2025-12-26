#pragma once

#include <pybind11/pybind11.h>

#include <memory>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
namespace tilemap
{
class TileSet
{
  public:
    class Terrain
    {
        std::string m_name;
        uint32_t m_tileID = static_cast<uint32_t>(-1);

      public:
        Terrain(const std::string& name, uint32_t tileID)
            : m_name(name),
              m_tileID(tileID)
        {
        }

        [[nodiscard]] std::string getName() const
        {
            return m_name;
        }
        [[nodiscard]] uint32_t getTileID() const
        {
            return m_tileID;
        }
    };

    struct Tile
    {
        uint32_t m_ID = 0;
        std::array<int32_t, 4> m_terrainIndices{};
        uint32_t m_probability = 100;
        Rect m_clipRect{};

      public:
        [[nodiscard]] uint32_t getID() const
        {
            return m_ID;
        }

        [[nodiscard]] const std::array<int32_t, 4>& getTerrainIndices() const
        {
            return m_terrainIndices;
        }

        [[nodiscard]] uint32_t getProbability() const
        {
            return m_probability;
        }

        [[nodiscard]] Rect getClipRect() const
        {
            return m_clipRect;
        }
    };

    TileSet() = default;
    ~TileSet() = default;

    [[nodiscard]] uint32_t getFirstGID() const;
    [[nodiscard]] uint32_t getLastGID() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Vec2 getTileSize() const;
    [[nodiscard]] uint32_t getSpacing() const;
    [[nodiscard]] uint32_t getMargin() const;
    [[nodiscard]] uint32_t getTileCount() const;
    [[nodiscard]] uint32_t getColumns() const;
    [[nodiscard]] Vec2 getTileOffset() const;
    [[nodiscard]] const std::vector<Terrain>& getTerrains() const;
    [[nodiscard]] const std::vector<Tile>& getTiles() const;
    [[nodiscard]] bool hasTile(uint32_t id) const;
    [[nodiscard]] const Tile* getTile(uint32_t id) const;
    [[nodiscard]] std::shared_ptr<Texture> getTexture() const;

  private:
    uint32_t m_firstGID = 0;
    uint32_t m_lastGID = 0;
    std::string m_name = "";
    Vec2 m_tileSize{};
    uint32_t m_spacing = 0;
    uint32_t m_margin = 0;
    uint32_t m_tileCount = 0;
    uint32_t m_columns = 0;
    Vec2 m_tileOffset{};
    std::vector<Terrain> m_terrains{};
    std::vector<Tile> m_tiles{};
    std::vector<std::uint32_t> m_tileIndex;
    std::shared_ptr<Texture> m_texture = nullptr;

    friend class Map;
};

class Layer
{
  public:
    double opacity = 1.0;
    bool visible = true;

    Layer() = default;
    virtual ~Layer() = default;

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Vec2 getOffset() const;
    [[nodiscard]] tmx::Layer::Type getType() const;

  private:
    tmx::Layer::Type m_type;
    std::string m_name = "";
    Vec2 m_offset{};

    friend class Map;
};

class TileLayer : public Layer
{
  public:
    class Tile
    {
        uint32_t id = 0;
        uint8_t flipFlags = 0;

      public:
        Tile(uint32_t tileID, uint8_t flags)
            : id(tileID),
              flipFlags(flags)
        {
        }

        [[nodiscard]] uint32_t getID() const
        {
            return id;
        }

        [[nodiscard]] uint8_t getFlipFlags() const
        {
            return flipFlags;
        }
    };

    TileLayer() = default;
    ~TileLayer() = default;

    [[nodiscard]] const std::vector<Tile>& getTiles() const;

  private:
    std::vector<Tile> m_tiles{};

    friend class Map;
};

struct TextProperties
{
    std::string fontFamily = "";
    uint32_t pixelSize = 16;
    bool wrap = false;
    Color color{};
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool strikethrough = false;
    bool kerning = true;
    Align align = Align::Left;
    std::string text = "";
};

class MapObject
{
  public:
    Vec2 position{};
    double rotation = 0.0;
    bool visible = true;

    MapObject() = default;
    ~MapObject() = default;

    [[nodiscard]] uint32_t getUID() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getType() const;
    [[nodiscard]] Rect getRect() const;
    [[nodiscard]] uint32_t getTileID() const;
    [[nodiscard]] tmx::Object::Shape getShapeType() const;
    [[nodiscard]] const std::vector<Vec2>& getVertices() const;
    [[nodiscard]] const TextProperties& getTextProperties() const;

  private:
    uint32_t m_uid = 0;
    std::string m_name = "";
    std::string m_type = "";
    Rect m_rect{};
    uint32_t m_tileId = 0;
    tmx::Object::Shape m_shape;
    std::vector<Vec2> m_vertices{};
    TextProperties m_text{};

    friend class Map;
};

class ObjectGroup : public Layer
{
  public:
    Color color{};

    ObjectGroup() = default;
    ~ObjectGroup() = default;

    [[nodiscard]] tmx::ObjectGroup::DrawOrder getDrawOrder() const;
    [[nodiscard]] const std::vector<MapObject>& getObjects() const;

  private:
    tmx::ObjectGroup::DrawOrder m_drawOrder;
    std::vector<MapObject> m_objects{};

    friend class Map;
};

class ImageLayer : public Layer
{
  public:
    ImageLayer() = default;
    ~ImageLayer() = default;

    [[nodiscard]] std::shared_ptr<Texture> getTexture() const;

  private:
    std::shared_ptr<Texture> m_texture = nullptr;

    friend class Map;
};

class Map
{
  public:
    Color backgroundColor{};

    Map() = default;
    ~Map() = default;

    void loadFromTMX(const std::string& tmxPath);
    // TODO: loadFromLDTK

    [[nodiscard]] tmx::Orientation getOrientation() const;
    [[nodiscard]] tmx::RenderOrder getRenderOrder() const;
    [[nodiscard]] Vec2 getMapSize() const;
    [[nodiscard]] Vec2 getTileSize() const;
    [[nodiscard]] Rect getBounds() const;
    [[nodiscard]] double getHexSideLength() const;
    [[nodiscard]] tmx::StaggerAxis getStaggerAxis() const;
    [[nodiscard]] tmx::StaggerIndex getStaggerIndex() const;
    [[nodiscard]] const std::vector<TileSet>& getTileSets() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Layer>>& getLayers() const;

  private:
    tmx::Orientation m_orient = tmx::Orientation::None;
    tmx::RenderOrder m_renderOrder = tmx::RenderOrder::None;
    Vec2 m_mapSize{};
    Vec2 m_tileSize{};
    Rect m_bounds{};
    double m_hexSideLength = 0.0;
    tmx::StaggerAxis m_staggerAxis = tmx::StaggerAxis::None;
    tmx::StaggerIndex m_staggerIndex = tmx::StaggerIndex::None;
    std::vector<TileSet> m_tileSets{};
    std::vector<std::shared_ptr<Layer>> m_layers{};
};

void _bind(py::module_& module);
}  // namespace tilemap
}  // namespace kn
