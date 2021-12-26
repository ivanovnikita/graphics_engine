#pragma once

#include "ge/common/logger.hpp"

#include "ge/render/render_i.h"

#include "ge/render/instance.h"
#include "ge/render/surface.h"
#include "ge/render/device.h"
#include "ge/render/queue.h"

#include "ge/render/vertex.h"
#include "ge/render/2d_graph/2d_graph.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    class Render2dGraph final : public RenderI
    {
    public:
        explicit Render2dGraph(const SurfaceParams&, const Logger&);
        ~Render2dGraph() override;

        void draw_frame() override;
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height) override;

        const Camera2d& get_camera() const override;
        void set_camera(Camera2d) override;

        void set_object_to_draw(const Graph&);

    private:
        InstanceData instance_data_;
        SurfaceData surface_data_;
        Camera2d camera_;

//        vk::PhysicalDevice physical_device_;
//        QueueFamilyIndices queue_family_indices_;
//        vk::UniqueDevice logical_device_;
//        Queues queues_;
    };
}
