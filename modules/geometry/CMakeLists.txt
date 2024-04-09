interfaces_in_dir_recurse(ge/geometry interfaces)
implementations_in_dir_recurse(ge/geometry implementations)

add_library(geometry)
target_sources(geometry
    PRIVATE
        ${implementations}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
)
target_compile_options(geometry PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(geometry PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_geometry)
target_link_libraries(geometry
    PUBLIC
        glm::glm
)
add_library(GE::geometry ALIAS geometry)
source_group(common FILES ${interfaces} ${implementations})

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    interfaces_in_dir_recurse(tests tests_interfaces)
    add_executable(tests_geometry)
    target_sources(tests_geometry
        PRIVATE
            FILE_SET CXX_MODULES FILES
                ${tests_interfaces}
    )
    target_link_libraries(tests_geometry
        gtest::gtest
        GE::geometry
        ${std_libs}
    )
    target_compile_options(tests_geometry PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_geometry PROPERTIES ${GE_TARGET_PROPERTIES})
    source_group(tests_geometry FILES ${tests_interfaces})
    gtest_discover_tests(tests_geometry)
endif()

install(
    TARGETS
        geometry
    EXPORT
        GraphicsEngine
    DESTINATION lib
    FILE_SET CXX_MODULES DESTINATION cxx_modules
    CXX_MODULES_BMI DESTINATION cxx_modules/bmi
)
