# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) when possible.

## [1.7.1] - TBA

### Added
- New `renderer.draw_9slice` function for drawing 9-slice textures.
- New `ui` submodule for designing and rendering user interfaces:
  - `Direction` enum for specifying layout direction.
  - `Align` enum for specifying alignment within containers.
  - `Style` class for defining reusable style properties.
  - `root`, `stack`, `row`, and `column` container functions for structuring UI elements.
  - `button`, `label`, and `image` functions for common UI elements.
- Physics `World` class now has a `debug_draw` method with extra configurable options for visualizing physics bodies and joints.
- New `Polygon` constructor for creating regular polygons by specifying the number of sides and radius.
- Added `restart` method to the `Timer` class for resetting and starting the timer in one call.
- `Tween` has a new `current_pos` read-only property for getting the current interpolated position.
- `fx.Effect` class now has a `clone` method for creating a copy of an effect with the same properties.
- The `pykraken` package now comes with a cli tool for:
  - Building exe files from Python scripts (with PyInstaller as a dependency)
  - Baking HLSL scripts into SPV, MSL, and DXIL shaders.
- `renderer` submodule now has a `set_render_backend` function for manually setting the rendering backend instead of relying on automatic selection.
- New `RenderBackend` enum for specifying rendering backends.
- `camera.unset()` function for unsetting the active camera.

### Changed
- Renamed `Align` enum to `TextAlign` for specificity.
- Physics `World` constructor's gravity parameter is now optional and defaults to zero gravity.
- `fx.Effect` objects are now stateful, single-use objects to be consumed by an `Orchestrator` timeline, rather than reusable templates.
- Rename `ShaderState` class to `Shader` for simplicity.

### Removed
- Physics `Body` types no longer have a `debug_draw` method.
- `Tween` class no longer has a `step` method as timing is handled by the engine.

### Fixed
- `Orchestrator` objects were copied upon calling `parallel` and `then` methods, causing effects to not be added to the original orchestrator timeline. Now returns reference to the original orchestrator.
- Some images wouldn't render when drawn while a shader was binded. This was due to unpredictable pixel formats when
loading images that shaders didn't like. Solved by forcing an RGBA32 format on all loaded images.
- *Pyinstaller* builds were broken since moving to Nanobind due to different naming conventions. They've been fixed and tested to work again.
- Fixed bug with tilemaps regarding tile id's not being mapped correctly to their tileset, causing wrong tiles to be drawn.
- Isometric, hexagonal, and staggered maps render correctly now.
- Tile rotation and flipping bug fixed.
- Fixed colorkeyed PixelArray → Texture sometimes not retaining the colorkey transparency.

## [1.7.0] - 2026-04-07

### Added
- New `diameter` and edge position properties for the `Circle` class; `left`, `right`, `top`, and `bottom`.
- More `Line` methods:
  - `moved` for non-mutating version of `move`.
  - `get_midpoint` for the line's midpoint.
  - `get_perpendicular` for a line's perpendicular bisector.
  - `get_angle` for the line's angle in radians.
  - `get_closest_point` for finding the closest point on the line to a given point.
- More `Polygon` methods:
  - `rotated` for non-mutating version of `rotate`.
  - `scaled_by` for non-mutating version of `scale_by`.
- More `Rect` methods and properties:
  - `pos` property as a clearer alias for `top_left`.
  - `moved` for non-mutating version of `move`.
  - `clamped` for a non-mutating version of `clamp`.
  - `scaled_by` for non-mutating version of `scale_by`.
  - `scaled_to` for non-mutating version of `scale_to`.
  - `get_corners` for getting the positions of all four corners as a list of `Vec2` objects.
  - `get_edges` for getting the edges of the rectangle as a list of `Line` objects.
- New `renderer` functions for more advanced configuration:
  - `unset_virtual_resolution()` for unsetting a previously set virtual resolution and returning to using the actual render target resolution.
  - `get_virtual_resolution()` for getting the currently set virtual resolution.
  - `get_output_resolution()` for getting the actual output resolution of the renderer.

### Changed
- `Polygon`'s `transform` method renamed to `move` and `scale` methods renamed to `scale_by` for consistency with other shapes.
- `renderer` submodule's `set_present_resolution` function renamed to `set_virtual_resolution` for clarity.
- `on_floor`, `on_ceiling`, and `on_wall` properties of `CharacterBody` changed from read-only properties to regular methods: `is_on_floor()`, `is_on_ceiling()`, and `is_on_wall()`.

### Removed
- `line` and `rect` submodules removed in favor of non-mutating methods in the `Line` and `Rect` classes.

## [1.6.5] - 2026-03-31

