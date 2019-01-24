#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device
{

    std::vector<const char*> get_required_device_extensions();

}

