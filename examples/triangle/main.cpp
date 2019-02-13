#include "ge/render/render.h"
#include "ge/window/window.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <thread>
#include <fstream>
#include <regex>

namespace
{
    class MainLoop final
    {
    public:
        MainLoop(ge::Window& window, ge::Render& render);

        bool stopped() const;
        void handle_window_events();

    private:
        template <typename T>
        void handle_window_event(const T&);

        ge::Window& window_;
        ge::Render& render_;
        bool stopped_;
    };


    template <>
    void MainLoop::handle_window_event<ge::WindowEventClose>(const ge::WindowEventClose&)
    {
        stopped_ = true;
    }

    template <>
    void MainLoop::handle_window_event<ge::WindowEventResize>(const ge::WindowEventResize&)
    {
    }

    MainLoop::MainLoop(ge::Window& window, ge::Render& render)
        : window_(window)
        , render_(render)
        , stopped_(false)
    {
    }

    bool MainLoop::stopped() const
    {
        return stopped_;
    }

    void MainLoop::handle_window_events()
    {
        const std::vector<ge::WindowEvent> events = window_.grab_events();
        for (const auto& event : events)
        {
            std::visit
            (
                [this] (const auto& e)
                {
                    this->handle_window_event(e);
                }
                , event
            );
        }
    }
}

int main()
{
#ifdef GE_DEBUG_LAYERS_ENABLED
    constexpr int override = 1;
    setenv("VK_LAYER_PATH", std::string{ge::VK_LAYER_PATH}.c_str(), override);
#endif

    constexpr uint16_t width = 500;
    constexpr uint16_t height = 500;
    auto window = ge::Window::create(width, height);

    ge::Render render
    (
        [&window] (const vk::Instance& instance)
        {
            return window->create_surface(instance);
        }
        , width
        , height
    );

    window->start_display();

    render.draw_frame();

    MainLoop main_loop(*window, render);
    while (not main_loop.stopped())
    {
        main_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
