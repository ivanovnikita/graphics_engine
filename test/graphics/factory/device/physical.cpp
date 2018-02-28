#include <gtest/gtest.h>

#include "exception.h"
#include "factory/device/physical.h"
#include "window/window.h"
#include "factory/test_tools.h"

TEST(PhysicalDeviceFactory, create_noQueue)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto[physical_device, queue_family_indeces] = factory::device::physical::create
    (
        options
        , create_instance_without_window().get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_FALSE(queue_family_indeces.graphics.has_value());
    EXPECT_FALSE(queue_family_indeces.present.has_value());
    EXPECT_FALSE(queue_family_indeces.compute.has_value());
    EXPECT_FALSE(queue_family_indeces.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_graphicsQueue_withWindow)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;
    using ge::impl::Window;

    Device options
    {
        Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_with_window();
    const auto window = Window::create(500, 500);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indeces] = factory::device::physical::create
    (
        options
        , instance.get()
        , surface.get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_TRUE(queue_family_indeces.graphics.has_value());
    EXPECT_TRUE(queue_family_indeces.present.has_value());
    EXPECT_FALSE(queue_family_indeces.compute.has_value());
    EXPECT_FALSE(queue_family_indeces.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_graphicsQueue_withoutWindow)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_without_window();

    EXPECT_THROW(factory::device::physical::create(options, instance.get()), ge::invalid_options);
}

TEST(PhysicalDeviceFactory, create_computeQueue)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{DISABLED}
      , Compute{ENABLED}
      , Transfer{DISABLED}
    };

    const auto[physical_device, queue_family_indeces] = factory::device::physical::create
    (
        options
        , create_instance_without_window().get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_FALSE(queue_family_indeces.graphics.has_value());
    EXPECT_FALSE(queue_family_indeces.present.has_value());
    EXPECT_TRUE(queue_family_indeces.compute.has_value());
    EXPECT_FALSE(queue_family_indeces.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_transferQueue)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{ENABLED}
    };

    const auto[physical_device, queue_family_indeces] = factory::device::physical::create
    (
        options
        , create_instance_without_window().get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_FALSE(queue_family_indeces.graphics.has_value());
    EXPECT_FALSE(queue_family_indeces.present.has_value());
    EXPECT_FALSE(queue_family_indeces.compute.has_value());
    EXPECT_TRUE(queue_family_indeces.transfer.has_value());
}

TEST(PhysicalDeviceFactory, create_allQueues)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;
    using ge::impl::Window;

    Device options
    {
        Graphics{ENABLED}
      , Compute{ENABLED}
      , Transfer{ENABLED}
    };
    const auto instance = create_instance_with_window();
    const auto window = Window::create(500, 500);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indeces] = factory::device::physical::create
    (
        options
        , instance.get()
        , surface.get()
    );

    EXPECT_TRUE(physical_device);
    EXPECT_TRUE(queue_family_indeces.graphics.has_value());
    EXPECT_TRUE(queue_family_indeces.present.has_value());
    EXPECT_TRUE(queue_family_indeces.compute.has_value());
    EXPECT_TRUE(queue_family_indeces.transfer.has_value());
}
