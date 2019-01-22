#pragma once

#include "window/window.h"
#include "queue.h"

namespace ge::impl
{
    class GraphicsEngineImpl final
    {
    public:
        GraphicsEngineImpl();
        void draw_frame();
        void main_loop();
    private:
        vk::UniqueDebugReportCallbackEXT create_debug_callback() const;
    private:
        vk::UniqueInstance                  instance_;
        vk::UniqueDebugReportCallbackEXT    debug_callback_;
        std::unique_ptr<Window>             window_;
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
    };

}
