#include <arba/core/version.hpp>
#include <gtest/gtest.h>

TEST(project_version_tests, test_version_core)
{
    constexpr unsigned major = 0;
    constexpr unsigned minor = 26;
    constexpr unsigned patch = 0;
    static_assert(arba::core::version.core() == arba::core::tri_version(major, minor, patch));
}
