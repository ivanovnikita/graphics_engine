cmake_minimum_required(VERSION 3.9)

file(GLOB_RECURSE TEST_SRC "*.cpp" "*.h*")

set(TEST_COVERAGE "${CMAKE_PROJECT_NAME}-test-coverage")
add_executable(${TEST_COVERAGE} ${TEST_SRC})
target_link_libraries(${TEST_COVERAGE} ge gtest_main)

add_test(NAME "Tests coverage" COMMAND ${TEST_COVERAGE})
