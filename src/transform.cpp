#include "Transform.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Surface.hpp"

#include <gfx/SDL3_rotozoom.h>

namespace transform
{
void _bind(py::module_& module)
{
    auto subTransform = module.def_submodule("transform", "Functions for transforming surfaces");

    subTransform.def("flip", &flip, py::arg("surface"), py::arg("flip_x"), py::arg("flip_y"),
                     R"doc(
Flip a surface horizontally, vertically, or both.

Args:
    surface (Surface): The surface to flip.
    flip_x (bool): Whether to flip horizontally (mirror left-right).
    flip_y (bool): Whether to flip vertically (mirror top-bottom).

Returns:
    Surface: A new surface with the flipped image.

Raises:
    RuntimeError: If surface creation fails.
    )doc");
    subTransform.def("scale_to", &scaleTo, py::arg("surface"), py::arg("size"), R"doc(
Scale a surface to a new exact size.

Args:
    surface (Surface): The surface to scale.
    size (Vec2): The target size as (width, height).

Returns:
    Surface: A new surface scaled to the specified size.

Raises:
    RuntimeError: If surface creation or scaling fails.
    )doc");
    subTransform.def("scale_by", py::overload_cast<const Surface&, double>(&scaleBy),
                     py::arg("surface"), py::arg("factor"), R"doc(
Scale a surface by a given factor.

Args:
    surface (Surface): The surface to scale.
    factor (float): The scaling factor (must be > 0). Values > 1.0 enlarge,
                   values < 1.0 shrink the surface.

Returns:
    Surface: A new surface scaled by the specified factor.

Raises:
    ValueError: If factor is <= 0.
    RuntimeError: If surface creation or scaling fails.
    )doc");
    subTransform.def("rotate", &rotate, py::arg("surface"), py::arg("angle"), R"doc(
Rotate a surface by a given angle.

Args:
    surface (Surface): The surface to rotate.
    angle (float): The rotation angle in degrees. Positive values rotate clockwise.

Returns:
    Surface: A new surface containing the rotated image. The output surface may be
            larger than the input to accommodate the rotated image.

Raises:
    RuntimeError: If surface rotation fails.
    )doc");
    subTransform.def("box_blur", &boxBlur, py::arg("surface"), py::arg("radius"),
                     py::arg("repeat_edge_pixels") = true, R"doc(
Apply a box blur effect to a surface.

Box blur creates a uniform blur effect by averaging pixels within a square kernel.
It's faster than Gaussian blur but produces a more uniform, less natural look.

Args:
    surface (Surface): The surface to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the surface bounds. Defaults to True.

Returns:
    Surface: A new surface with the box blur effect applied.

Raises:
    RuntimeError: If surface creation fails during the blur process.
    )doc");
    subTransform.def("gaussian_blur", &gaussianBlur, py::arg("surface"), py::arg("radius"),
                     py::arg("repeat_edge_pixels") = true, R"doc(
Apply a Gaussian blur effect to a surface.

Gaussian blur creates a natural, smooth blur effect using a Gaussian distribution
for pixel weighting. It produces higher quality results than box blur but is
computationally more expensive.

Args:
    surface (Surface): The surface to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the surface bounds. Defaults to True.

Returns:
    Surface: A new surface with the Gaussian blur effect applied.

Raises:
    RuntimeError: If surface creation fails during the blur process.
    )doc");
    subTransform.def("invert", &invert, py::arg("surface"), R"doc(
Invert the colors of a surface.

Creates a negative image effect by inverting each color channel (RGB).
The alpha channel is preserved unchanged.

Args:
    surface (Surface): The surface to invert.

Returns:
    Surface: A new surface with inverted colors.

Raises:
    RuntimeError: If surface creation fails.
    )doc");
    subTransform.def("grayscale", &grayscale, py::arg("surface"), R"doc(
Convert a surface to grayscale.

Converts the surface to grayscale using the standard luminance formula:
gray = 0.299 * red + 0.587 * green + 0.114 * blue

This formula accounts for human perception of brightness across different colors.
The alpha channel is preserved unchanged.

Args:
    surface (Surface): The surface to convert to grayscale.

Returns:
    Surface: A new surface converted to grayscale.

Raises:
    RuntimeError: If surface creation fails.
    )doc");
}

std::unique_ptr<Surface> flip(const Surface& surface, const bool flipX, const bool flipY)
{
    SDL_Surface* sdlSurface = surface.getSDL();
    SDL_Surface* flipped = SDL_CreateSurface(sdlSurface->w, sdlSurface->h, SDL_PIXELFORMAT_RGBA32);

    if (!flipped)
        throw std::runtime_error("Failed to create flipped surface.");

    int bpp = SDL_GetPixelFormatDetails(sdlSurface->format)->bytes_per_pixel;

    for (int y = 0; y < sdlSurface->h; ++y)
        for (int x = 0; x < sdlSurface->w; ++x)
        {
            int srcX = flipX ? (sdlSurface->w - 1 - x) : x;
            int srcY = flipY ? (sdlSurface->h - 1 - y) : y;

            Uint8* srcPixel = (Uint8*)sdlSurface->pixels + srcY * sdlSurface->pitch + srcX * bpp;
            Uint8* dstPixel = (Uint8*)flipped->pixels + y * flipped->pitch + x * bpp;

            memcpy(dstPixel, srcPixel, bpp);
        }

    return std::make_unique<Surface>(flipped);
}

std::unique_ptr<Surface> scaleTo(const Surface& surface, const Vec2& size)
{
    SDL_Surface* sdlSurface = surface.getSDL();

    const auto newW = static_cast<int>(size.x);
    const auto newH = static_cast<int>(size.y);

    SDL_Surface* scaled = SDL_CreateSurface(newW, newH, SDL_PIXELFORMAT_RGBA32);
    if (!scaled)
        throw std::runtime_error("Failed to create scaled surface.");

    SDL_Rect dstRect = {0, 0, newW, newH};
    if (!SDL_BlitSurfaceScaled(sdlSurface, nullptr, scaled, &dstRect, SDL_SCALEMODE_NEAREST))
    {
        SDL_DestroySurface(scaled);
        throw std::runtime_error("SDL_BlitScaled failed: " + std::string(SDL_GetError()));
    }

    return std::make_unique<Surface>(scaled);
}

std::unique_ptr<Surface> scaleBy(const Surface& surface, const double factor)
{
    if (factor <= 0.0)
        throw std::invalid_argument("Scale factor must be a positive value.");

    return scaleTo(surface, surface.getSize() * factor);
}

std::unique_ptr<Surface> scaleBy(const Surface& surface, const Vec2& factor)
{
    if (factor <= 0.0)
        throw std::invalid_argument("Scale factor must be a positive value.");

    const Vec2 originalSize = surface.getSize();
    return scaleTo(surface, {originalSize.x * factor.x, originalSize.y * factor.y});
}

std::unique_ptr<Surface> rotate(const Surface& surface, const double angle)
{
    SDL_Surface* sdlSurface = surface.getSDL();
    SDL_Surface* rotated =
        rotozoomSurface(sdlSurface, angle, 1.0, SMOOTHING_OFF); // rotate, don't scale
    if (!rotated)
        throw std::runtime_error("Failed to rotate surface.");

    return std::make_unique<Surface>(rotated);
}

std::unique_ptr<Surface> boxBlur(const Surface& surface, const int radius,
                                 const bool repeatEdgePixels)
{
    SDL_Surface* src = surface.getSDL();
    const int width = src->w;
    const int height = src->h;

    SDL_Surface* temp = SDL_CreateSurface(width, height, src->format);
    SDL_Surface* result = SDL_CreateSurface(width, height, src->format);
    if (!temp || !result)
        throw std::runtime_error("Failed to create surfaces for box blur.");

    auto clamp = [](const int v, const int low, const int high) -> int
    { return std::max(low, std::min(v, high)); };

    auto* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* tmpPx = static_cast<uint32_t*>(temp->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const int diameter = radius * 2 + 1;
    const auto srcDetails = SDL_GetPixelFormatDetails(src->format);
    const auto tmpDetails = SDL_GetPixelFormatDetails(temp->format);
    const auto resDetails = SDL_GetPixelFormatDetails(result->format);

    // Horizontal
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            uint8_t r = 0, g = 0, b = 0, a = 0;
            for (int dx = -radius; dx <= radius; ++dx)
            {
                int sx = repeatEdgePixels ? clamp(x + dx, 0, width - 1) : x + dx;
                if (sx < 0 || sx >= width)
                    continue;

                uint8_t pr, pg, pb, pa;
                SDL_GetRGBA(srcPx[y * width + sx], srcDetails, nullptr, &pr, &pg, &pb, &pa);
                r += pr;
                g += pg;
                b += pb;
                a += pa;
            }
            r /= diameter;
            g /= diameter;
            b /= diameter;
            a /= diameter;
            tmpPx[y * width + x] = SDL_MapRGBA(tmpDetails, nullptr, r, g, b, a);
        }

    // Vertical
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            uint8_t r = 0, g = 0, b = 0, a = 0;
            for (int dy = -radius; dy <= radius; ++dy)
            {
                int sy = repeatEdgePixels ? clamp(y + dy, 0, height - 1) : y + dy;
                if (sy < 0 || sy >= height)
                    continue;
                uint8_t pr, pg, pb, pa;
                SDL_GetRGBA(tmpPx[sy * width + x], tmpDetails, nullptr, &pr, &pg, &pb, &pa);
                r += pr;
                g += pg;
                b += pb;
                a += pa;
            }
            r /= diameter;
            g /= diameter;
            b /= diameter;
            a /= diameter;
            dstPx[y * width + x] = SDL_MapRGBA(resDetails, nullptr, r, g, b, a);
        }

