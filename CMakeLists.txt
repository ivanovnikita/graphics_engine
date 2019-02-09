cmake_minimum_required(VERSION 3.13)

project(graphics_engine)

include(CMakeDependentOption)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(default_debug_layers OFF)
    set(default_lto ON)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(default_debug_layers ON)
    set(default_lto OFF)
endif()

option(GE_ENABLE_LTO "Enable LTO?" ${default_lto})
option(GE_ENABLE_DEBUG_LAYERS "Enable vulkan debug layers?" ${default_debug_layers})
option(GE_BUILD_EXAMPLES "Build examples?" ON)
option(GE_BUILD_TESTS "Build internal tests?" ON)
cmake_dependent_option(
    GE_BUILD_GRAPHICS_TESTS
        "Build graphics tests?" ON
        "GE_BUILD_TESTS" OFF
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("third_party/cmake-conan/conan.cmake")

if (WIN32)
elseif(UNIX)
    list(APPEND conan_options with_xcb=True)
endif()

if (${GE_ENABLE_LTO})
    include(CheckIPOSupported)
    check_ipo_supported()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

    set(linker_flags "${linker_flags} -fuse-ld=gold -fuse-linker-plugin")

    list(APPEND conan_options lto=True)
endif()

if (${GE_ENABLE_DEBUG_LAYERS})
    list(APPEND conan_options with_debug_layers=True)
endif()

if (${GE_BUILD_TESTS})
    list(APPEND conan_options with_gtests=True)
endif()

conan_cmake_run(
    CONANFILE conanfile.py
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD missing
    NO_OUTPUT_DIRS
    OPTIONS ${conan_options}
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${linker_flags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${linker_flags}")

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
    list(APPEND cxx_flags -Ofast)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND cxx_flags -O0)
else()
    message(FATAL_ERROR "Unknown value of CMAKE_BUILD_TYPE!")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND warnings
    -Werror
    -Wall
    -Wextra
    -pedantic
    -pedantic-errors
)

set(std_libs -lstdc++fs)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.7")
        list(APPEND warnings
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
        list(APPEND warnings
            -Wduplicated-branches
            -Wduplicated-cond
            )
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND warnings
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

find_package(Threads REQUIRED)

if (WIN32)
elseif(UNIX)
    set(window_libs CONAN_PKG::xcb)
endif()

sources_in_dir_recurse(src/ge/window window_sources)
add_library(ge_window ${window_sources})
target_compile_options(ge_window PRIVATE ${compile_options})
set_target_properties(ge_window PROPERTIES ${target_properties})
target_compile_definitions(ge_window PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(ge_window
    PRIVATE
        CONAN_PKG::Vulkan-Loader
        ${window_libs}
)
target_include_directories(ge_window
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::window ALIAS ge_window)

sources_in_dir_recurse(src/ge/render render_sources)
add_library(ge_render ${render_sources})
target_compile_options(ge_render PRIVATE ${compile_options})
set_target_properties(ge_render PROPERTIES ${target_properties})
target_compile_definitions(ge_render PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(ge_render
    PRIVATE
        CONAN_PKG::shaderc
        CONAN_PKG::Vulkan-Loader
        Threads::Threads
        ${std_libs}
        dl
)
target_include_directories(ge_render
    SYSTEM PRIVATE
        $<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS_SHADERC}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::render ALIAS ge_render)

install(
    TARGETS
        ge_window
        ge_render
    EXPORT
        GraphicsEngine
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(
    FILES
        src/ge/render/render.h
    DESTINATION
        include/ge/render
)
install(
    FILES
        src/ge/window/window.h
    DESTINATION
        include/ge/window
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

set(VK_LAYER_PATH "${CONAN_VULKAN-VALIDATIONLAYERS_ROOT}/share/vulkan/explicit_layer.d")
configure_file("tests/graphics/graphics_config.txt.in" "${CMAKE_BINARY_DIR}/graphics_config.txt")

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

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
        target_link_libraries(tests_graphics
            GE::window
            GE::render
            GTest::Main
            CONAN_PKG::Vulkan-Headers
        )
        target_include_directories(tests_graphics PRIVATE "tests/graphics")
        gtest_discover_tests(tests_graphics)
        install(TARGETS tests_graphics DESTINATION tests/bin)

        file(GLOB shaders "tests/graphics/factory/shaders/*")
        install(FILES ${shaders} DESTINATION tests/bin/shaders)

        install(FILES "${CMAKE_BINARY_DIR}/graphics_config.txt" DESTINATION tests/bin)
    endif()
endif()

if (${GE_BUILD_EXAMPLES})
    add_executable(example_triangle examples/triangle/main.cpp)
    target_link_libraries(example_triangle
        GE::render
        GE::window
        CONAN_PKG::Vulkan-Headers
    )
    install(TARGETS example_triangle DESTINATION examples/triangle)
    file(GLOB shaders "examples/triangle/shaders/*")
    install(FILES ${shaders} DESTINATION examples/triangle/shaders)
    install(FILES "${CMAKE_BINARY_DIR}/graphics_config.txt" DESTINATION examples/triangle)
endif()
