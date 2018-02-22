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
        std::ifstream file("graphics_config.txt");
        std::string config;
        std::getline(file, config);
        std::regex layerRegex("(VK_LAYER_PATH)=([a-zA-Z0-9.\\-/]+explicit_layer.d)");
        std::smatch matches;
        if (std::regex_search(config, matches, layerRegex))
        {
            const int override = 1;
            setenv(matches[1].str().c_str(), matches[2].str().c_str(), override);
        }
    }
};

::testing::Environment* const graphics_environment = ::testing::AddGlobalTestEnvironment(new GraphicsTestsEnvironment);
