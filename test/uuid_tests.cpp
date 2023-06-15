#include <core/uuid.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

TEST(uuid_tests, test_uuid_constructor)
{
    core::uuid uuid;
    ASSERT_TRUE(uuid.is_nil());
    ASSERT_EQ(uuid.to_string(), "00000000-0000-0000-0000-000000000000");
}

TEST(uuid_tests, test_uuid_constructor_strv)
{
    core::uuid uuid("d84ec443-f5f5-4cbc-89d0-733f656caf5b");
    ASSERT_TRUE(!uuid.is_nil());
    ASSERT_EQ(uuid.to_string(), "d84ec443-f5f5-4cbc-89d0-733f656caf5b");
}

TEST(uuid_tests, test_uuid_cmp_eq)
{
    core::uuid uuid;
    core::uuid uuid_a("d84ec443-f5f5-4cbc-89d0-733f656caf5b");
    core::uuid uuid_b("d84ec443-f5f5-4cbc-89d0-733f656caf5b");
    ASSERT_EQ(uuid, uuid);
    ASSERT_NE(uuid, uuid_a);
    ASSERT_NE(uuid, uuid_b);
    ASSERT_EQ(uuid_a, uuid_b);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
