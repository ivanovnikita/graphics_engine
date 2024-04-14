module;

#include <vulkan/vulkan.hpp>

export module graph_2d.render_graph_2d;

export import graph_2d.graph_2d;
export import graph_2d.shaders;
export import graph_2d.graph_in_device_memory;
export import logger;
export import render_i;
export import render_base;
export import vulkan_common.buffer;

namespace ge::graph
{
    export class RenderGraph2d final : public RenderBase, public virtual Render2dI
    {
    public:
        explicit RenderGraph2d(const SurfaceParams&, const Logger&);
        ~RenderGraph2d() override;

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
