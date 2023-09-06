#include <arba/core/version/trinum_version.hpp>
#include <gtest/gtest.h>

static_assert(sizeof(core::trinum_version<1,2,3>) == 1);

TEST(constexpr_trinum_version_tests, trinum_version_constructor__x_y_z__no_exception)
{
    constexpr core::trinum_version<1,2,3> version;
    ASSERT_EQ(version.major(), 1);
    ASSERT_EQ(version.minor(), 2);
    ASSERT_EQ(version.patch(), 3);
}

TEST(constexpr_trinum_version_tests, is_major_compatible_with__compatible_version__expect_true)
{
    ASSERT_TRUE((core::trinum_version<1,4,1>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_TRUE((core::trinum_version<1,4,2>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_TRUE((core::trinum_version<1,5,0>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_TRUE((core::trinum_version<1,5,6>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
}

TEST(constexpr_trinum_version_tests, is_major_compatible_with__incompatible_version__expect_false)
{
    ASSERT_FALSE((core::trinum_version<0,4,1>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<1,4,0>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<1,3,1>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<2,4,1>()).is_major_compatible_with(core::trinum_version<1,4,1>()));
}

TEST(constexpr_trinum_version_tests, is_minor_compatible_with__compatible_version__expect_true)
{
    ASSERT_TRUE((core::trinum_version<1,4,1>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_TRUE((core::trinum_version<1,4,2>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
}

TEST(constexpr_trinum_version_tests, is_minor_compatible_with__incompatible_version__expect_false)
{
    ASSERT_FALSE((core::trinum_version<0,4,1>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<1,3,1>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<1,4,0>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<1,5,1>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
    ASSERT_FALSE((core::trinum_version<2,4,1>()).is_minor_compatible_with(core::trinum_version<1,4,1>()));
}

TEST(constexpr_trinum_version_tests, operator_eq__eq_version__expect_true)
{
    ASSERT_EQ((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,5>()));
}

TEST(constexpr_trinum_version_tests, operator_eq__ne_version__expect_false)
{
    ASSERT_NE((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,6>()));
}

TEST(constexpr_trinum_version_tests, operator_lt__lt_version__expect_true)
{
    ASSERT_LT((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,6>()));
    ASSERT_LT((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,10>()));
    ASSERT_LT((core::trinum_version<2,4,5>()), (core::trinum_version<2,5,5>()));
    ASSERT_LT((core::trinum_version<2,4,5>()), (core::trinum_version<2,5,0>()));
    ASSERT_LT((core::trinum_version<2,4,5>()), (core::trinum_version<3,4,5>()));
    ASSERT_LT((core::trinum_version<2,4,5>()), (core::trinum_version<3,0,0>()));
}

TEST(constexpr_trinum_version_tests, operator_le__le_version__expect_true)
{
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,5>()));
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,6>()));
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,10>()));
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<2,5,5>()));
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<2,5,0>()));
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<3,4,5>()));
    ASSERT_LE((core::trinum_version<2,4,5>()), (core::trinum_version<3,0,0>()));
}

TEST(constexpr_trinum_version_tests, operator_lt__ge_version__expect_false)
{
    ASSERT_FALSE((core::trinum_version<2,4,5>()) < (core::trinum_version<2,4,5>()));
    ASSERT_FALSE((core::trinum_version<2,4,6>()) < (core::trinum_version<2,4,5>()));
}

TEST(constexpr_trinum_version_tests, operator_gt__gt_version__expect_true)
{
    ASSERT_GT((core::trinum_version<2,4,6>()), (core::trinum_version<2,4,5>()));
    ASSERT_GT((core::trinum_version<2,4,10>()), (core::trinum_version<2,4,5>()));
    ASSERT_GT((core::trinum_version<2,5,5>()), (core::trinum_version<2,4,5>()));
    ASSERT_GT((core::trinum_version<2,5,0>()), (core::trinum_version<2,4,5>()));
    ASSERT_GT((core::trinum_version<3,4,5>()), (core::trinum_version<2,4,5>()));
    ASSERT_GT((core::trinum_version<3,0,0>()), (core::trinum_version<2,4,5>()));
}

TEST(constexpr_trinum_version_tests, operator_ge__ge_version__expect_true)
{
    ASSERT_GE((core::trinum_version<2,4,5>()), (core::trinum_version<2,4,5>()));
    ASSERT_GE((core::trinum_version<2,4,6>()), (core::trinum_version<2,4,5>()));
    ASSERT_GE((core::trinum_version<2,4,10>()), (core::trinum_version<2,4,5>()));
    ASSERT_GE((core::trinum_version<2,5,5>()), (core::trinum_version<2,4,5>()));
    ASSERT_GE((core::trinum_version<2,5,0>()), (core::trinum_version<2,4,5>()));
    ASSERT_GE((core::trinum_version<3,4,5>()), (core::trinum_version<2,4,5>()));
    ASSERT_GE((core::trinum_version<3,0,0>()), (core::trinum_version<2,4,5>()));
}

TEST(constexpr_trinum_version_tests, operator_gt__ge_version__expect_false)
{
    ASSERT_FALSE((core::trinum_version<2,4,5>()) > (core::trinum_version<2,4,5>()));
    ASSERT_FALSE((core::trinum_version<2,4,5>()) > (core::trinum_version<3,4,5>()));
}

TEST(constexpr_trinum_version_tests, std_format__version__correct_formatted_string)
{
    ASSERT_EQ(std::format("{}", core::trinum_version<2,10,5>()), "2.10.5");
}
