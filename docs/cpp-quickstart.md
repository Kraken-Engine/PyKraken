# Using Kraken from C++

Kraken is a C++20 game framework with optional Python bindings. C++ projects can build Kraken and
all of its dependencies from source, or use a prebuilt SDK published with Kraken releases.

## Create a project

The Kraken CLI is included in the `kraken-engine` Python package:

```bash
pip install kraken-engine
kraken init my-game --cpp
cd my-game
cmake --preset dev
cmake --build --preset dev
```

The generated project pins the matching Kraken release and uses the bundled source build. No
vcpkg installation or dependency manifest is required. The first build downloads and compiles the
dependency stack, so it takes longer than later builds.

Create the rendering demo instead of the minimal window:

```bash
kraken init my-game --cpp --demo
```

## Use a prebuilt SDK

Release SDKs avoid compiling Kraken and its dependencies locally:

```bash
kraken init my-game --cpp --sdk
cd my-game
cmake --preset dev
cmake --build --preset dev
```

The CLI downloads the SDK matching its own version and the current machine into
`.kraken/sdk`. SDK projects use the standard CMake package interface:

```cmake
find_package(KrakenEngine CONFIG REQUIRED)
target_link_libraries(my_game PRIVATE Kraken::Kraken)
```

Prebuilt SDKs are published for:

- macOS ARM64
- Windows x64
- Windows ARM64, initially as an experimental build

The SDK is currently an optimized Release build. The bundled source route remains available on
other architectures and when a different compiler or build configuration is needed.

## Requirements

Both routes require:

- A C++20 compiler
- CMake 3.24 or newer

The bundled source route also requires Git and internet access during its first configuration.
Using the `kraken` initializer currently requires Python 3.12 or newer, but the generated C++
project does not use Python.

## Manual bundled-source setup

Projects do not have to use the CLI. This is the complete CMake integration:

```cmake
cmake_minimum_required(VERSION 3.24)

project(MyKrakenGame LANGUAGES CXX)

include(FetchContent)

set(KRAKEN_BUILD_PYTHON OFF CACHE BOOL "" FORCE)
set(KRAKEN_INSTALL OFF CACHE BOOL "" FORCE)
set(KRAKEN_DEPENDENCIES BUNDLED CACHE STRING "" FORCE)

FetchContent_Declare(
  Kraken
  GIT_REPOSITORY https://github.com/Kraken-Engine/PyKraken.git
  GIT_TAG v1.7.4
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Kraken)

add_executable(my_game src/main.cpp)
target_link_libraries(my_game PRIVATE Kraken::Kraken)
```

Configure and build normally:

```bash
cmake -S . -B build
cmake --build build
```

`Kraken::Kraken` propagates the public headers, C++20 requirement, and dependency usage
requirements to the game target.

## Minimal program

```cpp
#include <kraken/KrakenEngine.hpp>

int main()
{
    kn::init();
    kn::window::create("My Kraken Game", 1280, 720);

    while (kn::window::isOpen())
    {
        kn::event::poll();
        kn::renderer::clear(kn::Color::BLACK);

        // Update and draw the game here.

        kn::renderer::present();
    }

    kn::quit();
    return 0;
}
```

Individual domains can be included instead of the umbrella header:

```cpp
#include <kraken/graphics/Color.hpp>
#include <kraken/graphics/Renderer.hpp>
#include <kraken/graphics/Window.hpp>
#include <kraken/input/Event.hpp>
```

## Dependency modes

Kraken exposes `KRAKEN_DEPENDENCIES` with three values:

- `BUNDLED`: download and build the complete dependency stack
- `SYSTEM`: require every dependency to be supplied by the toolchain or system
- `AUTO`: preserve the individual `*_VENDORED` options; this is used by Kraken's own builds

`BUNDLED` is recommended for source-based game projects. `SYSTEM` is intended for packagers and
advanced builds.

## Current C++ limitations

- Kraken currently builds as a static native library.
- The prebuilt SDKs currently contain Release libraries.
- The Windows ARM64 SDK remains experimental until its hosted workflow has accumulated coverage.
- The C++ API is evolving and does not yet promise ABI stability between releases.
