set(implementations main.cxx)

add_executable(example_hex_grid)
target_sources(example_hex_grid
    PRIVATE
        ${implementations}
)
target_compile_options(example_hex_grid PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_hex_grid PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_hex_grid
    GE::render
    GE::window
    GE::render_loop
    GE::geometry
    vulkan-headers::vulkan-headers
    ${std_libs}
)
source_group(example_hex_grid FILES ${implementations})
install(TARGETS example_hex_grid DESTINATION examples/hex_grid)
