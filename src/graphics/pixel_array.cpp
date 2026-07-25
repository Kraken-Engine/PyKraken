#include <SDL3_image/SDL_image.h>

#include <cstring>
#include <vector>

#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/math/Math.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
PixelArray::PixelArray(SDL_Surface* sdlSurface)
    : m_surface(sdlSurface)
{
}

PixelArray::PixelArray(const int width, const int height)
{
    m_surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    if (!m_surface)
        throw std::runtime_error("PixelArray failed to create: " + std::string(SDL_GetError()));
}

PixelArray::PixelArray(const std::filesystem::path& filePath)
{
    if (filePath.empty())
        throw std::invalid_argument("File path cannot be empty");

    // Load image as stupid format
    SDL_Surface* input = IMG_Load(filePath.string().c_str());
    if (!input)
        throw std::runtime_error(
            "Failed to load pixel array from file '" + filePath.string() +
            "': " + std::string(SDL_GetError())
        );

    // Create not stupid surface to draw to
    m_surface = SDL_CreateSurface(input->w, input->h, SDL_PIXELFORMAT_RGBA32);
    if (!m_surface)
    {
        SDL_DestroySurface(input);
        throw std::runtime_error(
            "Failed to create RGBA32 pixel array surface: " + std::string(SDL_GetError())
        );
    }

    // Draw stupid on not stupid
    if (!SDL_BlitSurface(input, nullptr, m_surface, nullptr))
    {
        SDL_DestroySurface(input);
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
        throw std::runtime_error(
            "Failed to convert pixel array surface to RGBA32: " + std::string(SDL_GetError())
        );
    }

    // Send stupid to hell
    SDL_DestroySurface(input);
}

PixelArray::~PixelArray()
{
    if (m_surface)
    {
        SDL_DestroySurface(m_surface);
        m_surface = nullptr;
    }
}

PixelArray::PixelArray(PixelArray&& other) noexcept
    : m_surface(other.m_surface)
{
    // Steal surface and nullify original
    other.m_surface = nullptr;
}

PixelArray& PixelArray::operator=(PixelArray&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing surface first
        if (m_surface)
            SDL_DestroySurface(m_surface);

        // Steal new surface
        m_surface = other.m_surface;

        // Nullify original
        other.m_surface = nullptr;
    }

    return *this;
}

void PixelArray::fill(const Color& color) const
{
    const auto colorMap = SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a);
    SDL_FillSurfaceRect(m_surface, nullptr, colorMap);
}

void PixelArray::blit(
    const PixelArray& other, const Vec2& pos, const Vec2& anchor, const Rect& srcRect
) const
{
    const Rect src = srcRect.getSize() ? other.getRect() : srcRect;
    const SDL_Rect srcSDL = static_cast<SDL_Rect>(src);

    // Calculate destination position based on anchor
    const Vec2 dstPos = pos - src.getSize() * anchor;
    const Rect dst{dstPos, src.w, src.h};
    const SDL_Rect dstSDL = static_cast<SDL_Rect>(dst);

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::blit(const PixelArray& other, const Rect& dstRect, const Rect& srcRect) const
{
    const auto dstSDL = static_cast<SDL_Rect>(dstRect);
    const auto srcSDL = static_cast<SDL_Rect>(
        srcRect.w == 0.0 && srcRect.h == 0.0 ? this->getRect() : srcRect
    );

    if (!SDL_BlitSurface(other.getSDL(), &srcSDL, m_surface, &dstSDL))
        throw std::runtime_error("Failed to blit pixel array: " + std::string(SDL_GetError()));
}

void PixelArray::setColorKey(const Color& color) const
{
    SDL_SetSurfaceColorKey(
        m_surface, true, SDL_MapSurfaceRGBA(m_surface, color.r, color.g, color.b, color.a)
    );
}

Color PixelArray::getColorKey() const
{
    uint32_t key;
    if (!SDL_GetSurfaceColorKey(m_surface, &key))
        throw std::runtime_error(
            "Failed to get pixel array color key: " + std::string(SDL_GetError())
        );

    Color color;
    color.r = static_cast<uint8_t>(key >> 24 & 0xFF);
    color.g = static_cast<uint8_t>(key >> 16 & 0xFF);
    color.b = static_cast<uint8_t>(key >> 8 & 0xFF);
    color.a = static_cast<uint8_t>(key & 0xFF);

    return color;
}

void PixelArray::setAlpha(const uint8_t alpha) const
{
    SDL_SetSurfaceAlphaMod(m_surface, alpha);
}

int PixelArray::getAlpha() const
{
    uint8_t alpha;
    if (!SDL_GetSurfaceAlphaMod(m_surface, &alpha))
        throw std::runtime_error("Failed to get pixel array alpha: " + std::string(SDL_GetError()));
    return alpha;
}

Color PixelArray::getAt(const int x, const int y) const
{
    if (x < 0 || x >= m_surface->w || y < 0 || y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for pixel array");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    const int pitch = m_surface->pitch;

    const uint32_t pixel = *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t));

    Color color;
    const auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    SDL_GetRGBA(pixel, formatDetails, nullptr, &color.r, &color.g, &color.b, &color.a);

    return color;
}

