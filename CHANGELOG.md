# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) when possible.

## [1.4.2] - 2025-12-21

### Added

- `renderer.set_target(target: Texture)` function for setting/unsetting render target textures
- `TextureAccess` enum for specifying texture access modes (STATIC/TARGET)
- `TextureScaleMode` enum for specifying texture scaling modes (LINEAR/NEAREST/PIXEL_ART)
- New `Texture` constructor that only requires size for creating render target textures
- `renderer.set_default_scale_mode(scale_mode: TextureScaleMode)` and `renderer.get_default_scale_mode()` functions for managing default texture scale modes

### Changed

- `renderer.get_res()` renamed to `renderer.get_target_resolution()`
- `window.create(title, resolution, scaled)` parameters changed to `window.create(title, size)`
- All `Texture` constructors updated to have `TextureAccess` and `TextureScaleMode` parameters

### Deprecated

- Removed `AnimationController.texture` property
- Removed unused `Animation` struct binding
- Removed `file_path` parameter from `AnimationController.load_sprite_sheet` method
