#include "ge/render/render.h"
#include "ge/render/render_impl.h"

namespace ge
{
    Render::Render()
        : impl_(new RenderImpl())
    {
    }

    Render::~Render() = default;

    void Render::draw_frame()
    {
        impl_->draw_frame();
    }

    void Render::process_events()
    {
        impl_->process_events();
    }

    bool Render::stopped() const
    {
        return impl_->stopped();
    }
}
