#include "ge/render/factory/tools.hpp"

namespace ge::factory
{
    std::vector<const char*> get_required_layers(const options::ValidationLayers& option_validation_layers)
    {
        std::vector<const char*> layers;

        if (option_validation_layers.enabled)
        {
            layers.emplace_back("VK_LAYER_LUNARG_standard_validation");
        }

        return layers;
    }
}
