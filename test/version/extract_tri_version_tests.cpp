#include <arba/core/version/_private/extract_tri_version.hpp>
#include <gtest/gtest.h>

inline namespace arba
{
namespace core
{
namespace private_
{

[[nodiscard]] constexpr bool
extract_version_number_(std::string_view version,
                        std::string_view& num_sv, std::string_view sep, bool end_is_ok)
{
    auto iter = version.cbegin();
    return extract_version_number_(iter, version.cend(), num_sv, sep, end_is_ok);
}

}
}
}

using namespace core::private_;

// extract_version_number

TEST(extract_tri_version, extract_version_number__empty_str__false)
{
    std::string_view num_sv;
    ASSERT_FALSE(extract_version_number_("", num_sv, ".-+", true));
}

TEST(extract_tri_version, extract_version_number__bad_char__false)
{
    std::string_view num_sv;
    ASSERT_FALSE(extract_version_number_("#", num_sv, ".-+", true));
}

TEST(extract_tri_version, extract_version_number__0_good_next__true)
{
    std::string_view num_sv;
    ASSERT_TRUE(extract_version_number_("0.", num_sv, ".", false));
    ASSERT_EQ(num_sv, "0");
}

TEST(extract_tri_version, extract_version_number__0_end_is_ok__true)
{
    std::string_view num_sv;
    ASSERT_TRUE(extract_version_number_("0", num_sv, "-+", true));
    ASSERT_EQ(num_sv, "0");
}

TEST(extract_tri_version, extract_version_number__0_bad_next__false)
{
    std::string_view num_sv;
    ASSERT_FALSE(extract_version_number_("00", num_sv, ".-+", false));
}

TEST(extract_tri_version, extract_version_number__0_end_is_not_ok__false)
{
    std::string_view num_sv;
    ASSERT_FALSE(extract_version_number_("0", num_sv, ".", false));
}

TEST(extract_tri_version, extract_version_number__pos_num_good_next__true)
{
    std::string_view num_sv;
    ASSERT_TRUE(extract_version_number_("112.", num_sv, ".", false));
    ASSERT_EQ(num_sv, "112");
}

TEST(extract_tri_version, extract_version_number__pos_num_end_is_ok__true)
{
    std::string_view num_sv;
    ASSERT_TRUE(extract_version_number_("112", num_sv, "-+", true));
    ASSERT_EQ(num_sv, "112");
}

TEST(extract_tri_version, extract_version_number__pos_num_bad_next__false)
{
    std::string_view num_sv;
    ASSERT_FALSE(extract_version_number_("112#", num_sv, ".-+", false));
}

TEST(extract_tri_version, extract_version_number__pos_num_end_is_not_ok__false)
{
    std::string_view num_sv;
    ASSERT_FALSE(extract_version_number_("112", num_sv, ".", false));
}

// extract_version_core

TEST(extract_tri_version, extract_version_core__empty_str__false)
{
    std::string_view major, minor, patch;
    ASSERT_FALSE(extract_tri_version_("", major, minor, patch));
}

TEST(extract_tri_version, extract_version_core__valid_end_is_ok__true)
{
    std::string_view major, minor, patch;
    ASSERT_TRUE(extract_tri_version_("0.0.0", major, minor, patch));
    ASSERT_EQ(major, "0");
    ASSERT_EQ(minor, "0");
    ASSERT_EQ(patch, "0");
    ASSERT_TRUE(extract_tri_version_("56.178.4", major, minor, patch));
    ASSERT_EQ(major, "56");
    ASSERT_EQ(minor, "178");
    ASSERT_EQ(patch, "4");
    ASSERT_TRUE(extract_tri_version_("5.1.42", major, minor, patch));
    ASSERT_EQ(major, "5");
    ASSERT_EQ(minor, "1");
    ASSERT_EQ(patch, "42");
}

TEST(extract_tri_version, extract_version_core__valid_good_next__true)
{
    std::string_view major, minor, patch;
    ASSERT_TRUE(extract_tri_version_("0.0.0-", major, minor, patch));
    ASSERT_EQ(major, "0");
    ASSERT_EQ(minor, "0");
    ASSERT_EQ(patch, "0");
    ASSERT_TRUE(extract_tri_version_("56.178.4-", major, minor, patch));
    ASSERT_EQ(major, "56");
    ASSERT_EQ(minor, "178");
    ASSERT_EQ(patch, "4");
    ASSERT_TRUE(extract_tri_version_("5.1.42-", major, minor, patch));
    ASSERT_EQ(major, "5");
    ASSERT_EQ(minor, "1");
    ASSERT_EQ(patch, "42");
    ASSERT_TRUE(extract_tri_version_("0.0.0+", major, minor, patch));
    ASSERT_EQ(major, "0");
    ASSERT_EQ(minor, "0");
    ASSERT_EQ(patch, "0");
    ASSERT_TRUE(extract_tri_version_("56.178.4+", major, minor, patch));
    ASSERT_EQ(major, "56");
    ASSERT_EQ(minor, "178");
    ASSERT_EQ(patch, "4");
    ASSERT_TRUE(extract_tri_version_("5.1.42+", major, minor, patch));
    ASSERT_EQ(major, "5");
    ASSERT_EQ(minor, "1");
    ASSERT_EQ(patch, "42");
}

TEST(extract_tri_version, extract_version_core__valid_bad_next__false)
{
    std::string_view major, minor, patch;
    ASSERT_FALSE(extract_tri_version_("0.0.0#", major, minor, patch));
    ASSERT_FALSE(extract_tri_version_("56.178.4#", major, minor, patch));
    ASSERT_FALSE(extract_tri_version_("5.1.42#", major, minor, patch));
}
