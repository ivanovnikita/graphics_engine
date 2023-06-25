#pragma once

#include "shaders.h"

#include "ge/common/logger.hpp"

#include "ge/render/render_i.h"

#include "ge/render/instance.h"
#include "ge/render/surface.h"
#include "ge/render/device.h"
#include "ge/render/swapchain.h"
#include "ge/render/buffer.h"

#include "ge/render/2d_graph/2d_graph.h"

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    class Render2dGraph final : public RenderI
    {
    public:
        explicit Render2dGraph(const SurfaceParams&, const Logger&);
        ~Render2dGraph() override;

        void draw_frame() override;
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height) override;

        const Camera2d& get_camera() const override;
        void set_camera(Camera2d) override;

        void set_object_to_draw(const Graph&);

    private:
        InstanceData instance_data_;
        SurfaceData surface_data_;
        Camera2d camera_;
        DeviceData device_data_;
        SwapchainData swapchain_data_;

        vk::UniqueDescriptorSetLayout descriptor_set_layout_;
        vk::UniquePipelineLayout pipeline_layout_;
        std::vector<BufferData> uniform_buffers_;
        vk::UniqueDescriptorPool descriptor_pool_;
        std::vector<vk::UniqueDescriptorSet> descriptor_sets_;

        vk::UniqueRenderPass render_pass_;
        Shaders shaders_;
        vk::UniquePipeline arcs_pipeline_;
        vk::UniquePipeline vertices_pipeline_;
    };
}
