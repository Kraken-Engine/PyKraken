#pragma once

#define KN_VERSION_MAJOR 1
#define KN_VERSION_MINOR 7
#define KN_VERSION_PATCH 4

namespace kn
{
namespace detail
{
inline constexpr const char* getPlatform()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(__unix__)
    return "Unix";
#else
    return "Unknown OS";
#endif
}

inline constexpr const char* getArchitecture()
{
#if defined(_M_X64) || defined(__x86_64__)
    return "x64";
#elif defined(_M_IX86) || defined(__i386__) || defined(__i686__)
    return "x86";
#elif defined(_M_ARM64) || defined(__aarch64__)
    return "ARM64";
#elif defined(_M_ARM) || defined(__arm__)
    return "ARM";
#elif defined(__ppc64__) || defined(__PPC64__)
    return "PowerPC64";
#elif defined(__ppc__) || defined(__PPC__)
    return "PowerPC";
#else
    return "Unknown Architecture";
#endif
}
}  // namespace detail

void init(const bool debug = false);

void quit();
}  // namespace kn
