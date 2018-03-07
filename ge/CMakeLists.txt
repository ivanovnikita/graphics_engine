cmake_minimum_required(VERSION 3.9)

project("ge")

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.h*" "include/*.h*")
set(INCLUDE_DIRS "src")
set(API_DIRS "include")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Vulkan REQUIRED)

if (WIN32)
else()
    find_package(XCB REQUIRED)
    set(WINDOW_SYSTEM XCB::XCB)
endif()

add_library(${PROJECT_NAME} ${SRC})
target_compile_options(${PROJECT_NAME} PRIVATE
    -Werror -Wall -Wextra
    -pedantic -pedantic-errors
    )
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wstrict-overflow=5
        -Wfloat-equal
        -Wshadow
        -Wpointer-arith
        -Wcast-qual
        -Wconversion
        -Wsign-conversion
        -Wlogical-op
        -Wmissing-declarations
        -Wredundant-decls
        -Winline
        -Wno-return-local-addr
        -Wsuggest-final-types
        -Wsuggest-final-methods
        -Wsuggest-override
        -Wduplicated-branches
        -Wduplicated-cond
        -Wno-unused-but-set-variable
        )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Weverything
        -Wno-c++98-compat
        -Wno-c++11-compat
        -Wno-c++14-compat
        -Wno-unreachable-code
        )
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE -DVK_USE_PLATFORM_XCB_KHR=1)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${WINDOW_SYSTEM} CONAN_PKG::shaderc)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${API_DIRS})
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CONAN_INCLUDE_DIRS_SHADERC})
