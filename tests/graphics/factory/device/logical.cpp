#include <gtest/gtest.h>

#include "factory/test_tools.h"

#include "ge/render/factory/device/physical.h"
#include "ge/render/factory/device/logical.h"

#include "ge/window/window.h"

TEST(LogicalDeviceFactory, create_graphicsQueueNoLayers)
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
    const auto window = ge::Window::create(StaticSize{500, 500}, {38, 38, 38, 1});
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
    EXPECT_TRUE(logical_device);
}

TEST(LogicalDeviceFactory, create_graphicsQueueWithLayers)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    const Device options
    {
        ValidationLayers{ENABLED}
      , Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_with_window(options.validation_layers.enabled);
    const auto window = ge::Window::create(StaticSize{500, 500}, {38, 38, 38, 1});
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
    EXPECT_TRUE(logical_device);
}

TEST(LogicalDeviceFactory, create_computeQueue)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    const Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{DISABLED}
      , Compute{ENABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_without_window(options.validation_layers.enabled);

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , instance.get()
    );

    const auto logical_device = factory::create_logical_device
    (
        options.validation_layers
      , physical_device
      , queue_family_indices
    );
    EXPECT_TRUE(logical_device);
}

TEST(LogicalDeviceFactory, create_transferQueue)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    const Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{ENABLED}
    };
    const auto instance = create_instance_without_window(options.validation_layers.enabled);

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , instance.get()
    );

    const auto logical_device = factory::create_logical_device
    (
        options.validation_layers
      , physical_device
      , queue_family_indices
    );
    EXPECT_TRUE(logical_device);
}
