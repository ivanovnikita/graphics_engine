#include "graphics_engine_impl.h"
#include "debug_callback.h"

#include <vulkan_ext.h>

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
        : instance_         (create_instance())
        , debug_callback_   (create_debug_callback())
        , window_           (Window::create())
        , surface_          (create_surface())
    {
    }

    vk::UniqueInstance GraphicsEngineImpl::create_instance() const
    {
        auto extension_properties = vk::enumerateInstanceExtensionProperties();

        vk::ApplicationInfo application_info;
        application_info.pApplicationName = "ge";
        application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        application_info.pEngineName = "no engine";
        application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        application_info.apiVersion = VK_API_VERSION_1_0;

        vk::InstanceCreateInfo create_info;
        create_info.pApplicationInfo = &application_info;

        std::vector<const char*> required_extensions;
        required_extensions.reserve(extension_properties.size());
        for (const auto& extension : extension_properties)
        {
            required_extensions.emplace_back(extension.extensionName);
        }

        create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
        create_info.ppEnabledExtensionNames = required_extensions.data();

        std::vector<const char*> required_layers{"VK_LAYER_LUNARG_standard_validation"};

    //    auto layer_properties = vk::enumerateInstanceLayerProperties();
    //    required_layers.reserve(layer_properties.size());
    //    for (const auto& layer : layer_properties)
    //    {
    //        required_layers.emplace_back(layer.layerName);
    //    }

        create_info.enabledLayerCount = static_cast<uint32_t>(required_layers.size());
        create_info.ppEnabledLayerNames = required_layers.data();

        auto instance = vk::createInstanceUnique(create_info);
        vkExtInitInstance(static_cast<VkInstance>(*instance));

        return instance;
    }

    vk::UniqueDebugReportCallbackEXT GraphicsEngineImpl::create_debug_callback() const
    {
        vk::DebugReportCallbackCreateInfoEXT create_info(vk::DebugReportFlagBitsEXT::eError |
                                                         vk::DebugReportFlagBitsEXT::ePerformanceWarning |
                                                         vk::DebugReportFlagBitsEXT::eWarning,
                                                         debug_callback);

        return instance_->createDebugReportCallbackEXTUnique(create_info);
    }

    vk::UniqueSurfaceKHR GraphicsEngineImpl::create_surface() const
    {
        return vk::UniqueSurfaceKHR(std::move(*(window_->create_surface(instance_).release())));
    }

} // namespace ge::impl
