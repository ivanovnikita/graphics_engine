interfaces_in_dir_recurse(ge/common interfaces)
implementations_in_dir_recurse(ge/common implementations)
headers_in_dir_recurse(ge/common headers)

add_library(common)
target_sources(common
    PRIVATE
        ${implementations}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
        FILE_SET HEADERS FILES
            ${headers}
)
target_include_directories(common
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_compile_options(common PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(common PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_common)
add_library(GE::common ALIAS common)
source_group(common FILES ${interfaces} ${implementations} ${headers})

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    interfaces_in_dir_recurse(tests tests_interfaces)
    add_executable(tests_common)
    target_sources(tests_common
        PRIVATE
            FILE_SET CXX_MODULES FILES
                ${tests_interfaces}
    )
    target_link_libraries(tests_common
        gtest::gtest
        GE::common
        ${std_libs}
    )
    target_compile_options(tests_common PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_common PROPERTIES ${GE_TARGET_PROPERTIES})
    source_group(tests_common FILES ${tests_interfaces})
    gtest_discover_tests(tests_common)
endif()

install(
    TARGETS
        common
    EXPORT
        GraphicsEngine
    DESTINATION lib
    FILE_SET HEADERS DESTINATION include
    FILE_SET CXX_MODULES DESTINATION cxx_modules
    CXX_MODULES_BMI DESTINATION cxx_modules/bmi
)
