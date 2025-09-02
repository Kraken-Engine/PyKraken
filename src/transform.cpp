#include "Math.hpp"
#include "PixelArray.hpp"

#include "Transform.hpp"
#include <gfx/SDL3_rotozoom.h>

namespace kn::transform
{
void _bind(py::module_& module)
{
    auto subTransform =
        module.def_submodule("transform", "Functions for transforming pixel arrays");

    subTransform.def("flip", &flip, py::arg("pixel_array"), py::arg("flip_x"), py::arg("flip_y"),
                     R"doc(
Flip a pixel array horizontally, vertically, or both.

Args:
    pixel_array (PixelArray): The pixel array to flip.
    flip_x (bool): Whether to flip horizontally (mirror left-right).
    flip_y (bool): Whether to flip vertically (mirror top-bottom).

Returns:
    PixelArray: A new pixel array with the flipped image.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc");
    subTransform.def("scale_to", &scaleTo, py::arg("pixel_array"), py::arg("size"), R"doc(
Scale a pixel array to a new exact size.

Args:
    pixel_array (PixelArray): The pixel array to scale.
    size (Vec2): The target size as (width, height).

Returns:
    PixelArray: A new pixel array scaled to the specified size.

Raises:
    RuntimeError: If pixel array creation or scaling fails.
    )doc");
    subTransform.def("scale_by", py::overload_cast<const PixelArray&, double>(&scaleBy),
                     py::arg("pixel_array"), py::arg("factor"), R"doc(
Scale a pixel array by a given factor.

Args:
    pixel_array (PixelArray): The pixel array to scale.
    factor (float): The scaling factor (must be > 0). Values > 1.0 enlarge,
                   values < 1.0 shrink the pixel array.

Returns:
    PixelArray: A new pixel array scaled by the specified factor.

Raises:
    ValueError: If factor is <= 0.
    RuntimeError: If pixel array creation or scaling fails.
    )doc");
    subTransform.def("rotate", &rotate, py::arg("pixel_array"), py::arg("angle"), R"doc(
Rotate a pixel array by a given angle.

Args:
    pixel_array (PixelArray): The pixel array to rotate.
    angle (float): The rotation angle in degrees. Positive values rotate clockwise.

Returns:
    PixelArray: A new pixel array containing the rotated image. The output pixel array may be
            larger than the input to accommodate the rotated image.

Raises:
    RuntimeError: If pixel array rotation fails.
    )doc");
    subTransform.def("box_blur", &boxBlur, py::arg("pixel_array"), py::arg("radius"),
                     py::arg("repeat_edge_pixels") = true, R"doc(
Apply a box blur effect to a pixel array.

Box blur creates a uniform blur effect by averaging pixels within a square kernel.
It's faster than Gaussian blur but produces a more uniform, less natural look.

Args:
    pixel_array (PixelArray): The pixel array to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the pixel array bounds. Defaults to True.

Returns:
    PixelArray: A new pixel array with the box blur effect applied.

Raises:
    RuntimeError: If pixel array creation fails during the blur process.
    )doc");
    subTransform.def("gaussian_blur", &gaussianBlur, py::arg("pixel_array"), py::arg("radius"),
                     py::arg("repeat_edge_pixels") = true, R"doc(
Apply a Gaussian blur effect to a pixel array.

Gaussian blur creates a natural, smooth blur effect using a Gaussian distribution
for pixel weighting. It produces higher quality results than box blur but is
computationally more expensive.

Args:
    pixel_array (PixelArray): The pixel array to blur.
    radius (int): The blur radius in pixels. Larger values create stronger blur.
    repeat_edge_pixels (bool, optional): Whether to repeat edge pixels when sampling
                                        outside the pixel array bounds. Defaults to True.

Returns:
    PixelArray: A new pixel array with the Gaussian blur effect applied.

Raises:
    RuntimeError: If pixel array creation fails during the blur process.
    )doc");
    subTransform.def("invert", &invert, py::arg("pixel_array"), R"doc(
Invert the colors of a pixel array.

Creates a negative image effect by inverting each color channel (RGB).
The alpha channel is preserved unchanged.

Args:
    pixel_array (PixelArray): The pixel array to invert.

Returns:
    PixelArray: A new pixel array with inverted colors.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc");
    subTransform.def("grayscale", &grayscale, py::arg("pixel_array"), R"doc(
Convert a pixel array to grayscale.

Converts the pixel array to grayscale using the standard luminance formula:
gray = 0.299 * red + 0.587 * green + 0.114 * blue

This formula accounts for human perception of brightness across different colors.
The alpha channel is preserved unchanged.

Args:
    pixel_array (PixelArray): The pixel array to convert to grayscale.

Returns:
    PixelArray: A new pixel array converted to grayscale.

Raises:
    RuntimeError: If pixel array creation fails.
    )doc");
}

std::unique_ptr<PixelArray> flip(const PixelArray& pixelArray, const bool flipX, const bool flipY)
{
    const SDL_Surface* sdlSurface = pixelArray.getSDL();
    SDL_Surface* flipped = SDL_CreateSurface(sdlSurface->w, sdlSurface->h, SDL_PIXELFORMAT_RGBA32);

    if (!flipped)
        throw std::runtime_error("Failed to create flipped pixel array.");

    const int bpp = SDL_GetPixelFormatDetails(sdlSurface->format)->bytes_per_pixel;

    for (int y = 0; y < sdlSurface->h; ++y)
        for (int x = 0; x < sdlSurface->w; ++x)
        {
            const int srcX = flipX ? sdlSurface->w - 1 - x : x;
            const int srcY = flipY ? sdlSurface->h - 1 - y : y;

            const uint8_t* srcPixel =
                static_cast<uint8_t*>(sdlSurface->pixels) + srcY * sdlSurface->pitch + srcX * bpp;
            uint8_t* dstPixel =
                static_cast<uint8_t*>(flipped->pixels) + y * flipped->pitch + x * bpp;

            memcpy(dstPixel, srcPixel, bpp);
        }

    return std::make_unique<PixelArray>(flipped);
}

std::unique_ptr<PixelArray> scaleTo(const PixelArray& pixelArray, const Vec2& size)
{
    SDL_Surface* sdlSurface = pixelArray.getSDL();

    const auto newW = static_cast<int>(size.x);
    const auto newH = static_cast<int>(size.y);

    SDL_Surface* scaled = SDL_CreateSurface(newW, newH, SDL_PIXELFORMAT_RGBA32);
    if (!scaled)
        throw std::runtime_error("Failed to create scaled pixel array.");

    const SDL_Rect dstRect = {0, 0, newW, newH};
    if (!SDL_BlitSurfaceScaled(sdlSurface, nullptr, scaled, &dstRect, SDL_SCALEMODE_NEAREST))
    {
        SDL_DestroySurface(scaled);
        throw std::runtime_error("SDL_BlitScaled failed: " + std::string(SDL_GetError()));
    }

    return std::make_unique<PixelArray>(scaled);
}

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, const double factor)
{
    if (factor <= 0.0)
        throw std::invalid_argument("Scale factor must be a positive value.");

    return scaleTo(pixelArray, pixelArray.getSize() * factor);
}

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, const Vec2& factor)
{
    if (factor <= Vec2{})
        throw std::invalid_argument("Scale factor must be a positive value.");

    const Vec2 originalSize = pixelArray.getSize();
    return scaleTo(pixelArray, {originalSize.x * factor.x, originalSize.y * factor.y});
}

std::unique_ptr<PixelArray> rotate(const PixelArray& pixelArray, const double angle)
{
    SDL_Surface* sdlSurface = pixelArray.getSDL();
    SDL_Surface* rotated =
        rotozoomSurface(sdlSurface, angle, 1.0, SMOOTHING_OFF); // rotate, don't scale
    if (!rotated)
        throw std::runtime_error("Failed to rotate pixel array.");

    return std::make_unique<PixelArray>(rotated);
}

std::unique_ptr<PixelArray> boxBlur(const PixelArray& pixelArray, const int radius,
                                    const bool repeatEdgePixels)
{
    const SDL_Surface* src = pixelArray.getSDL();
    const int width = src->w;
    const int height = src->h;

    SDL_Surface* temp = SDL_CreateSurface(width, height, src->format);
    SDL_Surface* result = SDL_CreateSurface(width, height, src->format);
    if (!temp || !result)
        throw std::runtime_error("Failed to create surfaces for box blur.");

    auto clamp = [](const int v, const int low, const int high) -> int
    { return std::max(low, std::min(v, high)); };

    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
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
                const int sx = repeatEdgePixels ? clamp(x + dx, 0, width - 1) : x + dx;
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
                const int sy = repeatEdgePixels ? clamp(y + dy, 0, height - 1) : y + dy;
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

    return std::make_unique<PixelArray>(result);
}

std::unique_ptr<PixelArray> gaussianBlur(const PixelArray& pixelArray, const int radius,
                                         const bool repeatEdgePixels)
{
    const SDL_Surface* src = pixelArray.getSDL();

    const int w = src->w, h = src->h;
    const int diameter = radius * 2 + 1;

    // Build Gaussian kernel (σ = radius/2)
    const float sigma = radius > 0 ? static_cast<float>(radius) / 2.f : 1.f;
    const float twoSigmaSq = 2.f * sigma * sigma;
    const auto invSigmaRoot = static_cast<float>(1.0 / (std::sqrt(2 * M_PI) * sigma));
    std::vector<float> kernel(diameter);
    for (int i = 0; i < diameter; ++i)
    {
        const int x = i - radius;
        kernel[i] = invSigmaRoot * std::exp(-static_cast<float>(x * x) / twoSigmaSq);
    }

    // Normalize
    float sum = 0;
    for (const float v : kernel)
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

    auto clamp = [](const int v, const int low, const int high) -> int
    { return std::max(low, std::min(v, high)); };
    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* tmpPx = static_cast<uint32_t*>(temp->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

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
                fr += static_cast<float>(pr) * kernel[k];
                fg += static_cast<float>(pg) * kernel[k];
                fb += static_cast<float>(pb) * kernel[k];
                fa += static_cast<float>(pa) * kernel[k];
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
                fr += static_cast<float>(pr) * kernel[k];
                fg += static_cast<float>(pg) * kernel[k];
                fb += static_cast<float>(pb) * kernel[k];
                fa += static_cast<float>(pa) * kernel[k];
            }
            dstPx[y * w + x] =
                SDL_MapRGBA(resDetails, nullptr, static_cast<uint8_t>(fr), static_cast<uint8_t>(fg),
                            static_cast<uint8_t>(fb), static_cast<uint8_t>(fa));
        }

