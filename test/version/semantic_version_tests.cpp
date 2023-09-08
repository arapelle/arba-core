#include <arba/core/version/semantic_version.hpp>
#include <gtest/gtest.h>

TEST(semantic_version_tests, semantic_version_constructor__x_y_z__no_exception)
{
    core::semantic_version version(1, 2, 3);
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release_version(), "");
    ASSERT_EQ(version.build_metadata(), "");
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_pr__no_exception)
{
    core::semantic_version version(1, 2, 3, "alpha.1");
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release_version(), "alpha.1");
    ASSERT_EQ(version.build_metadata(), "");
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_pr_bm__no_exception)
{
    core::semantic_version version(1, 2, 3, "alpha.1", "specific-build");
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
    ASSERT_EQ(version.pre_release_version(), "alpha.1");
    ASSERT_EQ(version.build_metadata(), "specific-build");
}

TEST(semantic_version_tests, semantic_version_constructor__x_y_z_bpr_bm__expect_exception)
{
    try
    {
        core::semantic_version version(1, 2, 3, "alpha_1", "specific-build");
        FAIL();
    }
    catch (const std::runtime_error&)
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
    catch (const std::runtime_error&)
    {
        SUCCEED();
    }
}

TEST(semantic_version_tests, semantic_version_constructor__sv__no_exception)
{
    core::semantic_version version_1("0.1.2-0.0+specific-build");
    ASSERT_EQ(version_1.major(), 0);
    ASSERT_EQ(version_1.minor(), 1);
    ASSERT_EQ(version_1.patch(), 2);
    ASSERT_EQ(version_1.pre_release_version(), "0.0");
    ASSERT_EQ(version_1.build_metadata(), "specific-build");
    core::semantic_version version_2("0.1.2-15.25+specific-build");
    ASSERT_EQ(version_2.major(), 0);
    ASSERT_EQ(version_2.minor(), 1);
    ASSERT_EQ(version_2.patch(), 2);
    ASSERT_EQ(version_2.pre_release_version(), "15.25");
    ASSERT_EQ(version_2.build_metadata(), "specific-build");
    core::semantic_version version_3("0.1.2-alpha.beta+specific-build");
    ASSERT_EQ(version_3.major(), 0);
    ASSERT_EQ(version_3.minor(), 1);
    ASSERT_EQ(version_3.patch(), 2);
    ASSERT_EQ(version_3.pre_release_version(), "alpha.beta");
    ASSERT_EQ(version_3.build_metadata(), "specific-build");
    core::semantic_version version_4("0.1.2+specific-build");
    ASSERT_EQ(version_4.major(), 0);
    ASSERT_EQ(version_4.minor(), 1);
    ASSERT_EQ(version_4.patch(), 2);
    ASSERT_EQ(version_4.pre_release_version(), "");
    ASSERT_EQ(version_4.build_metadata(), "specific-build");
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
    ASSERT_EQ(version.version_core(), core::trinum_version(0,1,2));
    const core::semantic_version c_version(0,1,2);
    ASSERT_EQ(c_version.version_core(), core::trinum_version(0,1,2));
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
    ASSERT_TRUE(core::semantic_version("1.2.3") < core::semantic_version("1.2.4"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.1") < core::semantic_version("1.2.3-alpha.2"));
    ASSERT_TRUE(core::semantic_version("1.2.3-1.4") < core::semantic_version("1.2.3-alpha.2"));
    ASSERT_TRUE(core::semantic_version("1.2.3-alpha.beta.4") < core::semantic_version("1.2.3-alpha.gamma.2"));
    ASSERT_TRUE(core::semantic_version("1.2.3-1456") < core::semantic_version("1.2.3--lol"));
    ASSERT_TRUE(core::semantic_version("1.2.3-1456") < core::semantic_version("1.2.3-1456.123"));

    ASSERT_FALSE(core::semantic_version("1.2.3-1456.123") < core::semantic_version("1.2.3-1456.123"));
    ASSERT_FALSE(core::semantic_version("1.2.3--lol") < core::semantic_version("1.2.3-1456"));
    ASSERT_FALSE(core::semantic_version("1.2.3-alpha.gamma") < core::semantic_version("1.2.3-alpha.beta"));
    ASSERT_FALSE(core::semantic_version("1.2.3-alpha.beta") < core::semantic_version("1.2.3-alpha"));
    ASSERT_FALSE(core::semantic_version("1.2.3-alpha.7") < core::semantic_version("1.2.3-alpha.6"));
    ASSERT_FALSE(core::semantic_version("1.2.3") < core::semantic_version("1.2.3-alpha.6"));
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
