#include "ge/render/storage/shaders.h"

namespace ge::factory
{
    vk::UniquePipelineLayout camera_2d_pipeline_layout
    (
        const vk::Device&
      , const vk::DescriptorSetLayout&
    );

    vk::UniquePipeline graph_arcs_pipeline
    (
        const vk::Device&
      , const vk::RenderPass&
      , const storage::Shaders&
      , const vk::Extent2D&
      , const vk::PipelineLayout&
    );

    vk::UniquePipeline graph_vertices_pipeline
    (
        const vk::Device&
      , const vk::RenderPass&
      , const storage::Shaders&
      , const vk::Extent2D&
      , const vk::PipelineLayout&
    );

    vk::UniquePipeline polygon_pipeline
    (
        const vk::Device&,
        const vk::RenderPass&,
        const storage::Shaders&,
        const vk::Extent2D&,
        const vk::PipelineLayout&
    );
}
