# Using Kraken from C++

Kraken can currently be consumed as a native C++20 static library by building it from source as
part of another CMake project. The recommended integration is `add_subdirectory()`.

Kraken does not yet publish a standalone native SDK or a fully supported installed
`find_package(KrakenEngine)` package. The instructions below describe the source-based workflow
that is supported today.

## Requirements

- A C++20 compiler
- CMake 3.15 or newer
- Git
- [vcpkg](https://github.com/microsoft/vcpkg)
- Ninja or another CMake-supported build tool
- Internet access during the first configuration, while vcpkg and CMake FetchContent obtain
  dependencies

Kraken builds its SDL stack from source by default. Its other native dependencies are resolved
through vcpkg.

## 1. Set up vcpkg

Clone and bootstrap vcpkg if it is not already installed:

```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh -disableMetrics
export VCPKG_ROOT="$HOME/vcpkg"
```

On Windows PowerShell:

```powershell
git clone https://github.com/microsoft/vcpkg.git "$HOME\vcpkg"
& "$HOME\vcpkg\bootstrap-vcpkg.bat" -disableMetrics
$env:VCPKG_ROOT = "$HOME\vcpkg"
```

The `VCPKG_ROOT` environment variable must be set when initially configuring the project.

## 2. Add Kraken to the project

The simplest current layout uses a Git submodule:

```text
my-game/
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── external/
    └── PyKraken/
```

Add Kraken:

```bash
git submodule add https://github.com/Kraken-Engine/PyKraken.git external/PyKraken
git submodule update --init --recursive
```

## 3. Create the CMake project

Create `CMakeLists.txt` in the root of the game project:

```cmake
cmake_minimum_required(VERSION 3.15)

project(MyKrakenGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build only the native Kraken library. Native installation/export is not
# needed when Kraken is embedded with add_subdirectory().
set(KRAKEN_BUILD_PYTHON OFF CACHE BOOL "" FORCE)
set(KRAKEN_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/PyKraken)

add_executable(my_game src/main.cpp)
target_link_libraries(my_game PRIVATE Kraken::Kraken)
```

`Kraken::Kraken` supplies Kraken's public include directory, C++20 requirement, and the usage
requirements of dependencies exposed by its public headers.

## 4. Create a minimal program

Create `src/main.cpp`:

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

Individual domains can also be included without the umbrella header:

```cpp
#include <kraken/graphics/Color.hpp>
#include <kraken/graphics/Renderer.hpp>
#include <kraken/graphics/Window.hpp>
#include <kraken/input/Event.hpp>
```

## 5. Configure and build

Point vcpkg at Kraken's manifest so it installs the dependency versions Kraken currently expects:

```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_MANIFEST_DIR="$PWD/external/PyKraken" \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build
```

For a multi-configuration generator such as Visual Studio:

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake" `
  -DVCPKG_MANIFEST_DIR="$PWD\external\PyKraken"

cmake --build build --config Release
```

The executable will normally be placed in `build/` for a single-configuration generator or
`build/Release/` for Visual Studio.

### Projects that already use a vcpkg manifest

A CMake configure can use only one active vcpkg manifest. If the game already has a root
`vcpkg.json`, merge the dependencies and version constraints from Kraken's `vcpkg.json` into the
game's manifest. Then omit `VCPKG_MANIFEST_DIR` from the configure command.

## Updating Kraken

When Kraken is a submodule:

```bash
git -C external/PyKraken pull
git add external/PyKraken
```

After changing Kraken versions or dependency options, reconfigure the CMake build. Deleting and
recreating the build directory is the simplest way to eliminate stale dependency settings.
Remember to supply the vcpkg toolchain and manifest arguments again if they are not stored in a
CMake preset.

## Current limitations

- Kraken currently supports a static native library build.
- The native build is tested in CI on Linux; the Python wheel builds also compile the engine on
  Windows, macOS, and Linux.
- Native prebuilt binaries are not published.
- Installing Kraken and consuming it with `find_package(KrakenEngine)` is not yet the recommended
  workflow. Vendored static dependencies still need a finalized export strategy.
- Building with `add_subdirectory()` may introduce Kraken's dependency options into the parent
  project's CMake cache.
- The C++ API is still evolving and does not yet promise ABI stability between releases.

For the current project options, see `cmake/KrakenOptions.cmake` in the Kraken source tree.
