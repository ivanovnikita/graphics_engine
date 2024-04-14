module;

#include <vulkan/vulkan.hpp>

export module image_3d.render_image_3d;

export import image_3d.shaders;
export import image_3d.polygons_in_device_memory;
export import logger;
export import io.image;
export import render_i;
export import render_base;
export import vulkan_common.texture_image;

namespace ge::image3d
{
    export class RenderImage3d final : public RenderBase, public virtual Render3dI
    {
    public:
        explicit RenderImage3d
        (
            const SurfaceParams&,
            Antialiasing,
            const Logger&
        );
        ~RenderImage3d() override;

        const Camera3d& get_camera() const override;
        void set_camera(Camera3d) override;

        void resize(const Extent<uint32_t> new_surface_extent) override;

        void set_object_to_draw(World3dCoords, const std::span<const Polygons>&, const Image&);

    private:
        void create_pipelines() override;
        void create_command_buffers() override;
        void update_uniform_buffer(size_t current_image) override;

        Camera3d camera_;

        vk::UniqueDescriptorSetLayout descriptor_set_layout_;
        vk::UniquePipelineLayout pipeline_layout_;
        Shaders shaders_;
        vk::UniqueDescriptorPool descriptor_pool_;
        vk::UniquePipeline image_pipeline_;

        std::optional<World3dCoords> object_pos_;
        std::optional<PolygonsInDeviceMemory> polygons_in_device_memory_;
        std::optional<TextureImageData> texture_image_data_;

        std::vector<BufferData> uniform_buffers_;
        std::vector<vk::UniqueDescriptorSet> descriptor_sets_;
    };
}
