#pragma once

#include "2d_graph.h"
#include "shaders.h"
#include "graph_in_device_memory.h"

#include "ge/common/logger.hpp"

#include "ge/render/render_i.h"
#include "ge/render/render_base.h"

#include "ge/render/vulkan_common/buffer.h"

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    class Render2dGraph final : public RenderBase, public virtual Render2dI
    {
    public:
        explicit Render2dGraph(const SurfaceParams&, const Logger&);
        ~Render2dGraph() override;

        const Camera2d& get_camera() const override;
        void set_camera(Camera2d) override;

        void resize(const Extent<uint32_t> new_surface_extent) override;

        void set_object_to_draw(const Graph&);

    private:
        void create_pipelines() override;
        void create_command_buffers() override;
        void update_uniform_buffer(size_t current_image) override;

        Camera2d camera_;

        vk::UniqueDescriptorSetLayout descriptor_set_layout_;
        vk::UniquePipelineLayout pipeline_layout_;
        Shaders shaders_;
        vk::UniqueDescriptorPool descriptor_pool_;
        vk::UniquePipeline arcs_pipeline_;
        vk::UniquePipeline vertices_pipeline_;

        std::optional<GraphInDeviceMemory> graph_in_device_memory_;

        std::vector<BufferData> uniform_buffers_;
        std::vector<vk::UniqueDescriptorSet> descriptor_sets_;
    };
}
