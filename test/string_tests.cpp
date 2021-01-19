#include <core/string.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(core_tests, test_left_trim_string)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    core::left_trim(str);
    ASSERT_EQ(str, "hot chocolate!  \t \n ");
}

TEST(core_tests, test_right_trim_string)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    core::right_trim(str);
    ASSERT_EQ(str, "   \t \n hot chocolate!");
}

TEST(core_tests, test_trim_string)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    core::trim(str);
    ASSERT_EQ(str, "hot chocolate!");
}

TEST(core_tests, test_left_trim_string_copy)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    std::string trimmed_str = core::left_trim_copy(str);
    ASSERT_EQ(trimmed_str, "hot chocolate!  \t \n ");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_right_trim_string_copy)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    std::string trimmed_str = core::right_trim_copy(str);
    ASSERT_EQ(trimmed_str, "   \t \n hot chocolate!");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_trim_string_copy)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    std::string trimmed_str = core::trim_copy(str);
    ASSERT_EQ(trimmed_str, "hot chocolate!");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_left_trim_string_view)
{
    std::string_view str("   \t \n hot chocolate!  \t \n ");
    core::left_trim(str);
    ASSERT_EQ(str, "hot chocolate!  \t \n ");
}

TEST(core_tests, test_right_trim_string_view)
{
    std::string_view str("   \t \n hot chocolate!  \t \n ");
    core::right_trim(str);
    ASSERT_EQ(str, "   \t \n hot chocolate!");
}

TEST(core_tests, test_trim_string_view)
{
    std::string_view str("   \t \n hot chocolate!  \t \n ");
    core::trim(str);
    ASSERT_EQ(str, "hot chocolate!");
}

TEST(core_tests, test_left_trim_string_view_copy)
{
    std::string_view str("   \t \n hot chocolate!  \t \n ");
    std::string_view trimmed_str = core::left_trim_copy(str);
    ASSERT_EQ(trimmed_str, "hot chocolate!  \t \n ");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_right_trim_string_view_copy)
{
    std::string_view str("   \t \n hot chocolate!  \t \n ");
    std::string_view trimmed_str = core::right_trim_copy(str);
    ASSERT_EQ(trimmed_str, "   \t \n hot chocolate!");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_trim_string_view_copy)
{
    std::string_view str("   \t \n hot chocolate!  \t \n ");
    std::string_view trimmed_str = core::trim_copy(str);
    ASSERT_EQ(trimmed_str, "hot chocolate!");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_left_trim_view_string)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    std::string_view trimmed_str = core::left_trim_view(str);
    ASSERT_EQ(trimmed_str, "hot chocolate!  \t \n ");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_right_trim_view_string)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    std::string_view trimmed_str = core::right_trim_view(str);
    ASSERT_EQ(trimmed_str, "   \t \n hot chocolate!");
    ASSERT_NE(trimmed_str, str);
}

TEST(core_tests, test_trim_view_string)
{
    std::string str("   \t \n hot chocolate!  \t \n ");
    std::string_view trimmed_str = core::trim_view(str);
    ASSERT_EQ(trimmed_str, "hot chocolate!");
    ASSERT_NE(trimmed_str, str);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