void PixelArray::setAt(const int x, const int y, const Color& color) const
{
    if (x < 0 || x >= m_surface->w || y < 0 || y >= m_surface->h)
        throw std::out_of_range("Coordinates out of bounds for pixel array");

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    const int pitch = m_surface->pitch;

    const auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    const uint32_t pixel = SDL_MapRGBA(formatDetails, nullptr, color.r, color.g, color.b, color.a);
    *reinterpret_cast<uint32_t*>(pixels + y * pitch + x * sizeof(uint32_t)) = pixel;
}

int PixelArray::getWidth() const
{
    return m_surface->w;
}

int PixelArray::getHeight() const
{
    return m_surface->h;
}

Vec2 PixelArray::getSize() const
{
    return {m_surface->w, m_surface->h};
}

Rect PixelArray::getRect() const
{
    return {0, 0, m_surface->w, m_surface->h};
}

PixelArray PixelArray::copy() const
{
    SDL_Surface* surfaceCopy = SDL_CreateSurface(m_surface->w, m_surface->h, m_surface->format);
    if (!surfaceCopy)
        throw std::runtime_error(
            "Failed to create copy pixel array: " + std::string(SDL_GetError())
        );

    if (!SDL_BlitSurface(m_surface, nullptr, surfaceCopy, nullptr))
        throw std::runtime_error("Failed to blit pixel array copy: " + std::string(SDL_GetError()));

    return PixelArray(surfaceCopy);
}

void PixelArray::scroll(const int dx, const int dy, const ScrollMode scrollMode) const
{
    if (!m_surface || (dx == 0 && dy == 0))
        return;

    const int width = m_surface->w;
    const int height = m_surface->h;
    const int pitch = m_surface->pitch;
    const auto formatDetails = SDL_GetPixelFormatDetails(m_surface->format);
    const int bytesPerPixel = formatDetails->bytes_per_pixel;

    // For REPEAT mode, optimize with modulo; for others, keep full offset
    int scrollX = dx;
    int scrollY = dy;

    if (scrollMode == ScrollMode::REPEAT)
    {
        scrollX = dx % width;
        scrollY = dy % height;

        if (scrollX == 0 && scrollY == 0)
            return;
    }

    auto* pixels = static_cast<uint8_t*>(m_surface->pixels);
    std::vector<uint8_t> tempBuffer(pitch * height);
    std::memcpy(tempBuffer.data(), pixels, pitch * height);

    // Process each destination row
    for (int dstY = 0; dstY < height; ++dstY)
    {
        int srcY = dstY - scrollY;

        // Handle Y boundary based on scroll mode
        switch (scrollMode)
        {
        case ScrollMode::REPEAT:
            srcY = (srcY % height + height) % height;
            break;

        case ScrollMode::ERASE:
            if (srcY < 0 || srcY >= height)
            {
                // Erase entire row
                std::memset(pixels + dstY * pitch, 0, width * bytesPerPixel);
                continue;
            }
            break;

        case ScrollMode::SMEAR:
            srcY = std::max(0, std::min(height - 1, srcY));
            break;
        }

        // Process row with optimized X handling
        uint8_t* dstRow = pixels + dstY * pitch;
        const uint8_t* srcRow = tempBuffer.data() + srcY * pitch;

        for (int dstX = 0; dstX < width; ++dstX)
        {
            int srcX = dstX - scrollX;

            // Handle X boundary based on scroll mode
            switch (scrollMode)
            {
            case ScrollMode::REPEAT:
                srcX = (srcX % width + width) % width;
                break;

            case ScrollMode::ERASE:
                if (srcX < 0 || srcX >= width)
                {
                    std::memset(dstRow + dstX * bytesPerPixel, 0, bytesPerPixel);
                    continue;
                }
                break;

            case ScrollMode::SMEAR:
                srcX = std::max(0, std::min(width - 1, srcX));
                break;
            }

            std::
                memcpy(dstRow + dstX * bytesPerPixel, srcRow + srcX * bytesPerPixel, bytesPerPixel);
        }
    }
}

