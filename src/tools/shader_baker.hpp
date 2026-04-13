#pragma once
#include <filesystem>

namespace kn::shaders
{
void bake(const std::filesystem::path& fragmentPath, const std::filesystem::path& outputBasePath);
}
