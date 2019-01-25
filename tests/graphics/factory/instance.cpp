#include <gtest/gtest.h>

#include "render/factory/instance.h"

TEST(InstanceFactory, create_withoutAll)
{
    using namespace ge;
    using namespace ge::factory::options;

    Instance options
    {
        Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
      , Window{DISABLED}
    };
    EXPECT_NO_THROW(factory::create_instance(options));
}

TEST(InstanceFactory, create_withDebugCallback)
{
    using namespace ge;
    using namespace ge::factory::options;

    Instance options
    {
        Debug{DebugCallback{ENABLED}, ValidationLayers{DISABLED}}
      , Window{DISABLED}
    };
    EXPECT_NO_THROW(factory::create_instance(options));
}

TEST(InstanceFactory, create_withValidationLayers)
{
    using namespace ge;
    using namespace ge::factory::options;

    Instance options
    {
        Debug{DebugCallback{DISABLED}, ValidationLayers{ENABLED}}
      , Window{DISABLED}
    };
    EXPECT_NO_THROW(factory::create_instance(options));
}

TEST(InstanceFactory, create_windowXcb)
{
    using namespace ge;
    using namespace ge::factory::options;

    Instance options
    {
        Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
      , Window{ENABLED, WindowType::XCB}
    };
    EXPECT_NO_THROW(factory::create_instance(options));
}
