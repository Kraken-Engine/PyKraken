#include <array>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include "shader_baker.hpp"

namespace
{
class DotAnimator
{
  public:
    explicit DotAnimator(std::string message)
        : m_message(std::move(message))
    {
    }

    ~DotAnimator()
    {
        stop();
    }

    void start()
    {
        if (m_running.exchange(true))
            return;

        m_thread = std::thread(
            [this]()
            {
                size_t dotIndex = 0;
                while (m_running.load())
                {
                    std::cout << "\r" << m_message << m_dots[dotIndex] << "   " << std::flush;
                    dotIndex = (dotIndex + 1) % m_dots.size();
                    std::this_thread::sleep_for(std::chrono::milliseconds(400));
                }

                std::cout << "\r" << m_message << "...   \n" << std::flush;
            }
        );
    }

    void stop()
    {
        m_running = false;
        if (m_thread.joinable())
            m_thread.join();
    }

  private:
    std::string m_message;
    std::atomic<bool> m_running{false};
    std::thread m_thread;
    const std::array<std::string, 4> m_dots{{"", ".", "..", "..."}};
};

void printUsage(const char* program)
{
    std::cerr << "Usage: " << program << " bake <input_shader> [-o|--out <output_base_path>]\n"
              << "   or: " << program << " <input_shader> [-o|--out <output_base_path>]\n";
}

struct CliOptions
{
    std::filesystem::path inputPath;
    std::filesystem::path outputPath{"."};
};

bool parseArgs(const int argc, char* argv[], CliOptions& out)
{
    if (argc < 2)
        return false;

    int index = 1;
    if (std::string(argv[index]) == "bake")
        ++index;

    if (index >= argc)
        return false;

    out.inputPath = argv[index++];

    while (index < argc)
    {
        const std::string arg = argv[index];

        if (arg == "-o" || arg == "--out")
        {
            if (index + 1 >= argc)
                throw std::invalid_argument("Missing value for --out");

            out.outputPath = argv[index + 1];
            index += 2;
            continue;
        }

        throw std::invalid_argument("Unknown argument: " + arg);
    }

    return true;
}
}  // namespace

int main(int argc, char* argv[])
{
    const bool wantsHelp = (argc >= 2 &&
                            (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) ||
                           (argc >= 3 && std::string(argv[1]) == "bake" &&
                            (std::string(argv[2]) == "-h" || std::string(argv[2]) == "--help"));
    if (wantsHelp)
    {
        printUsage(argv[0]);
        return EXIT_SUCCESS;
    }

    CliOptions options;

    try
    {
        if (!parseArgs(argc, argv, options))
        {
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing arguments: " << e.what() << "\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    DotAnimator animator("Baking: " + options.inputPath.string());
    try
    {
        animator.start();
        kn::shaders::bake(options.inputPath, options.outputPath);
        animator.stop();
        std::cout << "Shaders generated successfully.\n";
    }
    catch (const std::exception& e)
    {
        animator.stop();
        std::cerr << "Error baking shader: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}