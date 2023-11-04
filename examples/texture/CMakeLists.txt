add_executable(example_2d_texture main.cpp)
target_compile_options(example_2d_texture PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_2d_texture PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_2d_texture
    GE::render
    GE::window
    GE::render_loop
    CONAN_PKG::vulkan-headers
    ${std_libs}
)
define_source_group(example_2d_texture)
install(TARGETS example_2d_texture DESTINATION examples/2d_texture)
