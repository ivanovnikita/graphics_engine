#include <gtest/gtest.h>

#include "factory/test_tools.h"
#include "factory/device/physical.h"
#include "factory/device/logical.h"
#include "factory/swapchain.h"
#include "window/window.h"

TEST(Swapchain, create)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    const Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_with_window(options.validation_layers.enabled);
    const auto window = ge::impl::Window::create(500, 500);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indices] = factory::device::physical::create
    (
        options
        , instance.get()
        , surface.get()
    );

    const auto logical_device = factory::device::logical::create
    (
        options.validation_layers
      , physical_device
      , queue_family_indices
    );

    auto[swapchain, format] = factory::swapchain::create
    (
        physical_device
      , logical_device.get()
      , *window
      , surface.get()
      , queue_family_indices
    );

    EXPECT_TRUE(swapchain);
    EXPECT_NE(vk::Format::eUndefined, format);
}