    SDL_DestroySurface(temp);

    return std::make_unique<Surface>(result);
}

std::unique_ptr<Surface> gaussianBlur(const Surface& surface, int radius, bool repeatEdgePixels)
{
    SDL_Surface* src = surface.getSDL();

    const int w = src->w, h = src->h;
    int diameter = radius * 2 + 1;

    // Build Gaussian kernel (σ = radius/2)
    float sigma = radius > 0 ? (radius / 2.0f) : 1.0f;
    float twoSigmaSq = 2.0f * sigma * sigma;
    float invSigmaRoot = 1.0f / (std::sqrt(2.0f * M_PI) * sigma);
    std::vector<float> kernel(diameter);
    for (int i = 0; i < diameter; ++i)
    {
        int x = i - radius;
        kernel[i] = invSigmaRoot * std::exp(-(x * x) / twoSigmaSq);
    }

    // Normalize
    float sum = 0;
    for (float v : kernel)
        sum += v;
    for (float& v : kernel)
        v /= sum;

    // Create intermediate and output surfaces
    SDL_Surface* temp = SDL_CreateSurface(w, h, src->format);
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!temp)
        throw std::runtime_error("Failed to create temporary surface for gaussian blur.");
    if (!result)
        throw std::runtime_error("Failed to create result surface for gaussian blur.");

    auto clamp = [](int v, int low, int high) -> int { return std::max(low, std::min(v, high)); };
    Uint32* srcPx = static_cast<Uint32*>(src->pixels);
    Uint32* tmpPx = static_cast<Uint32*>(temp->pixels);
    Uint32* dstPx = static_cast<Uint32*>(result->pixels);

    const auto srcDetails = SDL_GetPixelFormatDetails(src->format);
    const auto tmpDetails = SDL_GetPixelFormatDetails(temp->format);
    const auto resDetails = SDL_GetPixelFormatDetails(result->format);

    // Horizontal pass
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            float fr = 0, fg = 0, fb = 0, fa = 0;
            for (int k = 0; k < diameter; ++k)
            {
                int sx = x + (k - radius);
                if (repeatEdgePixels)
                    sx = clamp(sx, 0, w - 1);
                if (sx < 0 || sx >= w)
                    continue;

                Uint8 pr, pg, pb, pa;
                SDL_GetRGBA(srcPx[y * w + sx], srcDetails, nullptr, &pr, &pg, &pb, &pa);
                fr += pr * kernel[k];
                fg += pg * kernel[k];
                fb += pb * kernel[k];
                fa += pa * kernel[k];
            }
            tmpPx[y * w + x] =
                SDL_MapRGBA(tmpDetails, nullptr, static_cast<Uint8>(fr), static_cast<Uint8>(fg),
                            static_cast<Uint8>(fb), static_cast<Uint8>(fa));
        }

    // Vertical pass
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x)
        {
            float fr = 0.f, fg = 0.f, fb = 0.f, fa = 0.f;
            for (int k = 0; k < diameter; ++k)
            {
                int sy = y + (k - radius);
                if (repeatEdgePixels)
                    sy = clamp(sy, 0, h - 1);
                if (sy < 0 || sy >= h)
                    continue;
                uint8_t pr, pg, pb, pa;
                SDL_GetRGBA(tmpPx[sy * w + x], tmpDetails, nullptr, &pr, &pg, &pb, &pa);
                fr += pr * kernel[k];
                fg += pg * kernel[k];
                fb += pb * kernel[k];
                fa += pa * kernel[k];
            }
            dstPx[y * w + x] =
                SDL_MapRGBA(resDetails, nullptr, static_cast<Uint8>(fr), static_cast<Uint8>(fg),
                            static_cast<Uint8>(fb), static_cast<Uint8>(fa));
        }

    SDL_DestroySurface(temp);

    return std::make_unique<Surface>(result);
}

