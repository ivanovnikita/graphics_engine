set(implementations main.cxx)

add_executable(example_texture_3d)
target_sources(example_texture_3d
    PRIVATE
        ${implementations}
)
target_compile_options(example_texture_3d PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_texture_3d PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_texture_3d
    GE::render
    GE::window
    GE::render_loop
    vulkan-headers::vulkan-headers
    ${std_libs}
)
source_group(example_texture_3d FILES ${implementations})
install(TARGETS example_texture_3d DESTINATION examples/texture_3d)
