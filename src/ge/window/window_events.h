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
        bool mod_2 = false; // num lock?
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

    struct MouseMoveEvent final
    {
        glm::vec2 pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

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

    enum class KeyType
    {
        Service,
        Latin,
        KeypadService,
        KeypadLatin,
        Unknown
    };

    enum class ServiceKey
    {
        // TODO: interpret as ascii?
        Enter,
        Tab,

        BackSpace,
        Linefeed,
        Clear,
        Pause,
        ScrollLock,
        SysReq,
        Escape,
        Delete,

        Home,
        Left,
        Up,
        Right,
        Down,
        PageUp,
        PageDown,
        End,
        Begin,

        Select,
        Print,
        Execute,
        Insert,
        Undo,
        Redo,
        Menu,
        Find,
        Cancel,
        Help,
        Break,
        ModeSwitch,
        NumLock,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        ShiftL,
        ShiftR,
        ControlL,
        ControlR,
        AltL,
        AltR,
        CapsLock,
        ShiftLock,
        MetaL,
        MetaR,
        SuperL,
        SuperR,
        HyperL,
        HyperR
    };

    enum class KeypadServiceKey
    {
        Space,
        Tab,
        Enter,
        F1,
        F2,
        F3,
        F4,
        Home,
        Left,
        Up,
        Right,
        Down,
        PageUp,
        PageDown,
        End,
        Begin,
        Insert,
        Delete,
        Decimal
    };

    enum class KeypadLatinKey
    {
        Equal,
        Multiply,
        Add,
        Separator,
        Subtract,
        Divide,

        KP_0,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9
    };

    template <KeyType, ButtonEvent>
    struct KeyEvent;

    template <ButtonEvent button_event>
    struct KeyEvent<KeyType::Service, button_event> final
    {
        ServiceKey key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    template <ButtonEvent button_event>
    struct KeyEvent<KeyType::Latin, button_event> final
    {
        char key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    template <ButtonEvent button_event>
    struct KeyEvent<KeyType::KeypadService, button_event> final
    {
        KeypadServiceKey key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    template <ButtonEvent button_event>
    struct KeyEvent<KeyType::KeypadLatin, button_event> final
    {
        char symbol;
        KeypadLatinKey key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    template <ButtonEvent button_event>
    struct KeyEvent<KeyType::Unknown, button_event> final
    {
        uint32_t key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    using ServiceKeyPress = KeyEvent<KeyType::Service, ButtonEvent::PRESS>;
    using ServiceKeyRelease = KeyEvent<KeyType::Service, ButtonEvent::RELEASE>;
    using LatinKeyPress = KeyEvent<KeyType::Latin, ButtonEvent::PRESS>;
    using LatinKeyRelease = KeyEvent<KeyType::Latin, ButtonEvent::RELEASE>;
    using KeypadServiceKeyPress = KeyEvent<KeyType::KeypadService, ButtonEvent::PRESS>;
    using KeypadServiceKeyRelease = KeyEvent<KeyType::KeypadService, ButtonEvent::RELEASE>;
    using KeypadLatinKeyPress = KeyEvent<KeyType::KeypadLatin, ButtonEvent::PRESS>;
    using KeypadLatinKeyRelease = KeyEvent<KeyType::KeypadLatin, ButtonEvent::RELEASE>;
    using UnknownKeyPress = KeyEvent<KeyType::Unknown, ButtonEvent::PRESS>;
    using UnknownKeyRelease = KeyEvent<KeyType::Unknown, ButtonEvent::RELEASE>;

    using WindowEvent = std::variant
    <
        WindowExposed,
        WindowEventClose,
        WindowEventResize,
        MouseButtonPress,
        MouseButtonRelease,
        WheelEvent,
        MouseMoveEvent,
        MouseEnterWindow,
        MouseLeaveWindow,

        ServiceKeyPress,
        ServiceKeyRelease,
        LatinKeyPress,
        LatinKeyRelease,
        KeypadServiceKeyPress,
        KeypadServiceKeyRelease,
        KeypadLatinKeyPress,
        KeypadLatinKeyRelease,
        UnknownKeyPress,
        UnknownKeyRelease
    >;
}
