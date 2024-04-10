#include <filesystem>
#include <iostream>
#include <string>

import shader_compiler;

int main(int argc, char** argv)
{
    using namespace ge;
    namespace fs = std::filesystem;

    try
    {
        if (argc < 5)
        {
            throw std::logic_error("Invalid input options");
        }

        const fs::path shaders_dir{argv[1]};
        const std::string target_namespace{argv[2]};
        const fs::path target_dir{argv[3]};
        const std::string target_name{argv[4]};

        const GeneratedCppSources generated = compile_shaders_into_cpp_sources
        (
            shaders_dir
            , target_namespace
            , target_name
        );

        if (not fs::exists(target_dir))
        {
            fs::create_directory(target_dir);
        }

        save_sources(generated, target_dir, target_name);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected compilation error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
