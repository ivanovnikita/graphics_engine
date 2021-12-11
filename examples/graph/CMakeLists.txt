add_executable(example_graph main.cpp)
target_compile_options(example_graph PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(example_graph PROPERTIES ${GE_TARGET_PROPERTIES})
target_link_libraries(example_graph
    GE::render
    GE::window
    GE::render_loop
    ${std_libs}
)
define_source_group(example_graph)
install(TARGETS example_graph DESTINATION examples/graph)
