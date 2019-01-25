cmake_minimum_required(VERSION 3.13)

project(graphics_engine)

include(CMakeDependentOption)

option(GE_BUILD_EXAMPLES "Build examples?" ON)
option(GE_BUILD_TESTS "Build internal tests?" ON)
cmake_dependent_option(
    GE_BUILD_GRAPHICS_TESTS
        "Build graphics tests?" ON
        "GE_BUILD_TESTS" OFF
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("third_party/cmake-conan/conan.cmake")

conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD missing
    NO_OUTPUT_DIRS
)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_BINARY_DIR}/build"
        CACHE
        PATH
        "Install path prefix"
        FORCE
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(cxx_flags -Ofast)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(cxx_flags -O0)
else()
    message(FATAL_ERROR "Unknown value of CMAKE_BUILD_TYPE!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(warnings
    -Werror
    -Wall
    -Wextra
    -pedantic
    -pedantic-errors
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(std_libs -lstdc++fs)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.7")
        set(warnings ${warnings}
            -Wfloat-equal
            -Wpointer-arith
            -Wcast-qual
            -Wconversion
            -Wsign-conversion
            -Wlogical-op
            -Wmissing-declarations
            -Wredundant-decls
            )
    endif()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0")
        set(warnings ${warnings}
            -Wduplicated-branches
            -Wduplicated-cond
            )
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(std_libs -lc++experimental)
    set(warnings ${warnings}
        -Weverything
        -Wno-c++98-compat
        -Wno-c++11-compat
        -Wno-c++14-compat
        -Wno-unreachable-code
        )
endif()

set(compile_options ${cxx_flags} ${warnings})

set(target_properties
    LINKER_LANGUAGE CXX
    POSITION_INDEPENDENT_CODE ON
)

function(sources_in_dir_recurse directory result)
    file(GLOB_RECURSE output
        "${directory}/*.cpp"
        "${directory}/*.h"
        "${directory}/*.hpp"
    )
    set(${result} ${output} PARENT_SCOPE)
endfunction()

find_package(Vulkan REQUIRED)

if (WIN32)
elseif(UNIX)
    find_package(XCB REQUIRED)
    set(window_libs XCB::XCB)
endif()

sources_in_dir_recurse(src/render render_sources)
add_library(render ${render_sources})
target_compile_options(render PRIVATE ${compile_options})
set_target_properties(render PROPERTIES ${target_properties})
target_compile_definitions(render PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(render
    PRIVATE
        CONAN_PKG::shaderc
        Vulkan::Vulkan
        ${window_libs}
        ${std_libs}
)
target_include_directories(render
    SYSTEM PRIVATE
        $<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS_SHADERC}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/render>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/render>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::render ALIAS render)

install(
    TARGETS
        render
    EXPORT
        GraphicsEngine
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(
    FILES
        src/render/render/render.h
    DESTINATION
        include/render
)
# TODO: install deps too (it is not usable now)
install(
    EXPORT
        GraphicsEngine
    FILE
        GraphicsEngineConfig.cmake
    NAMESPACE
        GE::
    DESTINATION
        cmake
)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    find_package(Threads REQUIRED)
    find_package(GTest REQUIRED)

    sources_in_dir_recurse(tests/common common_tests_sources)
    add_executable(tests_common ${common_tests_sources})
    target_link_libraries(tests_common GE::render GTest::Main Threads::Threads)
    gtest_discover_tests(tests_common)
    install(TARGETS tests_common DESTINATION tests/bin)

    if (${GE_BUILD_GRAPHICS_TESTS})
        sources_in_dir_recurse(tests/graphics graphics_tests_sources)
        add_executable(tests_graphics
            ${graphics_tests_sources}
            "tests/graphics/graphics_config.txt.in"
        )
        target_link_libraries(tests_graphics GE::render GTest::Main Threads::Threads Vulkan::Vulkan)
        target_include_directories(tests_graphics PRIVATE "tests/graphics")
        gtest_discover_tests(tests_graphics)
        install(TARGETS tests_graphics DESTINATION tests/bin)

        file(GLOB shaders "tests/graphics/factory/shaders/*")
        install(FILES ${shaders} DESTINATION tests/bin/shaders)

        set(VK_LAYER_PATH "${CONAN_VULKAN-LOADERANDVALIDATIONLAYERS_ROOT}/etc/vulkan/explicit_layer.d")
        configure_file("tests/graphics/graphics_config.txt.in" "${CMAKE_BINARY_DIR}/graphics_config.txt")
        install(FILES "${CMAKE_BINARY_DIR}/graphics_config.txt" DESTINATION tests/bin)
    endif()
endif()

if (${GE_BUILD_EXAMPLES})
    add_executable(example_triangle examples/triangle/main.cpp)
    target_link_libraries(example_triangle GE::render)
    install(TARGETS example_triangle DESTINATION examples/triangle)
    file(GLOB shaders "examples/triangle/shaders/*")
    install(FILES ${shaders} DESTINATION examples/triangle/shaders)
endif()
