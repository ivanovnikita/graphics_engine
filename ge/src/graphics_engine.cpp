#include "ge/graphics_engine.h"
#include "graphics_engine_impl.h"

namespace ge
{

    GraphicsEngine::GraphicsEngine()
        : impl_ (std::make_unique<impl::GraphicsEngineImpl>())
    {
    }

    GraphicsEngine::~GraphicsEngine() = default;

    void GraphicsEngine::main_loop()
    {
        impl_->main_loop();
    }

}
