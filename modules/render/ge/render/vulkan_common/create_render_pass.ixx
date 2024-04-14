module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

export module vulkan_common.create_render_pass;

import vulkan_common.exception;

namespace ge
{
    export vk::UniqueRenderPass create_render_pass(const vk::Device&, const vk::RenderPassCreateInfo&);
}

module : private;

namespace ge
{
    vk::UniqueRenderPass create_render_pass
    (
        const vk::Device& device,
        const vk::RenderPassCreateInfo& create_info
    )
    {
        vk::RenderPass render_pass;
        const vk::Result result = device.createRenderPass
        (
            &create_info,
            nullptr,
            &render_pass
        );
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Render pass creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Render pass creation failed");
        }
        }

        return vk::UniqueRenderPass
        {
            std::move(render_pass),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
