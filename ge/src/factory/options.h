#pragma once

#include <cstdint>

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

    enum class WindowType : uint8_t
    {
        XCB
      , WIN32
    };

    struct OptionWindow final
    {
        bool enabled = false;
        WindowType type = WindowType::XCB;
    };

    struct OptionGraphics
    {
        bool enabled = false;
    };

    struct OptionCompute
    {
        bool enabled = false;
    };

    struct OptionTransfer
    {
        bool enabled = false;
    };

    struct OptionsInstance final
    {
        OptionsDebug debug;
        OptionWindow window;
    };

    struct OptionsDevice
    {
        OptionGraphics graphics;
        OptionCompute compute;
        OptionTransfer transfer;
    };

} // ge::impl::factory
