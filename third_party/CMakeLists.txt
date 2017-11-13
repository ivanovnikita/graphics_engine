cmake_minimum_required(VERSION 3.9)

add_subdirectory(googletest)

set(SHADERC_THIRD_PARTY_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Third party path." FORCE)
set(SPIRV-Headers_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv-headers" CACHE STRING "SPIRV-Headers path")
set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_INSTALL ON)
set(SPIRV_SKIP_EXECUTABLES ON)
set(BUILD_TESTING OFF)
set(ENABLE_GLSLANG_BINARIES OFF)
add_subdirectory(shaderc)
