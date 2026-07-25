cmake_minimum_required(VERSION 3.24)

project({{PROJECT_IDENTIFIER}} LANGUAGES CXX)

include(FetchContent)

set(KRAKEN_BUILD_PYTHON OFF CACHE BOOL "" FORCE)
set(KRAKEN_INSTALL OFF CACHE BOOL "" FORCE)
set(KRAKEN_DEPENDENCIES BUNDLED CACHE STRING "" FORCE)

FetchContent_Declare(
  Kraken
  GIT_REPOSITORY https://github.com/Kraken-Engine/PyKraken.git
  GIT_TAG {{KRAKEN_TAG}}
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Kraken)

add_executable({{PROJECT_IDENTIFIER}} src/main.cpp)
target_link_libraries({{PROJECT_IDENTIFIER}} PRIVATE Kraken::Kraken)
