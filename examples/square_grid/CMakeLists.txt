add_executable(example_square_grid main.cpp)
target_compile_options(example_square_grid PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_square_grid PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_square_grid
    GE::render
    GE::window
    GE::render_loop
    GE::geometry
    CONAN_PKG::vulkan-headers
    ${std_libs}
)
define_source_group(example_square_grid)
install(TARGETS example_square_grid DESTINATION examples/square_grid)
