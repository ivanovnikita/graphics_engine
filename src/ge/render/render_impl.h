#pragma once

#include "ge/render/render.h"
#include "ge/render/queue.h"

namespace ge
{
    class Render::RenderImpl final
    {
    public:
        RenderImpl
        (
            const std::function<SurfaceCreator>&
          , vk::Extent2D surface_extent
        );
        ~RenderImpl();
        void draw_frame();

    private:
        vk::UniqueDebugReportCallbackEXT create_debug_callback() const;

        vk::UniqueInstance                  instance_;
        vk::UniqueDebugReportCallbackEXT    debug_callback_;
        vk::Extent2D                        surface_extent_;
        vk::UniqueSurfaceKHR                surface_;
        vk::PhysicalDevice                  physical_device_;
        QueueFamilyIndices                  queue_family_indices_;
        vk::UniqueDevice                    logical_device_;
        Queues                              queues_;
        vk::UniqueSwapchainKHR              swapchain_;
        std::vector<vk::Image>              images_;
        std::vector<vk::UniqueImageView>    image_views_;
        vk::UniqueRenderPass                render_pass_;
        vk::UniquePipelineLayout            pipeline_layout_;
        vk::UniquePipeline                  pipeline_;
        std::vector<vk::UniqueFramebuffer>  framebuffers_;
        vk::UniqueCommandPool               command_pool_;
        std::vector<vk::CommandBuffer>      command_buffers_;
        vk::UniqueSemaphore                 image_available_semaphore_;
        vk::UniqueSemaphore                 render_finished_semaphore_;
        vk::UniqueFence                     render_finished_fence_;
    };
}
