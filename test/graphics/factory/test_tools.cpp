#include "test_tools.h"

namespace test
{
    vk::UniqueInstance create_instance_with_window(bool validation_layers)
    {
        using namespace ge::impl;
        using namespace ge::impl::factory::options;
        using ge::impl::factory::options::Window;

        return factory::instance::create
        (
            Instance
            {
                Debug{DebugCallback{DISABLED}, ValidationLayers{validation_layers}}
              , Window{ENABLED, WindowType::XCB}
            }
        );
    }

    vk::UniqueInstance create_instance_without_window(bool validation_layers)
    {
        using namespace ge::impl;
        using namespace ge::impl::factory::options;
        using ge::impl::factory::options::Window;

        return factory::instance::create
        (
            Instance
            {
                Debug{DebugCallback{DISABLED}, ValidationLayers{validation_layers}}
              , Window{DISABLED, WindowType::XCB}
            }
        );
    }
}
