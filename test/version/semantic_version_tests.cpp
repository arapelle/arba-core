#include <arba/core/version/semantic_version.hpp>
#include <gtest/gtest.h>

TEST(semantic_version_tests, semantic_version_constructor__x_y_z__no_exception)
{
    core::semantic_version version(1, 2, 3);
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release(), "");
    ASSERT_EQ(version.build_metadata(), "");
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_pr__no_exception)
{
    core::semantic_version version(1, 2, 3, "alpha.1");
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release(), "alpha.1");
    ASSERT_EQ(version.build_metadata(), "");
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_pr_bm__no_exception)
{
    core::semantic_version version(1, 2, 3, "alpha.1", "specific-build");
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release(), "alpha.1");
    ASSERT_EQ(version.build_metadata(), "specific-build");
}

TEST(semantic_version_tests, semantic_version_constructor__ver_pr__no_exception)
{
    core::tri_version version_core(1, 2, 3);
    core::semantic_version version(version_core, "pre-release.1");
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release(), "pre-release.1");
    ASSERT_EQ(version.build_metadata(), "");
}

TEST(semantic_version_tests, semantic_version_constructor__ver_pr_bm__no_exception)
{
    core::tri_version version_core(1, 2, 3);
    core::semantic_version version(version_core, "alpha.1", "specific-build");
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release(), "alpha.1");
    ASSERT_EQ(version.build_metadata(), "specific-build");
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_bpr_bm__expect_exception)
{
    try
    {
        core::semantic_version version(1, 2, 3, "alpha_1", "specific-build");
        FAIL();
    }
    catch (...)
    {
        SUCCEED();
    }
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_pr_bbm__expect_exception)
{
    try
    {
        core::semantic_version version(1, 2, 3, "alpha.1", "specific_build");
        FAIL();
    }
    catch (...)
    {
        SUCCEED();
    }
}

void test_semver_valid_sv(unsigned major, unsigned minor, unsigned patch,
                    std::string_view pre_release = "", std::string_view build_metadata = "")
{
    std::string version_str = std::format("{}.{}.{}", major, minor, patch);
    if (!pre_release.empty())
        version_str = std::format("{}-{}", version_str, pre_release);
    if (!build_metadata.empty())
        version_str = std::format("{}+{}", version_str, build_metadata);
    std::string_view version_strv(version_str);
    core::semantic_version semver(version_strv);
    ASSERT_EQ(semver.major(), major);
    ASSERT_EQ(semver.minor(), minor);
    ASSERT_EQ(semver.patch(), patch);
    ASSERT_EQ(semver.pre_release(), pre_release);
    ASSERT_EQ(semver.build_metadata(), build_metadata);
}

TEST(semantic_version_tests, semantic_version_constructor__sv_triv__no_exception)
{
    test_semver_valid_sv(0, 11,  0);
    test_semver_valid_sv(0, 11,  1);
    test_semver_valid_sv(0, 11, 12);
    test_semver_valid_sv(0, 11,  0, "pre");
    test_semver_valid_sv(0, 11,  1, "pre");
    test_semver_valid_sv(0, 11, 12, "pre");
    test_semver_valid_sv(0, 11,  0, "", "bm");
    test_semver_valid_sv(0, 11,  1, "", "bm");
    test_semver_valid_sv(0, 11, 12, "", "bm");
}

