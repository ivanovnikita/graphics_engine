cmake_minimum_required(VERSION 3.9)

project(graphics_engine)

option(GE_BUILD_TESTS "Build internal tests?" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)
add_subdirectory(ge)

if (${GE_BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()
