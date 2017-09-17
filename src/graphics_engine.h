#pragma once

#include <memory>

namespace ge
{

namespace impl
{
    class GraphicsEngineImpl;
} // namespace impl

    class GraphicsEngine final
    {
    public:
        GraphicsEngine();
        ~GraphicsEngine();
    private:
        std::unique_ptr<impl::GraphicsEngineImpl> impl_;
    };

} // namespace ge
