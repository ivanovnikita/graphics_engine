module;

#include <vulkan/vulkan.hpp>

module graph_2d.render_graph_2d;

import graph_2d.uniform_buffers;
import graph_2d.descriptor_set_layout;
import graph_2d.pipeline_layout;
import graph_2d.descriptor_pool;
import graph_2d.descriptor_set;
import graph_2d.pipelines;
import graph_2d.draw_graph_commands;

import vulkan_common.shader_module;

import generated_shaders;

namespace ge::graph
{
    RenderGraph2d::RenderGraph2d(const SurfaceParams& surface_params, const Logger& logger)
        : RenderBase
        {
            surface_params,
            logger,
            DeviceFeaturesFlags
            {
                DeviceFeatures::FillModeNonSolid,
                DeviceFeatures::WideLines
            },
            vk::Format::eB8G8R8A8Unorm,
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
                .lines_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::line_2d_camera_Vertex)
                ),
                .points_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::point_2d_camera_Vertex)
                ),
                .fragment = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::simple_color_Fragment)
                )
            }
        }
        , descriptor_pool_{create_descriptor_pool(*device_data_.logical_device, swapchain_data_.images.size())}
        , arcs_pipeline_
        {
            create_arcs_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.lines_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                antialiasing_,
                logger_
            )
        }
        , vertices_pipeline_
        {
            create_vertices_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.points_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                antialiasing_,
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

    RenderGraph2d::~RenderGraph2d() = default;

    const Camera2d& RenderGraph2d::get_camera() const
    {
        return camera_;
    }

    void RenderGraph2d::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void RenderGraph2d::resize(const Extent<uint32_t> new_surface_extent)
    {
        RenderBase::resize(new_surface_extent);
        camera_.set_surface_extent(new_surface_extent);
    }

    void RenderGraph2d::set_object_to_draw(const Graph& graph)
    {
        graph_in_device_memory_.emplace
        (
            load_graph_to_device
            (
                device_data_,
                *command_pool_,
                *transfer_finished_fence_,
                graph
            )
        );

        create_command_buffers();
    }

    void RenderGraph2d::create_pipelines()
    {
        vertices_pipeline_.reset();
        arcs_pipeline_.reset();

        arcs_pipeline_ = create_arcs_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.lines_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            antialiasing_,
            logger_
        );
        vertices_pipeline_ = create_vertices_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.points_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            antialiasing_,
            logger_
        );
    }

    void RenderGraph2d::create_command_buffers()
    {
        assert(graph_in_device_memory_.has_value());

        if (not command_buffers_.empty())
        {
            wait_idle(*device_data_.logical_device);
            command_buffers_.clear();
        }

        command_buffers_ = draw_graph_commands
        (
            *device_data_.logical_device,
            *command_pool_,
            framebuffers_,
            *render_pass_,
            swapchain_data_.extent,
            surface_data_.background_color,
            *arcs_pipeline_,
            *vertices_pipeline_,
            *pipeline_layout_,
            descriptor_sets_,
           *graph_in_device_memory_
        );
    }

    void RenderGraph2d::update_uniform_buffer(const size_t current_image)
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
