#include "graphics_engine_impl.h"
#include "debug_callback.h"
#include "exception.h"

#include <functional>

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

        template<typename T, typename U, typename Comparator>
        bool all_contained_in(const T& what, const U& where, Comparator compare)
        {
            using T_DATA = std::remove_pointer_t<decltype(std::data(what))>;
            using U_DATA = std::remove_pointer_t<decltype(std::data(where))>;

            return std::all_of(std::begin(what), std::end(what),
                               [&where, &compare] (const T_DATA& required)
                               {
                                   return std::any_of(std::begin(where), std::end(where),
                                                      [&required, &compare](const U_DATA& available)
                                                      {
                                                           return compare(required, available);
                                                      });
                               });
        }

        bool compare(const char* lhs, const std::string& rhs)
        {
            return strcmp(lhs, rhs.c_str()) == 0;
        }

        std::vector<std::string> get_instance_extensions()
        {
            auto extension_properties = vk::enumerateInstanceExtensionProperties();

            std::vector<std::string> result;
            result.reserve(extension_properties.size());
            for (const auto& extension : extension_properties)
            {
                result.emplace_back(std::move(extension.extensionName));
            }

            return result;
        }

        const auto& get_required_instance_extensions()
        {
            static constexpr const char* required_extensions[] =
            {
                  VK_KHR_SURFACE_EXTENSION_NAME
                , VK_KHR_XCB_SURFACE_EXTENSION_NAME
                , VK_EXT_DEBUG_REPORT_EXTENSION_NAME
            };
            const auto available_extensions = get_instance_extensions();

            if (all_contained_in(required_extensions, available_extensions, compare))
            {
                return required_extensions;
            }
            GE_THROW(device_capabilities_error, "Some required extensions are not available!");
        }

        std::vector<std::string> get_instance_layers()
        {
            auto layer_properties = vk::enumerateInstanceLayerProperties();

            std::vector<std::string> result;
            result.reserve(layer_properties.size());
            for (const auto& layer : layer_properties)
            {
                result.emplace_back(std::move(layer.layerName));
            }

            return result;
        }

        const auto& get_required_instance_layers()
        {
            static constexpr const char* required_layers[] = {"VK_LAYER_LUNARG_standard_validation"};
            const auto available_layers = get_instance_layers();

            if (all_contained_in(required_layers, available_layers, compare))
            {
                return required_layers;
            }
            GE_THROW(device_capabilities_error, "Some required layers are not available!");
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

        const auto& required_layers = get_required_instance_layers();
        const auto& required_extensions = get_required_instance_extensions();

        const vk::InstanceCreateInfo create_info
        {
              vk::InstanceCreateFlags()
            , &application_info
            , static_cast<uint32_t>(std::size(required_layers))
            , required_layers
            , static_cast<uint32_t>(std::size(required_extensions))
            , required_extensions
        };

        return vk::createInstanceUnique(create_info);
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
