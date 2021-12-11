add_executable(example_hex_grid main.cpp)
target_compile_options(example_hex_grid PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_hex_grid PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_hex_grid
    GE::render
    GE::window
    GE::render_loop
    GE::geometry
    CONAN_PKG::vulkan-headers
    ${std_libs}
)
define_source_group(example_hex_grid)
install(TARGETS example_hex_grid DESTINATION examples/hex_grid)
