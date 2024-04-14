module;

#include <vulkan/vulkan.hpp>

#include <span>

export module image_2d.draw_image_commands;

export import image_2d.polygons_in_device_memory;

namespace ge::image2d
{
    export std::vector<vk::UniqueCommandBuffer> draw_image_commands
    (
        const vk::Device&,
        const vk::CommandPool&,
        const std::span<const vk::UniqueFramebuffer>&,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline&,
        const vk::PipelineLayout&,
        const std::span<const vk::UniqueDescriptorSet>,
        const PolygonsInDeviceMemory&
    );
}
