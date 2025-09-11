#pragma once

#include <memory>
#include <pugixml/pugixml.hpp>
#include <pybind11/pybind11.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Rect.hpp"

namespace py = pybind11;

namespace kn
{
class Texture;

namespace tile_map
{
void _bind(py::module_& module);
}

struct Tile
{
    std::shared_ptr<Layer> layer;

    Rect src;
    Rect dst;
    Rect collider;

    bool hFlip;
    bool vFlip;
    bool antiDiagFlip;
    double angle;
};

class Layer
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

    Layer(Type type, bool isVisible, const std::string& name,
          const std::shared_ptr<Texture>& tileSetTexture);
    ~Layer() = default;

    void render() const;

  private:
    const std::shared_ptr<Texture>& m_tileSetTexture;
};

class TileMap
{
  public:
    explicit TileMap(const std::string& tmxPath, int borderSize = 0);
    ~TileMap() = default;

    const Layer* getLayer(const std::string& name, Layer::Type type = Layer::TILE) const;
    const std::vector<std::shared_ptr<Layer>>& getLayers() const;

    void drawMap() const;

    static std::vector<Tile> getTileCollection(const std::vector<const Layer*>& layers);

  private:
    std::string m_dirPath;
    std::vector<std::shared_ptr<Layer>> m_layerVec;
    std::string getTexturePath(const pugi::xml_node& mapNode);
};
} // namespace kn
