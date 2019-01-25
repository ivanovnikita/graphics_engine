#include <gtest/gtest.h>

#include "factory/test_tools.h"

#include "ge/render/exception.h"
#include "ge/render/factory/device/physical.h"

#include "ge/window/window.h"

TEST(PhysicalDeviceFactory, create_noQueue)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , create_instance_without_window(options.validation_layers.enabled).get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_FALSE(queue_family_indices.graphics.has_value());
    EXPECT_FALSE(queue_family_indices.present.has_value());
    EXPECT_FALSE(queue_family_indices.compute.has_value());
    EXPECT_FALSE(queue_family_indices.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_graphicsQueue_withWindow)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;
    using ge::Window;

    Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_with_window(options.validation_layers.enabled);
    const auto window = Window::create(500, 500);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , instance.get()
        , surface.get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_TRUE(queue_family_indices.graphics.has_value());
    EXPECT_TRUE(queue_family_indices.present.has_value());
    EXPECT_FALSE(queue_family_indices.compute.has_value());
    EXPECT_FALSE(queue_family_indices.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_graphicsQueue_withoutWindow)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_without_window(options.validation_layers.enabled);

    EXPECT_THROW(factory::create_physical_device(options, instance.get()), ge::invalid_options);
}

TEST(PhysicalDeviceFactory, create_computeQueue)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{DISABLED}
      , Compute{ENABLED}
      , Transfer{DISABLED}
    };

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , create_instance_without_window(options.validation_layers.enabled).get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_FALSE(queue_family_indices.graphics.has_value());
    EXPECT_FALSE(queue_family_indices.present.has_value());
    EXPECT_TRUE(queue_family_indices.compute.has_value());
    EXPECT_FALSE(queue_family_indices.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_transferQueue)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;

    Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{ENABLED}
    };

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , create_instance_without_window(options.validation_layers.enabled).get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_FALSE(queue_family_indices.graphics.has_value());
    EXPECT_FALSE(queue_family_indices.present.has_value());
    EXPECT_FALSE(queue_family_indices.compute.has_value());
    EXPECT_TRUE(queue_family_indices.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_allQueues)
{
    using namespace test;
    using namespace ge;
    using namespace ge::factory::options;
    using ge::Window;

    Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{ENABLED}
      , Compute{ENABLED}
      , Transfer{ENABLED}
    };
    const auto instance = create_instance_with_window(options.validation_layers.enabled);
    const auto window = Window::create(500, 500);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indices] = factory::create_physical_device
    (
        options
        , instance.get()
        , surface.get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_TRUE(queue_family_indices.graphics.has_value());
    EXPECT_TRUE(queue_family_indices.present.has_value());
    EXPECT_TRUE(queue_family_indices.compute.has_value());
    EXPECT_TRUE(queue_family_indices.transfer.has_value());
}
