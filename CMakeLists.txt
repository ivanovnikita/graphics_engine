cmake_minimum_required(VERSION 3.13)

project(graphics_engine)

option(GE_BUILD_TESTS "Build internal tests?" ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("third_party/cmake-conan/conan.cmake")

conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD missing
)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURGET_BINARY_DIR}"
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
        ${CONAN_INCLUDE_DIRS_SHADERC}
    PRIVATE
        src/render
    SYSTEM INTERFACE
        src/render
)
add_library(GE::Render ALIAS render)

if (${GE_BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
    add_subdirectory(examples)
endif()

# TODO: install targets (disable install by conan)
