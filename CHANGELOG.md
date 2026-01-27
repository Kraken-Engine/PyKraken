# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) when possible.

## [1.5.1] - No Release Date Yet

### Added
- Added `Vec2.ZERO`, `Vec2.UP`, `Vec2.DOWN`, `Vec2.LEFT`, and `Vec2.RIGHT` constants for common vector directions.
- Added the ability to multiply two `Vec2` objects element-wise using the `*` operator.
- Added docstrings to every enum and their values, as well as to any properties of tilemap-related objects that were missing them.
- New `transform` submodule with `compose` and `compose_chain` functions for parenting transforms.
- New `draw.circles` function for drawing multiple circles in a single call (~3.5x faster than multiple `draw.circle` calls in testing).
- Added `texture.clip_area` property for getting/setting the texture's clipping area.

### Changed
- `Vec2` boolean conversion now checks for both components being exactly zero, rather than using a tolerance.
- `fx.move_to` function now requires a `Vec2` for the `pos` parameter instead of accepting an optional argument of any object type.
- Greatly improved quality and performance of `draw.circle` by switching to a GPU-based rendering approach.
- Animation controller's `load_sprite_sheet` method renamed to `add_sheet` and `clip` property renamed to `frame_area`.
- `renderer.draw` function now accepts optional `anchor` and `pivot` parameters for specifying the drawing anchor and pivot points, both of `Vec2` type.
- Both text and pixel array drawing functions now accept an optional `anchor` parameter of `Vec2` type for specifying the drawing anchor point.
- `Anchor` enum changed to a class with static constants for common anchor points represented as `Vec2` objects.

### Removed
- Implicit conversians from sequences to `Vec2`, `Rect`, `Line`, `Color`, and `PolarCoordinate` have been removed for better type safety. Use explicit constructors instead.
- Removed `src` parameter from `renderer.draw` function.
- Removed `clip` attribute from `Sprite` class.
- `Transform` class no longer contains `pivot`, `size`, or `anchor` attributes.

## [1.5.0] - 2025-12-31

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
