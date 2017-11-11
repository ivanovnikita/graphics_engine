#pragma once

namespace ge::impl::factory
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

} // ge::impl::factory