std::unique_ptr<Surface> invert(const Surface& surface)
{
    SDL_Surface* src = surface.getSDL();

    const int w = src->w;
    const int h = src->h;

    // Create an output surface matching the source format
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!result)
        throw std::runtime_error("Failed to create result surface for invert.");

    auto srcPx = static_cast<uint32_t*>(src->pixels);
    auto dstPx = static_cast<uint32_t*>(result->pixels);

    auto srcDetails = SDL_GetPixelFormatDetails(src->format);
    auto resDetails = SDL_GetPixelFormatDetails(result->format);

    uint8_t r, g, b, a;
    for (int i = 0; i < w * h; ++i)
    {
        SDL_GetRGBA(srcPx[i], srcDetails, nullptr, &r, &g, &b, &a);
        dstPx[i] = SDL_MapRGBA(resDetails, nullptr, 255 - r, 255 - g, 255 - b, a);
    }

    return std::make_unique<Surface>(result);
}

std::unique_ptr<Surface> grayscale(const Surface& surface)
{
    SDL_Surface* src = surface.getSDL();

    const int w = src->w;
    const int h = src->h;

    // Create output surface with its own memory
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!result)
        throw std::runtime_error("Failed to create surface for grayscale.");

    auto srcPx = static_cast<uint32_t*>(src->pixels);
    auto dstPx = static_cast<uint32_t*>(result->pixels);

    auto srcDetails = SDL_GetPixelFormatDetails(src->format);
    auto resDetails = SDL_GetPixelFormatDetails(result->format);

    uint8_t r, g, b, a;
    for (int i = 0; i < w * h; ++i)
    {
        SDL_GetRGBA(srcPx[i], srcDetails, nullptr, &r, &g, &b, &a);
        auto gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
        dstPx[i] = SDL_MapRGBA(resDetails, nullptr, gray, gray, gray, a);
    }

    return std::make_unique<Surface>(result);
}
} // namespace transform
