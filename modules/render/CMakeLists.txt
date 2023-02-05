file(GLOB shaders_sources ge/render/shaders/*)
# TODO compile each shader into separate file
set(generated_shaders_sources
    "${CMAKE_CURRENT_BINARY_DIR}/generated_shaders/generated_shaders.h"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_shaders/generated_shaders.cpp"
)
add_custom_command(
    OUTPUT
        ${generated_shaders_sources}
    COMMAND
        ge_shader_compiler
            "${CMAKE_CURRENT_SOURCE_DIR}/ge/render/shaders"
            ge
            "${CMAKE_CURRENT_BINARY_DIR}/generated_shaders"
            generated_shaders
    DEPENDS
        ge_shader_compiler
        ${shaders_sources}
)

sources_in_dir_recurse(ge/render render_sources)
if (${GE_ENABLE_DEBUG_LAYERS})
    set(VK_LAYER_PATH "${CONAN_VULKAN-VALIDATIONLAYERS_ROOT}/res/vulkan/explicit_layer.d")
    set(template "ge/render/vk_layer_path.h.in")
    set(generated "${CMAKE_CURRENT_BINARY_DIR}/generated_include/vk_layer_path.h")
    configure_file(${template} ${generated})
    list(APPEND render_sources ${template} ${generated})
endif()
add_library(render ${render_sources} ${shaders_sources} ${generated_shaders_sources})
target_compile_options(render PRIVATE ${GE_COMPILE_OPTIONS})
set_target_properties(render PROPERTIES ${GE_TARGET_PROPERTIES} OUTPUT_NAME ge_render)
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
        GE::common
        CONAN_PKG::glm
        CONAN_PKG::vulkan-headers
    PRIVATE
        CONAN_PKG::vulkan-loader
        Threads::Threads
        ${std_libs}
        dl
)
target_include_directories(render
    SYSTEM PRIVATE
        $<BUILD_INTERFACE:${CONAN_INCLUDE_DIRS_SHADERC}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_shaders>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_shaders>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_include>
        $<INSTALL_INTERFACE:include>
)
add_library(GE::render ALIAS render)
define_source_group(render)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    sources_in_dir_recurse(tests tests_sources)
    add_executable(tests_render ${tests_sources})
    target_link_libraries(tests_render
        CONAN_PKG::gtest
        GE::render
        ${std_libs}
    )
    target_compile_options(tests_render PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_render PROPERTIES ${GE_TARGET_PROPERTIES})
    define_source_group(tests_render)
    gtest_discover_tests(tests_render)
endif()

install(
    TARGETS
        render
    EXPORT
        GraphicsEngine
    DESTINATION lib
)
install(
    FILES
        ge/render/render_i.h
        ge/render/render_impl.h
        ge/render/vertex.h
        ge/render/2d_graph/2d_graph.h
        ge/render/2d_graph/render_2d_graph.h
    DESTINATION
        include/ge/render
)
install(
    DIRECTORY
        ${CONAN_LIB_DIRS_VULKAN-LOADER}/
    DESTINATION
        lib
    FILES_MATCHING
        PATTERN "libvulkan.so*"
)
if (${GE_ENABLE_DEBUG_LAYERS})
    install(
        FILES
            "${CONAN_LIB_DIRS_VULKAN-VALIDATIONLAYERS}/libVkLayer_khronos_validation.so"
        DESTINATION
            lib
    )
endif()
