#include <core/htow.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(htow_tests, test_htow_u16)
{
    uint16_t value = 0x1122;
    uint16_t swapped_value = core::htow(value);
    uint16_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = 0x2211;
    else
        expected_value = 0x1122;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_u32)
{
    uint32_t value = 0x11223344;
    uint32_t swapped_value = core::byte_swap(value);
    uint32_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = 0x44332211;
    else
        expected_value = 0x11223344;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_u64)
{
    uint64_t value = 0x1122334455667788;
    uint64_t swapped_value = core::byte_swap(value);
    uint64_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = 0x8877665544332211;
    else
        expected_value = 0x1122334455667788;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_i16)
{
    int16_t value = static_cast<int16_t>(0x1122);
    int16_t swapped_value = core::htow(value);
    int16_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = static_cast<int16_t>(0x2211);
    else
        expected_value = static_cast<int16_t>(0x1122);
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_i32)
{
    int32_t value = static_cast<int32_t>(0x11223344);
    int32_t swapped_value = core::byte_swap(value);
    int32_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = static_cast<int32_t>(0x44332211);
    else
        expected_value = static_cast<int32_t>(0x11223344);
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_i64)
{
    int64_t value = static_cast<int64_t>(0x1122334455667788);
    int64_t swapped_value = core::byte_swap(value);
    int64_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = static_cast<int64_t>(0x8877665544332211);
    else
        expected_value = static_cast<int64_t>(0x1122334455667788);
    ASSERT_EQ(swapped_value, expected_value);
}

enum enum_u16 : uint16_t { Value_u16 = 0xaabb, Swapped_value_u16 = 0xbbaa };
enum enum_u32 : uint32_t { Value_u32 = 0xaabbccdd, Swapped_value_u32 = 0xddccbbaa };
enum enum_u64 : uint64_t { Value_u64 = 0x1122334455667788, Swapped_value_u64 = 0x8877665544332211 };

TEST(htow_tests, test_htow_enum_u16)
{
    enum_u16 value = Value_u16;
    enum_u16 swapped_value = core::byte_swap(value);
    enum_u16 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = Swapped_value_u16;
    else
        expected_value = Value_u16;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_u32)
{
    enum_u32 value = Value_u32;
    enum_u32 swapped_value = core::byte_swap(value);
    enum_u32 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = Swapped_value_u32;
    else
        expected_value = Value_u32;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_u64)
{
    enum_u64 value = Value_u64;
    enum_u64 swapped_value = core::byte_swap(value);
    enum_u64 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = Swapped_value_u64;
    else
        expected_value = Value_u64;
    ASSERT_EQ(swapped_value, expected_value);
}

enum enum_i16 : int16_t { Value_i16 = -4097, Swapped_value_i16 = -17 };
enum enum_i32 : int32_t { Value_i32 = -268435457, Swapped_value_i32 = -17 };
enum enum_i64 : int64_t { Value_i64 = -1152921504606846977, Swapped_value_i64 = -17 };

TEST(htow_tests, test_htow_enum_i16)
{
    enum_i16 value = Value_i16;
    enum_i16 swapped_value = core::byte_swap(value);
    enum_i16 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = Swapped_value_i16;
    else
        expected_value = Value_i16;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_i32)
{
    enum_i32 value = Value_i32;
    enum_i32 swapped_value = core::byte_swap(value);
    enum_i32 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = Swapped_value_i32;
    else
        expected_value = Value_i32;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_i64)
{
    enum_i64 value = Value_i64;
    enum_i64 swapped_value = core::byte_swap(value);
    enum_i64 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = Swapped_value_i64;
    else
        expected_value = Value_i64;
    ASSERT_EQ(swapped_value, expected_value);
}

enum class enum_class_u16 : uint16_t { Value = 0xaabb, Swapped_value = 0xbbaa };
enum class enum_class_u32 : uint32_t { Value = 0xaabbccdd, Swapped_value = 0xddccbbaa };
enum class enum_class_u64 : uint64_t { Value = 0x1122334455667788, Swapped_value = 0x8877665544332211 };

TEST(htow_tests, test_htow_enum_class_u16)
{
    enum_class_u16 value = enum_class_u16::Value;
    enum_class_u16 swapped_value = core::byte_swap(value);
    enum_class_u16 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = enum_class_u16::Swapped_value;
    else
        expected_value = enum_class_u16::Value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_class_u32)
{
    enum_class_u32 value = enum_class_u32::Value;
    enum_class_u32 swapped_value = core::byte_swap(value);
    enum_class_u32 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = enum_class_u32::Swapped_value;
    else
        expected_value = enum_class_u32::Value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_class_u64)
{
    enum_class_u64 value = enum_class_u64::Value;
    enum_class_u64 swapped_value = core::byte_swap(value);
    enum_class_u64 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = enum_class_u64::Swapped_value;
    else
        expected_value = enum_class_u64::Value;
    ASSERT_EQ(swapped_value, expected_value);
}

enum class enum_class_i16 : int16_t { Value = -4097, Swapped_value = -17 };
enum class enum_class_i32 : int32_t { Value = -268435457, Swapped_value = -17 };
enum class enum_class_i64 : int64_t { Value = -1152921504606846977, Swapped_value = -17 };

TEST(htow_tests, test_htow_enum_class_i16)
{
    enum_class_i16 value = enum_class_i16::Value;
    enum_class_i16 swapped_value = core::byte_swap(value);
    enum_class_i16 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = enum_class_i16::Swapped_value;
    else
        expected_value = enum_class_i16::Value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_class_i32)
{
    enum_class_i32 value = enum_class_i32::Value;
    enum_class_i32 swapped_value = core::byte_swap(value);
    enum_class_i32 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = enum_class_i32::Swapped_value;
    else
        expected_value = enum_class_i32::Value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_enum_class_i64)
{
    enum_class_i64 value = enum_class_i64::Value;
    enum_class_i64 swapped_value = core::byte_swap(value);
    enum_class_i64 expected_value;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = enum_class_i64::Swapped_value;
    else
        expected_value = enum_class_i64::Value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_tests, test_htow_float)
{
    float value = -14.325f;
    float swapped_value = core::byte_swap(value);
    if constexpr (std::endian::native == std::endian::little)
    {
        float expected_value = 4.176923e-08f;
        ASSERT_FLOAT_EQ(swapped_value, expected_value);
    }
    else
    {
        float expected_value = -14.325f;
        ASSERT_EQ(swapped_value, expected_value);
    }
}

TEST(htow_tests, test_htow_double)
{
    double value = -14489.325;
    double swapped_value = core::byte_swap(value);
    if constexpr (std::endian::native == std::endian::little)
    {
        double expected_value = -1.542348770045492e-180;
        ASSERT_DOUBLE_EQ(swapped_value, expected_value);
    }
    else
    {
        double expected_value = -14489.325;
        ASSERT_EQ(swapped_value, expected_value);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
