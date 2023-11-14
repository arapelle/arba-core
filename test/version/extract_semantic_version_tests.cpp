#include <arba/core/version/_private/extract_semantic_version.hpp>
#include <gtest/gtest.h>

inline namespace arba
{
namespace core
{
namespace private_
{

[[nodiscard]] constexpr bool
advance_alphanum_str_(std::string_view str)
{
    auto iter = str.cbegin();
    bool res = advance_alphanum_str_(iter, str.cend());
    return res;
}

}
}
}

using namespace core::private_;

// advance_alphanum_str

TEST(extract_semantic_version, advance_alphanum_str__valid_end_is_ok__true)
{
    ASSERT_TRUE(advance_alphanum_str_("0"));
    ASSERT_TRUE(advance_alphanum_str_("10"));
    ASSERT_TRUE(advance_alphanum_str_("00a"));
    ASSERT_TRUE(advance_alphanum_str_("00A"));
    ASSERT_TRUE(advance_alphanum_str_("00-"));
    ASSERT_TRUE(advance_alphanum_str_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"));
}

TEST(extract_semantic_version, advance_alphanum_str__zero_num__false)
{
    ASSERT_FALSE(advance_alphanum_str_("00"));
    ASSERT_FALSE(advance_alphanum_str_("01"));
}

TEST(extract_semantic_version, advance_alphanum_str__bad_char__false)
{
    ASSERT_FALSE(advance_alphanum_str_("#"));
    ASSERT_FALSE(advance_alphanum_str_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-#"));
}

TEST(extract_semantic_version, advance_alphanum_str__valid_dot_sep__true)
{
    std::string_view::const_iterator iter;
    std::string_view str;
    str = "0."; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '.');
    str = "10."; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '.');
    str = "00a."; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '.');
    str = "00A."; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '.');
    str = "00-."; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '.');
    str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-."; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '.');
}

TEST(extract_semantic_version, advance_alphanum_str__valid_plus_sep__true)
{
    std::string_view::const_iterator iter;
    std::string_view str;
    str = "0+"; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '+');
    str = "10+"; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '+');
    str = "00a+"; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '+');
    str = "00A+"; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '+');
    str = "00-+"; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '+');
    str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-+"; iter = str.cbegin();
    ASSERT_TRUE(advance_alphanum_str_(iter, str.cend()));
    ASSERT_EQ(*iter, '+');
}

// extract_pre_release

TEST(extract_semantic_version, extract_pre_release__empty_str__false)
{
    std::string_view pr_sv;
    ASSERT_FALSE(extract_pre_release_("", pr_sv));
}

TEST(extract_semantic_version, extract_pre_release__first_char_is_sep__false)
{
    std::string_view pr_sv;
    ASSERT_FALSE(extract_pre_release_(".", pr_sv));
    ASSERT_FALSE(extract_pre_release_("+", pr_sv));
}

