#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/render_i.h"
#include "ge/render/instance.h"
#include "ge/render/surface_params.h"
#include "ge/render/surface.h"
#include "ge/render/queue.h"
#include "ge/render/storage/shaders.h"
#include "ge/render/vertex.h"
#include "ge/render/camera_2d.h"
#include "ge/render/factory/buffer.hpp"
#include "ge/render/2d_graph/2d_graph.h"

namespace ge
{
    enum class DrawMode
    {
        POLYGONS,
        GRAPH
    };

    class RenderImpl final : public RenderI
    {
    public:
        RenderImpl
        (
            const SurfaceParams&,
            DrawMode,
            const Logger&
        );
        ~RenderImpl() override;

        void set_object_to_draw(const std::span<const Polygons>&);
        void set_object_to_draw(const Graph&);

        void draw_frame() override;
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height) override;

        const Camera2d& get_camera() const override;
        void set_camera(Camera2d) override;

    private:
        void create_uniform_buffers();
        void create_descriptor_sets();
        void create_graphics_pipeline();
        void create_command_buffers();
        void update_uniform_buffer(size_t current_image);

        DrawMode                            draw_mode_;

        InstanceData instance_data_;
        SurfaceData surface_data_;
        vk::PhysicalDevice                  physical_device_;
        QueueFamilyIndices                  queue_family_indices_;
        vk::UniqueDevice                    logical_device_;
        Queues                              queues_;

        storage::Shaders                    shaders_storage_;

        vk::UniqueSwapchainKHR              swapchain_;
        std::vector<vk::Image>              images_;
        std::vector<vk::UniqueImageView>    image_views_;

        vk::UniqueDescriptorSetLayout       camera_2d_descriptor_set_layout_;
        vk::UniquePipelineLayout            camera_2d_pipeline_layout_;
        std::vector<vk::UniqueDeviceMemory> camera_2d_uniform_buffers_memory_;
        std::vector<vk::UniqueBuffer>       camera_2d_uniform_buffers_;
        vk::UniqueDescriptorPool            descriptor_pool_;
        std::vector<vk::DescriptorSet>      descriptor_sets_;

        vk::UniqueRenderPass                render_pass_;

        vk::UniquePipeline                  triangles_pipeline_;
        vk::UniquePipeline                  lines_pipeline_;

        vk::UniquePipeline                  graph_acrs_pipeline_;
        vk::UniquePipeline                  graph_vertices_pipeline_;

        std::vector<vk::UniqueFramebuffer>  framebuffers_;

        factory::PolygonsInDeviceMemory     polygon_in_device_mem_;

        factory::GraphInDeviceMemory        graph_in_device_mem_;

        vk::UniqueCommandPool               command_pool_;
        std::vector<vk::CommandBuffer>      command_buffers_;

        vk::UniqueSemaphore                 image_available_semaphore_;
        vk::UniqueSemaphore                 render_finished_semaphore_;
        vk::UniqueFence                     render_finished_fence_;
        vk::UniqueFence                     transfer_finished_fence_;

        Camera2d                            camera_;
    };
}
