cmake_minimum_required(VERSION 3.0)

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

set(VULKAN_LOADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/extern/vulkan")
file(MAKE_DIRECTORY ${VULKAN_LOADER_DIR})
file(COPY "extern/Vulkan-Docs/src/ext_loader/" DESTINATION ${VULKAN_LOADER_DIR})
file(GLOB VULKAN_LODAER_SRC "${VULKAN_LOADER_DIR}/*")

add_executable(${PROJECT_NAME} ${SRC_C} ${VULKAN_LODAER_SRC})
targeT_compile_definitions(${PROJECT_NAME} PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan -ldl ${WINDOW_SYSTEM})
target_include_directories(${PROJECT_NAME} PRIVATE "src" "src/window")
target_include_directories(${PROJECT_NAME} PRIVATE ${VULKAN_LOADER_DIR})
