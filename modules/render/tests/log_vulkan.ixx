module;

#include <vulkan/vulkan.hpp>

#include <gtest/gtest.h>

export module tests.log_vulkan;

import vulkan_common.log_vulkan;

module : private;

TEST(log_vulkan, flags)
{
    using namespace ge;

    const vk::MemoryHeapFlags flags{vk::MemoryHeapFlagBits::eDeviceLocal};

    log(LogDestination::StdOut, flags);
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}
