#pragma once

#include <memory>
#include <pugixml/pugixml.hpp>
#include <pybind11/pybind11.h>
#include <string>
#include <vector>

#include "Rect.hpp"

namespace py = pybind11;

namespace kn
{
class Texture;
class Layer;

namespace tile_map
{
void _bind(const py::module_& module);
}

struct Tile
{
    std::weak_ptr<const Layer> layer;

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

    Layer(Type type, bool isVisible, std::string name,
          const std::shared_ptr<Texture>& tileSetTexture);
    ~Layer() = default;

    void render() const;

  private:
    std::shared_ptr<Texture> m_tileSetTexture;
};

class TileMap
{
  public:
    explicit TileMap(const std::string& tmxPath, int borderSize = 0);
    ~TileMap() = default;

    [[nodiscard]] std::shared_ptr<const Layer> getLayer(const std::string& name,
                                                        Layer::Type type = Layer::TILE) const;
    [[nodiscard]] const std::vector<std::shared_ptr<const Layer>>& getLayers() const;

    void render() const;

    static std::vector<Tile>
    getTileCollection(const std::vector<std::shared_ptr<const Layer>>& layers);

  private:
    std::string m_dirPath;
    std::vector<std::shared_ptr<const Layer>> m_layerVec;

    [[nodiscard]] std::string getTexturePath(const pugi::xml_node& mapNode) const;
    static Rect getFittedRect(SDL_Surface* surface, const Rect& srcRect, const Vec2& position);
};
} // namespace kn
