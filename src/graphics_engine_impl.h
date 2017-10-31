#pragma once

#include "window/window.h"
#include "queue.h"

namespace ge::impl
{
    class GraphicsEngineImpl final
    {
    public:
        GraphicsEngineImpl();
    private:
        vk::UniqueDebugReportCallbackEXT create_debug_callback() const;
        vk::UniqueSurfaceKHR create_surface() const;
    private:
        vk::UniqueInstance                  instance_;
        vk::UniqueDebugReportCallbackEXT    debug_callback_;
        std::unique_ptr<Window>             window_;
        vk::UniqueSurfaceKHR                surface_;
        vk::PhysicalDevice                  physical_device_;
        QueueFamilyIndices                  queue_family_indeces_;
        vk::UniqueDevice                    logical_device_;
        Queues                              queues_;
    };

} // namespace ge::impl
