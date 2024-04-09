module;

#include <chrono>
#include <optional>
#include <variant>

#include <cstdint>

export module window_events;

export import surface_coords;
export import window_size;

namespace ge
{
    // TODO: xcb_timestamp_t may not be steady, use another clock type
    export using EventTimestamp = std::chrono::steady_clock::time_point;

    export struct WindowExposed final
    {
    };

    export struct WindowEventClose final
    {
    };

    export struct WindowEventResize final
    {
        Size new_size;
    };

    export enum class MouseButton
    {
        LEFT = 1
        , RIGHT
        , MIDDLE
    };

    export enum class ScrollButton
    {
        UP
        , DOWN
    };

    export enum class ButtonEvent
    {
        PRESS
        , RELEASE
    };

    // TODO: use some sort of bitset flags
    export struct ModifiersState final
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

    export template <typename ButtonType, ButtonEvent>
    struct MouseButtonEvent final
    {
        ButtonType button;
        SurfaceCoords pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export template <ButtonEvent button_event>
    struct MouseButtonEvent<ScrollButton, button_event> final
    {
        ScrollButton direction;
        SurfaceCoords pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export using MouseButtonPress = MouseButtonEvent<MouseButton, ButtonEvent::PRESS>;
    export using MouseButtonRelease = MouseButtonEvent<MouseButton, ButtonEvent::RELEASE>;
    export using WheelEvent = MouseButtonEvent<ScrollButton, ButtonEvent::PRESS>;

    export struct MouseMoveEvent final
    {
        SurfaceCoords pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export enum class CrossEvent
    {
        ENTER
        , LEAVE
    };

    export template <CrossEvent>
    struct MouseCrossWindowBorderEvent final
    {
        SurfaceCoords pos;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export using MouseEnterWindow = MouseCrossWindowBorderEvent<CrossEvent::ENTER>;
    export using MouseLeaveWindow = MouseCrossWindowBorderEvent<CrossEvent::LEAVE>;

    export enum class KeyType
    {
        Service,
        Latin,
        KeypadService,
        KeypadLatin,
        Unknown
    };

    export enum class ServiceKey
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

    export enum class KeypadServiceKey
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

    export enum class KeypadLatinKey
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

    export template <KeyType, ButtonEvent>
    struct KeyEvent;

    export template <ButtonEvent button_event>
    struct KeyEvent<KeyType::Service, button_event> final
    {
        ServiceKey key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export template <ButtonEvent button_event>
    struct KeyEvent<KeyType::Latin, button_event> final
    {
        char key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export template <ButtonEvent button_event>
    struct KeyEvent<KeyType::KeypadService, button_event> final
    {
        KeypadServiceKey key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export template <ButtonEvent button_event>
    struct KeyEvent<KeyType::KeypadLatin, button_event> final
    {
        char symbol;
        KeypadLatinKey key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export template <ButtonEvent button_event>
    struct KeyEvent<KeyType::Unknown, button_event> final
    {
        uint32_t key;
        ModifiersState modifiers;
        EventTimestamp timestamp;
    };

    export using ServiceKeyPress = KeyEvent<KeyType::Service, ButtonEvent::PRESS>;
    export using ServiceKeyRelease = KeyEvent<KeyType::Service, ButtonEvent::RELEASE>;
    export using LatinKeyPress = KeyEvent<KeyType::Latin, ButtonEvent::PRESS>;
    export using LatinKeyRelease = KeyEvent<KeyType::Latin, ButtonEvent::RELEASE>;
    export using KeypadServiceKeyPress = KeyEvent<KeyType::KeypadService, ButtonEvent::PRESS>;
    export using KeypadServiceKeyRelease = KeyEvent<KeyType::KeypadService, ButtonEvent::RELEASE>;
    export using KeypadLatinKeyPress = KeyEvent<KeyType::KeypadLatin, ButtonEvent::PRESS>;
    export using KeypadLatinKeyRelease = KeyEvent<KeyType::KeypadLatin, ButtonEvent::RELEASE>;
    export using UnknownKeyPress = KeyEvent<KeyType::Unknown, ButtonEvent::PRESS>;
    export using UnknownKeyRelease = KeyEvent<KeyType::Unknown, ButtonEvent::RELEASE>;

    export using WindowEvent = std::variant
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
