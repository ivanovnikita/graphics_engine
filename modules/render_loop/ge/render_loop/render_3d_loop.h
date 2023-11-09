#pragma once

#include "ge/render/render_i.h"
#include "ge/window/window_i.h"

#include <optional>
#include <functional>

namespace ge
{
    class Render3dLoop final
    {
    public:
        enum class NeedRedraw : uint8_t
        {
            No = 0,
            Yes = 1
        };

        explicit Render3dLoop
        (
            WindowI&,
            Render3dI&
        );

        bool stopped() const;
        void handle_window_events();

    private:
        template <typename T>
        void handle_window_event(const T&);

        void combine_need_redraw(NeedRedraw);

        WindowI& window_;
        Render3dI& render_;
        bool stopped_;

        NeedRedraw need_redraw_;
    };
}
