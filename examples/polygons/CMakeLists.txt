add_executable(example_2d_polygons main.cpp)
target_compile_options(example_2d_polygons PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_2d_polygons PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_2d_polygons
    GE::render
    GE::window
    GE::render_loop
    vulkan-headers::vulkan-headers
    ${std_libs}
)
define_source_group(example_2d_polygons)
install(TARGETS example_2d_polygons DESTINATION examples/2d_polygons)
