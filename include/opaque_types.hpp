#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>

#include <array>
#include <memory>
#include <vector>

#include "AnimationController.hpp"
#include "Circle.hpp"
#include "Input.hpp"
#include "Polygon.hpp"
#include "TileMap.hpp"
#include "Transform.hpp"

// Tilemap opaque types
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet::Terrain>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet::Tile>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileLayer::Tile>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::MapObject>);
NB_MAKE_OPAQUE(std::vector<std::shared_ptr<kn::tilemap::Layer>>);
NB_MAKE_OPAQUE(std::array<int32_t, 4>);

#endif  // KRAKEN_ENABLE_PYTHON
