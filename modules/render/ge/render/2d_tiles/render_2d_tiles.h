#pragma once

#include "shaders.h"
#include "polygons_in_device_memory.h"

#include "ge/common/logger.hpp"

#include "ge/render/render_i.h"

#include "ge/render/instance.h"
#include "ge/render/surface.h"
#include "ge/render/device.h"
#include "ge/render/swapchain.h"
#include "ge/render/buffer.h"

#include <vulkan/vulkan.hpp>

namespace ge::tiles
{
    class Render2dTiles final : public RenderI
    {
    public:
        explicit Render2dTiles(const SurfaceParams&, const Logger&);
        ~Render2dTiles() override;

        void draw_frame() override;
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height) override;

        const Camera2d& get_camera() const override;
        void set_camera(Camera2d) override;

        void set_object_to_draw(const std::span<const Polygons>&);

    private:
        void create_command_buffers();
        void update_uniform_buffer(size_t current_image);

        std::reference_wrapper<const Logger> logger_;

        InstanceData instance_data_;
        SurfaceData surface_data_;
        Camera2d camera_;
        DeviceData device_data_;

        vk::UniqueDescriptorSetLayout descriptor_set_layout_;
        vk::UniquePipelineLayout pipeline_layout_;
        Shaders shaders_;

        SwapchainData swapchain_data_;

        std::vector<BufferData> uniform_buffers_; // for Camera2D
        vk::UniqueDescriptorPool descriptor_pool_;
        std::vector<vk::UniqueDescriptorSet> descriptor_sets_;

        vk::UniqueRenderPass render_pass_;

        vk::UniquePipeline triangles_pipeline_;
        vk::UniquePipeline lines_pipeline_;
        std::vector<vk::UniqueFramebuffer> framebuffers_;

        vk::UniqueSemaphore image_available_semaphore_;
        vk::UniqueSemaphore render_finished_semaphore_;
        vk::UniqueFence render_finished_fence_;
        vk::UniqueFence transfer_finished_fence_;

        vk::UniqueCommandPool command_pool_;

        std::optional<PolygonsInDeviceMemory> polygons_in_device_memory_;

        std::vector<vk::UniqueCommandBuffer> command_buffers_;
    };
}
