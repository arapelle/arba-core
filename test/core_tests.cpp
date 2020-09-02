#include <core/core.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

TEST(core_tests, basic_test)
{
    ASSERT_EQ(module_name(), "core");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
