#include <arba/core/template/default_type.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

TEST(default_type_tests, test_parameter_type_or_t__expect_parameter_type)
{
    using tparam = int;
    using number_type = core::parameter_type_or_t<tparam, float>;
    static_assert(std::is_same_v<number_type, int>);
}

TEST(default_type_tests, test_parameter_type_or_t__expect_default_type)
{
    using tparam = core::default_t;
    using number_type = core::parameter_type_or_t<tparam, float>;
    static_assert(std::is_same_v<number_type, float>);
}
