set(KRAKEN_CORE_SOURCES
  # Animation
  src/animation/animation_controller.cpp
  src/animation/ease.cpp
  src/animation/orchestrator.cpp

  # Audio
  src/audio/mixer.cpp

  # Core services
  src/core/constants.cpp
  src/core/log.cpp
  src/core/time.cpp

  # Geometry
  src/geometry/capsule.cpp
  src/geometry/circle.cpp
  src/geometry/collision.cpp
  src/geometry/line.cpp
  src/geometry/polygon.cpp
  src/geometry/rect.cpp

  # Graphics and windowing
  src/graphics/camera.cpp
  src/graphics/color.cpp
  src/graphics/draw.cpp
  src/graphics/font.cpp
  src/graphics/mask.cpp
  src/graphics/pixel_array.cpp
  src/graphics/renderer.cpp
  src/graphics/shaders.cpp
  src/graphics/text.cpp
  src/graphics/texture.cpp
  src/graphics/viewport.cpp
  src/graphics/window.cpp

  # Input
  src/input/event.cpp
  src/input/gamepad.cpp
  src/input/input.cpp
  src/input/key.cpp
  src/input/mouse.cpp

  # Math
  src/math/math.cpp
  src/math/transform.cpp

  # Physics
  src/physics/world.cpp
  src/physics/bodies/body.cpp
  src/physics/bodies/character_body.cpp
  src/physics/bodies/rigid_body.cpp
  src/physics/bodies/static_body.cpp
  src/physics/joints/distance_joint.cpp
  src/physics/joints/filter_joint.cpp
  src/physics/joints/joint.cpp
  src/physics/joints/motor_joint.cpp
  src/physics/joints/mouse_joint.cpp
  src/physics/joints/prismatic_joint.cpp
  src/physics/joints/revolute_joint.cpp
  src/physics/joints/weld_joint.cpp
  src/physics/joints/wheel_joint.cpp

  # Tile maps and UI
  src/tilemap/tile_map.cpp
  src/ui/ui.cpp
)

file(GLOB KRAKEN_PYTHON_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/*.cpp"
)

set(KRAKEN_BAKER_SOURCES src/tools/shader_baker.cpp)
