cmake_minimum_required(VERSION 3.18.2...3.30)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED) # PyBind11
find_package(SDL3 REQUIRED) # SDL3 core
find_package(SDL3_image REQUIRED) # SDL3 image

pybind11_add_module(_core MODULE
  src/_core.cpp
  src/gfx/SDL3_gfxPrimitives.cpp
  src/gfx/SDL3_rotozoom.cpp
  src/camera.cpp
  src/circle.cpp
  src/color.cpp
  src/constants.cpp
  src/draw.cpp
  src/ease.cpp
  src/event.cpp
  src/gamepad.cpp
  src/input.cpp
  src/key.cpp
  src/line.cpp
  src/math.cpp
  src/mixer.cpp
  src/mouse.cpp
  src/pixel_array.cpp
  src/polygon.cpp
  src/rect.cpp
  src/renderer.cpp
  src/texture.cpp
  src/time.cpp
  src/transform.cpp
  src/window.cpp
  WITH_SOABI
)
target_link_libraries(_core PRIVATE pybind11::headers SDL3::SDL3 SDL3_image::SDL3_image)

target_include_directories(_core PRIVATE
  ${SDL3_INCLUDE_DIRS}
  ${SDL3_image_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS _core DESTINATION pykraken)
