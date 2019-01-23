#pragma once

#include <memory>

namespace ge
{
    namespace impl
    {
        class GraphicsEngineImpl;
    }

    class GraphicsEngine final
    {
    public:
        GraphicsEngine();
        ~GraphicsEngine();
        void draw_frame();
        void process_events();
        bool stopped() const;
    private:
        std::unique_ptr<impl::GraphicsEngineImpl> impl_;
    };
}
