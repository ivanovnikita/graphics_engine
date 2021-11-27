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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    option(GE_USE_LIBC++ "Use libc++?" ON)

    if (${GE_USE_LIBC++})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()

set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (${GE_ENABLE_LTO})
    include(CheckIPOSupported)
    check_ipo_supported()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

    list(APPEND linker_flags "-fuse-ld=gold -fuse-linker-plugin")

    list(APPEND conan_options enable_lto=True)
endif()

if (${GE_ENABLE_DEBUG_LAYERS})
    list(APPEND conan_options enable_debug_layers=True)
endif()

if (${GE_BUILD_TESTS})
    list(APPEND conan_options build_tests=True)
    if (${GE_BUILD_GRAPHICS_TESTS})
        list(APPEND conan_options build_graphics_tests=True)
    endif()
endif()

if(NOT CONAN_EXPORTED)
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    include("third_party/cmake-conan/conan.cmake")
    conan_cmake_autodetect(settings)
    list(APPEND settings compiler.cppstd=20)
    conan_cmake_install(
        PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
        BUILD missing
        SETTINGS ${settings}
        OPTIONS ${conan_options}
    )
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

list(APPEND CMAKE_SHARED_LINKER_FLAGS ${linker_flags})
list(APPEND CMAKE_EXE_LINKER_FLAGS ${linker_flags})

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/builded_package"
        CACHE
        PATH
        "Install path prefix"
        FORCE
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND cxx_flags -O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND cxx_flags -O0)
else()
    message(FATAL_ERROR "Unknown value of CMAKE_BUILD_TYPE!")
endif()

list(APPEND warnings
    -Werror
    -Wall
    -Wextra
    -pedantic
    -pedantic-errors
)

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

    set(std_libs -lstdc++fs)
    #list(APPEND cxx_flags -fanalyzer)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    list(APPEND warnings
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-c++11-compat
        -Wno-c++14-compat
        -Wno-unreachable-code
        -Wno-padded

        # Or it is better to not ignore?
        -Wno-exit-time-destructors

        # for gtest
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-global-constructors
        -Wno-weak-vtables
        )

    if (${GE_USE_LIBC++})
        set(std_libs -lc++ -lc++abi -lc++experimental)
    endif()
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

function(define_source_group target)
    get_target_property(src ${target} SOURCES)
    get_target_property(name ${target} NAME)
    source_group(${name} FILES ${src})
endfunction()

find_package(Threads REQUIRED)

sources_in_dir_recurse(src/ge/common common_sources)
add_library(common ${common_sources})
target_compile_options(common PRIVATE ${compile_options})
set_target_properties(common PROPERTIES ${target_properties} OUTPUT_NAME ge_common)
target_include_directories(common
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::common ALIAS common)
define_source_group(common)

sources_in_dir_recurse(src/ge/geometry geometry_sources)
add_library(geometry ${geometry_sources})
target_compile_options(geometry PRIVATE ${compile_options})
set_target_properties(geometry PROPERTIES ${target_properties} OUTPUT_NAME ge_geometry)
target_include_directories(geometry
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::geometry ALIAS geometry)
define_source_group(geometry)

if (WIN32)
elseif(UNIX)
    set(window_libs
        xcb
        xcb-util
        xcb-errors
        xcb-keysyms
        xcb-icccm
    )
endif()

