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

    struct Graphics final
    {
        bool enabled = false;
    };

    struct Compute final
    {
        bool enabled = false;
    };

    struct Transfer final
    {
        bool enabled = false;
    };

    struct Instance final
    {
        static Instance create_default();

        Debug debug;
        Window window;
    };

    struct Device final
    {
        ValidationLayers validation_layers;
        Graphics graphics;
        Compute compute;
        Transfer transfer;
    };

}
