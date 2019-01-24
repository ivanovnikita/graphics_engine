#include "storage/shaders.h"
#include "window/window.h"

namespace ge::impl::factory::pipeline::graphics
{

    std::tuple<vk::UniquePipeline, vk::UniquePipelineLayout, vk::UniqueRenderPass> create
    (
        const vk::Device&
      , const vk::Format&
      , const storage::Shaders&
      , const Window&
    );

}
