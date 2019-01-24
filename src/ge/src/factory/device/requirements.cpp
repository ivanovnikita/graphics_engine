#include "requirements.h"

namespace ge::impl::factory::device
{

    std::vector<const char*> get_required_device_extensions()
    {
        return
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    }

}
