#include <arba/core/random.hpp>
#include <experimental/random>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>
#include <cstdlib>

using namespace ::testing;

TEST(random_tests, test_reseed)
{
    core::reseed(42);
    ASSERT_EQ(core::rand_int(0, 1000000), 755156);
    core::reseed(42);
    ASSERT_EQ(core::rand_int(0, 1000000), 755156);
}

TEST(random_tests, test_rand_int_min_max)
{
    core::reseed(42);

    std::array<unsigned, 256> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter){ return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint16_t index = core::rand_int<uint16_t>(1, counters.size() - 1);
        ASSERT_THAT(index, AllOf(Ge(1), Le(counters.size() - 1)));
        ++counters[index];
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter)
    {
        EXPECT_GE(counter, 0.9 * factor);
    });
}

TEST(random_tests, test_rand_int_0_max)
{
    core::reseed(42);

    std::array<unsigned, 256> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter){ return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = counters.size() * factor; times; --times)
    {
        uint16_t index = core::rand_int<uint16_t>(counters.size() - 1);
        ASSERT_THAT(index, AllOf(Ge(0), Le(counters.size() - 1)));
        ++counters[index];
    }

    std::ranges::for_each(counters,
                          [=](const auto& counter)
    {
        EXPECT_GE(counter, 0.9 * factor);
    });
}
