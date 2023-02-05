#include "ge/render/log_vulkan.hpp"

#include <gtest/gtest.h>

TEST(log_vulkan, flags)
{
    using namespace ge;

    const vk::MemoryHeapFlags flags{vk::MemoryHeapFlagBits::eDeviceLocal};

    log(LogDestination::StdOut, flags);
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}
