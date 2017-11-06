#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    enum class WindowOptions
    {
        NONE
      , XCB
      , WIN32
    };

    template
    <
        bool debug_callback_enabled
      , bool validation_layers_enabled
      , WindowOptions window_options
    >
    vk::UniqueInstance create();

} // namespace ge::impl::factory::instance

#include "instance_factory_impl.hpp"
