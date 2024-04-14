module;

#include <optional>
#include <functional>

export module render_2d_loop;

export import flags;
export import render_i;
export import window_i;
export import camera_2d_mover;

namespace ge
{
    namespace world2d
    {
        export struct MouseButtonPress final
        {
            World2dCoords pos;
            MouseButton button;
        };

        export struct MouseButtonRelease final
        {
            World2dCoords pos;
            MouseButton button;
        };

        export struct MouseMoveEvent final
        {
            World2dCoords pos;
        };
    }

    export enum class InputState
    {
        CameraDragMove = 1
    };

    export class Render2dLoop final
    {
    public:
        enum class NeedRedraw : uint8_t
        {
            No = 0,
            Yes = 1
        };

        using MouseButtonPressCallback = std::function<NeedRedraw(const world2d::MouseButtonPress&)>;
        using MouseButtonReleaseCallback = std::function<NeedRedraw(const world2d::MouseButtonRelease&)>;
        using MouseMoveCallback = std::function<NeedRedraw(const world2d::MouseMoveEvent&)>;

        explicit Render2dLoop
        (
            WindowI&,
            Render2dI&
        );

        bool stopped() const;
        void handle_window_events();

        void set_mouse_press_callback(MouseButtonPressCallback);
        void set_mouse_release_callback(MouseButtonReleaseCallback);
        void set_mouse_move_callback(MouseMoveCallback);

    private:
        template <typename T>
        void handle_window_event(const T&);

        void combine_need_redraw(NeedRedraw);

        void deactivate_state(InputState);

        WindowI& window_;
        Render2dI& render_;
        bool stopped_;

        Flags<InputState> active_states_;

        Camera2dMover camera_mover_;

        NeedRedraw need_redraw_;

        MouseButtonPressCallback mouse_press_callback_;
        MouseButtonReleaseCallback mouse_release_callback_;
        MouseMoveCallback mouse_move_callback_;
    };
}
