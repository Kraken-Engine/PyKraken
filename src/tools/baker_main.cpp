#include <iostream>

#include "shader_baker.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: KrakenShaderBaker <input.hlsl> <output_base_name>\n";
        return EXIT_FAILURE;
    }

    std::filesystem::path inputPath = argv[1];
    std::filesystem::path outputPath = argv[2];

    try
    {
        std::cout << "Baking shader: " << inputPath << "...\n";
        kn::shaders::bake(inputPath, outputPath);
        std::cout << "Successfully generated .spv, .dxil, and .msl files.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return EXIT_SUCCESS;
}