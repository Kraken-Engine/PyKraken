#pragma once

#include <SDL3/SDL.h>

#include <optional>
#include <span>
#include <vector>

#include "kraken/core/_globals.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/math/Transform.hpp"

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

enum class BlendMode
{
    Normal = SDL_BLENDMODE_BLEND,
    Additive = SDL_BLENDMODE_ADD,
    Multiply = SDL_BLENDMODE_MUL,
};

struct DrawOptions
{
    std::optional<Rect> source;
    Color tint{255, 255, 255, 255};
    float alpha = 1.0f;
    bool flipH = false;
    bool flipV = false;
    BlendMode blend = BlendMode::Normal;
};

namespace renderer
{
class Batcher;

void _init(SDL_Window* window, int width, int height);
void _quit();
SDL_Renderer* _get();
SDL_GPUDevice* _getGPUDevice();
bool _primaryActive();

// Internal helpers shared with the Python draw wrappers.
Rect _validateDrawOptions(const Texture& texture, const DrawOptions& options);
void _applyDrawOptions(const Texture& texture, const DrawOptions& options);

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
    const Vec2& pivot = Anchor::CENTER, const DrawOptions& options = {}
);

void draw(
    const Texture& texture, Rect dst, double angle = 0.0, const Vec2& pivot = Anchor::CENTER,
    const DrawOptions& options = {}
);

void draw9Slice(
    const Texture& texture, const Rect& dst, const Rect& slice, const DrawOptions& options = {}
);

// Contiguous rows: [x, y, (angle), (scale or scale_x, scale_y),
// (source_x, source_y, source_w, source_h)].
// Supported column counts: 2, 3, 4, 5, 9.
// Submits geometry, splitting only on per-instance blend-mode changes.
void drawBatch(
    const Texture& texture, std::span<const double> data, uint8_t columns,
    Batcher* batcher = nullptr, const DrawOptions& options = {},
    const Vec2& anchor = Anchor::TOP_LEFT, const Vec2& pivot = Anchor::CENTER,
    std::span<const DrawOptions> instanceOptions = {}, std::span<const Rect> clipRects = {}
);

class Batcher
{
  public:
    // Reusable CPU geometry buffers; contains no renderer/GPU resources.
    void preallocate(size_t nSprites);
    void free();

  private:
    struct Run
    {
        BlendMode blend;
        int firstIndex;
        int indexCount;
    };

    std::vector<SDL_Vertex> m_vertices;
    std::vector<int> m_indices;
    std::vector<Run> m_runs;

    friend void drawBatch(
        const Texture&, std::span<const double>, uint8_t, Batcher*, const DrawOptions&, const Vec2&,
        const Vec2&, std::span<const DrawOptions>, std::span<const Rect>
    );
};

inline void draw(const Texture& texture, const Transform& transform, const DrawOptions& options)
{
    draw(texture, transform, Anchor::TOP_LEFT, Anchor::CENTER, options);
}

inline void draw(const Texture& texture, Rect dst, const DrawOptions& options)
{
    draw(texture, dst, 0.0, Anchor::CENTER, options);
}

}  // namespace renderer
}  // namespace kn
