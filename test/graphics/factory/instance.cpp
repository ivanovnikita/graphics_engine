#include <gtest/gtest.h>

#include "factory/instance.h"

TEST(InstanceFactory, create_withoutAll)
{
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Instance options
    {
        Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
      , Window{DISABLED}
    };
    EXPECT_NO_THROW(factory::instance::create(options));
}

TEST(InstanceFactory, create_withDebugCallback)
{
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Instance options
    {
        Debug{DebugCallback{ENABLED}, ValidationLayers{DISABLED}}
      , Window{DISABLED}
    };
    EXPECT_NO_THROW(factory::instance::create(options));
}

TEST(InstanceFactory, create_withValidationLayers)
{
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Instance options
    {
        Debug{DebugCallback{DISABLED}, ValidationLayers{ENABLED}}
      , Window{DISABLED}
    };
    EXPECT_NO_THROW(factory::instance::create(options));
}

TEST(InstanceFactory, create_windowXcb)
{
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    Instance options
    {
        Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
      , Window{ENABLED, WindowType::XCB}
    };
    EXPECT_NO_THROW(factory::instance::create(options));
}