TEST(semantic_version_tests, semantic_version_constructor__sv_triv_prer__no_exception)
{
    // w/o bm
    test_semver_valid_sv(0, 1, 0, "0.0");
    test_semver_valid_sv(0, 1, 0, "7.7");
    test_semver_valid_sv(0, 1, 0, "12.23");
    test_semver_valid_sv(0, 1, 0, "123.234");

    test_semver_valid_sv(0, 1, 0, "0a.0a");
    test_semver_valid_sv(0, 1, 0, "0ab.0ab");

    test_semver_valid_sv(0, 1, 0, "7a.7a");
    test_semver_valid_sv(0, 1, 0, "7ab.7ab");

    test_semver_valid_sv(0, 1, 0, "72a.72a");
    test_semver_valid_sv(0, 1, 0, "72ab.72ab");

    test_semver_valid_sv(0, 1, 0, "alpha.alpha");
    test_semver_valid_sv(0, 1, 0, "alpha0.alpha0");
    test_semver_valid_sv(0, 1, 0, "-56.-56");
    test_semver_valid_sv(0, 1, 0, "alpha-56.alpha-56");

    // +bm
    test_semver_valid_sv(0, 1, 0, "0.0", "bm");
    test_semver_valid_sv(0, 1, 0, "7.7", "bm");
    test_semver_valid_sv(0, 1, 0, "12.23", "bm");
    test_semver_valid_sv(0, 1, 0, "123.234", "bm");

    test_semver_valid_sv(0, 1, 0, "0a.0a", "bm");
    test_semver_valid_sv(0, 1, 0, "0ab.0ab", "bm");

    test_semver_valid_sv(0, 1, 0, "7a.7a", "bm");
    test_semver_valid_sv(0, 1, 0, "7ab.7ab", "bm");

    test_semver_valid_sv(0, 1, 0, "72a.72a", "bm");
    test_semver_valid_sv(0, 1, 0, "72ab.72ab", "bm");

    test_semver_valid_sv(0, 1, 0, "alpha.alpha", "bm");
    test_semver_valid_sv(0, 1, 0, "alpha0.alpha0", "bm");
    test_semver_valid_sv(0, 1, 0, "-56.-56", "bm");
    test_semver_valid_sv(0, 1, 0, "alpha-56.alpha-56", "bm");
}

TEST(semantic_version_tests, semantic_version_constructor__sv_full__no_exception)
{
    test_semver_valid_sv(0, 1, 0, "pre", "0aZ-.0aZ-");
    test_semver_valid_sv(0, 1, 0, "pre", "a-Z0.a-Z0");
    test_semver_valid_sv(0, 1, 0, "pre", "00");
}

void test_semver_invalid_sv(std::string_view version_strv)
{
    try
    {
        core::semantic_version semver(version_strv);
        FAIL();
    }
    catch (const std::exception&)
    {
        SUCCEED();
    }
}

TEST(semantic_version_tests, semantic_version_constructor__invalid_sv__expect_exception)
{
    test_semver_invalid_sv("");
    test_semver_invalid_sv(".");
    test_semver_invalid_sv("..");
    test_semver_invalid_sv("..");
    test_semver_invalid_sv("0..");
    test_semver_invalid_sv("0.1.");
    test_semver_invalid_sv("0.1.");
    test_semver_invalid_sv("0.1.0.");
    test_semver_invalid_sv("0.1a.0");
    test_semver_invalid_sv("0.1.0a");

    test_semver_invalid_sv("0.1.0-$0.pre");
    test_semver_invalid_sv("0.1.0-0$.pre");
    test_semver_invalid_sv("0.1.0-pre.0$");

    test_semver_invalid_sv("0.1.0-");
    test_semver_invalid_sv("0.1.0-+");

    test_semver_invalid_sv("0.1.0-00.0a");
    test_semver_invalid_sv("0.1.0-0a.00");
    test_semver_invalid_sv("0.1.0-06.0a");
    test_semver_invalid_sv("0.1.0-0a.06");
    test_semver_invalid_sv("0.1.0-00+bm");
    test_semver_invalid_sv("0.1.0-06+bm");

    test_semver_invalid_sv("0.1.0-pre+");

    test_semver_invalid_sv("0.1.0-pre+$bm");
    test_semver_invalid_sv("0.1.0-pre+.bm");
    test_semver_invalid_sv("0.1.0-pre+bm.");
    test_semver_invalid_sv("0.1.0-pre+b..m");
}

TEST(semantic_version_tests, semantic_version_constructor__bad_sv__expect_exception)
{
    try
    {
        core::semantic_version version_1("0.1.2-alpha_0+specific-build");
        FAIL();
    }
    catch (const std::exception&)
    {
        SUCCEED();
    }

    try
    {
        core::semantic_version version_2("0.1.2-alpha-0+specific_build");
        FAIL();
    }
    catch (const std::exception&)
    {
        SUCCEED();
    }

    try
    {
        core::semantic_version version_3("0.1.2-alpha.bad_item+item.specific_build");
        FAIL();
    }
    catch (const std::exception&)
    {
        SUCCEED();
    }

    try
    {
        core::semantic_version version_4("0.1.2-alpha-0+item.specific_build");
        FAIL();
    }
    catch (const std::exception&)
    {
        SUCCEED();
    }
}

