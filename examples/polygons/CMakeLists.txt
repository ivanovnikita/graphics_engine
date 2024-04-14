set(implementations main.cxx)

add_executable(example_2d_polygons)
target_sources(example_2d_polygons
    PRIVATE
        ${implementations}
)
target_compile_options(example_2d_polygons PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_2d_polygons PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_2d_polygons
    GE::render
    GE::window
    GE::render_loop
    vulkan-headers::vulkan-headers
    ${std_libs}
)
source_group(example_2d_polygons FILES ${implementations})
install(TARGETS example_2d_polygons DESTINATION examples/2d_polygons)
