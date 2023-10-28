#include "vertex_description.h"
#include "2d_graph.h"
#include "ge/common/exception.h"

namespace ge::graph
{
    // layout (location = x) присваивает индекс каждому атрибуту, чтобы в дальнейшем можно было ссылаться на них.
    // Важно знать, что некоторые типы, например 64-битные векторы dvec3, используют несколько слотов.
    // Это значит, что следующий индекс после него должен быть как минимум на 2 больше:
    //
    // layout(location = 0) in dvec3 inPosition;
    // layout(location = 2) in vec3 inColor;
    //
    // Дополнительную информацию о layout квалификаторе можно найти в OpenGL wiki:
    // https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)#Vertex_shader_attribute_index
    // https://www.khronos.org/registry/vulkan/specs/1.2/html/chap21.html

    // Location sizes: Interfaces between programs can be of various types, even user-defined structs and arrays.
    // Some such types consume multiple locations. When a type that consumes multiple locations is used,
    // they will consume locations sequentially. It is a compile/link-time error to have ranges of variables overlap
    // their locations (unless they have different components specifiers and doesn't overlap their component usage).

    // Scalars and vector types that are not doubles all take up one location. The double and dvec2 types also
    // take one location, while dvec3 and dvec4 take up 2 locations. Structs take up locations based on their member
    // types, in-order. Arrays also take up locations based on their array sizes.

    // Here are some examples:

    // struct OutData
    // {
    //  vec3 data1;
    //  dvec4 data2;
    //  float val[3];
    // };

    // layout(location = 0) out vec3 vals[4];    // Consumes 4 locations
    // layout(location = 4) out OutData myOut;   // Consumes 6 locations. dvec4 uses 2, and `val[3]` uses 3 total
    // layout(location = 10) out vec2 texCoord;  // Consumes 1 location

    // The number of available locations is implementation-defined, and it cannot be queried.
    // However, it will be at least one-fourth of GL_MAX_VARYING_COMPONENTS.

    // There are limits on the locations of input and output variables (of all kinds). These limits are based on an
    // implicit assumption in the API that the resources being passed around are done so as groups of 4-element
    // vectors of data. This is why a mat2 takes up 2 locations, while a vec4 only takes up 1, even though they
    // both are 4 floats in size. The mat2 is considered to be passed as two vec4s; the last two components of
    // each vector simply go unused.

    // It is possible to reclaim some of this unused space. To do this, you declare two (or more) variables that use
    // the same location, but use different components within that location.

    // This is done using the component layout qualifier. You must use a location qualifier when using component,
    // even if the variable would have an implicitly assigned location from an interface block declaration.
    // If a variable does not have an explicit component qualifier, it is as through it were set to 0.

    // The component qualifier specifies the starting component that the variable will use. It is illegal to set
    // the component on a variable whose type would cause it to exceed the boundaries of a 4-element vector.
    // So if you have a vec3, the component can only be 0 or 1.

    // If you set two interface variables to the same location, the variables must use components such that
    // the used space in that location does not overlap. So you can't pack a vec3 in the same location with a vec2,
    // but you can pack two floats with a vec2.

    // Arrays (and arrays of arrays) can also have components assigned. This will assign the component to each location
    // that the array covers. So it is possible to do the following:

    // layout(location = 0) out vec2 arr1[5];
    // layout(location = 0, component = 2) out vec2 arr2[4]; //Different sizes are fine.
    // layout(location = 4, component = 2) out float val;    //A non-array takes the last two fields from location 4.

    // Two (or more) variables that share the same location must satisfy the following conditions:

    //    * Share the same base data type. So you can have alias locations for floating-point types, or integer types
    //     (signed and unsigned does not matter for this), but you cannot have a float sit alongside an integer.
    //    * Use the same interpolation qualifiers.

    // The component qualifier can be used for any shader stage input/output declaration. This includes interfaces
    // between shaders, fragment shader outputs, tessellation patch variables, and so forth.
    // However, it may not be used on:
    //    * Variables of matrix types
    //    * Variables of structs types
    //    * Interface blocks (this qualifier can be on members of a block, but not on the block itself)
    //    * Arrays of types on this list (arrays of non-matrix basic types are fine).

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
        constexpr uint32_t vertex_stride = sizeof(Vertex);
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
        constexpr uint32_t vertex_offset = offsetof(Vertex, pos);
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
