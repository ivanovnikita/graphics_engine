#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    struct OptionDebugCallback final
    {
        bool enabled = false;
    };

    struct OptionValidationLayers final
    {
        bool enabled = false;
    };

    struct OptionsDebug final
    {
        OptionDebugCallback debug_callback;
        OptionValidationLayers validation_layers;
    };

    enum class WindowType
    {
        XCB
      , WIN32
    };

    struct OptionWindow final
    {
        bool enabled = false;
        WindowType type = WindowType::XCB;
    };

    struct OptionsInstance final
    {
        OptionsDebug debug;
        OptionWindow window;
    };

    vk::UniqueInstance create(const OptionsInstance&);

} // namespace ge::impl::factory::instance
