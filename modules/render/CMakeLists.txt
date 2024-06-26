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

set(generated_sources_dir "${CMAKE_CURRENT_BINARY_DIR}/generated_src")
set(generated_vulkan_sources
    "${generated_sources_dir}/to_string_view_enum.h"
    "${generated_sources_dir}/to_string_view_enum.cpp"
    "${generated_sources_dir}/vulkan_fwds.h"
    "${generated_sources_dir}/vulkan_refl.hpp"
    "${generated_sources_dir}/invoke_for_downcasted.h"
)
set(refl_generator_script "${CMAKE_SOURCE_DIR}/code_generators/generate_vulkan_reflection.py")
list (GET VulkanHeaders_INCLUDE_DIRS 0 vulkan_headers_include_dir)
add_custom_command(
    OUTPUT
        ${generated_vulkan_sources}
    COMMAND
        "${refl_generator_script}"
            -i
                "${vulkan_headers_include_dir}/vk.xml"
            -o
                "${generated_sources_dir}"
    DEPENDS
        ${refl_generator_script}
)

sources_in_dir_recurse(ge/render render_sources)
if (${GE_ENABLE_DEBUG_LAYERS})
    set(VK_LAYER_PATH "${vulkan-validationlayers_INCLUDE_DIR}/../res/vulkan/explicit_layer.d")
    set(template "ge/render/vk_layer_path.h.in")
    set(generated "${generated_sources_dir}/vk_layer_path.h")
    configure_file(${template} ${generated})
    list(APPEND render_sources ${template} ${generated})
endif()
list(APPEND render_sources ${generated_vulkan_sources})
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
        GE::geometry
        GE::io
        GE::window
        glm::glm
        vulkan-headers::vulkan-headers
    PRIVATE
        Vulkan::Vulkan
        Threads::Threads
        ${std_libs}
        dl
)
target_include_directories(render
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_shaders>
        $<BUILD_INTERFACE:${generated_sources_dir}>
    SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated_shaders>
        $<BUILD_INTERFACE:${generated_sources_dir}>
        $<INSTALL_INTERFACE:include>
)
target_precompile_headers(render
    PUBLIC
        <vulkan/vulkan.hpp>
)
add_library(GE::render ALIAS render)
define_source_group(render)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    sources_in_dir_recurse(tests tests_sources)
    add_executable(tests_render ${tests_sources})
    target_link_libraries(tests_render
        gtest::gtest
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
list (GET Vulkan_INCLUDE_DIRS 0 vulkan_include_dir)
install(
    DIRECTORY
        "vulkan_include_dir/../lib/"
    DESTINATION
        bin
    FILES_MATCHING
        PATTERN "libvulkan.so*"
)
if (${GE_ENABLE_DEBUG_LAYERS})
    install(
        FILES
            "${vulkan-validationlayers_INCLUDE_DIR}/../lib/libVkLayer_khronos_validation.so"
        DESTINATION
            bin
    )
endif()
