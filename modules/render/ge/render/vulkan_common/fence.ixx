module;

#include <vulkan/vulkan.hpp>

#include <chrono>
#include <span>

export module vulkan_common.fence;

namespace ge
{
    export vk::UniqueFence create_fence(const vk::Device&);
    export void reset_fences(const vk::Device&, std::span<const vk::Fence>);

    // TODO: return enum Success/Timeout
    export void wait_for_fences
    (
        const vk::Device&,
        std::span<const vk::Fence>,
        bool wait_all,
        std::chrono::nanoseconds timeout
    );
}
