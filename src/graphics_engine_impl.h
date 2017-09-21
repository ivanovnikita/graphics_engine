#pragma once

#include "window/window.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl
{
    class GraphicsEngineImpl final
    {
    public:
        GraphicsEngineImpl();
    private:
        vk::UniqueInstance create_instance() const;
        vk::UniqueDebugReportCallbackEXT create_debug_callback() const;
        vk::UniqueSurfaceKHR create_surface() const;
    private:
        vk::UniqueInstance instance_;
        vk::UniqueDebugReportCallbackEXT debug_callback_;
        std::unique_ptr<Window> window_;
        vk::UniqueSurfaceKHR surface_;
    };

} // namespace ge::impl
