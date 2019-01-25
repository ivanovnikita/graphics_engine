#include "render/factory/instance.h"

namespace test
{
    vk::UniqueInstance create_instance_with_window(bool validation_layers);
    vk::UniqueInstance create_instance_without_window(bool validation_layers);
}
