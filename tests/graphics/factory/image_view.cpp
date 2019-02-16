#include <gtest/gtest.h>

#include "factory/test_tools.h"
#include "ge/render/factory/device/physical.h"
#include "ge/render/factory/device/logical.h"
#include "ge/render/factory/swapchain.h"
#include "ge/render/factory/image_view.h"

#include "ge/window/window.h"

TEST(ImageView, create)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    const Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_with_window(options.validation_layers.enabled);
    constexpr uint16_t width = 500;
    constexpr uint16_t height = 500;
    constexpr StaticSize size{width, height};
    const auto window = ge::Window::create(size);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , instance.get()
        , surface.get()
    );

    const auto logical_device = factory::create_logical_device
    (
        options.validation_layers
      , physical_device
      , queue_family_indices
    );

    auto[swapchain, format] = factory::create_swapchain
    (
        physical_device
      , logical_device.get()
      , vk::Extent2D{}.setWidth(width).setHeight(height)
      , surface.get()
      , queue_family_indices
    );

    const auto images = logical_device->getSwapchainImagesKHR(swapchain.get());
    const auto image_views = factory::create_image_view(images, format, logical_device.get());

    EXPECT_EQ(images.size(), image_views.size());

    for (const auto& image_view : image_views)
    {
        EXPECT_TRUE(image_view);
    }
}

