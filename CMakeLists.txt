cmake_minimum_required(VERSION 3.9)

project("graphics_engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_C "src/*")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_FIND_MODULE_DIR})

find_package(Vulkan)

if (WIN32)
else()
    find_package(XCB)
    set(WINDOW_SYSTEM XCB::XCB)
endif()

add_executable(${PROJECT_NAME} ${SRC_C})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${WINDOW_SYSTEM})
target_include_directories(${PROJECT_NAME} PRIVATE "src" "src/window")
