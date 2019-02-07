#include "ge/render/storage/shaders.h"

namespace ge::factory
{
    std::tuple<vk::UniquePipeline, vk::UniquePipelineLayout, vk::UniqueRenderPass> create_graphics_pipeline
    (
        const vk::Device&
      , const vk::Format&
      , const storage::Shaders&
      , const vk::Extent2D&
    );
}