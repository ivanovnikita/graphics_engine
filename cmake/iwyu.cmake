
if (${GE_ENABLE_IWYU})

    find_program(IWYU_EXECUTABLE NAMES include-what-you-use iwyu REQIRED)

    set(IWYU_ARGS "${IWYU_EXECUTABLE}")

    set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${IWYU_ARGS})
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_ARGS})

endif()
