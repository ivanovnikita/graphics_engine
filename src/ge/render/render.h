#pragma once

#include <memory>

namespace ge
{
    class Render final
    {
    public:
        Render();
        ~Render();
        void draw_frame();
        void process_events();
        bool stopped() const;
    private:
        class RenderImpl;
        std::unique_ptr<RenderImpl> impl_;
    };
}
