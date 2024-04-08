#pragma once

#define GE_THROW_ERROR(exception, ...) \
    throw exception(__func__, __LINE__, __VA_ARGS__)

#define GE_THROW_EXPECTED_ERROR(...) \
    GE_THROW_ERROR(expected_error, __VA_ARGS__)

#define GE_THROW_UNEXPECTED_ERROR(...) \
    GE_THROW_ERROR(unexpected_error, __VA_ARGS__)
