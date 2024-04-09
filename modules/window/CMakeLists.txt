if (WIN32)
elseif(UNIX)
    set(window_libs
        xcb
        xcb-util
        xcb-errors
        xcb-keysyms
        xcb-icccm
    )
endif()

interfaces_in_dir_recurse(ge/window interfaces)
implementations_in_dir_recurse(ge/window implementations)
add_library(window)
target_sources(window
    PRIVATE
        ${implementations}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
)
target_compile_options(window PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(window PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_window)
#target_compile_definitions(window PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(window
    PRIVATE
        GE::common
        glm::glm
        Vulkan::Vulkan
        ${window_libs}
)
add_library(GE::window ALIAS window)
source_group(window FILES ${interfaces} ${implementations})

install(
    TARGETS
        window
    EXPORT
        GraphicsEngine
    DESTINATION lib
    FILE_SET CXX_MODULES DESTINATION cxx_modules
    CXX_MODULES_BMI DESTINATION cxx_modules/bmi
)
