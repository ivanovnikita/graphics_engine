#pragma once

#include "ge/render/render.h"
#include "ge/render/queue.h"
#include "ge/render/storage/shaders.h"
#include "ge/render/vertex.h"

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
        void set_object_to_draw(const std::span<const Vertex>, const std::span<const uint16_t> indices);
        void draw_frame();
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height);

        void set_camera_pos(const glm::vec2&);
        void set_camera_scale(float);

    private:
        vk::UniqueDebugReportCallbackEXT create_debug_callback() const;
        void create_uniform_buffers();
        void create_descriptor_sets();
        void create_graphics_pipeline();
        void create_command_buffers();
        void update_uniform_buffer(size_t current_image);

        vk::UniqueInstance                  instance_;
        vk::UniqueDebugReportCallbackEXT    debug_callback_;
        vk::Extent2D                        surface_extent_;
        vk::UniqueSurfaceKHR                surface_;
        vk::PhysicalDevice                  physical_device_;
        QueueFamilyIndices                  queue_family_indices_;
        vk::UniqueDevice                    logical_device_;
        Queues                              queues_;

        storage::Shaders                    shaders_storage_;

        vk::UniqueSwapchainKHR              swapchain_;
        std::vector<vk::Image>              images_;
        std::vector<vk::UniqueImageView>    image_views_;

        vk::UniqueDescriptorSetLayout       descriptor_set_layout_;
        std::vector<vk::UniqueDeviceMemory> uniform_buffer_memory_;
        std::vector<vk::UniqueBuffer>       uniform_buffer_;
        vk::UniqueDescriptorPool            descriptor_pool_;
        std::vector<vk::DescriptorSet>      descriptor_sets_;

        vk::UniqueRenderPass                render_pass_;
        vk::UniquePipelineLayout            pipeline_layout_;
        vk::UniquePipeline                  pipeline_;
        std::vector<vk::UniqueFramebuffer>  framebuffers_;

        vk::UniqueDeviceMemory              vertex_buffer_memory_;
        vk::UniqueBuffer                    vertex_buffer_;
        vk::UniqueDeviceMemory              index_buffer_memory_;
        vk::UniqueBuffer                    index_buffer_;

        vk::UniqueCommandPool               command_pool_;
        std::vector<vk::CommandBuffer>      command_buffers_;

        vk::UniqueSemaphore                 image_available_semaphore_;
        vk::UniqueSemaphore                 render_finished_semaphore_;
        vk::UniqueFence                     render_finished_fence_;
        vk::UniqueFence                     transfer_finished_fence_;

        std::vector<Vertex>                 vertices_;
        std::vector<uint16_t>               indices_;

        glm::vec2                           camera_pos_;
        float                               camera_scale_;
    };
}
