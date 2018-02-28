#include "tools.hpp"

namespace ge::impl::factory::tools
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
