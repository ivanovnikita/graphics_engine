sources_in_dir_recurse(ge/shader_compiler shader_compiler_sources)
add_executable(ge_shader_compiler ${shader_compiler_sources})
target_compile_options(ge_shader_compiler PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(ge_shader_compiler PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(ge_shader_compiler
    PRIVATE
        CONAN_PKG::vulkan-headers
        CONAN_PKG::shaderc
        Threads::Threads
        ${std_libs}
)
define_source_group(ge_shader_compiler)
