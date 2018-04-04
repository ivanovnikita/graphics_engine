cmake_minimum_required(VERSION 3.9)

include(CMakeDependentOption)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

file(GLOB_RECURSE COMMON_TEST_SRC "common/*.cpp")

set(COMMON_TEST "${CMAKE_PROJECT_NAME}-common-test")
add_executable(${COMMON_TEST} ${COMMON_TEST_SRC})
target_link_libraries(${COMMON_TEST} ge GTest::Main Threads::Threads)

add_test(NAME ${COMMON_TEST} COMMAND ${COMMON_TEST})

install(TARGETS ${COMMON_TEST} DESTINATION bin)

cmake_dependent_option(GE_BUILD_GRAPHICS_TESTS "Build graphics tests?" ON
                       "GE_BUILD_TESTS" OFF)

if (${GE_BUILD_GRAPHICS_TESTS})
    set(VK_LAYER_PATH "${CONAN_VULKAN-LOADERANDVALIDATIONLAYERS_ROOT}/etc/vulkan/explicit_layer.d")
    configure_file("graphics/graphics_config.txt.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/graphics_config.txt")

    file(GLOB_RECURSE GRAPHICS_TEST_SRC "graphics/*.*")

    set(GRAPHICS_TEST "${CMAKE_PROJECT_NAME}-graphics-test")
    add_executable(${GRAPHICS_TEST} ${GRAPHICS_TEST_SRC})
    target_link_libraries(${GRAPHICS_TEST} ge GTest::Main Threads::Threads)
    target_include_directories(${GRAPHICS_TEST} PRIVATE "graphics")

    add_test(NAME ${GRAPHICS_TEST} COMMAND ${GRAPHICS_TEST})

    file(GLOB SHADERS "shaders/*")
    install(FILES ${SHADERS} DESTINATION bin/shaders)
    install(TARGETS ${GRAPHICS_TEST} DESTINATION bin)
endif()