TEST(semantic_version_tests, version_core__normal__no_exception)
{
    core::semantic_version version(0,1,2);
    ASSERT_EQ(version.core(), core::tri_version(0,1,2));
    const core::semantic_version c_version(0,1,2);
    ASSERT_EQ(c_version.core(), core::tri_version(0,1,2));
}

TEST(semantic_version_tests, operator_eq__normal__no_exception)
{
    ASSERT_EQ(core::semantic_version(0,1,2,"alpha.1","build-title"), core::semantic_version(0,1,2,"alpha.1","build-title"));
    ASSERT_EQ(core::semantic_version(0,1,2,"alpha.1","build-title"), core::semantic_version(0,1,2,"alpha.1"));
    ASSERT_EQ(core::semantic_version(0,1,2,"alpha.1","build-title"), core::semantic_version("0.1.2-alpha.1+build-title"));
    ASSERT_EQ(core::semantic_version(0,1,2,"alpha.1","build-title"), core::semantic_version("0.1.2-alpha.1"));
}

TEST(semantic_version_tests, operator_ne__normal__no_exception)
{
    ASSERT_NE(core::semantic_version(0,1,2,"alpha.1","build-title"), core::semantic_version(0,1,2,"alpha.2","build-title"));
    ASSERT_NE(core::semantic_version(0,1,3,"alpha.1","build-title"), core::semantic_version(0,1,2,"alpha.1","build-title"));
}

TEST(semantic_version_tests, operator_lt__normal__no_exception)
{
    EXPECT_TRUE(core::semantic_version("1.2.3") < core::semantic_version("1.2.4"));
    EXPECT_TRUE(core::semantic_version("1.2.3-1234") < core::semantic_version("1.2.3-123a"));
    EXPECT_TRUE(core::semantic_version("1.2.3-alpha.99") < core::semantic_version("1.2.3-alpha.00a"));
    EXPECT_TRUE(core::semantic_version("1.2.3-alpha.9999") < core::semantic_version("1.2.3-alpha.00a"));
    EXPECT_TRUE(core::semantic_version("1.2.3-alpha.7") < core::semantic_version("1.2.3-alpha.22"));
    EXPECT_TRUE(core::semantic_version("1.2.3-alpha.7") < core::semantic_version("1.2.3-alpha.77"));
    EXPECT_TRUE(core::semantic_version("1.2.3-alpha.1") < core::semantic_version("1.2.3-alpha.-v"));
    EXPECT_TRUE(core::semantic_version("1.2.3-1.4") < core::semantic_version("1.2.3-alpha.2"));
    EXPECT_TRUE(core::semantic_version("1.2.3-alpha.beta.4") < core::semantic_version("1.2.3-alpha.gamma.2"));
    EXPECT_TRUE(core::semantic_version("1.2.3-1456") < core::semantic_version("1.2.3--lol"));
    EXPECT_TRUE(core::semantic_version("1.2.3-1456") < core::semantic_version("1.2.3-1456.123"));
    core::semantic_version va("1.2.3-045a");
    core::semantic_version vb("1.2.3-145a");
    EXPECT_TRUE(va < vb);

    EXPECT_FALSE(core::semantic_version("1.2.3-alpha.22") < core::semantic_version("1.2.3-alpha.7"));
    EXPECT_FALSE(core::semantic_version("1.2.3-alpha.77") < core::semantic_version("1.2.3-alpha.7"));
    EXPECT_FALSE(core::semantic_version("1.2.3-1456.123") < core::semantic_version("1.2.3-1456.123"));
    EXPECT_FALSE(core::semantic_version("1.2.3--lol") < core::semantic_version("1.2.3-1456"));
    EXPECT_FALSE(core::semantic_version("1.2.3-alpha.gamma") < core::semantic_version("1.2.3-alpha.beta"));
    EXPECT_FALSE(core::semantic_version("1.2.3-alpha.beta") < core::semantic_version("1.2.3-alpha"));
    EXPECT_FALSE(core::semantic_version("1.2.3-alpha.7") < core::semantic_version("1.2.3-alpha.6"));
    EXPECT_FALSE(core::semantic_version("1.2.3") < core::semantic_version("1.2.3-alpha.6"));
}

