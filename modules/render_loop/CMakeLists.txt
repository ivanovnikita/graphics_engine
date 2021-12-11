sources_in_dir_recurse(ge/render_loop render_loop_sources)
add_library(render_loop ${render_loop_sources})
target_compile_options(render_loop PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(render_loop PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_render_loop)
target_include_directories(render_loop
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(render_loop
    PRIVATE
        GE::render
        GE::window
        ${std_libs}
)
add_library(GE::render_loop ALIAS render_loop)
define_source_group(render_loop)

install(
    TARGETS
        render_loop
    EXPORT
        GraphicsEngine
    DESTINATION lib
)
install(
    FILES
        ge/render_loop/render_loop.h
    DESTINATION
        include/ge/render_loop
)
