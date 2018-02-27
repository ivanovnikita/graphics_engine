#include <gtest/gtest.h>

#include "exception.h"
#include "factory/instance.h"
#include "factory/device/physical.h"
#include "window/window.h"

namespace
{
    vk::UniqueInstance create_instance_with_window()
    {
        using namespace ge::impl;
        using namespace ge::impl::factory::options;
        using ge::impl::factory::options::Window;

        return factory::instance::create
        (
            Instance
            {
                Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
              , Window{ENABLED, WindowType::XCB}
            }
        );
    }

    vk::UniqueInstance create_instance_without_window()
    {
        using namespace ge::impl;
        using namespace ge::impl::factory::options;
        using ge::impl::factory::options::Window;

        return factory::instance::create
        (
            Instance
            {
                Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
              , Window{DISABLED, WindowType::XCB}
            }
        );
    }
}

TEST(PhysicalDeviceFactory, create_noQueue)
{
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    EXPECT_NO_THROW(factory::device::physical::create(options, create_instance_without_window().get()));
}

TEST(PhysicalDeviceFactory, create_graphicsQueue_withWindow)
{
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

    EXPECT_NO_THROW(factory::device::physical::create(options, instance.get(), surface.get()));
}

TEST(PhysicalDeviceFactory, create_graphicsQueue_withoutWindow)
{
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
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{DISABLED}
      , Compute{ENABLED}
      , Transfer{DISABLED}
    };
    EXPECT_NO_THROW(factory::device::physical::create(options, create_instance_without_window().get()));
}

TEST(PhysicalDeviceFactory, create_transferQueue)
{
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Device options
    {
        Graphics{DISABLED}
      , Compute{DISABLED}
      , Transfer{ENABLED}
    };
    EXPECT_NO_THROW(factory::device::physical::create(options, create_instance_without_window().get()));
}

TEST(PhysicalDeviceFactory, create_allQueues)
{
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

    EXPECT_NO_THROW(factory::device::physical::create(options, instance.get(), surface.get()));
}
