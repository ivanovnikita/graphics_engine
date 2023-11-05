#include "render_2d_tiles.h"
#include "uniform_buffers.h"
#include "descriptor_set_layout.h"
#include "pipeline_layout.h"
#include "descriptor_pool.h"
#include "descriptor_set.h"
#include "pipelines.h"
#include "draw_tiles_commands.h"

#include "ge/render/vulkan_common/shader_module.h"

#include "generated_shaders.h"

namespace ge::tiles
{
    Render2dTiles::Render2dTiles(const SurfaceParams& surface_params, const Logger& logger)
        : RenderBase
        {
            surface_params,
            logger,
            DeviceFeaturesFlags
            {
                DeviceFeatures::FillModeNonSolid,
                DeviceFeatures::WideLines
            },
            vk::Format::eB8G8R8A8Unorm
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
                .triangles_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::polygon_2d_camera_Vertex)
                ),
                .lines_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::line_2d_camera_Vertex)
                ),
                .fragment = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::simple_color_Fragment)
                )
            }
        }
        , descriptor_pool_{create_descriptor_pool(*device_data_.logical_device, swapchain_data_.images.size())}
        , triangles_pipeline_
        {
            create_triangles_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.triangles_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                logger_
            )
        }
        , lines_pipeline_
        {
            create_lines_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.lines_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                logger_
            )
        }
        , uniform_buffers_{create_uniform_buffers(device_data_, swapchain_data_.images.size())}
        , descriptor_sets_
        {
            create_descriptor_sets
            (
                *device_data_.logical_device,
                *descriptor_pool_,
                *descriptor_set_layout_,
                swapchain_data_.images.size(),
                uniform_buffers_
            )
        }
    {
    }

    Render2dTiles::~Render2dTiles() = default;

    const Camera2d& Render2dTiles::get_camera() const
    {
        return camera_;
    }

    void Render2dTiles::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void Render2dTiles::resize(const Extent<uint32_t> new_surface_extent)
    {
        RenderBase::resize(new_surface_extent);
        camera_.set_surface_extent(new_surface_extent);
    }

    void Render2dTiles::set_object_to_draw(const std::span<const Polygons>& polygons)
    {
        polygons_in_device_memory_.emplace
        (
            load_polygons_to_device
            (
                device_data_,
                *command_pool_,
                *transfer_finished_fence_,
                polygons
            )
        );

        create_command_buffers();
    }

    void Render2dTiles::create_pipelines()
    {
        triangles_pipeline_.reset();
        lines_pipeline_.reset();

        triangles_pipeline_ = create_triangles_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.triangles_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            logger_
        );
        lines_pipeline_ = create_lines_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.lines_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            logger_
        );
    }

    void Render2dTiles::create_command_buffers()
    {
        assert(polygons_in_device_memory_.has_value());

        if (not command_buffers_.empty())
        {
            wait_idle(*device_data_.logical_device);
            command_buffers_.clear();
        }

        command_buffers_ = draw_tiles_commands
        (
            *device_data_.logical_device,
            *command_pool_,
            framebuffers_,
            *render_pass_,
            swapchain_data_.extent,
            surface_data_.background_color,
            *triangles_pipeline_,
            *lines_pipeline_,
            *pipeline_layout_,
            descriptor_sets_,
           *polygons_in_device_memory_
        );
    }

    void Render2dTiles::update_uniform_buffer(const size_t current_image)
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
