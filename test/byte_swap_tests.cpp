#include <core/byte_swap.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(byte_swap_tests, test_byte_swap_u16)
{
    uint16_t value = 0x1122;
    uint16_t swapped_value = core::byte_swap(value);
    uint16_t expected_value = 0x2211;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_u32)
{
    uint32_t value = 0x11223344;
    uint32_t swapped_value = core::byte_swap(value);
    uint32_t expected_value = 0x44332211;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_u64)
{
    uint64_t value = 0x1122334455667788;
    uint64_t swapped_value = core::byte_swap(value);
    uint64_t expected_value = 0x8877665544332211;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_i16)
{
    int16_t value = -4097;
    int16_t swapped_value = core::byte_swap(value);
    int16_t expected_value = -17;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_i32)
{
    int32_t value = -268435457;
    int32_t swapped_value = core::byte_swap(value);
    int32_t expected_value = -17;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_i64)
{
    int64_t value = -1152921504606846977;
    int64_t swapped_value = core::byte_swap(value);
    int64_t expected_value = -17;
    ASSERT_EQ(swapped_value, expected_value);
}

enum enum_u16 : uint16_t { Value_16 = 0xaabb, Swapped_value_16 = 0xbbaa };
enum enum_u32 : uint32_t { Value_32 = 0xaabbccdd, Swapped_value_32 = 0xddccbbaa };
enum enum_u64 : uint64_t { Value_64 = 0x1122334455667788, Swapped_value_64 = 0x8877665544332211 };

enum enum_i16 : int16_t { Value_i16 = -4097, Swapped_value_i16 = -17 };
enum enum_i32 : int32_t { Value_i32 = -268435457, Swapped_value_i32 = -17 };
enum enum_i64 : int64_t { Value_i64 = -1152921504606846977, Swapped_value_i64 = -17 };

TEST(byte_swap_tests, test_byte_swap_enum_u16)
{
    enum_u16 value = Value_16;
    enum_u16 swapped_value = core::byte_swap(value);
    enum_u16 expected_value = Swapped_value_16;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_u32)
{
    enum_u32 value = Value_32;
    enum_u32 swapped_value = core::byte_swap(value);
    enum_u32 expected_value = Swapped_value_32;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_u64)
{
    enum_u64 value = Value_64;
    enum_u64 swapped_value = core::byte_swap(value);
    enum_u64 expected_value = Swapped_value_64;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_i16)
{
    enum_i16 value = Value_i16;
    enum_i16 swapped_value = core::byte_swap(value);
    enum_i16 expected_value = Swapped_value_i16;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_i32)
{
    enum_i32 value = Value_i32;
    enum_i32 swapped_value = core::byte_swap(value);
    enum_i32 expected_value = Swapped_value_i32;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_i64)
{
    enum_i64 value = Value_i64;
    enum_i64 swapped_value = core::byte_swap(value);
    enum_i64 expected_value = Swapped_value_i64;
    ASSERT_EQ(swapped_value, expected_value);
}

enum class enum_class_u16 : uint16_t { Value = 0xaabb, Swapped_value = 0xbbaa };
enum class enum_class_u32 : uint32_t { Value = 0xaabbccdd, Swapped_value = 0xddccbbaa };
enum class enum_class_u64 : uint64_t { Value = 0x1122334455667788, Swapped_value = 0x8877665544332211 };

enum class enum_class_i16 : int16_t { Value = -4097, Swapped_value = -17 };
enum class enum_class_i32 : int32_t { Value = -268435457, Swapped_value = -17 };
enum class enum_class_i64 : int64_t { Value = -1152921504606846977, Swapped_value = -17 };

TEST(byte_swap_tests, test_byte_swap_enum_class_u16)
{
    enum_class_u16 value = enum_class_u16::Value;
    enum_class_u16 swapped_value = core::byte_swap(value);
    enum_class_u16 expected_value = enum_class_u16::Swapped_value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_class_u32)
{
    enum_class_u32 value = enum_class_u32::Value;
    enum_class_u32 swapped_value = core::byte_swap(value);
    enum_class_u32 expected_value = enum_class_u32::Swapped_value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_class_u64)
{
    enum_class_u64 value = enum_class_u64::Value;
    enum_class_u64 swapped_value = core::byte_swap(value);
    enum_class_u64 expected_value = enum_class_u64::Swapped_value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_class_i16)
{
    enum_class_i16 value = enum_class_i16::Value;
    enum_class_i16 swapped_value = core::byte_swap(value);
    enum_class_i16 expected_value = enum_class_i16::Swapped_value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_class_i32)
{
    enum_class_i32 value = enum_class_i32::Value;
    enum_class_i32 swapped_value = core::byte_swap(value);
    enum_class_i32 expected_value = enum_class_i32::Swapped_value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_class_i64)
{
    enum_class_i64 value = enum_class_i64::Value;
    enum_class_i64 swapped_value = core::byte_swap(value);
    enum_class_i64 expected_value = enum_class_i64::Swapped_value;
    ASSERT_EQ(swapped_value, expected_value);
}

std::ostream& operator<<(std::ostream& stream, const char16_t& ch)
{
    const char* cstr = reinterpret_cast<const char*>(&ch);
    return stream << cstr[0] << cstr[1];
}

TEST(byte_swap_tests, test_byte_swap_char16_t)
{
    char16_t value = u'\x1234';
    char16_t swapped_value = core::byte_swap(value);
    char16_t expected_value = u'\x3412';
    ASSERT_EQ(swapped_value, expected_value);
}

std::ostream& operator<<(std::ostream& stream, const char32_t& ch)
{
    const char* cstr = reinterpret_cast<const char*>(&ch);
    return stream << cstr[0] << cstr[1] << cstr[2] << cstr[3];
}

TEST(byte_swap_tests, test_byte_swap_char32_t)
{
    char32_t value = U'\x12345678';
    char32_t swapped_value = core::byte_swap(value);
    char32_t expected_value = U'\x78563412';
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_float)
{
    float value = -12.358f;
    float swapped_value = core::byte_swap(value);
    float expected_value = -12.358f;
    ASSERT_NE(swapped_value, expected_value);
    ASSERT_FLOAT_EQ(swapped_value, 6.7111732e+18);
    swapped_value = core::byte_swap(swapped_value);
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(byte_swap_tests, test_byte_swap_enum_double)
{
    double value = -12.358;
    double swapped_value = core::byte_swap(value);
    double expected_value = -12.358;
    ASSERT_NE(swapped_value, expected_value);
    ASSERT_DOUBLE_EQ(swapped_value, -1.125795992503723e-159);
    swapped_value = core::byte_swap(swapped_value);
    ASSERT_EQ(swapped_value, expected_value);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
