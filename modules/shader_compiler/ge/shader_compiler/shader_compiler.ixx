module;

#include <filesystem>
#include <string>
#include <string_view>

export module shader_compiler;

namespace ge
{
    namespace fs = std::filesystem;

    export struct GeneratedCppSources final
    {
        std::string interface;
        std::string implementation;
    };

    export GeneratedCppSources compile_shaders_into_cpp_sources
    (
        const fs::path& shaders_dir
        , const std::string_view target_namespace
        , const std::string_view target_filename
    );

    export void save_sources
    (
        const GeneratedCppSources& sources
        , const fs::path& target_dir
        , const std::string_view target_filename
    );
}
