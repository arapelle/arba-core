#include <arba/core/range/div_range.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <vector>
#include <list>

TEST(div_range_tests, div_range__vector__ok)
{
    std::vector<int> data = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};

    const std::size_t chunk_size = 6;
    auto [chunks, remain_chunk] = core::div_range(data, chunk_size);
    ASSERT_EQ(chunks.size(), 4);

    auto iter = data.cbegin();
    for (std::span<int> chunk : chunks)
    {
        ASSERT_EQ(chunk.size(), chunk_size);
        for (int value : chunk)
            ASSERT_EQ(*iter++, value);
    }

    std::span<int> first_data = chunks.front();
    first_data[0] = 42;
    ASSERT_EQ(data.at(0), 42);
    first_data[1] = 42;
    ASSERT_EQ(data.at(1), 42);
    first_data[2] = 42;
    ASSERT_EQ(data.at(2), 42);

    ASSERT_EQ(remain_chunk.size(), 3);
    ASSERT_EQ(remain_chunk[0], 24);
    ASSERT_EQ(remain_chunk[1], 25);
    ASSERT_EQ(remain_chunk[2], 26);
}

TEST(div_range_tests, div_range__vector_view__ok)
{
    std::vector<int> data = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};

    const std::size_t chunk_size = 6;
    auto [chunks, remain_chunk] = core::div_range(std::views::take(data, 26), chunk_size);
    ASSERT_EQ(chunks.size(), 4);

    auto iter = data.cbegin();
    for (std::span<int> chunk : chunks)
    {
        ASSERT_EQ(chunk.size(), chunk_size);
        for (int value : chunk)
            ASSERT_EQ(*iter++, value);
    }

    std::span<int> first_data = chunks.front();
    first_data[0] = 42;
    ASSERT_EQ(data.at(0), 42);
    first_data[1] = 42;
    ASSERT_EQ(data.at(1), 42);
    first_data[2] = 42;
    ASSERT_EQ(data.at(2), 42);

    ASSERT_EQ(remain_chunk.size(), 2);
    ASSERT_EQ(remain_chunk[0], 24);
    ASSERT_EQ(remain_chunk[1], 25);
}
