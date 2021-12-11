sources_in_dir_recurse(ge/geometry geometry_sources)
add_library(geometry ${geometry_sources})
target_compile_options(geometry PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(geometry PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_geometry)
target_include_directories(geometry
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::geometry ALIAS geometry)
define_source_group(geometry)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    sources_in_dir_recurse(tests tests_sources)
    add_executable(tests_geometry ${tests_sources})
    target_link_libraries(tests_geometry
        CONAN_PKG::gtest
        GE::geometry
        ${std_libs}
    )
    target_compile_options(tests_geometry PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_geometry PROPERTIES ${GE_TARGET_PROPERTIES})
    define_source_group(tests_geometry)
    gtest_discover_tests(tests_geometry)
endif()

install(
    TARGETS
        geometry
    EXPORT
        GraphicsEngine
    DESTINATION lib
)
install(
    DIRECTORY
        ge/geometry
    DESTINATION
        include/ge
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
)
