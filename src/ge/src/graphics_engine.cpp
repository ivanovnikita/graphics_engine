#include "ge/graphics_engine.h"
#include "graphics_engine_impl.h"

namespace ge
{
    GraphicsEngine::GraphicsEngine()
        : impl_ (std::make_unique<impl::GraphicsEngineImpl>())
    {
    }

    GraphicsEngine::~GraphicsEngine() = default;

    void GraphicsEngine::draw_frame()
    {
        impl_->draw_frame();
    }

    void GraphicsEngine::process_events()
    {
        impl_->process_events();
    }

    bool GraphicsEngine::stopped() const
    {
        return impl_->stopped();
    }
}
