#include "ge/render/render.h"
#include "ge/window/window.h"

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
#ifndef NDEBUG
    std::ifstream file("graphics_config.txt");
    std::string config;
    std::getline(file, config);
    std::regex layerRegex("(VK_LAYER_PATH)=([a-zA-Z0-9.\\-/]+explicit_layer.d)");
    std::smatch matches;
    if (std::regex_search(config, matches, layerRegex))
    {
        const int override = 1;
        setenv(matches[1].str().c_str(), matches[2].str().c_str(), override);
    }
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
