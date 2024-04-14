module;

#include <vulkan/vulkan.hpp>

export module render_base;

export import render_i;

export import vulkan_common.create_antialiasing;
export import vulkan_common.instance;
export import vulkan_common.surface;
export import vulkan_common.device;
export import vulkan_common.swapchain;
export import vulkan_common.depth_buffer;
export import logger;

namespace ge
{
    export class RenderBase : public virtual DrawableI
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
