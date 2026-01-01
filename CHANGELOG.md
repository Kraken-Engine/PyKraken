# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) when possible.

## [1.5.0] - 2025-12-30

### Added
- Added `build.py` script for automating build and compilation tasks.
- Added `tilemap` submodule for working with tile maps.
- Added `Vec2` methods `move_toward` and non-mutating `moved_toward` for moving a vector towards a target vector by a specified maximum distance.

### Fixed
- Most evident with animations and tilemaps, fixed a visual artifact where textures would render slightly offset when using the camera. The fix? Flooring the camera's position before rendering.

### Changed
- Completely reworked tile maps in Kraken, supporting:
    - Tile, Object (of all kinds), and Image layers
    - Terrains in tilesets
    - More than one tileset per map
    - Text properties
- Binded functions that return or accept references to list types no longer make copies of the list, but use opaque references to the original list.
- Improved error handling in rendering and draw functions.
- Rendering and draw operations now properly cull objects outside the current viewport; zero-opacity objects are also culled.
- Moved `math` functions to `Vec2` class methods:
    - `math.scale_to_length` -> `Vec2.scaled_to_length`
    - `math.normalize` -> `Vec2.normalized`
    - `math.rotate` -> `Vec2.rotated`

### Removed
- Removed `<`, `>`, `<=`, `>=` operators from `Vec2` class due to ambiguity with element-wise comparisons.
- Removed `from_polar` function from the `math` submodule (see "Changed" for other math functions).

## [1.4.2] - 2025-12-21

### Added
- `renderer.set_target(target: Texture)` function for setting/unsetting render target textures.
- `TextureAccess` enum for specifying texture access modes (STATIC/TARGET).
- `TextureScaleMode` enum for specifying texture scaling modes (LINEAR/NEAREST/PIXEL_ART).
- New `Texture` constructor that only requires size for creating render target textures.
- `renderer.set_default_scale_mode(scale_mode: TextureScaleMode)` and `renderer.get_default_scale_mode()` functions for managing default texture scale modes.

### Changed
- `renderer.get_res()` renamed to `renderer.get_target_resolution()`.
- `window.create(title, resolution, scaled)` parameters changed to `window.create(title, size)`.
- All `Texture` constructors updated to have `TextureAccess` and `TextureScaleMode` parameters.

### Removed
- Removed `AnimationController.texture` property.
- Removed unused `Animation` struct binding.
- Removed `file_path` parameter from `AnimationController.load_sprite_sheet` method.
