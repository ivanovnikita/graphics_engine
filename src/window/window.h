 #pragma once

#include <vulkan/vulkan.hpp>

#include <memory>

namespace ge::impl
{

    class Window
    {
    public:
        virtual ~Window();

        static std::unique_ptr<Window> create();

        virtual std::unique_ptr<vk::UniqueSurfaceKHR> create_surface(const vk::UniqueInstance& instance) = 0;

    protected:
        Window();
    };

} // namespace ge::impl
