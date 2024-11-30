#include <arba/core/version.hpp>
#include <gtest/gtest.h>

TEST(project_version_tests, test_version_core)
{
    constexpr unsigned major = 0;
    constexpr unsigned minor = 27;
    constexpr unsigned patch = 0;
    static_assert(arba::core::version.core() == arba::vrsn::numver(major, minor, patch));
}
