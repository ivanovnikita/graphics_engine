file(GLOB shaders_sources ge/render/shaders/*)
# TODO compile each shader into separate file
set(generated_shaders_interfaces
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/generated_shaders.ixx"
)
set(generated_shaders_implementations
    "${CMAKE_CURRENT_SOURCE_DIR}/generated/generated_shaders.cxx"
)
add_custom_command(
    OUTPUT
        ${generated_shaders_interfaces}
        ${generated_shaders_implementations}
    COMMAND
        ge_shader_compiler
            "${CMAKE_CURRENT_SOURCE_DIR}/ge/render/shaders"
            ge
            "${CMAKE_CURRENT_SOURCE_DIR}/generated"
            generated_shaders
    DEPENDS
        ge_shader_compiler
        ${shaders_sources}
)

set(generated_sources_dir "${CMAKE_CURRENT_SOURCE_DIR}/generated")
set(generated_vulkan_interfaces
    "${generated_sources_dir}/to_string_view_enum.ixx"
    "${generated_sources_dir}/vulkan_refl.ixx"
    "${generated_sources_dir}/invoke_for_downcasted.ixx"
)
set(generated_vulkan_implementations
    "${generated_sources_dir}/to_string_view_enum.cxx"
)
set(generated_vulkan_headers
    "${generated_sources_dir}/vulkan_fwds.h"
)
set(refl_generator_script "${CMAKE_SOURCE_DIR}/code_generators/generate_vulkan_reflection.py")
list (GET VulkanHeaders_INCLUDE_DIRS 0 vulkan_headers_include_dir)
add_custom_command(
    OUTPUT
        ${generated_vulkan_interfaces}
        ${generated_vulkan_implementations}
        ${generated_vulkan_headers}
    COMMAND
        "${refl_generator_script}"
            -i
                "${vulkan_headers_include_dir}/vk.xml"
            -o
                "${generated_sources_dir}"
    DEPENDS
        ${refl_generator_script}
)

interfaces_in_dir_recurse(ge/render interfaces)
implementations_in_dir_recurse(ge/render implementations)
headers_in_dir_recurse(ge/render headers)
legacy_sources_in_dir_recurse(ge/render legacy_sources)
if (${GE_ENABLE_DEBUG_LAYERS})
    set(VK_LAYER_PATH "${vulkan-validationlayers_INCLUDE_DIR}/../res/vulkan/explicit_layer.d")
    set(template "ge/render/vk_layer_path.ixx.in")
    set(generated "${generated_sources_dir}/vk_layer_path.ixx")
    configure_file(${template} ${generated})
    list(APPEND interfaces ${generated})
    list(APPEND implementations ${template})
endif()
add_library(render)
target_sources(render
    PRIVATE
        ${implementations}
        ${generated_shaders_implementations}
        ${generated_vulkan_implementations}
        ${shaders_sources}
        ${legacy_sources}
    PUBLIC
        FILE_SET CXX_MODULES FILES
            ${interfaces}
            ${generated_shaders_interfaces}
            ${generated_vulkan_interfaces}
        FILE_SET HEADERS FILES
            ${headers}
            ${generated_vulkan_headers}
)
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
add_library(GE::render ALIAS render)
source_group(render
    FILES
        ${interfaces}
        ${implementations}
        ${generated_shaders_interfaces}
        ${generated_shaders_implementations}
        ${generated_vulkan_interfaces}
        ${generated_vulkan_implementations}
        ${generated_vulkan_headers}
        ${shaders_sources}
        ${headers}
        ${legacy_sources}
)

if (${GE_BUILD_TESTS})
    enable_testing()

    include(GoogleTest)

    interfaces_in_dir_recurse(tests tests_interfaces)
    add_executable(tests_render)
    target_sources(tests_render
        PRIVATE
            FILE_SET CXX_MODULES FILES
                ${tests_interfaces}
    )
    target_link_libraries(tests_render
        gtest::gtest
        GE::render
        ${std_libs}
    )
    target_compile_options(tests_render PRIVATE ${GE_COMPILE_OPTIONS})
    set_target_properties(tests_render PROPERTIES ${GE_TARGET_PROPERTIES})
    source_group(tests_common FILES ${tests_interfaces})
    gtest_discover_tests(tests_render)
endif()

install(
    TARGETS
        render
    EXPORT
        GraphicsEngine
    DESTINATION lib
    FILE_SET HEADERS DESTINATION include
    FILE_SET CXX_MODULES DESTINATION cxx_modules
    CXX_MODULES_BMI DESTINATION cxx_modules/bmi
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
