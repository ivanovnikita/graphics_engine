#include <gtest/gtest.h>

#include "factory/test_tools.h"
#include "factory/shader/module.h"
#include "factory/device/physical.h"
#include "factory/device/logical.h"
#include "window/window.h"

TEST(ShaderModule, create_vertex)
{
    using namespace test;
    using namespace ge::impl;
    using namespace ge::impl::factory::options;

    const Device options
    {
        ValidationLayers{DISABLED}
      , Graphics{ENABLED}
      , Compute{DISABLED}
      , Transfer{DISABLED}
    };
    const auto instance = create_instance_with_window(options.validation_layers.enabled);
    const auto window = ge::impl::Window::create(500, 500);
    const auto surface = window->create_surface(instance.get());

    const auto[physical_device, queue_family_indices] = factory::device::physical::create
    (
        options
        , instance.get()
        , surface.get()
    );

    const auto logical_device = factory::device::logical::create
    (
        options.validation_layers
      , physical_device
      , queue_family_indices
    );

    const auto shader_module =  factory::shader::module::create(*logical_device, "shaders/triangle.vert");
    EXPECT_TRUE(shader_module);
}
