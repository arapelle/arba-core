#include <arba/core/version/trinum_version.hpp>
#include <gtest/gtest.h>

static_assert(sizeof(core::trinum_version<>) == sizeof(uint64_t) * 2);

TEST(trinum_version_tests, trinum_version_constructor__x_y_z__no_exception)
{
    core::trinum_version version(1, 2, 3);
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
}

TEST(trinum_version_tests, trinum_version_constructor__string__no_exception)
{
    core::trinum_version version("0.1.2");
    ASSERT_EQ(version.major(), 0);
    ASSERT_EQ(version.minor(), 1);
    ASSERT_EQ(version.patch(), 2);
}

TEST(trinum_version_tests, trinum_version_constructor__static_trinum_version__no_exception)
{
    core::trinum_version<> version(core::trinum_version<1, 2, 3>{});
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
}

TEST(trinum_version_tests, trinum_version_constructor__string_quadnum__expect_exception)
{
    try
    {
        core::trinum_version version("1.2.3.4");
        FAIL();
    }
    catch (const std::runtime_error& err)
    {
        SUCCEED();
    }
}

TEST(trinum_version_tests, trinum_version_constructor__string_00__expect_exception)
{
    try
    {
        core::trinum_version version("00.2.3");
        FAIL();
    }
    catch (const std::runtime_error& err)
    {
        SUCCEED();
    }
}

TEST(trinum_version_tests, trinum_version_constructor__string_binum__expect_exception)
{
    try
    {
        core::trinum_version version("0.1");
        FAIL();
    }
    catch (const std::runtime_error& err)
    {
        SUCCEED();
    }
}

TEST(trinum_version_tests, set_major__nominal_case__no_exception)
{
    core::trinum_version version(1, 2, 3);
    version.set_major(999);
    ASSERT_EQ(version.major(), 999);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
}

TEST(trinum_version_tests, set_minor__nominal_case__no_exception)
{
    core::trinum_version version(1, 2, 3);
    version.set_minor(999);
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 999);
    ASSERT_EQ(version.patch(), 3);
}

TEST(trinum_version_tests, set_patch__nominal_case__no_exception)
{
    core::trinum_version version(1, 2, 3);
    version.set_patch(999);
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 999);
}

TEST(trinum_version_tests, is_major_compatible_with__compatible_version__expect_true)
{
    ASSERT_TRUE(core::trinum_version("1.4.1").is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_TRUE(core::trinum_version("1.4.1").is_major_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_TRUE(core::trinum_version("1.4.2").is_major_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_TRUE(core::trinum_version("1.5.0").is_major_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_TRUE(core::trinum_version("1.5.6").is_major_compatible_with(core::trinum_version("1.4.1")));
}

TEST(trinum_version_tests, is_major_compatible_with__incompatible_version__expect_false)
{
    ASSERT_FALSE(core::trinum_version("0.4.1").is_major_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("1.4.0").is_major_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("1.3.1").is_major_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("2.4.1").is_major_compatible_with(core::trinum_version("1.4.1")));
}

TEST(trinum_version_tests, is_minor_compatible_with__compatible_version__expect_true)
{
    ASSERT_TRUE(core::trinum_version("1.4.1").is_minor_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_TRUE(core::trinum_version("1.4.1").is_minor_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_TRUE(core::trinum_version("1.4.2").is_minor_compatible_with(core::trinum_version("1.4.1")));
}

TEST(trinum_version_tests, is_minor_compatible_with__incompatible_version__expect_false)
{
    ASSERT_FALSE(core::trinum_version("0.4.1").is_minor_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("1.3.1").is_minor_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("1.4.0").is_minor_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("1.5.1").is_minor_compatible_with(core::trinum_version("1.4.1")));
    ASSERT_FALSE(core::trinum_version("2.4.1").is_minor_compatible_with(core::trinum_version("1.4.1")));
}

TEST(trinum_version_tests, operator_eq__eq_version__expect_true)
{
    ASSERT_EQ(core::trinum_version("2.4.5"), core::trinum_version("2.4.5"));
    ASSERT_EQ(core::trinum_version("2.4.5"), (core::trinum_version<2,4,5>()));
}

TEST(trinum_version_tests, operator_ne__ne_version__expect_true)
{
    ASSERT_NE(core::trinum_version("2.4.5"), core::trinum_version("2.4.6"));
    ASSERT_NE(core::trinum_version("2.4.5"), (core::trinum_version<2,4,6>()));
}

TEST(trinum_version_tests, operator_lt__lt_version__expect_true)
{
    ASSERT_LT(core::trinum_version("2.4.5"), core::trinum_version("2.4.6"));
    ASSERT_LT(core::trinum_version("2.4.5"), core::trinum_version("2.4.10"));
    ASSERT_LT(core::trinum_version("2.4.5"), core::trinum_version("2.5.5"));
    ASSERT_LT(core::trinum_version("2.4.5"), core::trinum_version("2.5.0"));
    ASSERT_LT(core::trinum_version("2.4.5"), core::trinum_version("3.4.5"));
    ASSERT_LT(core::trinum_version("2.4.5"), core::trinum_version("3.0.0"));
    ASSERT_LT(core::trinum_version("2.4.5"), (core::trinum_version<3,0,0>()));
}

TEST(trinum_version_tests, operator_le__le_version__expect_true)
{
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("2.4.5"));
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("2.4.6"));
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("2.4.10"));
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("2.5.5"));
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("2.5.0"));
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("3.4.5"));
    ASSERT_LE(core::trinum_version("2.4.5"), core::trinum_version("3.0.0"));
    ASSERT_LE(core::trinum_version("2.4.5"), (core::trinum_version<3,0,0>()));
}

