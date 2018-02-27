 #pragma once

#include <vulkan/vulkan.hpp>

#include <memory>

namespace ge::impl
{

    class Window
    {
    public:
        virtual ~Window();

        static std::unique_ptr<Window> create(uint16_t width, uint16_t height);

        virtual vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) = 0;
        virtual std::pair<uint16_t, uint16_t> extent() const = 0;

    protected:
        Window();
    };

} // namespace ge::impl
