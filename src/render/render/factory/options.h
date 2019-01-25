#pragma once

#include <cstdint>

namespace ge::factory::options
{
    constexpr bool ENABLED = true;
    constexpr bool DISABLED = false;

    struct DebugCallback final
    {
        bool enabled = false;
    };

    struct ValidationLayers final
    {
        bool enabled = false;
    };

    struct Debug final
    {
        DebugCallback debug_callback;
        ValidationLayers validation_layers;
    };

    enum class WindowType : uint8_t
    {
        XCB
      , WIN32
    };

    struct Window final
    {
        bool enabled = false;
        WindowType type = WindowType::XCB;
    };

    struct Graphics
    {
        bool enabled = false;
    };

    struct Compute
    {
        bool enabled = false;
    };

    struct Transfer
    {
        bool enabled = false;
    };

    struct Instance final
    {
        Debug debug;
        Window window;
    };

    struct Device
    {
        ValidationLayers validation_layers;
        Graphics graphics;
        Compute compute;
        Transfer transfer;
    };

}
