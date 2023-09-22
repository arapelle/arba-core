#include <arba/core/debug/breakpoint.hpp>
#include <arba/core/debug/assert.hpp>
#include <arba/core/debug/assert_msg.hpp>
#include <gtest/gtest.h>


TEST(debug_tests, test_assert_macros)
{
    ARBA_ASSERT(true);
    ARBA_ASSERT_MSG(true, "message");
    ARBA_ASSERT_MSG(true, "bad value: {}", 5);
    SUCCEED();
}