sources_in_dir_recurse(src/ge/window window_sources)
add_library(window ${window_sources})
target_compile_options(window PRIVATE ${compile_options})
set_target_properties(window PROPERTIES ${target_properties} OUTPUT_NAME ge_window)
target_compile_definitions(window PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(window
    PRIVATE
        CONAN_PKG::glm
        CONAN_PKG::vulkan-loader
        ${window_libs}
)
target_include_directories(window
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::window ALIAS window)
define_source_group(window)

sources_in_dir_recurse(src/ge/shader_compiler shader_compiler_sources)
add_executable(ge_shader_compiler ${shader_compiler_sources})
target_compile_options(ge_shader_compiler PRIVATE ${compile_options})
set_target_properties(ge_shader_compiler PROPERTIES ${target_properties})
target_link_libraries(ge_shader_compiler
    PRIVATE
        CONAN_PKG::vulkan-headers
        CONAN_PKG::shaderc
        Threads::Threads
        ${std_libs}
)
define_source_group(ge_shader_compiler)

file(GLOB shaders_sources src/ge/render/shaders/*)
set(generated_shaders_sources
    "${CMAKE_CURRENT_BINARY_DIR}/generated_shaders/generated_shaders.h"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_shaders/generated_shaders.cpp"
)
add_custom_command(
    OUTPUT
        ${generated_shaders_sources}
    COMMAND
        ge_shader_compiler
            "${CMAKE_CURRENT_SOURCE_DIR}/src/ge/render/shaders"
            ge
            "${CMAKE_CURRENT_BINARY_DIR}/generated_shaders"
            generated_shaders
    DEPENDS
        ge_shader_compiler
        ${shaders_sources}
)

sources_in_dir_recurse(src/ge/render render_sources)
if (${GE_ENABLE_DEBUG_LAYERS})
    set(VK_LAYER_PATH "${CONAN_RES_DIRS_VULKAN-VALIDATIONLAYERS}/vulkan/explicit_layer.d")
    set(template "src/ge/render/vk_layer_path.h.in")
    set(generated "${CMAKE_CURRENT_BINARY_DIR}/generated_include/vk_layer_path.h")
    configure_file(${template} ${generated})
    list(APPEND render_sources ${template} ${generated})
endif()
add_library(render ${render_sources} ${shaders_sources} ${generated_shaders_sources})
target_compile_options(render PRIVATE ${compile_options})
set_target_properties(render PROPERTIES ${target_properties} OUTPUT_NAME ge_render)
target_compile_definitions(render
    PRIVATE
        -DVK_USE_PLATFORM_XCB_KHR=1
        #-DVULKAN_HPP_DISABLE_IMPLICIT_RESULT_VALUE_CAST=1
)
if (${GE_ENABLE_DEBUG_LAYERS})
    target_compile_definitions(render PUBLIC -DGE_DEBUG_LAYERS_ENABLED=1)
endif()

target_link_libraries(render
    PUBLIC
        CONAN_PKG::glm
    PRIVATE
        CONAN_PKG::vulkan-headers
        CONAN_PKG::vulkan-loader
        Threads::Threads
        ${std_libs}
        dl
)
target_include_directories(render
    SYSTEM PRIVATE
        $<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS_SHADERC}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_shaders>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_shaders>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::render ALIAS render)
define_source_group(render)

sources_in_dir_recurse(src/ge/render_loop render_loop_sources)
add_library(render_loop ${render_loop_sources})
target_compile_options(render_loop PRIVATE ${compile_options})
set_target_properties(render_loop PROPERTIES ${target_properties} OUTPUT_NAME ge_render_loop)
target_include_directories(render_loop
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(render_loop
    PRIVATE
        GE::render
        GE::window
        ${std_libs}
)
add_library(GE::render_loop ALIAS render_loop)
define_source_group(render_loop)

install(
    TARGETS
        window
        render
        render_loop
        geometry
    EXPORT
        GraphicsEngine
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(
    FILES
        src/ge/render/render.h
        src/ge/render/vertex.h
    DESTINATION
        include/ge/render
)
install(
    FILES
        src/ge/window/window.h
        src/ge/window/window_events.h
        src/ge/window/window_size.h
    DESTINATION
        include/ge/window
)
install(
    FILES
        src/ge/render_loop/render_loop.h
    DESTINATION
        include/ge/render_loop
)
install(
    DIRECTORY
        src/ge/geometry
    DESTINATION
        include/ge
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
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

    sources_in_dir_recurse(tests/common common_tests_sources)
    add_executable(tests_common ${common_tests_sources})
    target_link_libraries(tests_common
        CONAN_PKG::gtest
        CONAN_PKG::vulkan-headers
        GE::common
        GE::geometry
        GE::render
        Threads::Threads
        ${std_libs}
    )
    target_compile_options(tests_common PRIVATE ${compile_options})
    set_target_properties(tests_common PROPERTIES ${target_properties})
    define_source_group(tests_common)
    gtest_discover_tests(tests_common)

    if (${GE_BUILD_GRAPHICS_TESTS})
        sources_in_dir_recurse(tests/graphics graphics_tests_sources)
        add_executable(tests_graphics ${graphics_tests_sources})
        target_compile_options(tests_graphics PRIVATE ${compile_options})
        set_target_properties(tests_graphics PROPERTIES ${target_properties})
        target_link_libraries(tests_graphics
            GE::window
            GE::render
            CONAN_PKG::gtest
            CONAN_PKG::vulkan-headers
            ${std_libs}
        )
        target_include_directories(tests_graphics
            PRIVATE
                "tests/graphics"
                "${CMAKE_CURRENT_BINARY_DIR}/generated_include"
        )
        define_source_group(tests_graphics)
        gtest_discover_tests(tests_graphics)
    endif()
endif()

if (${GE_BUILD_EXAMPLES})
    add_executable(example_2d_polygons examples/polygons/main.cpp)
    target_compile_options(example_2d_polygons PRIVATE ${compile_options})
    set_target_properties(example_2d_polygons PROPERTIES ${target_properties})
    target_link_libraries(example_2d_polygons
        GE::render
        GE::window
        GE::render_loop
        CONAN_PKG::vulkan-headers
        ${std_libs}
    )
    target_include_directories(example_2d_polygons PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
    define_source_group(example_2d_polygons)
    install(TARGETS example_2d_polygons DESTINATION examples/2d_polygons)

    add_executable(example_hex_grid examples/hex_grid/main.cpp)
    target_compile_options(example_hex_grid PRIVATE ${compile_options})
    set_target_properties(example_hex_grid PROPERTIES ${target_properties})
    target_link_libraries(example_hex_grid
        GE::render
        GE::window
        GE::render_loop
        GE::geometry
        CONAN_PKG::vulkan-headers
        ${std_libs}
    )
    target_include_directories(example_hex_grid PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
    define_source_group(example_hex_grid)
    install(TARGETS example_hex_grid DESTINATION examples/hex_grid)

    add_executable(example_square_grid examples/square_grid/main.cpp)
    target_compile_options(example_square_grid PRIVATE ${compile_options})
    set_target_properties(example_square_grid PROPERTIES ${target_properties})
    target_link_libraries(example_square_grid
        GE::render
        GE::window
        GE::render_loop
        GE::geometry
        CONAN_PKG::vulkan-headers
        ${std_libs}
    )
    target_include_directories(example_square_grid PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
    define_source_group(example_square_grid)
    install(TARGETS example_square_grid DESTINATION examples/square_grid)

    add_executable(example_graph examples/graph/main.cpp)
    target_compile_options(example_graph PRIVATE ${compile_options})
    set_target_properties(example_graph PROPERTIES ${target_properties})
    target_link_libraries(example_graph
        GE::render
        GE::window
        GE::render_loop
        CONAN_PKG::vulkan-headers
        ${std_libs}
    )
    target_include_directories(example_graph PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated_include")
    define_source_group(example_graph)
    install(TARGETS example_graph DESTINATION examples/graph)
endif()
