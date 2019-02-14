#pragma once

#include <experimental/filesystem>

namespace ge
{
    namespace fs = std::experimental::filesystem;

    struct GeneratedCppSources final
    {
        std::string header;
        std::string source;
    };

    GeneratedCppSources compile_shaders_into_cpp_sources
    (
        const fs::path& shaders_dir
        , const std::string_view target_namespace
        , const std::string_view target_filename
    );

    void save_sources
    (
        const GeneratedCppSources& sources
        , const fs::path& target_dir
        , const std::string_view target_filename
    );
}
