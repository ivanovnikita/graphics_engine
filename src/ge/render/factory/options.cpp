#include "options.h"

namespace ge::factory::options
{
    Instance Instance::create_default()
    {
        return Instance
        {
#ifndef NDEBUG
            Debug{DebugCallback{ENABLED}, ValidationLayers{ENABLED}},
#else
            Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}},
#endif

#ifdef linux
            options::Window{ENABLED, WindowType::XCB}
#else
    #error Other platforms are not supported
#endif
        };
    }
}
