#include "render_2d_graph.h"
#include "uniform_buffers.h"
#include "descriptor_set_layout.h"
#include "pipeline_layout.h"
#include "descriptor_pool.h"
#include "descriptor_set.h"
#include "pipelines.h"
#include "draw_graph_commands.h"

#include "ge/render/vulkan_common/shader_module.h"

#include "generated_shaders.h"

namespace ge::graph
{
    Render2dGraph::Render2dGraph(const SurfaceParams& surface_params, const Logger& logger)
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

    Render2dGraph::~Render2dGraph() = default;

    const Camera2d& Render2dGraph::get_camera() const
    {
        return camera_;
    }

    void Render2dGraph::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void Render2dGraph::resize(const Extent<uint32_t> new_surface_extent)
    {
        RenderBase::resize(new_surface_extent);
        camera_.set_surface_extent(new_surface_extent);
    }

    void Render2dGraph::set_object_to_draw(const Graph& graph)
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

    void Render2dGraph::create_pipelines()
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
            logger_
        );
    }

    void Render2dGraph::create_command_buffers()
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

    void Render2dGraph::update_uniform_buffer(const size_t current_image)
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
