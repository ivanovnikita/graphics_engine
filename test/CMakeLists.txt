cmake_minimum_required(VERSION 3.9)

file(GLOB_RECURSE TEST_SRC "*.cpp" "*.h*")

set(TEST "${CMAKE_PROJECT_NAME}-test")
add_executable(${TEST} ${TEST_SRC})
target_link_libraries(${TEST} ge gtest_main)

add_test(NAME ${TEST} COMMAND ${TEST})
