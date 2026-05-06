#pragma once

#include <vulkan/vulkan.hpp>

#include "ge/common/exception_macro.h"

namespace ge
{
#define GE_THROW_EXPECTED_RESULT(vk_result, message) \
    GE_THROW_ERROR(expected_error, message, vk::to_string(vk_result))

#define GE_THROW_UNEXPECTED_RESULT(vk_result, message) \
    GE_THROW_ERROR(unexpected_error, message, vk::to_string(vk_result))
}
