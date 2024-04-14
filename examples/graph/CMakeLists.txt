set(implementations main.cxx)

add_executable(example_graph)
target_sources(example_graph
    PRIVATE
        ${implementations}
)
target_compile_options(example_graph PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_graph PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_graph
    GE::render
    GE::window
    GE::render_loop
    ${std_libs}
)
source_group(example_graph FILES ${implementations})
install(TARGETS example_graph DESTINATION examples/graph)