TEST(extract_semantic_version, extract_pre_release__valid_one_token__true)
{
    std::string_view pr_sv;
    ASSERT_TRUE(extract_pre_release_("0", pr_sv));
    ASSERT_EQ(pr_sv, "0");
    ASSERT_TRUE(extract_pre_release_("10", pr_sv));
    ASSERT_EQ(pr_sv, "10");
    ASSERT_TRUE(extract_pre_release_("00a", pr_sv));
    ASSERT_EQ(pr_sv, "00a");
    ASSERT_TRUE(extract_pre_release_("00A", pr_sv));
    ASSERT_EQ(pr_sv, "00A");
    ASSERT_TRUE(extract_pre_release_("00-", pr_sv));
    ASSERT_EQ(pr_sv, "00-");
    ASSERT_TRUE(extract_pre_release_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-", pr_sv));
    ASSERT_EQ(pr_sv, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-");
}

TEST(extract_semantic_version, extract_pre_release__valid_multi_token__true)
{
    std::string_view pr_sv;
    ASSERT_TRUE(extract_pre_release_("0.al07", pr_sv));
    ASSERT_EQ(pr_sv, "0.al07");
    ASSERT_TRUE(extract_pre_release_("10.al07", pr_sv));
    ASSERT_EQ(pr_sv, "10.al07");
    ASSERT_TRUE(extract_pre_release_("00a.al07", pr_sv));
    ASSERT_EQ(pr_sv, "00a.al07");
    ASSERT_TRUE(extract_pre_release_("00A.al07", pr_sv));
    ASSERT_EQ(pr_sv, "00A.al07");
    ASSERT_TRUE(extract_pre_release_("00-.al07", pr_sv));
    ASSERT_EQ(pr_sv, "00-.al07");
    ASSERT_TRUE(extract_pre_release_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.al07", pr_sv));
    ASSERT_EQ(pr_sv, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.al07");
}

TEST(extract_semantic_version, extract_pre_release__bad_ch__false)
{
    std::string_view pr_sv;
    ASSERT_FALSE(extract_pre_release_("#", pr_sv));
    ASSERT_FALSE(extract_pre_release_("12354daz#", pr_sv));
    ASSERT_FALSE(extract_pre_release_("123.54daz#", pr_sv));
}

TEST(extract_semantic_version, extract_pre_release__bad_num__false)
{
    std::string_view pr_sv;
    ASSERT_FALSE(extract_pre_release_("00", pr_sv));
    ASSERT_FALSE(extract_pre_release_("01", pr_sv));
    ASSERT_FALSE(extract_pre_release_("12.00", pr_sv));
    ASSERT_FALSE(extract_pre_release_("1a.01", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00.a", pr_sv));
    ASSERT_FALSE(extract_pre_release_("01.a", pr_sv));
    ASSERT_FALSE(extract_pre_release_("12.00.a", pr_sv));
    ASSERT_FALSE(extract_pre_release_("1a.01.a", pr_sv));
}

TEST(extract_semantic_version, extract_pre_release__two_consecutive_dots__false)
{
    std::string_view pr_sv;
    ASSERT_FALSE(extract_pre_release_("01az..7c", pr_sv));
}

TEST(extract_semantic_version, extract_pre_release__last_char_is_dot__false)
{
    std::string_view pr_sv;
    ASSERT_FALSE(extract_pre_release_("0.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("10.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00a.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00A.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00-.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("0.al07.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("10.al07.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00a.al07.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00A.al07.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("00-.al07.", pr_sv));
    ASSERT_FALSE(extract_pre_release_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.al07.", pr_sv));
}

TEST(extract_semantic_version, extract_pre_release__valid_last_char_is_plus__true)
{
    std::string_view pr_sv;
    std::string_view str;
    std::string_view::const_iterator iter;
    str = "0+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "0");
    ASSERT_EQ(*iter, '+');
    str = "10+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "10");
    ASSERT_EQ(*iter, '+');
    str = "00a+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "00a");
    ASSERT_EQ(*iter, '+');
    str = "00A+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "00A");
    ASSERT_EQ(*iter, '+');
    str = "00-+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "00-");
    ASSERT_EQ(*iter, '+');
    str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-");
    ASSERT_EQ(*iter, '+');
    str = "0.al07+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "0.al07");
    ASSERT_EQ(*iter, '+');
    str = "10.al07+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "10.al07");
    ASSERT_EQ(*iter, '+');
    str = "00a.al07+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "00a.al07");
    ASSERT_EQ(*iter, '+');
    str = "00A.al07+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "00A.al07");
    ASSERT_EQ(*iter, '+');
    str = "00-.al07+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "00-.al07");
    ASSERT_EQ(*iter, '+');
    str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.al07+"; iter = str.cbegin();
    ASSERT_TRUE(extract_pre_release_(iter, str.cend(), pr_sv));
    ASSERT_EQ(pr_sv, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.al07");
    ASSERT_EQ(*iter, '+');
}

// check_build_metadata

TEST(extract_semantic_version, check_build_metadata__valid_one_token__true)
{
    ASSERT_TRUE(check_build_metadata_("0"));
    ASSERT_TRUE(check_build_metadata_("00"));
    ASSERT_TRUE(check_build_metadata_("10"));
    ASSERT_TRUE(check_build_metadata_("az"));
    ASSERT_TRUE(check_build_metadata_("AZ"));
    ASSERT_TRUE(check_build_metadata_("-"));
    ASSERT_TRUE(check_build_metadata_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"));
}

TEST(extract_semantic_version, check_build_metadata__valid_multi_token__true)
{
    ASSERT_TRUE(check_build_metadata_("0.al07"));
    ASSERT_TRUE(check_build_metadata_("00.al07"));
    ASSERT_TRUE(check_build_metadata_("10.al07"));
    ASSERT_TRUE(check_build_metadata_("az.al07"));
    ASSERT_TRUE(check_build_metadata_("AZ.al07"));
    ASSERT_TRUE(check_build_metadata_("-.al07"));
    ASSERT_TRUE(check_build_metadata_("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-.al07"));
}

TEST(extract_semantic_version, check_build_metadata__empty_str__false)
{
    ASSERT_FALSE(check_build_metadata_(""));
}

TEST(extract_semantic_version, check_build_metadata__valid_first_char_is_dot__false)
{
    ASSERT_FALSE(check_build_metadata_(".al-07"));
}

TEST(extract_semantic_version, check_build_metadata__valid_last_char_is_dot__false)
{
    ASSERT_FALSE(check_build_metadata_("al-07."));
}

TEST(extract_semantic_version, check_build_metadata__valid_two_consecutive_dots__false)
{
    ASSERT_FALSE(check_build_metadata_("al-07..bl0-a07"));
}

TEST(extract_semantic_version, check_build_metadata__bad_char__false)
{
    ASSERT_FALSE(check_build_metadata_("#"));
    ASSERT_FALSE(check_build_metadata_("al-07.bl0#a07"));
}

// extract_semantic_version

TEST(extract_semantic_version, extract_semantic_version__valid__true)
{
    std::string_view major, minor, patch, pre_rel, bdata;
    ASSERT_TRUE(extract_semantic_version_("72.20.37-alpha.1+20231114-004100.hope", major, minor, patch, pre_rel, bdata));
    ASSERT_EQ(major, "72");
    ASSERT_EQ(minor, "20");
    ASSERT_EQ(patch, "37");
    ASSERT_EQ(pre_rel, "alpha.1");
    ASSERT_EQ(bdata, "20231114-004100.hope");
    ASSERT_TRUE(extract_semantic_version_("0.1.0+20231114-004300.n1ce", major, minor, patch, pre_rel, bdata));
    ASSERT_EQ(major, "0");
    ASSERT_EQ(minor, "1");
    ASSERT_EQ(patch, "0");
    ASSERT_EQ(pre_rel, "");
    ASSERT_EQ(bdata, "20231114-004300.n1ce");
    ASSERT_TRUE(extract_semantic_version_("15.17.18", major, minor, patch, pre_rel, bdata));
    ASSERT_EQ(major, "15");
    ASSERT_EQ(minor, "17");
    ASSERT_EQ(patch, "18");
    ASSERT_EQ(pre_rel, "");
    ASSERT_EQ(bdata, "");
}

TEST(extract_semantic_version, extract_semantic_version__invalid__false)
{
    std::string_view major, minor, patch, pr, bm;
    ASSERT_FALSE(extract_semantic_version_("1.20.72#-alpha.1+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha.1#+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha.1+20231114-004700.fa1lure#", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20-alpha.1+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_(".1.20.72-alpha.1+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72.-alpha.1+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-.alpha.1+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha..1+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha.1.+20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha.1+.20231114-004700.fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha.1+20231114-004700..fa1lure", major, minor, patch, pr, bm));
    ASSERT_FALSE(extract_semantic_version_("1.20.72-alpha.1+20231114-004700.fa1lure.", major, minor, patch, pr, bm));
}
