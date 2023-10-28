#pragma once

#include "ge/common/exception.h"
#include "to_string_view_enum.h"

#include <vulkan/vulkan.hpp>

#include <stdexcept>
#include <string_view>

namespace ge
{
#define GE_THROW_EXPECTED_RESULT(vk_result, message) \
    GE_THROW_ERROR(expected_error, message, vk::to_string_view(vk_result))

#define GE_THROW_UNEXPECTED_RESULT(vk_result, message) \
    GE_THROW_ERROR(unexpected_error, message, vk::to_string_view(vk_result))
}
