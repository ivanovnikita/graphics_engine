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

import image_3d.uniform_buffers;
import image_3d.descriptor_set_layout;
import image_3d.pipeline_layout;
import image_3d.descriptor_pool;
import image_3d.descriptor_set;
import image_3d.pipelines;
import image_3d.draw_image_commands;
import vulkan_common.shader_module;
import generated_shaders;

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

module : private;

namespace ge::image3d
{
    RenderImage3d::RenderImage3d
    (
        const SurfaceParams& surface_params,
        Antialiasing antialiasing,
        const Logger& logger
    )
        : RenderBase
        {
            surface_params,
            logger,
            DeviceFeaturesFlags
            {
                DeviceFeatures::SamplerAnisotropy,
                DeviceFeatures::FillModeNonSolid,
                DeviceFeatures::WideLines
            },
            vk::Format::eB8G8R8A8Srgb,
            std::move(antialiasing)
        }
        , camera_
        {
            Extent<uint32_t>{.width = surface_params.width, .height = surface_params.height},
            45.f,
            0.1f,
            10.f
        }
        , descriptor_set_layout_{create_descriptor_set_layout(*device_data_.logical_device)}
        , pipeline_layout_{create_pipeline_layout(*device_data_.logical_device, *descriptor_set_layout_)}
        , shaders_
        {
            Shaders
            {
                .image_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::tex_polygon_3d_camera_Vertex)
                ),
                .image_fragment = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::simple_texture_Fragment)
                )
            }
        }
        , descriptor_pool_{create_descriptor_pool(*device_data_.logical_device, swapchain_data_.images.size())}
        , image_pipeline_
        {
            create_image_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.image_vertex,
                *shaders_.image_fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                antialiasing_,
                logger_
            )
        }
        , uniform_buffers_{create_uniform_buffers(device_data_, swapchain_data_.images.size())}
    {
    }

    RenderImage3d::~RenderImage3d() = default;

    const Camera3d& RenderImage3d::get_camera() const
    {
        return camera_;
    }

    void RenderImage3d::set_camera(Camera3d camera)
    {
        camera_ = std::move(camera);
    }

    void RenderImage3d::resize(const Extent<uint32_t> new_surface_extent)
    {
        RenderBase::resize(new_surface_extent);
        camera_.set_surface_extent(new_surface_extent);
    }

    void RenderImage3d::set_object_to_draw
    (
        const World3dCoords object_pos,
        const std::span<const Polygons>& polygons,
        const Image& image
    )
    {
        object_pos_ = object_pos;

        polygons_in_device_memory_ = load_polygons_to_device
        (
            device_data_,
            *command_pool_,
            *transfer_finished_fence_,
            polygons
        );

        const bool generate_mipmaps = true;
        texture_image_data_ = TextureImageData::create_from_image
        (
            device_data_,
            *command_pool_,
            *transfer_finished_fence_,
            image,
            generate_mipmaps
        );

        descriptor_sets_ = create_descriptor_sets
        (
            *device_data_.logical_device,
            *descriptor_pool_,
            *descriptor_set_layout_,
            swapchain_data_.images.size(),
            uniform_buffers_,
            texture_image_data_.value()
        );

        create_command_buffers();
    }

    void RenderImage3d::create_pipelines()
    {
        image_pipeline_.reset();
        image_pipeline_ = create_image_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.image_vertex,
            *shaders_.image_fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            antialiasing_,
            logger_
        );
    }

    void RenderImage3d::create_command_buffers()
    {
        assert(polygons_in_device_memory_.has_value());
        assert(texture_image_data_.has_value());
        assert(not descriptor_sets_.empty());

        if (not command_buffers_.empty())
        {
            wait_idle(*device_data_.logical_device);
            command_buffers_.clear();
        }

        command_buffers_ = draw_image_commands
        (
            *device_data_.logical_device,
            *command_pool_,
            framebuffers_,
            *render_pass_,
            swapchain_data_.extent,
            surface_data_.background_color,
            *image_pipeline_,
            *pipeline_layout_,
            descriptor_sets_,
            *polygons_in_device_memory_
        );
    }

    void RenderImage3d::update_uniform_buffer(const size_t current_image)
    {
        assert(current_image < swapchain_data_.images.size());
        assert(object_pos_.has_value());

        constexpr uint32_t offset = 0;
        void* const data = map_memory
        (
            *device_data_.logical_device,
            *uniform_buffers_[current_image].device_memory,
            offset,
            sizeof(Mvp3d),
            vk::MemoryMapFlags{}
        );
        const Mvp3d mvp = camera_.get_mvp(*object_pos_);
        std::memcpy(data, &mvp, sizeof(Mvp3d));
        device_data_.logical_device->unmapMemory(*uniform_buffers_[current_image].device_memory);
    }
}
