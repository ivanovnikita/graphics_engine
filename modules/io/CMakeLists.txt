sources_in_dir_recurse(ge/io io_sources)
add_library(io ${io_sources})
target_compile_options(io PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(io PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_io)
target_include_directories(io
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(io
    PUBLIC
        GE::common
        GE::geometry
    PRIVATE
        CONAN_PKG::stb
        CONAN_PKG::tinyobjloader
)

add_library(GE::io ALIAS io)
define_source_group(io)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    sources_in_dir_recurse(tests tests_sources)
    add_executable(tests_io ${tests_sources})
    target_link_libraries(tests_io
        CONAN_PKG::gtest
        GE::io
        ${std_libs}
    )
    target_compile_options(tests_io PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_io PROPERTIES ${GE_TARGET_PROPERTIES})
    define_source_group(tests_io)
    gtest_discover_tests(tests_io)
endif()

install(
    TARGETS
        io
    EXPORT
        GraphicsEngine
    DESTINATION lib
)
