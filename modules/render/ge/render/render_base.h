#pragma once

#include "ge/render/render_i.h"

#include "ge/render/antialiasing/antialiasing.h"
#include "ge/render/vulkan_common/instance.h"
#include "ge/render/vulkan_common/surface.h"
#include "ge/render/vulkan_common/device.h"
#include "ge/render/vulkan_common/swapchain.h"
#include "ge/render/vulkan_common/depth_buffer.h"

#include "ge/common/logger.hpp"

#include <vulkan/vulkan.hpp>

namespace ge
{
    class RenderBase : public virtual DrawableI
    {
    public:
        explicit RenderBase
        (
            const SurfaceParams&,
            const Logger&,
            DeviceFeaturesFlags,
            vk::Format desired_surface_format,
            const Antialiasing&
        );
        ~RenderBase() override;

        void draw_frame() override;
        void resize(const Extent<uint32_t> new_surface_extent) override;

    protected:
        virtual void create_pipelines() = 0;
        virtual void create_command_buffers() = 0;
        virtual void update_uniform_buffer(size_t current_image) = 0;

        std::reference_wrapper<const Logger> logger_;

        InstanceData instance_data_;
        SurfaceData surface_data_;
        DeviceData device_data_;

        Antialiasing antialiasing_;

        vk::UniqueSemaphore image_available_semaphore_;
        vk::UniqueSemaphore render_finished_semaphore_;
        vk::UniqueFence render_finished_fence_;
        vk::UniqueFence transfer_finished_fence_;

        vk::UniqueCommandPool command_pool_;

        vk::Format desired_surface_format_;
        SwapchainData swapchain_data_;
        DepthBuffer depth_buffer_;
        std::optional<ImageData> msaa_data_;
        vk::UniqueRenderPass render_pass_;

        std::vector<vk::UniqueFramebuffer> framebuffers_;

        std::vector<vk::UniqueCommandBuffer> command_buffers_;
    };
}
