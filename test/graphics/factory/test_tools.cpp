#include "test_tools.h"

namespace test
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
