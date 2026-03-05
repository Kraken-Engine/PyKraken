#pragma once

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>

#include <memory>
#include <vector>

#include "Color.hpp"
#include "Rect.hpp"
#include "Transform.hpp"
#include "_globals.hpp"

namespace nb = nanobind;

namespace kn
{
class Texture;
class PixelArray;
enum class TextureScaleMode;

namespace renderer
{
void _bind(nb::module_& module);
void _init(SDL_Window* window, int width, int height);
void _quit();
SDL_Renderer* _get();
SDL_GPUDevice* _getGPUDevice();

void clear(const Color& color = {});
void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
void present();
Vec2 getTargetResolution();
std::unique_ptr<PixelArray> readPixels(const Rect& src = {});

void setDefaultScaleMode(TextureScaleMode scaleMode);
TextureScaleMode getDefaultScaleMode();

void setTarget(const std::shared_ptr<Texture>& target);

void draw(
    const Texture& texture, const Transform& transform = {}, const Vec2& anchor = Anchor::TOP_LEFT,
    const Vec2& pivot = Anchor::CENTER
);

void drawBatch(
    const Texture& texture, const std::vector<Transform>& transforms,
    const Vec2& anchor = Anchor::TOP_LEFT, const Vec2& pivot = Anchor::CENTER
);
}  // namespace renderer
}  // namespace kn
