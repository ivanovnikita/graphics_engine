interfaces_in_dir_recurse(ge/io interfaces)
implementations_in_dir_recurse(ge/io implementations)

add_library(io)
target_sources(io
    PRIVATE
        ${implementations}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
)
target_compile_options(io PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(io PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_io)
target_link_libraries(io
    PUBLIC
        GE::common
        GE::geometry
    PRIVATE
        stb::stb
        tinyobjloader::tinyobjloader
)

add_library(GE::io ALIAS io)
source_group(io FILES ${interfaces} ${implementations})

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    interfaces_in_dir_recurse(tests tests_interfaces)
    add_executable(tests_io)
    target_sources(tests_io
        PRIVATE
            FILE_SET CXX_MODULES FILES
                ${tests_interfaces}
    )
    target_link_libraries(tests_io
        gtest::gtest
        GE::io
        ${std_libs}
    )
    target_compile_options(tests_io PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_io PROPERTIES ${GE_TARGET_PROPERTIES})
    source_group(tests_io FILES ${tests_interfaces})
    gtest_discover_tests(tests_io)
endif()

install(
    TARGETS
        io
    EXPORT
        GraphicsEngine
    DESTINATION lib
    FILE_SET CXX_MODULES DESTINATION cxx_modules
    CXX_MODULES_BMI DESTINATION cxx_modules/bmi
)