TEST(semantic_version_tests, operator_le__normal__no_exception)
{
    ASSERT_TRUE(core::semantic_version("1.2.3") <= core::semantic_version("1.2.3"));
    ASSERT_TRUE(core::semantic_version("1.2.3") <= core::semantic_version("1.2.4"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.1") <= core::semantic_version("1.2.3-alpha.2"));
    ASSERT_TRUE(core::semantic_version("1.2.3-1.4") <= core::semantic_version("1.2.3-alpha.2"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.beta.4") <= core::semantic_version("1.2.3-alpha.gamma.2"));
    ASSERT_TRUE(core::semantic_version("1.2.3-1456") <= core::semantic_version("1.2.3--lol"));
    ASSERT_TRUE(core::semantic_version("1.2.3-1456") <= core::semantic_version("1.2.3-1456.123"));
}

TEST(semantic_version_tests, operator_gt__normal__no_exception)
{
    ASSERT_TRUE(core::semantic_version("1.2.3--lol") > core::semantic_version("1.2.3-1456"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.gamma") > core::semantic_version("1.2.3-alpha.beta"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.beta") > core::semantic_version("1.2.3-alpha"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.7") > core::semantic_version("1.2.3-alpha.6"));
    ASSERT_TRUE(core::semantic_version("1.2.3") > core::semantic_version("1.2.3-alpha.6"));

    ASSERT_FALSE(core::semantic_version("1.2.3-1456.123") > core::semantic_version("1.2.3-1456.123"));
    ASSERT_FALSE(core::semantic_version("1.2.3") > core::semantic_version("1.2.4"));
    ASSERT_FALSE(core::semantic_version("1.2.3-alpha.1") > core::semantic_version("1.2.3-alpha.2"));
    ASSERT_FALSE(core::semantic_version("1.2.3-1.4") > core::semantic_version("1.2.3-alpha.2"));
    ASSERT_FALSE(core::semantic_version("1.2.3-alpha.beta.4") > core::semantic_version("1.2.3-alpha.gamma.2"));
    ASSERT_FALSE(core::semantic_version("1.2.3-1456") > core::semantic_version("1.2.3--lol"));
    ASSERT_FALSE(core::semantic_version("1.2.3-1456") > core::semantic_version("1.2.3-1456.123"));
}

TEST(semantic_version_tests, operator_ge__normal__no_exception)
{
    ASSERT_TRUE(core::semantic_version("1.2.3--lol") >= core::semantic_version("1.2.3-1456"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.gamma") >= core::semantic_version("1.2.3-alpha.beta"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.beta") >= core::semantic_version("1.2.3-alpha"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.7") >= core::semantic_version("1.2.3-alpha.6"));
    ASSERT_TRUE(core::semantic_version("1.2.3") >= core::semantic_version("1.2.3-alpha.6"));
}

TEST(semantic_version__tests, constexpr__valid_args__no_compile_error)
{
    static constexpr core::semantic_version version_1(0, 1, 2, "0.0", "specific-build");
    static_assert(version_1.major() == 0);
    static_assert(version_1.minor() == 1);
    static_assert(version_1.patch() == 2);
    static_assert(version_1.pre_release() == "0.0");
    static_assert(version_1.build_metadata() == "specific-build");

    static constexpr core::semantic_version version_2("98.7.66-0.0+specific-build");
    static_assert(version_2.major() == 98);
    static_assert(version_2.minor() == 7);
    static_assert(version_2.patch() == 66);
    static_assert(version_2.pre_release() == "0.0");
    static_assert(version_2.build_metadata() == "specific-build");

    static_assert(version_1 != version_2);
    static_assert(version_1 < version_2);
}
