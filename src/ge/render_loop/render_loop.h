#pragma once

namespace ge
{
    class Window;
    class Render;

    class RenderLoop final
    {
    public:
        RenderLoop(ge::Window& window, ge::Render& render);

        bool stopped() const;
        void handle_window_events();

    private:
        template <typename T>
        void handle_window_event(const T&);

        ge::Window& window_;
        ge::Render& render_;
        bool stopped_;
    };
}
