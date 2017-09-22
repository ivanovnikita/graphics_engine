#include "graphics_engine_impl.h"
#include "debug_callback.h"
#include "exception.h"

#include <vulkan_ext.h>

#include <unordered_set>

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
        : instance_         (create_instance())
        , debug_callback_   (create_debug_callback())
        , window_           (Window::create())
        , surface_          (create_surface())
    {
    }

    namespace
    {

        std::unordered_set<std::string> get_instance_extensions()
        {
            const auto extension_properties = vk::enumerateInstanceExtensionProperties();

            std::unordered_set<std::string> result;
            for (const auto& extension : extension_properties)
            {
                result.emplace(extension.extensionName);
            }

            return result;
        }

        std::vector<const char*> get_required_instance_extensions()
        {
            const std::vector<const char*> required_extesions{VK_KHR_SURFACE_EXTENSION_NAME,
                                                              VK_KHR_XCB_SURFACE_EXTENSION_NAME,
                                                              VK_EXT_DEBUG_REPORT_EXTENSION_NAME};
            const auto available_extensions = get_instance_extensions();

            if (std::all_of(required_extesions.begin(), required_extesions.end(),
                            [&available_extensions] (const char* extension)
                            {
                                return available_extensions.count(extension) > 0;
                            }))
            {
                return required_extesions;
            }
            else
            {
                GE_THROW(device_capabilities_error, "Some required extensions are not available!");
            }
        }

        std::unordered_set<std::string> get_instance_layers()
        {
            const auto layer_properties = vk::enumerateInstanceLayerProperties();

            std::unordered_set<std::string> result;
            for (const auto& layer : layer_properties)
            {
                result.emplace(layer.layerName);
            }

            return result;
        }

        std::vector<const char*> get_required_instance_layers()
        {
            const std::vector<const char*> required_layers{"VK_LAYER_LUNARG_standard_validation"};
            const auto available_layers = get_instance_layers();

            if (std::all_of(required_layers.begin(), required_layers.end(),
                            [&available_layers] (const char* layer)
                            {
                                return available_layers.count(layer) > 0;
                            }))
            {
                return required_layers;
            }
            else
            {
                GE_THROW(device_capabilities_error, "Some required layers are not available!");
            }
        }

    } // unnamed namespace

    vk::UniqueInstance GraphicsEngineImpl::create_instance() const
    {
        const vk::ApplicationInfo application_info
        {
              "graphics engine"
            , VK_MAKE_VERSION(1, 0, 0)
            , "no engine"
            , VK_MAKE_VERSION(1, 0, 0)
            , VK_MAKE_VERSION(1, 0, 0)
        };

        const std::vector<const char*> required_layers = get_required_instance_layers();
        const std::vector<const char*> required_extensions = get_required_instance_extensions();

        const vk::InstanceCreateInfo create_info
        {
              vk::InstanceCreateFlags()
            , &application_info
            , static_cast<uint32_t>(required_layers.size())
            , required_layers.data()
            , static_cast<uint32_t>(required_extensions.size())
            , required_extensions.data()
        };

        auto instance = vk::createInstanceUnique(create_info);
        vkExtInitInstance(static_cast<VkInstance>(*instance));

        return instance;
    }

    vk::UniqueDebugReportCallbackEXT GraphicsEngineImpl::create_debug_callback() const
    {
        const vk::DebugReportCallbackCreateInfoEXT create_info
        (
              vk::DebugReportFlagBitsEXT::eError
            | vk::DebugReportFlagBitsEXT::ePerformanceWarning
            | vk::DebugReportFlagBitsEXT::eWarning
            , debug_callback
        );

        return instance_->createDebugReportCallbackEXTUnique(create_info);
    }

    vk::UniqueSurfaceKHR GraphicsEngineImpl::create_surface() const
    {
        return vk::UniqueSurfaceKHR(std::move(*(window_->create_surface(instance_).release())));
    }

} // namespace ge::impl
