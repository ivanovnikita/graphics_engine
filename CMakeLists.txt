cmake_minimum_required(VERSION 3.9)

project("graphics_engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h*")
set(INCLUDE_DIRS
    "src"
    "src/window"
    "src/factory"
    "src/factory/instance"
    "src/factory/device"
    "src/factory/device/logical"
    "src/factory/device/physical"
    "src/factory/swapchain"
    "src/factory/image_view")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

find_package(Vulkan REQUIRED)

if (WIN32)
else()
    find_package(XCB REQUIRED)
    set(WINDOW_SYSTEM XCB::XCB)
endif()

set(SHADERC_THIRD_PARTY_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party" CACHE STRING "Third party path." FORCE)
set(SPIRV-Headers_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/spirv-headers" CACHE STRING "SPIRV-Headers path")
set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_INSTALL ON)
set(SPIRV_SKIP_EXECUTABLES ON)
set(BUILD_TESTING OFF)
set(ENABLE_GLSLANG_BINARIES OFF)
add_subdirectory(third_party/shaderc)

add_executable(${PROJECT_NAME} ${SRC})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${WINDOW_SYSTEM})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
