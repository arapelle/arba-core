#include <arba/core/version.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(project_version_tests, test_version_macros)
{
    ASSERT_EQ(ARBA_CORE_VERSION_MAJOR, 0);
    ASSERT_EQ(ARBA_CORE_VERSION_MINOR, 17);
    ASSERT_EQ(ARBA_CORE_VERSION_PATCH, 0);
    ASSERT_STREQ(ARBA_CORE_VERSION, "0.17.0");
}
