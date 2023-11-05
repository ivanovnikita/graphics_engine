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

sources_in_dir_recurse(ge/window window_sources)
add_library(window ${window_sources})
target_compile_options(window PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(window PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_window)
#target_compile_definitions(window PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(window
    PRIVATE
        GE::common
        CONAN_PKG::glm
        CONAN_PKG::vulkan-loader
        ${window_libs}
)
target_include_directories(window
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::window ALIAS window)
define_source_group(window)

install(
    TARGETS
        window
    EXPORT
        GraphicsEngine
    DESTINATION lib
)
