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
# NOTE: add path to bin to LD_LIBRARY_PATH (bin dir must contain libVkLayer_khronos_validation.so)
option(GE_ENABLE_DEBUG_LAYERS "Enable vulkan debug layers?" ${default_debug_layers})
option(GE_BUILD_EXAMPLES "Build examples?" ON)
option(GE_BUILD_TESTS "Build tests?" ON)
option(GE_ENABLE_IWYU "Enable running include-what-you-use tool?" OFF)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    option(GE_USE_LIBC++ "Use libc++?" ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(compiler_flags)
include(linker_flags)

include(source_tools)
include(iwyu)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install path prefix" FORCE)
endif()

include(install_deps)

find_package(Threads REQUIRED)

add_subdirectory(modules/common)
add_subdirectory(modules/geometry)
add_subdirectory(modules/window)
add_subdirectory(modules/shader_compiler)
add_subdirectory(modules/render)
add_subdirectory(modules/render_loop)

if (${GE_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()

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
