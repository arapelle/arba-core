#include <arba/core/string/string_conversion.hpp>
#include <cstdlib>
#include <gtest/gtest.h>

TEST(string_conversion_tests, stoi64__string_0__ok)
{
    ASSERT_EQ(core::stoi64("0"), 0);
    ASSERT_EQ(core::stoi64("+0"), 0);
    ASSERT_EQ(core::stoi64("-0"), 0);
    ASSERT_EQ(core::stoi64("+000"), 0);
    ASSERT_EQ(core::stoi64("-000"), 0);
    ASSERT_EQ(core::stoi64("  +000  "), 0);
    ASSERT_EQ(core::stoi64("  -000  "), 0);
}

TEST(string_conversion_tests, stoi64__string_negative__ok)
{
    ASSERT_EQ(core::stoi64("-1"), -1);
    ASSERT_EQ(core::stoi64("-521"), -521);
    ASSERT_EQ(core::stoi64("  -521  "), -521);
}

TEST(string_conversion_tests, stoi64__string_positive__ok)
{
    ASSERT_EQ(core::stoi64("1"), 1);
    ASSERT_EQ(core::stoi64("+1"), 1);
    ASSERT_EQ(core::stoi64("521"), 521);
    ASSERT_EQ(core::stoi64("+521"), 521);
    ASSERT_EQ(core::stoi64("  521  "), 521);
    ASSERT_EQ(core::stoi64("  +521  "), 521);
}

TEST(string_conversion_tests, stoi64__string_empty__exception)
{
    try
    {
        core::stoi64("");
        FAIL();
    }
    catch (std::invalid_argument)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(string_conversion_tests, stoi64__string_full_of_spaces__exception)
{
    try
    {
        core::stoi64(" \f\n\r\t\v");
        FAIL();
    }
    catch (std::invalid_argument)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(string_conversion_tests, stoi64__bad_ch_after_number__exception)
{
    try
    {
        core::stoi64(" 654 b  ");
        FAIL();
    }
    catch (std::invalid_argument)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(string_conversion_tests, stoi64__bad_ch_in_number__exception)
{
    try
    {
        core::stoi64(" 65.4  ");
        FAIL();
    }
    catch (std::invalid_argument)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(string_conversion_tests, stoi64__bad_ch_before_number__exception)
{
    try
    {
        core::stoi64(" .654  ");
        FAIL();
    }
    catch (std::invalid_argument)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}
