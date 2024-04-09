module;

#include <vector>
#include <string_view>

export module window_i;

export import window_events;
export import window_size;

namespace ge
{
    export class WindowI
    {
    public:
        virtual ~WindowI();

        virtual void start_display() = 0;
        virtual std::vector<WindowEvent> grab_events() = 0;
        virtual void resize(const Size&) = 0;
        virtual void set_window_title(std::string_view) = 0;
    };
}
