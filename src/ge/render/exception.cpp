#include "ge/render/exception.h"

namespace ge
{
    std::string_view to_string_view(const vk::Result& result) noexcept
    {
        switch (result)
        {
        case vk::Result::eSuccess: return "Success";
        case vk::Result::eNotReady: return "NotReady";
        case vk::Result::eTimeout: return "Timeout";
        case vk::Result::eEventSet: return "EventSet";
        case vk::Result::eEventReset: return "EventReset";
        case vk::Result::eIncomplete: return "Incomplete";
        case vk::Result::eErrorOutOfHostMemory: return "ErrorOutOfHostMemory";
        case vk::Result::eErrorOutOfDeviceMemory: return "ErrorOutOfDeviceMemory";
        case vk::Result::eErrorInitializationFailed: return "ErrorInitializationFailed";
        case vk::Result::eErrorDeviceLost: return "ErrorDeviceLost";
        case vk::Result::eErrorMemoryMapFailed: return "ErrorMemoryMapFailed";
        case vk::Result::eErrorLayerNotPresent: return "ErrorLayerNotPresent";
        case vk::Result::eErrorExtensionNotPresent: return "ErrorExtensionNotPresent";
        case vk::Result::eErrorFeatureNotPresent: return "ErrorFeatureNotPresent";
        case vk::Result::eErrorIncompatibleDriver: return "ErrorIncompatibleDriver";
        case vk::Result::eErrorTooManyObjects: return "ErrorTooManyObjects";
        case vk::Result::eErrorFormatNotSupported: return "ErrorFormatNotSupported";
        case vk::Result::eErrorFragmentedPool: return "ErrorFragmentedPool";
        case vk::Result::eErrorUnknown: return "ErrorUnknown";
        case vk::Result::eErrorOutOfPoolMemory: return "ErrorOutOfPoolMemory";
        case vk::Result::eErrorInvalidExternalHandle: return "ErrorInvalidExternalHandle";
        case vk::Result::eErrorFragmentation: return "ErrorFragmentation";
        case vk::Result::eErrorInvalidOpaqueCaptureAddress: return "ErrorInvalidOpaqueCaptureAddress";
        case vk::Result::eErrorSurfaceLostKHR: return "ErrorSurfaceLostKHR";
        case vk::Result::eErrorNativeWindowInUseKHR: return "ErrorNativeWindowInUseKHR";
        case vk::Result::eSuboptimalKHR: return "SuboptimalKHR";
        case vk::Result::eErrorOutOfDateKHR: return "ErrorOutOfDateKHR";
        case vk::Result::eErrorIncompatibleDisplayKHR: return "ErrorIncompatibleDisplayKHR";
        case vk::Result::eErrorValidationFailedEXT: return "ErrorValidationFailedEXT";
        case vk::Result::eErrorInvalidShaderNV: return "ErrorInvalidShaderNV";
        case vk::Result::eErrorInvalidDrmFormatModifierPlaneLayoutEXT: return "ErrorInvalidDrmFormatModifierPlaneLayoutEXT";
        case vk::Result::eErrorNotPermittedEXT: return "ErrorNotPermittedEXT";
#ifdef VK_USE_PLATFORM_WIN32_KHR
        case vk::Result::eErrorFullScreenExclusiveModeLostEXT: return "ErrorFullScreenExclusiveModeLostEXT";
#endif
        case vk::Result::eThreadIdleKHR: return "ThreadIdleKHR";
        case vk::Result::eThreadDoneKHR: return "ThreadDoneKHR";
        case vk::Result::eOperationDeferredKHR: return "OperationDeferredKHR";
        case vk::Result::eOperationNotDeferredKHR: return "OperationNotDeferredKHR";
        case vk::Result::ePipelineCompileRequiredEXT: return "PipelineCompileRequiredEXT";
        default: return "Unknown result type";
        }
    }

    exception::~exception() = default;

    vulkan_error::vulkan_error(const std::string& what)
        : std::runtime_error (what)
    {
    }

    vulkan_error::vulkan_error(const char* what)
        : std::runtime_error (what)
    {
    }

    vulkan_error::~vulkan_error() = default;

    const char* vulkan_error::what() const noexcept
    {
        return std::runtime_error::what();
    }

    device_capabilities_error::device_capabilities_error(const std::string& what)
        : vulkan_error (what)
    {
    }

    device_capabilities_error::device_capabilities_error(const char* what)
        : vulkan_error (what)
    {
    }

    device_capabilities_error::~device_capabilities_error() = default;

    window_error::window_error(const std::string& what)
        : std::runtime_error (what)
    {
    }

    window_error::window_error(const char* what)
        : std::runtime_error (what)
    {
    }

    window_error::~window_error() = default;

    const char* window_error::what() const noexcept
    {
        return std::runtime_error::what();
    }

    overflow_error::~overflow_error() = default;

    const char* overflow_error::what() const noexcept
    {
        return std::overflow_error::what();
    }

    invalid_options::~invalid_options() = default;

    const char* invalid_options::what() const noexcept
    {
        return std::logic_error::what();
    }

    invalid_shader::~invalid_shader() = default;

    const char* invalid_shader::what() const noexcept
    {
        return std::logic_error::what();
    }
}