### Added
- New `renderer.Batcher` class for batch draw memory preallocation and management.
- When `debug` mode is enabled, the engine now attempts to provide more detailed error messages and stack traces for segfaults using the `faulthandler` module.
- Log warning about excessive texture swapping in a single frame when using Direct3D 12.
- The list-based `renderer.draw_batch` function now accepts an optional list of `Rect` objects for per-instance clipping.
- About the ndarray-based `renderer.draw_batch` function:
  - 9-column arrays now accepted for per-instance clipping (x, y, width, height).
  - Optional `Batcher` parameter for using preallocated memory for batch drawing, significantly improving performance.
  - Manually batch draw all transformed textures instead of relying on SDL's backend, improving performance.

### Changed
- Vulkan is now the default rendering backend on all platforms, falling back to the system's default graphics API if Vulkan is not supported.
- Functions with parameters for file or directory paths now accept `PathLike` objects in addition to strings for improved flexibility and type safety.

### Fixed
- When drawing multiple different textures excessively within a single frame (swapping) on Windows, the engine would segfault likely due to descriptor heap exhaustion on Direct3D 12. The engine now uses Vulkan on Windows by default, if available, which in turn also improves performance.
- MP3 files were not loading due to a missing decoder in the backend. Fix should also encompass FLAC and OGG files.

## [1.6.4] - 2026-03-23

### Added
- `CharacterBody` additions:
  - `MotionMode` enum (`GROUNDED`, `FLOATING`) and corresponding `motion_mode` property.
  - Movement properties: `max_speed`, `acceleration`, `friction`, `stop_speed`, and `air_steer`.
  - Collision state properties: `on_floor`, `on_ceiling`, and `on_wall`.
  - Shape property: `capsule_shape`.
- Math & Vectors:
  - `math.move_toward(current, target, max_delta)` function.
  - `Vec2`: Added in-place rounding methods (`floor`, `ceil`, `round`) and surface sliding methods (`slide`, `slid`).

### Changed
- `Map` and `PixelArray` `set_at` / `get_at` methods now require two integers (x, y) instead of a `Vec2` for improved type safety.

### Fixed
- Passing an empty rect to `renderer.read_pixels` now correctly reads the entire render target instead of crashing.
- `CharacterBody.move_and_slide` now uses Box2D's Mover API, utilizing a geometric solver to eliminate snagging on floor seams and provide smoother sliding along complex surfaces.
- `Event.type` now returns an `EventType` enum type instead of an integer (for built-in event types; user events still return integers).

### Removed
- `CharacterBody` removals:
  - `floor_max_angle` and `floor_snap_distance` properties.
  - `is_on_floor()`, `is_on_ceiling()`, and `is_on_wall()` methods (replaced by the new properties mentioned in "Added").

## [1.6.3] - 2026-03-17

### Fixed
- Tilemap objects with polygon shapes were not being transformed to their correct world positions, causing them to render and collide in the wrong place.

## [1.6.2] - 2026-03-17

### Added
- New `Vec2` non-mutating methods: `.floored()`, `.ceiled()`, and `.rounded()` for coordinate manipulation.
- `Rect` constructor for accepting only a `Vec2` size parameter.
- New `draw_batch` function for efficiently drawing a texture with many varying transforms.
    - Comes with an override for passing an ndarray of transforms for even faster drawing - good for particle sims.
- `Texture.get_rect()` method for getting the dimensions of a texture as a `Rect` object.
- Debug info messages now show GPU driver information.
- Some more pytest scripts for important functions and objects.
- New `renderer.set_present_resolution` function for setting the resolution at which the final rendered image is presented to the screen.
- `math.DEG2RAD` and `math.RAD2DEG` constants for converting between degrees and radians.
- Added optional `handle_close` parameter to `window.create` for whether the window should handle the window quit event and close itself, or leave it to the user to handle.

### Changed
- Migrated the Python binding layer from `pybind11` to `nanobind` for improved performance and smaller binary sizes.
    - In my testing, migrating the same engine API from pybind11 to nanobind increased transform-heavy sprite throughput by about 8–9× under the same Python script and workload.
- Wheels now build against the Python 3.12 stable ABI, allowing a single binary to work across future Python versions.
- Renamed `EasingAnimation` class to `Tween` to follow common terminology.
- `renderer.get_target_resolution` function renamed to `renderer.get_current_resolution` for clarity.

### Removed
- `math.to_deg()` and `math.to_rad()` functions have been removed in favor of `math.DEG2RAD` and `math.RAD2DEG` constants for conversion.
- Removed lesser used `renderer.clear` function that accepted 4 separate color components.

### Fixed
- `input.get_direction()` no longer stacks redundant bindings on the same axis, which caused biased directions after normalization. It also now preserves partial analog stick deflection instead of always normalizing to a unit vector.
- `input.get_axis()` no longer double-negates analog values for negative-direction bindings.
- `input.is_pressed()`, `input.is_just_pressed()`, and `input.is_just_released()` now respect the gamepad slot specified in each `InputAction`.
- A lot of physics bindings didn't have docstrings, now they do.
- `Color.hex` property getter was writing raw int bytes instead of hex-formatted integers, producing invalid UTF-8 strings.
- `log` submodule functions were still logging even when disabled.

## [1.6.1] - 2026-02-15

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
