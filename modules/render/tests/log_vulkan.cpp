#include "ge/render/log_vulkan.hpp"

#include <gtest/gtest.h>

namespace vk
{
    std::string_view to_string_view(const vk::MemoryHeapFlagBits bits);

    std::string_view to_string_view(const vk::MemoryHeapFlagBits bits)
    {
        switch (bits)
        {
        case vk::MemoryHeapFlagBits::eDeviceLocal: return "DeviceLocal";
        case vk::MemoryHeapFlagBits::eMultiInstance: return "MultiInstance";
        }
        __builtin_unreachable();
    }
}

TEST(log_vulkan, flags)
{
    using namespace ge;

    const vk::MemoryHeapFlags flags{vk::MemoryHeapFlagBits::eDeviceLocal};

    log(LogDestination::StdOut, flags);
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}
