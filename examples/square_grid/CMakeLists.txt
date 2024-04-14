set(implementations main.cxx)

add_executable(example_square_grid)
target_sources(example_square_grid
    PRIVATE
        ${implementations}
)
target_compile_options(example_square_grid PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_square_grid PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_square_grid
    GE::render
    GE::window
    GE::render_loop
    GE::geometry
    vulkan-headers::vulkan-headers
    ${std_libs}
)
source_group(example_square_grid FILES ${implementations})
install(TARGETS example_square_grid DESTINATION examples/square_grid)
