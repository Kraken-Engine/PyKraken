#include "shader_baker.hpp"

#include <SDL3_shadercross/SDL_shadercross.h>

#include <fstream>
#include <stdexcept>
#include <vector>

#include "Log.hpp"

namespace kn::shaders
{

static std::vector<char> readFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + path.string());
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    return buffer;
}

void bake(const std::filesystem::path& fragmentPath, const std::filesystem::path& outputBasePath)
{
    if (!SDL_ShaderCross_Init())
        throw std::runtime_error("Failed to initialize SDL_ShaderCross");

    struct ShaderCrossGuard
    {
        ~ShaderCrossGuard()
        {
            SDL_ShaderCross_Quit();
        }
    } guard;

    std::vector<char> source = readFile(fragmentPath);
    source.push_back('\0');

    SDL_ShaderCross_HLSL_Info hlslInfo{
        .source = source.data(),
        .entrypoint = "main",
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT,
    };

    // Compile HLSL to DXIL -----------------------------------------
    size_t dxilSize;
    void* dxilBuffer = SDL_ShaderCross_CompileDXILFromHLSL(&hlslInfo, &dxilSize);
    if (!dxilBuffer)
        log::warn("Failed to compile HLSL to DXIL.");
    else
    {
        std::filesystem::path dxilPath = outputBasePath.string() + ".dxil";
        std::ofstream dxilFile(dxilPath, std::ios::binary);
        dxilFile.write(reinterpret_cast<const char*>(dxilBuffer), dxilSize);
        dxilFile.close();
        SDL_free(dxilBuffer);
    }

    // Compile HLSL to SPIR-V ---------------------------------------
    size_t spirvSize;
    void* spirvBuffer = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlslInfo, &spirvSize);
    if (!spirvBuffer)
    {
        log::warn("Failed to compile HLSL to SPIR-V. Skipping MSL transpilation.");
        return;
    }

    std::filesystem::path spvPath = outputBasePath.string() + ".spv";
    std::ofstream spvFile(spvPath, std::ios::binary);
    spvFile.write(reinterpret_cast<const char*>(spirvBuffer), spirvSize);
    spvFile.close();

    // Transpile SPIR-V to Metal -------------------------------
    SDL_ShaderCross_SPIRV_Info spirvInfo{
        .bytecode = static_cast<const Uint8*>(spirvBuffer),
        .bytecode_size = spirvSize,
        .entrypoint = "main",
        .shader_stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT,
    };

    void* metalBuffer = SDL_ShaderCross_TranspileMSLFromSPIRV(&spirvInfo);
    if (!metalBuffer)
    {
        log::warn("Failed to transpile SPIR-V to MSL.");
        SDL_free(spirvBuffer);
        return;
    }

    const size_t metalSize = SDL_strlen(static_cast<const char*>(metalBuffer));
    std::filesystem::path mslPath = outputBasePath.string() + ".msl";
    std::ofstream mslFile(mslPath, std::ios::binary);
    mslFile.write(reinterpret_cast<const char*>(metalBuffer), metalSize);
    mslFile.close();

    SDL_free(metalBuffer);
    SDL_free(spirvBuffer);
}

}  // namespace kn::shaders