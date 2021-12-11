sources_in_dir_recurse(ge/common common_sources)
add_library(common ${common_sources})
target_compile_options(common PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(common PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_common)
target_include_directories(common
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::common ALIAS common)
define_source_group(common)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    sources_in_dir_recurse(tests tests_sources)
    add_executable(tests_common ${tests_sources})
    target_link_libraries(tests_common
        CONAN_PKG::gtest
        GE::common
        ${std_libs}
    )
    target_compile_options(tests_common PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_common PROPERTIES ${GE_TARGET_PROPERTIES})
    define_source_group(tests_common)
    gtest_discover_tests(tests_common)
endif()

install(
    TARGETS
        common
    EXPORT
        GraphicsEngine
    DESTINATION lib
)
