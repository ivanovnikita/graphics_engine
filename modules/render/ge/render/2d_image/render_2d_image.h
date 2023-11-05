#pragma once

#include "shaders.h"
#include "polygons_in_device_memory.h"

#include "ge/common/logger.hpp"

#include "ge/io/image.h"

#include "ge/render/render_i.h"
#include "ge/render/render_base.h"

#include "ge/render/vulkan_common/texture_image.h"

#include <vulkan/vulkan.hpp>

namespace ge::image2d
{
    class Render2dImage final : public RenderBase, public virtual Render2dI
    {
    public:
        explicit Render2dImage(const SurfaceParams&, const Logger&);
        ~Render2dImage() override;

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