    SDL_DestroySurface(temp);

    return std::make_unique<PixelArray>(result);
}

std::unique_ptr<PixelArray> invert(const PixelArray& pixelArray)
{
    const SDL_Surface* src = pixelArray.getSDL();

    const int w = src->w;
    const int h = src->h;

    // Create an output surface matching the source format
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!result)
        throw std::runtime_error("Failed to create result surface for invert.");

    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const SDL_PixelFormatDetails* srcDetails = SDL_GetPixelFormatDetails(src->format);
    const SDL_PixelFormatDetails* resDetails = SDL_GetPixelFormatDetails(result->format);

    uint8_t r, g, b, a;
    for (int i = 0; i < w * h; ++i)
    {
        SDL_GetRGBA(srcPx[i], srcDetails, nullptr, &r, &g, &b, &a);
        dstPx[i] = SDL_MapRGBA(resDetails, nullptr, 255 - r, 255 - g, 255 - b, a);
    }

    return std::make_unique<PixelArray>(result);
}

std::unique_ptr<PixelArray> grayscale(const PixelArray& pixelArray)
{
    const SDL_Surface* src = pixelArray.getSDL();

    const int w = src->w;
    const int h = src->h;

    // Create an output surface with its own memory
    SDL_Surface* result = SDL_CreateSurface(w, h, src->format);
    if (!result)
        throw std::runtime_error("Failed to create surface for grayscale.");

    const uint32_t* srcPx = static_cast<uint32_t*>(src->pixels);
    auto* dstPx = static_cast<uint32_t*>(result->pixels);

    const SDL_PixelFormatDetails* srcDetails = SDL_GetPixelFormatDetails(src->format);
    const SDL_PixelFormatDetails* resDetails = SDL_GetPixelFormatDetails(result->format);

    uint8_t r, g, b, a;
    for (int i = 0; i < w * h; ++i)
    {
        SDL_GetRGBA(srcPx[i], srcDetails, nullptr, &r, &g, &b, &a);
        const auto gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
        dstPx[i] = SDL_MapRGBA(resDetails, nullptr, gray, gray, gray, a);
    }

    return std::make_unique<PixelArray>(result);
}
} // namespace kn::transform