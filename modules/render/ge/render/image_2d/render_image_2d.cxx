module;

#include <vulkan/vulkan.hpp>

module image_2d.render_image_2d;

import image_2d.uniform_buffers;
import image_2d.descriptor_set_layout;
import image_2d.pipeline_layout;
import image_2d.descriptor_pool;
import image_2d.descriptor_set;
import image_2d.pipelines;
import image_2d.draw_image_commands;

import vulkan_common.shader_module;
import generated_shaders;

namespace ge::image2d
{
    RenderImage2d::RenderImage2d(const SurfaceParams& surface_params, const Logger& logger)
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
            NoAntialiasing{}
        }
        , camera_
        {
            World2dCoords{{0.f, 0.f}},
            1.f,
            Extent<uint32_t>{.width = surface_params.width, .height = surface_params.height}
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
                    get_shader(ShaderName::tex_polygon_2d_camera_Vertex)
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

    RenderImage2d::~RenderImage2d() = default;

    const Camera2d& RenderImage2d::get_camera() const
    {
        return camera_;
    }

    void RenderImage2d::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void RenderImage2d::resize(const Extent<uint32_t> new_surface_extent)
    {
        RenderBase::resize(new_surface_extent);
        camera_.set_surface_extent(new_surface_extent);
    }

    void RenderImage2d::set_object_to_draw(const std::span<const Polygons>& polygons, const Image& image)
    {
        polygons_in_device_memory_ = load_polygons_to_device
        (
            device_data_,
            *command_pool_,
            *transfer_finished_fence_,
            polygons
        );

        const bool generate_mipmaps = false;
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

    void RenderImage2d::create_pipelines()
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

    void RenderImage2d::create_command_buffers()
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

    void RenderImage2d::update_uniform_buffer(const size_t current_image)
    {
        assert(current_image < swapchain_data_.images.size());

        constexpr uint32_t offset = 0;
        void* const data = map_memory
        (
            *device_data_.logical_device,
            *uniform_buffers_[current_image].device_memory,
            offset,
            sizeof(ViewProj2d),
            vk::MemoryMapFlags{}
        );
        std::memcpy(data, &camera_.get_view_proj_2d(), sizeof(ViewProj2d));
        device_data_.logical_device->unmapMemory(*uniform_buffers_[current_image].device_memory);
    }
}
