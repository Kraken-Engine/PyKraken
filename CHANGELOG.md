# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) when possible.

## [1.6.1] - No Release Date

### Added
- New `mixer` submodule for advanced audio management.
- `AudioPriority` enum for managing hardware track acquisition (MUSIC, UI, SFX).
- Support for polyphonic sound effects via the `Sample` class and `max_polyphony` attribute.
- Priority-based track stealing: high-priority sounds can now interrupt lower-priority ones if the 64-track limit is reached.
- Global master volume control via `mixer.set_master_volume`.
- `draw.ellipses` and `draw.lines` functions for drawing multiple ellipses or lines in a single call.
- Added `is_convex` and `is_concave` methods to the `Polygon` class for checking polygon convexity.
- Added `Circle` default constructor and another accepting just a radius.
- New `physics` submodule with a `World` class, bodies, and joints for basic 2D physics simulation.
    - `Body` abstract class for physics bodies:
        - `RigidBody` for simulating solid objects with mass, velocity, and forces.
        - `CharacterBody` for simulating character-like movement with floor detection and snapping.
        - `StaticBody` for immovable objects that can still collide with other bodies.
    - `Joint` abstract class for the following joint types:
        - `DistanceJoint` for maintaining a fixed distance between two bodies.
        - `FilterJoint` for filtering collisions between two bodies.
        - `MotorJoint` for applying a motor force to maintain a relative position between two bodies.
        - `PrismaticJoint` for allowing relative movement along a specified axis between two bodies.
        - `RevoluteJoint` for allowing relative rotation between two bodies.
        - `MouseJoint` for dragging a body with the mouse cursor.
        - `WeldJoint` for rigidly connecting two bodies together.
        - `WheelJoint` for allowing relative rotation and translation along a specified axis between two bodies.
- `Collision` class for representing collision information between two bodies. Provided via `World.get_collisions()` method.
- `CastHit` class for representing the result of a ray or shape cast in the physics world.
- Added `Capsule` shape class for physics bodies, defined by a line segment and a radius.
- Added `draw.capsule` and `draw.capsules` functions for drawing capsules.
- `get/set_fixed_delta` and `get/set_max_substeps` functions for managing physics stepping parameters.
- `add_fixed_update` and `clear_fixed_updates` functions and `fixed_callback` decorator for registering functions to be called at a fixed interval during the automatic physics update loop.
- Add `Map.tile_layers`, `Map.object_groups`, and `Map.image_layers` properties for easier access to specific layer types.
- Add `Map.get_layer(name)` method for retrieving a layer by name.
- Add `World.from_map_layer(world, layer)` method for creating physics bodies from a tilemap layer.
- `Vec2` can be divided by another `Vec2` element-wise using the `/` operator.
- New `draw.bezier` and `draw.sector` functions for drawing Bezier curves and circular sectors.
- Added `draw.polyline` for drawing connected line segments.
- `world_to_screen` and `screen_to_world` functions/methods for converting between world and screen coordinates.
- `camera.get_active_pos` function for getting the position of the currently active camera.

### Changed
- Refactored the audio backend to use SDL3_mixer.
    - Renamed `Audio` class to `Sample` (for short sound effects).
    - Renamed `AudioStream` class to `Stream` (for long music files).
- Audio loading functions moved to `mixer.load_sample` and `mixer.load_stream`.
- `Stream.looping` is now an RW property instead of just a setter method.
- `Stream` playback position renamed from `current_time` to `playback_pos`.
- Circle drawing (and now ellipses) default segment count reduced from 36 to 24.
- Line drawing thickness can now be a float.
- Rename `Map.layers` to `Map.all_layers`
- `Texture` constructor involving a `Vec2` size parameter changed to accept separate `width` and `height` integer parameters for type safety.
- `AnimationController.add_sheet` method `Vec2` frame size parameter changed to separate `frame_width` and `frame_height` integer parameters for type safety.
- `draw.rect` and `draw.rects` functions now have parameters for corner radii.

### Fixed
- Fixed a bug where textures wouldn't render at all (hopefully). Likely related to the internal SDL2->SDL3 transition.

### Removed
- Removed `rewind` method from audio stream class (use `seek(0)` or restart playback).
- Removed `miniaudio` dependency.
- Removed `SGL_gfx` dependency.
- Due to the new CharacterBody class, I've decided to remove the `Sprite` class as it become redundant.

## [1.6.0] - 2026-01-29

### Added
- Added `Vec2.ZERO`, `Vec2.UP`, `Vec2.DOWN`, `Vec2.LEFT`, and `Vec2.RIGHT` constants for common vector directions.
- Added the ability to multiply two `Vec2` objects element-wise using the `*` operator.
- New `transform` submodule with `compose` and `compose_chain` functions for parenting transforms.
- New `draw.circles` function for drawing multiple circles in a single call (~3.5x faster than multiple `draw.circle` calls in testing).
- New `draw.polygons` function for drawing multiple polygons in a single call.
- Added `texture.clip_area` property for getting/setting the texture's clipping area.
- Added `draw.geometry` function for drawing arbitrary vertex geometry, optionally with a texture.
- New `Vertex` class representing a single vertex with position, color, and texture coordinate attributes.

### Changed
- `Vec2` boolean conversion now checks for both components being exactly zero, rather than using a tolerance.
- `fx.move_to` function now requires a `Vec2` for the `pos` parameter instead of accepting an optional argument of any object type.
- Greatly improved quality and performance of `draw.circle` and `draw.polygon` by switching to a GPU-based rendering approach.
- Animation controller's `load_sprite_sheet` method renamed to `add_sheet` and `clip` property renamed to `frame_area`.
- `renderer.draw` function now accepts optional `anchor` and `pivot` parameters for specifying the drawing anchor and pivot points, both of `Vec2` type.
- Both text and pixel array drawing functions now accept an optional `anchor` parameter of `Vec2` type for specifying the drawing anchor point.
- `Anchor` enum changed to a class with static constants for common anchor points represented as `Vec2` objects.
- Renamed tilemap layer `render` methods to `draw` for consistency with other drawing functions.

### Removed
- Implicit conversians from sequences to `Vec2`, `Rect`, `Line`, `Color`, and `PolarCoordinate` have been removed for better type safety. Use explicit constructors instead.
- Removed `src` parameter from `renderer.draw` function.
- Removed `clip` attribute from `Sprite` class.
- `Transform` class no longer contains `pivot`, `size`, or `anchor` attributes.
- Removed most opaque list types except for tilemap related ones due to... uselessness.

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
