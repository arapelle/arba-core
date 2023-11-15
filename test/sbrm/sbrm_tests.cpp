#include <arba/core/sbrm/sbrm.hpp>
#include <gtest/gtest.h>

TEST(sbrm_tests, test_sbrm)
{
    bool active = true;
    {
        core::sbrm sentry([&active]{ active = false; });
        ASSERT_EQ(active, true);
    }
    ASSERT_EQ(active, false);
}
