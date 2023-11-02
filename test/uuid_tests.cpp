#include <arba/core/uuid.hpp>
#include <arba/core/random.hpp>
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

TEST(uuid_tests, test_random_uuid)
{
    core::reseed(42);
    core::uuid uuid = core::uuid::make_random_uuid();
    ASSERT_EQ(uuid.to_string(), "d6e2e56e-7ddf-41c1-a802-25b9b98f97a3");
    ASSERT_EQ(uuid.data()[8] >> 6, 0b00000010);
    ASSERT_EQ(uuid.data()[6] >> 4, 0b00000100);
}

TEST(uuid_tests, test_random_uuid_urng)
{
    core::urng_u64 urng(45);
    core::uuid uuid = core::uuid::make_random_uuid(urng);
    ASSERT_EQ(uuid.to_string(), "db7d5d12-5b01-48e7-b079-ba5eb7c4eeb1");
    ASSERT_EQ(uuid.data()[8] >> 6, 0b00000010);
    ASSERT_EQ(uuid.data()[6] >> 4, 0b00000100);
}
