#pragma once

#include "ge/window/window_events.h"
#include "ge/window/window_size.h"

#include <vector>
#include <string_view>

namespace ge
{
    class WindowI
    {
    public:
        virtual ~WindowI();

        virtual void start_display() = 0;
        virtual std::vector<WindowEvent> grab_events() = 0;
        virtual void resize(const Size&) = 0;
        virtual void set_window_title(std::string_view) = 0;
    };
}
