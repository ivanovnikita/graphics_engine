#pragma once

#include "ge/window/window_events.h"

namespace ge
{
    enum class KeyboardKey
    {
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
        HyperR,

        Q, W, E, R, T, Y, U, I, O, P, L_curl_brace, R_curl_brace,
        A, S, D, F, G, H, J, K, L, Colon, Quotes, Slash,
        Z, X, C, V, B, N, M, L_triangle_brace, R_triangle_brace, Question,
        Space
    };

    std::optional<KeyboardKey> map(ServiceKey);
    std::optional<KeyboardKey> map(char);
}
