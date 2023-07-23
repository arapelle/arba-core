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
        ++counters.at(index);
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
        ++counters.at(index);
    }

    std::ranges::for_each(counters,
                          [=](const auto& counter)
                          {
                              EXPECT_GE(counter, 0.9 * factor);
                          });
}

TEST(random_tests, test_urng_u8_min_max)
{
    core::urng_u8<1, 100> rng(42);
    ASSERT_EQ(rng.min(), 1);
    ASSERT_EQ(rng.max(), 100);

    std::array<unsigned, decltype(rng)::max() + 1> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter){ return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint8_t index = rng();
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter)
                          {
                              EXPECT_GE(counter, 0.9 * factor);
                          });
}

TEST(random_tests, test_urng_u8_max)
{
    core::urng_u8<100> rng(42);
    ASSERT_EQ(rng.min(), 0);
    ASSERT_EQ(rng.max(), 100);

    std::array<unsigned, decltype(rng)::max() + 1> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter){ return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = counters.size() * factor; times; --times)
    {
        uint8_t index = rng();
        ++counters.at(index);
    }

    std::ranges::for_each(counters,
                          [=](const auto& counter)
                          {
                              EXPECT_GE(counter, 0.9 * factor);
                          });
}

TEST(random_tests, test_urng_u8)
{
    core::urng_u8<> rng(42, 1, 100);
    ASSERT_EQ(rng.min(), std::mt19937::min());
    ASSERT_EQ(rng.max(), std::mt19937::max());

    std::array<unsigned, 101> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter){ return counter == 0; }));

    constexpr unsigned factor = 1000;
    for (unsigned times = (counters.size() - 1) * factor; times; --times)
    {
        uint8_t index = rng();
        ++counters.at(index);
    }

    EXPECT_EQ(counters.front(), 0);
    std::ranges::for_each(std::ranges::subrange(counters.begin() + 1, counters.end()),
                          [=](const auto& counter)
                          {
                              EXPECT_GE(counter, 0.9 * factor);
                          });
}

TEST(random_tests, test_urng_i8_min_max)
{
    core::urng_i8<-2, 3> rng(42);
    ASSERT_EQ(rng.min(), -2);
    ASSERT_EQ(rng.max(), 3);

    std::array<unsigned, 6> counters{ 0 };
    ASSERT_TRUE(std::ranges::all_of(counters, [](auto counter){ return counter == 0; }));

    constexpr unsigned factor = 200;
    for (unsigned times = counters.size() * factor; times; --times)
    {
        uint8_t index = rng() + 2;
        ++counters.at(index);
    }

    std::ranges::for_each(counters,
                          [=](const auto& counter)
                          {
                              EXPECT_GE(counter, 0.9 * factor);
                          });
}
