#pragma once

#include <pybind11/pybind11.h>

#include <memory>
#include <optional>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

#include "Color.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
namespace tilemap
{
class Map;

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
        uint32_t m_id = 0;
        std::array<int, 4> m_terrainIndices{};
        uint32_t m_probability = 100;
        Rect m_clipArea{};

      public:
        [[nodiscard]] uint32_t getID() const
        {
            return m_id;
        }

        [[nodiscard]] const std::array<int32_t, 4>& getTerrainIndices() const
        {
            return m_terrainIndices;
        }

        [[nodiscard]] uint32_t getProbability() const
        {
            return m_probability;
        }

        [[nodiscard]] Rect getClipArea() const
        {
            return m_clipArea;
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
    std::vector<uint32_t> m_tileIndex;
    std::shared_ptr<Texture> m_texture = nullptr;

    friend class Map;
};

class Layer
{
  public:
    bool visible = true;
    Vec2 offset{};

    Layer() = default;
    virtual ~Layer() = default;

    virtual void draw() = 0;
    virtual void setOpacity(double value) = 0;
    virtual double getOpacity() const = 0;

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] tmx::Layer::Type getType() const;

  protected:
    Map* m_map = nullptr;
    tmx::Layer::Type m_type;
    std::string m_name = "";
    double m_opacity = 1.0;

    friend class Map;
};

class TileLayer : public Layer
{
  public:
    class Tile
    {
        uint32_t m_id = 0;
        uint8_t m_flipFlags = 0;
        uint8_t m_tilesetIdx = static_cast<uint8_t>(-1);  // -1 = unknown

        friend class Map;

      public:
        [[nodiscard]] uint32_t getID() const
        {
            return m_id;
        }

        [[nodiscard]] uint8_t getTilesetIndex() const
        {
            return m_tilesetIdx;
        }

        [[nodiscard]] uint8_t getFlipFlags() const
        {
            return m_flipFlags;
        }
    };

    struct TileResult
    {
        Tile tile;
        Rect rect;
    };

    TileLayer() = default;
    ~TileLayer() = default;

    [[nodiscard]] const std::vector<Tile>& getTiles() const;
    [[nodiscard]] std::vector<TileResult> getFromArea(const Rect& area) const;
    [[nodiscard]] std::optional<TileResult> getFromPoint(const Vec2& position) const;

    void draw() override;
    void setOpacity(double value) override;
    double getOpacity() const override;

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
    Transform transform{};
    bool visible = true;

    MapObject() = default;
    ~MapObject() = default;

    [[nodiscard]] uint32_t getUID() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getType() const;
    [[nodiscard]] Rect getRect() const;
    [[nodiscard]] uint32_t getTileID() const;
    [[nodiscard]] tmx::Object::Shape getShapeType() const;
    [[nodiscard]] std::vector<Vec2> getVertices() const;
    [[nodiscard]] const TextProperties& getTextProperties() const;

  private:
    uint32_t m_uid = 0;
    std::string m_name = "";
    std::string m_type = "";
    Rect m_rect{};
    uint32_t m_tileId = 0;
    tmx::Object::Shape m_shape = tmx::Object::Shape::Rectangle;
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
    void draw() override;
    void setOpacity(double value) override;
    double getOpacity() const override;

  private:
    tmx::ObjectGroup::DrawOrder m_drawOrder = tmx::ObjectGroup::DrawOrder::TopDown;
    std::vector<MapObject> m_objects{};

    friend class Map;
};

class ImageLayer : public Layer
{
  public:
    Transform transform{};

    ImageLayer() = default;
    ~ImageLayer() = default;

    [[nodiscard]] std::shared_ptr<Texture> getTexture() const;
    void draw() override;
    void setOpacity(double value) override;
    double getOpacity() const override;

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

    void draw();
    void load(const std::string& tmxPath);

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
