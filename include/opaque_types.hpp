#pragma once
#include <pybind11/pybind11.h>

#include <vector>

#include "AnimationController.hpp"
#include "Input.hpp"
#include "TileMap.hpp"

// Tilemap opaque types
PYBIND11_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet::Terrain>);
PYBIND11_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet::Tile>);
PYBIND11_MAKE_OPAQUE(std::vector<kn::tilemap::TileLayer::Tile>);
PYBIND11_MAKE_OPAQUE(std::vector<kn::tilemap::TileLayer::TileResult>);
PYBIND11_MAKE_OPAQUE(std::vector<kn::tilemap::MapObject>);
PYBIND11_MAKE_OPAQUE(std::vector<kn::tilemap::TileSet>);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<kn::tilemap::Layer>>);
PYBIND11_MAKE_OPAQUE(std::array<int32_t, 4>);

// AnimationController opaque types
PYBIND11_MAKE_OPAQUE(std::vector<kn::SheetStrip>);

// Shape draws
PYBIND11_MAKE_OPAQUE(std::vector<kn::Vec2>);
PYBIND11_MAKE_OPAQUE(std::vector<kn::Rect>);

// InputAction
PYBIND11_MAKE_OPAQUE(std::vector<kn::InputAction>);
