module;

#include <gtest/gtest.h>

#include <limits>
#include <vector>
#include <array>
#include <span>

#include <cmath>

export module tests_log;

import log;
import bits_range;
import reflection;

namespace
{
    template <typename T>
    struct TypeName final
    {
    };

    template <>
    struct TypeName<char> final
    {
        static constexpr std::string_view NAME = "char";
    };

    template <>
    struct TypeName<uint8_t> final
    {
        static constexpr std::string_view NAME = "uint8_t";
    };

    template <>
    struct TypeName<uint16_t> final
    {
        static constexpr std::string_view NAME = "uint16_t";
    };

    template <>
    struct TypeName<uint32_t> final
    {
        static constexpr std::string_view NAME = "uint32_t";
    };

    template <>
    struct TypeName<uint64_t> final
    {
        static constexpr std::string_view NAME = "uint64_t";
    };

    template <>
    struct TypeName<int8_t> final
    {
        static constexpr std::string_view NAME = "int8_t";
    };

    template <>
    struct TypeName<int16_t> final
    {
        static constexpr std::string_view NAME = "int16_t";
    };

    template <>
    struct TypeName<int32_t> final
    {
        static constexpr std::string_view NAME = "int32_t";
    };

    template <>
    struct TypeName<int64_t> final
    {
        static constexpr std::string_view NAME = "int64_t";
    };

    template <>
    struct TypeName<float> final
    {
        static constexpr std::string_view NAME = "float";
    };

    template <>
    struct TypeName<double> final
    {
        static constexpr std::string_view NAME = "double";
    };

    template <>
    struct TypeName<long double> final
    {
        static constexpr std::string_view NAME = "long double";
    };
}

TEST(log, string_view)
{
    using namespace ge;
    using ge::log;

    log(LogDestination::StdOut, "test\n");
    log_flush(LogDestination::StdOut);

    log(LogDestination::StdErr, "test\n");
    log_flush(LogDestination::StdErr);

    SUCCEED();
}

template <class T>
class log_integral : public testing::Test
{
};

using Intergrals = testing::Types
<
    char,

    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,

    int8_t,
    int16_t,
    int32_t,
    int64_t
>;

TYPED_TEST_SUITE(log_integral, Intergrals);

TYPED_TEST(log_integral, integral)
{
    using namespace ge;

    using T = TypeParam;

    log(LogDestination::StdOut, TypeName<T>::NAME);

    log(LogDestination::StdOut, ": min = ");
    log(LogDestination::StdOut, std::numeric_limits<T>::min());

    log(LogDestination::StdOut, "; max = ");
    log(LogDestination::StdOut, std::numeric_limits<T>::max());

    log(LogDestination::StdOut, "; 10 = ");
    log(LogDestination::StdOut, static_cast<T>(10));
    // standart says: "Digits in the range 10..35 (inclusive) are represented as lowercase characters a..z"
    // But I don't that for any integral type

    log(LogDestination::StdOut, ";\n");

    log_flush(LogDestination::StdOut);
}

template <class T>
class log_floating_point : public testing::Test
{
};

using FloatingPoints = testing::Types
<
    float,
    double,
    long double
>;

TYPED_TEST_SUITE(log_floating_point, FloatingPoints);

TYPED_TEST(log_floating_point, floating_point)
{
    using namespace ge;

    using T = TypeParam;

    log(LogDestination::StdOut, TypeName<T>::NAME);

    log(LogDestination::StdOut, ": min = ");
    log(LogDestination::StdOut, std::numeric_limits<T>::min());

    log(LogDestination::StdOut, "; max = ");
    log(LogDestination::StdOut, std::numeric_limits<T>::max());

    log(LogDestination::StdOut, "; nan = ");
    log(LogDestination::StdOut, static_cast<T>(NAN));

    log(LogDestination::StdOut, "; inf = ");
    log(LogDestination::StdOut, static_cast<T>(INFINITY));

    log(LogDestination::StdOut, ";\n");

    log_flush(LogDestination::StdOut);
}

TEST(log, bool)
{
    using namespace ge;

    log(LogDestination::StdOut, "true: ");
    log(LogDestination::StdOut, true);

    log(LogDestination::StdOut, " false: ");
    log(LogDestination::StdOut, false);

    log(LogDestination::StdOut, "\n");

    log_flush(LogDestination::StdOut);
}


TEST(log, ranges)
{
    using namespace ge;

    log(LogDestination::StdOut, std::vector{1, 2, 3});
    log(LogDestination::StdOut, "\n");

    log(LogDestination::StdOut, std::array{1, 2, 3});
    log(LogDestination::StdOut, "\n");

    const std::vector<int> buf{1, 2, 3};
    log(LogDestination::StdOut, std::span{buf});
    log(LogDestination::StdOut, "\n");

    log(LogDestination::StdOut, std::string{"must not be printed as range"});
    log(LogDestination::StdOut, "\n");

    log_flush(LogDestination::StdOut);
}

TEST(log, variadic)
{
    using namespace ge;

    log(LogDestination::StdOut, 1, " ", 2.3f, "\n");

    log_flush(LogDestination::StdOut);
}

namespace
{
    enum class BitFlags : uint8_t
    {
        Foo = 1,
        Bar = 2,
        Baz = 4
    };

    std::string_view to_string_view(const BitFlags flags)
    {
        switch (flags)
        {
        case BitFlags::Foo: return "Foo";
        case BitFlags::Bar: return "Bar";
        case BitFlags::Baz: return "Baz";
        }

        __builtin_unreachable();
    }
}

TEST(log, enums)
{
    using namespace ge;

    BitFlags flag = BitFlags::Foo;

    log(LogDestination::StdOut, flag);
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}

TEST(log, bit_flags)
{
    using namespace ge;

    const uint8_t flags =
        static_cast<uint8_t>(BitFlags::Foo) |
        static_cast<uint8_t>(BitFlags::Baz);

    log(LogDestination::StdOut, to_enum_bits_range<BitFlags>(flags));
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}

namespace
{
    struct SomeStruct final
    {
        int value;
    };

    void log_non_trivial(const ge::LogDestination destination, const SomeStruct& value) noexcept
    {
        using namespace ge;

        log(destination, "SomeStruct: ", value.value);
    }
}

TEST(log, non_trivial)
{
    using namespace ge;

    const SomeStruct non_trivial_value{1};
    log(LogDestination::StdOut, non_trivial_value);
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}

namespace
{
    struct Foo final
    {
        int integer = 1;
        std::string_view string = "text";
    };

    struct Bar final
    {
        float floating = 2.3f;
        Foo foo = {};
    };

    constexpr auto register_members(const Foo*) noexcept
    {
        using namespace ge;
        return std::tuple
        {
            Member<&Foo::integer>{"integer"},
            Member<&Foo::string>{"string"},
        };
    }

    constexpr auto register_members(const Bar*) noexcept
    {
        using namespace ge;
        return std::tuple
        {
            Member<&Bar::floating>{"floating"},
            Member<&Bar::foo>{"foo"},
        };
    }
}

TEST(log, reflection)
{
    using namespace ge;

    const Bar bar;

    log(LogDestination::StdOut, bar);
    log(LogDestination::StdOut, "\n");
    log_flush(LogDestination::StdOut);
}