SDL_Surface* PixelArray::getSDL() const
{
    return m_surface;
}

namespace pixel_array
{
static SDL_Surface* _rotateSurface(SDL_Surface* src, double angle);

PixelArray flip(const PixelArray& pixelArray, const bool flipX, const bool flipY)
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

            const uint8_t* srcPixel = static_cast<uint8_t*>(sdlSurface->pixels) +
                                      srcY * sdlSurface->pitch + srcX * bpp;
            uint8_t* dstPixel = static_cast<uint8_t*>(flipped->pixels) + y * flipped->pitch +
                                x * bpp;

            memcpy(dstPixel, srcPixel, bpp);
        }

    return PixelArray(flipped);
}

PixelArray scaleTo(const PixelArray& pixelArray, const Vec2& size)
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

    return PixelArray(scaled);
}

PixelArray scaleBy(const PixelArray& pixelArray, const double factor)
{
    if (factor <= 0.0)
        throw std::invalid_argument("Scale factor must be a positive value.");

    return scaleTo(pixelArray, pixelArray.getSize() * factor);
}

PixelArray scaleBy(const PixelArray& pixelArray, const Vec2& factor)
{
    if (factor.x <= 0.0 || factor.y <= 0.0)
        throw std::invalid_argument("Scale factors must be positive values.");

    const Vec2 originalSize = pixelArray.getSize();
    return scaleTo(pixelArray, {originalSize.x * factor.x, originalSize.y * factor.y});
}

PixelArray rotate(const PixelArray& pixelArray, const double angle)
{
    SDL_Surface* sdlSurface = pixelArray.getSDL();
    SDL_Surface* rotated = _rotateSurface(sdlSurface, angle);
    if (!rotated)
        throw std::runtime_error("Failed to rotate pixel array.");

    return PixelArray(rotated);
}

PixelArray boxBlur(const PixelArray& pixelArray, const int radius, const bool repeatEdgePixels)
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

    return PixelArray(result);
}

PixelArray gaussianBlur(const PixelArray& pixelArray, const int radius, const bool repeatEdgePixels)
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
            tmpPx[y * w + x] = SDL_MapRGBA(
                tmpDetails, nullptr, static_cast<Uint8>(fr), static_cast<Uint8>(fg),
                static_cast<Uint8>(fb), static_cast<Uint8>(fa)
            );
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
            dstPx[y * w + x] = SDL_MapRGBA(
                resDetails, nullptr, static_cast<uint8_t>(fr), static_cast<uint8_t>(fg),
                static_cast<uint8_t>(fb), static_cast<uint8_t>(fa)
            );
        }

    SDL_DestroySurface(temp);

    return PixelArray(result);
}

PixelArray invert(const PixelArray& pixelArray)
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

    return PixelArray(result);
}

PixelArray grayscale(const PixelArray& pixelArray)
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

    return PixelArray(result);
}

SDL_Surface* _rotateSurface(SDL_Surface* src, double angle)
{
    const double rad = -angle * M_PI / 180.0;
    const double cosA = std::cos(rad);
    const double sinA = std::sin(rad);

    const int srcW = src->w;
    const int srcH = src->h;

    const auto dstW = static_cast<int>(std::ceil(std::abs(srcW * cosA) + std::abs(srcH * sinA)));
    const auto dstH = static_cast<int>(std::ceil(std::abs(srcW * sinA) + std::abs(srcH * cosA)));

    SDL_Surface* dst = SDL_CreateSurface(dstW, dstH, src->format);
    if (!dst)
        return nullptr;

    const double centerX = srcW / 2.0;
    const double centerY = srcH / 2.0;
    const double dstCenterX = dstW / 2.0;
    const double dstCenterY = dstH / 2.0;

    const auto srcPixels = static_cast<uint32_t*>(src->pixels);
    auto dstPixels = static_cast<uint32_t*>(dst->pixels);

    for (int y = 0; y < dstH; ++y)
    {
        for (int x = 0; x < dstW; ++x)
        {
            const double tx = x - dstCenterX;
            const double ty = y - dstCenterY;

            const double sx = tx * cosA + ty * sinA + centerX;
            const double sy = -tx * sinA + ty * cosA + centerY;

            const auto isx = static_cast<int>(std::floor(sx));
            const auto isy = static_cast<int>(std::floor(sy));

            const bool inSrc = isx >= 0 && isx < srcW && isy >= 0 && isy < srcH;
            dstPixels[y * dstW + x] = inSrc ? srcPixels[isy * srcW + isx] : 0;
        }
    }

    return dst;
}

}  // namespace pixel_array
}  // namespace kn
