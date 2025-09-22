#include <arba/core/byte/byte.hpp>

#include <gtest/gtest.h>

#include <cstdlib>

TEST(byte_tests, std_formatter_byte__default_format__ok)
{
    const std::byte byte{250};
    const std::string value = std::format("{}", byte);
    ASSERT_EQ(value, "250");
}

TEST(byte_tests, std_formatter_byte__format_u__ok)
{
    const std::byte byte{250};
    const std::string value = std::format("{:u}", byte);
    ASSERT_EQ(value, "250");
}

TEST(byte_tests, std_formatter_byte__format_i__ok)
{
    const std::byte byte{250};
    const std::string value = std::format("{:i}", byte);
    ASSERT_EQ(value, "-6");
}

TEST(byte_tests, std_formatter_byte__format_x__ok)
{
    const std::byte byte{255};
    const std::string value = std::format("{:x}", byte);
    ASSERT_EQ(value, "ff");
}

TEST(byte_tests, std_formatter_byte__format_b__ok)
{
    const std::byte byte{0b10101010};
    const std::string value = std::format("{:b}", byte);
    ASSERT_EQ(value, "10101010");
}

TEST(byte_tests, std_formatter_byte__format_o__ok)
{
    const std::byte byte{255};
    const std::string value = std::format("{:o}", byte);
    ASSERT_EQ(value, "377");
}

TEST(byte_tests, std_formatter_byte__format_c__ok)
{
    const std::byte byte{'A'};
    const std::string value = std::format("{:c}", byte);
    ASSERT_EQ(value, "A");
}

TEST(byte_tests, std_formatter_byte__format_0u__ok)
{
    const std::byte byte{65};
    const std::string value = std::format("{:0u}", byte);
    ASSERT_EQ(value, "065");
}

TEST(byte_tests, std_formatter_byte__format_0i__ok)
{
    const std::byte byte{250};
    const std::string value = std::format("{:0i}", byte);
    ASSERT_EQ(value, "-006");
}

TEST(byte_tests, std_formatter_byte__format_0x__ok)
{
    const std::byte byte{255};
    const std::string value = std::format("{:0x}", byte);
    ASSERT_EQ(value, "0xff");
}

TEST(byte_tests, std_formatter_byte__format_0b__ok)
{
    const std::byte byte{0b10101010};
    const std::string value = std::format("{:0b}", byte);
    ASSERT_EQ(value, "0b10101010");
}

TEST(byte_tests, std_formatter_byte__format_0o__ok)
{
    const std::byte byte{255};
    const std::string value = std::format("{:0o}", byte);
    ASSERT_EQ(value, "0o377");
}

TEST(byte_tests, std_formatter_byte__bad_format__err)
{
    const std::byte byte{255};
    try
    {
        std::ignore = std::vformat("{:z}", std::make_format_args(byte));
        FAIL();
    }
    catch (const std::runtime_error&)
    {
        SUCCEED();
    }
}
