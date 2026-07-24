#pragma once

#include <nanobind/nanobind.h>

#include <array>
#include <memory>
#include <vector>

#include "kraken/animation/AnimationController.hpp"
#include "kraken/geometry/Circle.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/input/Input.hpp"
#include "kraken/math/Transform.hpp"
#include "kraken/tilemap/TileMap.hpp"

// Tilemap opaque types
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet::Terrain>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet::Tile>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileLayer::Tile>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet>);
NB_MAKE_OPAQUE(std::vector<kn::tilemap::MapObject>);
NB_MAKE_OPAQUE(std::vector<std::shared_ptr<kn::tilemap::Layer>>);
NB_MAKE_OPAQUE(std::array<int32_t, 4>);
