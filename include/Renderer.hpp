#pragma once

#include <SDL3/SDL.h>

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <optional>
#include <vector>

#include "Color.hpp"
#include "Rect.hpp"
#include "Transform.hpp"
#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Texture;
class PixelArray;

enum class RenderBackend
{
    Auto,
    Legacy,
    Vulkan,
    Metal,
    Direct3d12,
};

namespace renderer
{

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

void _init(SDL_Window* window, int width, int height);
void _quit();
SDL_Renderer* _get();
SDL_GPUDevice* _getGPUDevice();
bool _primaryActive();

void setRenderBackend(RenderBackend backend);

void clear(const Color& color = {});
void present();

void setVirtualResolution(int width, int height);
void unsetVirtualResolution();

Vec2 getVirtualScale();
Vec2 getVirtualResolution();
Vec2 getCurrentResolution();
Vec2 getOutputResolution();

PixelArray readPixels(const Rect& src = {});

void setDefaultFilterMode(FilterMode filter);
FilterMode getDefaultFilterMode();

void setTarget(const Texture* target);

void draw(
    const Texture& texture, const Transform& transform = {}, const Vec2& anchor = Anchor::TOP_LEFT,
    const Vec2& pivot = Anchor::CENTER
);

void draw(
    const Texture& texture, Rect dst, double angle = 0.0, const Vec2& pivot = Anchor::CENTER
);

void draw9Slice(
    const Texture& texture, const Rect& dst, const Rect& slice,
    const Vec2& anchor = Anchor::TOP_LEFT, const Vec2& pivot = Anchor::CENTER
);

void drawBatch(
    const Texture& texture, const std::vector<Transform>& transforms,
    const Vec2& anchor = Anchor::TOP_LEFT, const Vec2& pivot = Anchor::CENTER,
    const std::optional<std::vector<Rect>>& clipRects = std::nullopt
);

}  // namespace renderer
}  // namespace kn
