add_executable(example_texture_2d main.cpp)
target_compile_options(example_texture_2d PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_texture_2d PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_texture_2d
    GE::render
    GE::window
    GE::render_loop
    vulkan-headers::vulkan-headers
    ${std_libs}
)
define_source_group(example_texture_2d)
install(TARGETS example_texture_2d DESTINATION examples/texture_2d)
