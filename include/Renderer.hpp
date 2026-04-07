#pragma once

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>

#include <memory>
#include <optional>
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
class Batcher;

void _bind(nb::module_& module);
void _init(SDL_Window* window, int width, int height);
void _quit();
SDL_Renderer* _get();
SDL_GPUDevice* _getGPUDevice();
bool _primaryActive();

void clear(const Color& color = {});
void present();

void setVirtualResolution(int width, int height);
void unsetVirtualResolution();

Vec2 getVirtualScale();
Vec2 getVirtualResolution();
Vec2 getCurrentResolution();
Vec2 getOutputResolution();

std::unique_ptr<PixelArray> readPixels(const Rect& src = {});

void setDefaultScaleMode(TextureScaleMode scaleMode);
TextureScaleMode getDefaultScaleMode();

void setTarget(const Texture* target);

void draw(
    const Texture& texture, const Transform& transform = {}, const Vec2& anchor = Anchor::TOP_LEFT,
    const Vec2& pivot = Anchor::CENTER
);

void draw(const Texture& texture, Rect dst);

void draw9Slice(
    const Texture& texture, Rect dst, const Rect& slice, const Vec2& anchor = Anchor::TOP_LEFT,
    const Vec2& pivot = Anchor::CENTER
);

void drawBatch(
    const Texture& texture, const std::vector<Transform>& transforms,
    const Vec2& anchor = Anchor::TOP_LEFT, const Vec2& pivot = Anchor::CENTER,
    const std::optional<std::vector<Rect>>& clipRects = std::nullopt
);

void drawBatchNDArray(
    const Texture& texture,
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr,
    const Vec2& anchor = Anchor::TOP_LEFT, const Vec2& pivot = Anchor::CENTER,
    Batcher* batcher = nullptr
);

class Batcher
{
  public:
    Batcher() = default;
    ~Batcher() = default;

    void preallocate(size_t nSprites);
    void free();

  private:
    friend void drawBatchNDArray(
        const Texture& texture,
        nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr,
        const Vec2& anchor, const Vec2& pivot, Batcher* batcher
    );

    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;
};

}  // namespace renderer
}  // namespace kn
