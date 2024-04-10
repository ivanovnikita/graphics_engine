interfaces_in_dir_recurse(ge/shader_compiler interfaces)
implementations_in_dir_recurse(ge/shader_compiler implementations)

add_executable(ge_shader_compiler)
target_sources(ge_shader_compiler
    PRIVATE
        ${implementations}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
)
target_compile_options(ge_shader_compiler PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(ge_shader_compiler PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(ge_shader_compiler
    PRIVATE
        vulkan-headers::vulkan-headers
        shaderc_shared # SPIRV-Tools # from system
        Threads::Threads
        ${std_libs}
)
source_group(ge_shader_compiler FILES ${interfaces} ${implementations})
