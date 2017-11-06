#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    using OPTIONAL = std::bool_constant<true>;
    using REQUIRED = std::bool_constant<false>;

    using ENABLED = std::bool_constant<true>;
    using DISABLED = std::bool_constant<false>;

    template<typename IS_ENABLED = DISABLED, typename IS_OPTIONAL = OPTIONAL>
    struct OptionDebugCallback
    {
        using enabled = IS_ENABLED;
        using optional = IS_OPTIONAL;
    };

    template<typename IS_ENABLED = DISABLED, typename IS_OPTIONAL = OPTIONAL>
    struct OptionValidationLayers
    {
        using enabled = IS_ENABLED;
        using optional = IS_OPTIONAL;
    };

    template
    <
        typename DEBUG_CALLBACK = OptionDebugCallback<>
      , typename VALIDATION_LAYERS = OptionValidationLayers<>
    >
    struct OptionsDebug
    {
        using debug_callback = DEBUG_CALLBACK;
        using validation_layers = VALIDATION_LAYERS;
    };

    enum class WindowType
    {
        XCB
      , WIN32
    };

    template<WindowType window_type, typename IS_ENABLED = DISABLED, typename IS_OPTIONAL = OPTIONAL>
    struct OptionWindow
    {
        using enabled = IS_ENABLED;
        using optional = IS_OPTIONAL;
        static constexpr WindowType type = window_type;
    };

    template
    <
        typename DEBUG = OptionsDebug<>
      , typename WINDOW = OptionWindow<WindowType::XCB>
    >
    struct OptionsInstance
    {
        using debug = DEBUG;
        using window = WINDOW;
    };

    template<typename OPTIONS = OptionsInstance<>>
    vk::UniqueInstance create();

} // namespace ge::impl::factory::instance

#include "instance_factory_impl.hpp"