TEST(trinum_version_tests, operator_lt__ge_version__expect_false)
{
    ASSERT_FALSE(core::trinum_version("2.4.5") < core::trinum_version("2.4.5"));
    ASSERT_FALSE(core::trinum_version("2.4.6") < core::trinum_version("2.4.5"));
}

TEST(trinum_version_tests, operator_gt__gt_version__expect_true)
{
    ASSERT_GT(core::trinum_version("2.4.6"), core::trinum_version("2.4.5"));
    ASSERT_GT(core::trinum_version("2.4.10"), core::trinum_version("2.4.5"));
    ASSERT_GT(core::trinum_version("2.5.5"), core::trinum_version("2.4.5"));
    ASSERT_GT(core::trinum_version("2.5.0"), core::trinum_version("2.4.5"));
    ASSERT_GT(core::trinum_version("3.4.5"), core::trinum_version("2.4.5"));
    ASSERT_GT(core::trinum_version("3.0.0"), core::trinum_version("2.4.5"));
    ASSERT_GT(core::trinum_version("3.0.0"), (core::trinum_version<2,4,5>()));
}

TEST(trinum_version_tests, operator_ge__ge_version__expect_true)
{
    ASSERT_GE(core::trinum_version("2.4.5"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("2.4.6"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("2.4.10"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("2.5.5"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("2.5.0"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("3.4.5"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("3.0.0"), core::trinum_version("2.4.5"));
    ASSERT_GE(core::trinum_version("3.0.0"), (core::trinum_version<2,4,5>()));
}

TEST(trinum_version_tests, operator_gt__ge_version__expect_false)
{
    ASSERT_FALSE(core::trinum_version("2.4.5") > core::trinum_version("2.4.5"));
    ASSERT_FALSE(core::trinum_version("2.4.5") > core::trinum_version("3.4.5"));
}

TEST(trinum_version_tests, up_major__nominal_case__no_exception)
{
    core::trinum_version version(1, 2, 3);
    version.up_major();
    ASSERT_EQ(version, core::trinum_version(2, 0, 0));
}

TEST(trinum_version_tests, up_minor__nominal_case__no_exception)
{
    core::trinum_version version(1, 2, 3);
    version.up_minor();
    ASSERT_EQ(version, core::trinum_version(1, 3, 0));
}

TEST(trinum_version_tests, up_patch__nominal_case__no_exception)
{
    core::trinum_version version(1, 2, 3);
    version.up_patch();
    ASSERT_EQ(version, core::trinum_version(1, 2, 4));
}

TEST(trinum_version_tests, std_format__version__correct_formatted_string)
{
    ASSERT_EQ(std::format("{}", core::trinum_version(2,10,5)), "2.10.5");
}
