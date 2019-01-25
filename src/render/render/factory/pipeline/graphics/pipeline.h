#include "render/storage/shaders.h"
#include "render/window/window.h"

namespace ge::factory
{
    std::tuple<vk::UniquePipeline, vk::UniquePipelineLayout, vk::UniqueRenderPass> create_graphics_pipeline
    (
        const vk::Device&
      , const vk::Format&
      , const storage::Shaders&
      , const Window&
    );
}
