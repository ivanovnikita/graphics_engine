interfaces_in_dir_recurse(ge/render_loop interfaces)
implementations_in_dir_recurse(ge/render_loop implementations)

add_library(render_loop)
target_sources(render_loop
    PRIVATE
        ${implementations}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
)
target_compile_options(render_loop PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(render_loop PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_render_loop)
target_link_libraries(render_loop
    PRIVATE
        GE::render
        GE::window
        ${std_libs}
)
add_library(GE::render_loop ALIAS render_loop)
source_group(render FILES ${interfaces} ${implementations})

install(
    TARGETS
        render_loop
    EXPORT
        GraphicsEngine
    DESTINATION lib
    FILE_SET CXX_MODULES DESTINATION cxx_modules
    CXX_MODULES_BMI DESTINATION cxx_modules/bmi
)
