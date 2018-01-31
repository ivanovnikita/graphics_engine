cmake_minimum_required(VERSION 3.9)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_SRC "*.cpp" "*.h*")

set(TEST "${CMAKE_PROJECT_NAME}-test")
add_executable(${TEST} ${TEST_SRC})
target_link_libraries(${TEST} ge GTest::Main Threads::Threads)

add_test(NAME ${TEST} COMMAND ${TEST})
