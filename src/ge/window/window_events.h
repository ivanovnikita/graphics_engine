#pragma once

#include "ge/window/window_size.h"

#include <glm/vec2.hpp>

#include <chrono>
#include <optional>
#include <variant>

#include <cstdint>

namespace ge
{
    // TODO: xcb_timestamp_t may not be steady, use another clock type
    using EventTimestamp = std::chrono::steady_clock::time_point;

    struct WindowExposed final
    {
    };

    struct WindowEventClose final
    {
    };

    struct WindowEventResize final
    {
        Size new_size;
    };

    enum class MouseButton
    {
        LEFT = 1
        , RIGHT
        , MIDDLE
    };

    enum class ScrollButton
    {
        UP
        , DOWN
    };

    enum class ButtonEvent
    {
        PRESS
        , RELEASE
    };

    // TODO: use some sort of bitset flags
    struct ModifiersState final
    {
        bool mouse_left = false;
        bool mouse_right = false;
        bool mouse_middle = false;
        bool scroll_up = false;
        bool scroll_down = false;
        bool shift = false;
        bool caps_lock = false;
        bool ctrl = false;
        bool mod_1 = false; // alt?
        bool mod_2 = false;
        bool mod_3 = false;
        bool mod_4 = false;
        bool mod_5 = false;

        uint8_t count = 0;
    };

    template <typename ButtonType, ButtonEvent>
    struct MouseButtonEvent final
    {
        ButtonType button;
        glm::vec2 pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    template <ButtonEvent button_event>
    struct MouseButtonEvent<ScrollButton, button_event> final
    {
        ScrollButton direction;
        glm::vec2 pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    using MouseButtonPress = MouseButtonEvent<MouseButton, ButtonEvent::PRESS>;
    using MouseButtonRelease = MouseButtonEvent<MouseButton, ButtonEvent::RELEASE>;
    using WheelEvent = MouseButtonEvent<ScrollButton, ButtonEvent::PRESS>;

    template <auto... ButtonsPressed>
    struct MouseMoveEvent final
    {
        glm::vec2 pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    template <>
    struct MouseMoveEvent<> final
    {
        glm::vec2 pos;
        EventTimestamp timestamp;
    };

    template <auto ButtonPressed>
    struct MouseMoveEvent<ButtonPressed> final
    {
        glm::vec2 pos;
        EventTimestamp timestamp;
    };

    using MouseMovePointerEvent = MouseMoveEvent<>;
    using MouseMovePressedLeftEvent = MouseMoveEvent<MouseButton::LEFT>;
    using MouseMovePressedRightEvent = MouseMoveEvent<MouseButton::RIGHT>;
    using MouseMovePressedMiddleEvent = MouseMoveEvent<MouseButton::MIDDLE>;
    using MouseMovePressedManyEvent = MouseMoveEvent
    <
        MouseButton::LEFT
      , MouseButton::RIGHT
      , MouseButton::MIDDLE
    >;

    enum class CrossEvent
    {
        ENTER
        , LEAVE
    };

    template <CrossEvent>
    struct MouseCrossWindowBorderEvent final
    {
        glm::vec2 pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    using MouseEnterWindow = MouseCrossWindowBorderEvent<CrossEvent::ENTER>;
    using MouseLeaveWindow = MouseCrossWindowBorderEvent<CrossEvent::LEAVE>;

    using WindowEvent = std::variant
    <
        WindowExposed
      , WindowEventClose
      , WindowEventResize
      , MouseButtonPress
      , MouseButtonRelease
      , WheelEvent
      , MouseMovePointerEvent
      , MouseMovePressedLeftEvent
      , MouseMovePressedRightEvent
      , MouseMovePressedMiddleEvent
      , MouseMovePressedManyEvent
      , MouseEnterWindow
      , MouseLeaveWindow
    >;
}
