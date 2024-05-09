#pragma once

#include "shaders.h"
#include "polygons_in_device_memory.h"

#include "ge/common/logger.hpp"

#include "ge/io/image.h"

#include "ge/render/object_transform.h"
#include "ge/render/render_i.h"
#include "ge/render/render_base.h"

#include "ge/render/vulkan_common/texture_image.h"

#include <vulkan/vulkan.hpp>

namespace ge::image3d
{
    class Render3dImage final : public RenderBase, public virtual Render3dI
    {
    public:
        explicit Render3dImage
        (
            const SurfaceParams&,
            Antialiasing,
            const Logger&
        );
        ~Render3dImage() override;

        const Camera3d& get_camera() const override;
        Camera3d& get_camera() override;
        void set_camera(Camera3d) override;

        void resize(const Extent<uint32_t> new_surface_extent) override;

        void set_object_to_draw(ObjectTransform, const std::span<const Polygons>&, const Image&);

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

        std::optional<ObjectTransform> object_transform_;
        std::optional<PolygonsInDeviceMemory> polygons_in_device_memory_;
        std::optional<TextureImageData> texture_image_data_;

        std::vector<BufferData> uniform_buffers_;
        std::vector<vk::UniqueDescriptorSet> descriptor_sets_;
    };
}
