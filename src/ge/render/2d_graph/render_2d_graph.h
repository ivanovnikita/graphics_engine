#pragma once

#include "ge/render/instance.h"
#include "ge/render/vertex.h"
#include "ge/render/surface_params.h"
#include "ge/render/queue.h"
#include "ge/render/2d_graph/2d_graph.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    class Render2dGraph final
    {
    public:
        explicit Render2dGraph(const SurfaceParams&);
        ~Render2dGraph();

        void set_object_to_draw(const Graph&);

        void draw_frame();
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height);

        glm::vec2 camera_pos() const;
        void set_camera_pos(const glm::vec2&);
        float camera_scale() const;
        void set_camera_scale(float);

        glm::vec2 normalize_in_proj_space(const glm::vec2&) const;
        glm::vec2 proj_to_model_space(const glm::vec2&) const;

    private:
        InstanceData instance_data_;

//        vk::Extent2D surface_extent_;
//        vk::UniqueSurfaceKHR surface_;
//        vk::ClearColorValue surface_background_color_;

//        vk::PhysicalDevice physical_device_;
//        QueueFamilyIndices queue_family_indices_;
//        vk::UniqueDevice logical_device_;
//        Queues queues_;
    };
}
