#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <gtest/gtest.h>

#include <cstdlib>
#include <fstream>
#include <regex>
#include <iostream>

class GraphicsTestsEnvironment final : public ::testing::Environment
{
public:
    void SetUp() final
    {
#ifdef GE_DEBUG_LAYERS_ENABLED
        constexpr int override = 1;
        setenv("VK_LAYER_PATH", std::string{ge::VK_LAYER_PATH}.c_str(), override);
#endif
    }
};

::testing::Environment* const graphics_environment = ::testing::AddGlobalTestEnvironment(new GraphicsTestsEnvironment);
