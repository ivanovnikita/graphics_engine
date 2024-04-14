module;

#include <vulkan/vulkan.hpp>

export module image_2d.render_image_2d;

export import image_2d.shaders;
export import image_2d.polygons_in_device_memory;
export import logger;
export import io.image;
export import render_i;
export import render_base;
export import vulkan_common.texture_image;

namespace ge::image2d
{
    export class RenderImage2d final : public RenderBase, public virtual Render2dI
    {
    public:
        explicit RenderImage2d(const SurfaceParams&, const Logger&);
        ~RenderImage2d() override;

        const Camera2d& get_camera() const override;
        void set_camera(Camera2d) override;

        void resize(const Extent<uint32_t> new_surface_extent) override;

        void set_object_to_draw(const std::span<const Polygons>&, const Image&);

    private:
        void create_pipelines() override;
        void create_command_buffers() override;
        void update_uniform_buffer(size_t current_image) override;

        Camera2d camera_;

        vk::UniqueDescriptorSetLayout descriptor_set_layout_;
        vk::UniquePipelineLayout pipeline_layout_;
        Shaders shaders_;
        vk::UniqueDescriptorPool descriptor_pool_;
        vk::UniquePipeline image_pipeline_;

        std::optional<PolygonsInDeviceMemory> polygons_in_device_memory_;
        std::optional<TextureImageData> texture_image_data_;

        std::vector<BufferData> uniform_buffers_;
        std::vector<vk::UniqueDescriptorSet> descriptor_sets_;
    };
}
