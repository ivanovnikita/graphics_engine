#include "vertex_description.h"
#include "ge/common/exception.h"
#include "ge/render/color.h"

#include "ge/geometry/world_2d_coords.h"

namespace ge::graph
{
    // Описывает какой кусок данных нужно грузить для каждой вершины
    std::array
    <
        vk::VertexInputBindingDescription,
        VERTEX_DESCRIPTION_COUNT
    > get_vertex_binding_descriptions
    (
        const vk::PhysicalDeviceLimits& device_limits,
        const Logger& logger
    )
    {
        constexpr uint32_t vertex_stride = sizeof(World2dCoords);
        constexpr uint32_t color_stride = sizeof(Color);

        const auto validate_stride = [&device_limits, &logger]
        (
            const uint32_t stride
        )
        {
            if (stride > device_limits.maxVertexInputBindingStride)
            {
                if (logger.enabled(LogType::Error))
                {
                    logger.log
                    (
                        LogType::Error,
                        "Vertex stride (", stride, " bytes) "
                        "doesn't fit to device limits (", device_limits.maxVertexInputBindingStride, ")"
                    );
                }
                GE_THROW_EXPECTED_ERROR("Creating vertex binding description failed");
            }
        };
        validate_stride(vertex_stride);
        validate_stride(color_stride);

        // All binding numbers must be distinct
        const std::array result
        {
            vk::VertexInputBindingDescription{}
                // привязка к команде bindVertexBuffers
                .setBinding(0)
                .setStride(vertex_stride)
                .setInputRate(vk::VertexInputRate::eVertex)
            , vk::VertexInputBindingDescription{}
                .setBinding(1)
                .setStride(color_stride)
                .setInputRate(vk::VertexInputRate::eVertex)
        };

        if (result.size() > device_limits.maxVertexInputBindings)
        {
            if (logger.enabled(LogType::Error))
            {
                logger.log
                (
                    LogType::Error,
                    "Vertex bindings count (", result.size(), ") "
                    "doesn't fit to device limits (", device_limits.maxVertexInputBindings, ")"
                );
            }
            GE_THROW_EXPECTED_ERROR("Creating vertex binding description failed");
        }

        return result;
    }

    // Описывает, как разделить на конкретные типы данных кусок буфера, описанного в vertex_binding_description
    std::array
    <
        vk::VertexInputAttributeDescription,
        VERTEX_DESCRIPTION_COUNT
    > get_vertex_attribute_descriptions
    (
        const vk::PhysicalDeviceLimits& device_limits,
        const Logger& logger
    )
    {
        constexpr uint32_t vertex_offset = offsetof(World2dCoords, coords);
        constexpr uint32_t color_offset = offsetof(Color, color);

        const auto validate_offset = [&device_limits, &logger]
        (
            const uint32_t offset
        )
        {
            if (offset > device_limits.maxVertexInputAttributeOffset)
            {
                if (logger.enabled(LogType::Error))
                {
                    logger.log
                    (
                        LogType::Error,
                        "Vertex offset (", offset, " bytes) "
                        "doesn't fit to device limits (", device_limits.maxVertexInputAttributeOffset, ")"
                    );
                }
                GE_THROW_EXPECTED_ERROR("Creating vertex attribute description failed");
            }
        };
        validate_offset(vertex_offset);
        validate_offset(color_offset);

        // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap39.html#_identification_of_formats
        // Every 'binding' here must be present in vertex_binding_description
        // All binding numbers must be distinct
        const std::array result
        {
            vk::VertexInputAttributeDescription{}
                .setBinding(0) // привязка к команде bindVertexBuffers
                .setLocation(0) // layout(location = x) в шейдере

                // На RGB в названии не надо обращать внимание, это не имеет отношения к цвету:
                //    float: VK_FORMAT_R32_SFLOAT
                //    vec2: VK_FORMAT_R32G32_SFLOAT
                //    vec3: VK_FORMAT_R32G32B32_SFLOAT
                //    vec4: VK_FORMAT_R32G32B32A32_SFLOAT
                // Если количество каналов меньше количества компонентов, компоненты GBA будут использовать значения
                // по умолчанию (0, 0, 1). Тип цвета (SFLOAT, UINT, SINT) и разрядность также должны соответствовать
                // типу входных данных шейдера. Ниже представлены примеры:
                //    ivec2: VK_FORMAT_R32G32_SINT, двухкомпонентный вектор 32-битных целых чисел со знаком
                //    uvec4: VK_FORMAT_R32G32B32A32_UINT, 4-компонентный вектор 32-битных целых чисел без знака
                //    double: VK_FORMAT_R64_SFLOAT, число с плавающей запятой двойной точности (64-битное)
                .setFormat(vk::Format::eR32G32Sfloat) // 2 float'а по 32 бит каждый

                .setOffset(vertex_offset) // Смещение от начала куска буфера, считанного для вершины
            , vk::VertexInputAttributeDescription{}
                .setBinding(1)
                .setLocation(1)
                .setFormat(vk::Format::eR32G32B32Sfloat)
                .setOffset(color_offset)
        };

        if (result.size() > device_limits.maxVertexInputAttributes)
        {
            if (logger.enabled(LogType::Error))
            {
                logger.log
                (
                    LogType::Error,
                    "Vertex attributes count (", result.size(), ") "
                    "doesn't fit to device limits (", device_limits.maxVertexInputAttributes, ")"
                );
            }
            GE_THROW_EXPECTED_ERROR("Creating vertex attribute description failed");
        }

        return result;
    }
}
