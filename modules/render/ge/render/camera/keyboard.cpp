#include "keyboard.h"

namespace ge
{
    std::optional<KeyboardKey> map(const ServiceKey k)
    {
        switch (k)
        {
        case ServiceKey::Enter: return KeyboardKey::Enter;
        case ServiceKey::Tab: return KeyboardKey::Tab;

        case ServiceKey::BackSpace: return KeyboardKey::BackSpace;
        case ServiceKey::Linefeed: return KeyboardKey::Linefeed;
        case ServiceKey::Clear: return KeyboardKey::Clear;
        case ServiceKey::Pause: return KeyboardKey::Pause;
        case ServiceKey::ScrollLock: return KeyboardKey::ScrollLock;
        case ServiceKey::SysReq: return KeyboardKey::SysReq;
        case ServiceKey::Escape: return KeyboardKey::Escape;
        case ServiceKey::Delete: return KeyboardKey::Delete;

        case ServiceKey::Home: return KeyboardKey::Home;
        case ServiceKey::Left: return KeyboardKey::Left;
        case ServiceKey::Up: return KeyboardKey::Up;
        case ServiceKey::Right: return KeyboardKey::Right;
        case ServiceKey::Down: return KeyboardKey::Down;
        case ServiceKey::PageUp: return KeyboardKey::PageUp;
        case ServiceKey::PageDown: return KeyboardKey::PageDown;
        case ServiceKey::End: return KeyboardKey::End;
        case ServiceKey::Begin: return KeyboardKey::Begin;

        case ServiceKey::Select: return KeyboardKey::Select;
        case ServiceKey::Print: return KeyboardKey::Print;
        case ServiceKey::Execute: return KeyboardKey::Execute;
        case ServiceKey::Insert: return KeyboardKey::Insert;
        case ServiceKey::Undo: return KeyboardKey::Undo;
        case ServiceKey::Redo: return KeyboardKey::Redo;
        case ServiceKey::Menu: return KeyboardKey::Menu;
        case ServiceKey::Find: return KeyboardKey::Find;
        case ServiceKey::Cancel: return KeyboardKey::Cancel;
        case ServiceKey::Help: return KeyboardKey::Help;
        case ServiceKey::Break: return KeyboardKey::Break;
        case ServiceKey::ModeSwitch: return KeyboardKey::ModeSwitch;
        case ServiceKey::NumLock: return KeyboardKey::NumLock;

        case ServiceKey::F1: return KeyboardKey::F1;
        case ServiceKey::F2: return KeyboardKey::F2;
        case ServiceKey::F3: return KeyboardKey::F3;
        case ServiceKey::F4: return KeyboardKey::F4;
        case ServiceKey::F5: return KeyboardKey::F5;
        case ServiceKey::F6: return KeyboardKey::F6;
        case ServiceKey::F7: return KeyboardKey::F7;
        case ServiceKey::F8: return KeyboardKey::F8;
        case ServiceKey::F9: return KeyboardKey::F9;
        case ServiceKey::F10: return KeyboardKey::F10;
        case ServiceKey::F11: return KeyboardKey::F11;
        case ServiceKey::F12: return KeyboardKey::F12;

        case ServiceKey::ShiftL: return KeyboardKey::ShiftL;
        case ServiceKey::ShiftR: return KeyboardKey::ShiftR;
        case ServiceKey::ControlL: return KeyboardKey::ControlL;
        case ServiceKey::ControlR: return KeyboardKey::ControlR;
        case ServiceKey::AltL: return KeyboardKey::AltL;
        case ServiceKey::AltR: return KeyboardKey::AltR;
        case ServiceKey::CapsLock: return KeyboardKey::CapsLock;
        case ServiceKey::ShiftLock: return KeyboardKey::ShiftLock;
        case ServiceKey::MetaL: return KeyboardKey::MetaL;
        case ServiceKey::MetaR: return KeyboardKey::MetaR;
        case ServiceKey::SuperL: return KeyboardKey::SuperL;
        case ServiceKey::SuperR: return KeyboardKey::SuperR;
        case ServiceKey::HyperL: return KeyboardKey::HyperL;
        case ServiceKey::HyperR: return KeyboardKey::HyperR;
        }

        return std::nullopt;
    }

    std::optional<KeyboardKey> map(const char c)
    {
        switch (c)
        {
        case 'q':
        case 'Q': return KeyboardKey::Q;
        case 'w':
        case 'W': return KeyboardKey::W;
        case 'e':
        case 'E': return KeyboardKey::E;
        case 'r':
        case 'R': return KeyboardKey::R;
        case 't':
        case 'T': return KeyboardKey::T;
        case 'y':
        case 'Y': return KeyboardKey::Y;
        case 'u':
        case 'U': return KeyboardKey::U;
        case 'i':
        case 'I': return KeyboardKey::I;
        case 'o':
        case 'O': return KeyboardKey::O;
        case 'p':
        case 'P': return KeyboardKey::P;
        case '[':
        case '{': return KeyboardKey::L_curl_brace;
        case ']':
        case '}': return KeyboardKey::R_curl_brace;
        case 'a':
        case 'A': return KeyboardKey::A;
        case 's':
        case 'S': return KeyboardKey::S;
        case 'd':
        case 'D': return KeyboardKey::D;
        case 'f':
        case 'F': return KeyboardKey::F;
        case 'g':
        case 'G': return KeyboardKey::G;
        case 'h':
        case 'H': return KeyboardKey::H;
        case 'j':
        case 'J': return KeyboardKey::J;
        case 'k':
        case 'K': return KeyboardKey::K;
        case 'l':
        case 'L': return KeyboardKey::L;
        case ';':
        case ':': return KeyboardKey::Colon;
        case '"':
        case '\'': return KeyboardKey::Quotes;
        case '\\':
        case '|': return KeyboardKey::Slash;
        case 'z':
        case 'Z': return KeyboardKey::Z;
        case 'x':
        case 'X': return KeyboardKey::X;
        case 'c':
        case 'C': return KeyboardKey::C;
        case 'v':
        case 'V': return KeyboardKey::V;
        case 'b':
        case 'B': return KeyboardKey::B;
        case 'n':
        case 'N': return KeyboardKey::N;
        case 'm':
        case 'M': return KeyboardKey::M;
        case ',':
        case '<': return KeyboardKey::L_triangle_brace;
        case '.':
        case '>': return KeyboardKey::R_triangle_brace;
        case '/':
        case '?': return KeyboardKey::Question;
        default: break;
        }

        return std::nullopt;
    }
}
