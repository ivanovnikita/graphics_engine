#include "graphics_engine.h"
#include "graphics_engine_impl.h"

namespace ge
{

    GraphicsEngine::GraphicsEngine()
        : impl_ (std::make_unique<impl::GraphicsEngineImpl>())
    {
    }

    GraphicsEngine::~GraphicsEngine() = default;

} // namespace ge
