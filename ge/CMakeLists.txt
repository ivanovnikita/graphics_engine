cmake_minimum_required(VERSION 3.9)

project("ge")

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h*" "include/*.h*")
set(INCLUDE_DIRS
    "src"
    "src/window"
    "src/factory"
    "src/factory/instance"
    "src/factory/device"
    "src/factory/device/logical"
    "src/factory/device/physical"
    "src/factory/swapchain"
    "src/factory/image_view"
)
set(API_DIRS "include")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

find_package(Vulkan REQUIRED)

if (WIN32)
else()
    find_package(XCB REQUIRED)
    set(WINDOW_SYSTEM XCB::XCB)
endif()

add_library(${PROJECT_NAME} ${SRC})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${WINDOW_SYSTEM})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${API_DIRS})
