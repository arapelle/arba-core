#include <arba/core/range/regular_chunk_view.hpp>

#include <gtest/gtest.h>

#include <cstdlib>
#include <vector>
#include <list>

TEST(regular_chunk_view_tests, regular_chunk_range__fit_vector__ok)
{
    std::vector<int> data = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

    using range_iterator_t = std::ranges::iterator_t<decltype(data)>;
    using regular_chunk_iterator_t = core::regular_chunk_iterator<range_iterator_t>;
    static_assert(std::incrementable<regular_chunk_iterator_t>);
    static_assert(std::indirectly_readable<regular_chunk_iterator_t>);
    static_assert(std::forward_iterator<regular_chunk_iterator_t>);
    static_assert(std::bidirectional_iterator<regular_chunk_iterator_t>);
    static_assert(std::random_access_iterator<regular_chunk_iterator_t>);

    auto data_view = std::views::all(data);
    using data_view_t = decltype(data_view);
    using regular_chunk_view_t = core::regular_chunk_view<data_view_t>;
    static_assert(std::ranges::forward_range<regular_chunk_view_t>);
    static_assert(std::ranges::bidirectional_range<regular_chunk_view_t>);
    static_assert(std::ranges::random_access_range<regular_chunk_view_t>);
    static_assert(std::ranges::sized_range<regular_chunk_view_t>);

    const std::size_t chunk_size = 6;
    core::regular_chunk_view chunks(data_view, chunk_size);
    ASSERT_EQ(chunks.size(), 4);

    auto iter = data.cbegin();
    for (std::span<int> chunk : chunks)
    {
        ASSERT_EQ(chunk.size(), chunk_size);
        for (int value : chunk)
            ASSERT_EQ(*iter++, value);
    }

    chunks = core::views::regular_chunk(data, chunk_size);

    iter = data.cbegin();
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
}

TEST(regular_chunk_view_tests, regular_chunk_range__vector__ok)
{
    std::vector<int> data = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28};

    auto data_view = std::views::all(data);

    const std::size_t chunk_size = 6;
    core::regular_chunk_view chunks(data_view, chunk_size);
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
}

TEST(regular_chunk_view_tests, regular_chunk_range__fit_list__ok)
{
    std::list<int> data = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

    using range_iterator_t = std::ranges::iterator_t<decltype(data)>;
    using regular_chunk_iterator_t = core::regular_chunk_iterator<range_iterator_t>;
    static_assert(std::incrementable<regular_chunk_iterator_t>);
    static_assert(std::indirectly_readable<regular_chunk_iterator_t>);
    static_assert(std::forward_iterator<regular_chunk_iterator_t>);
    static_assert(std::bidirectional_iterator<regular_chunk_iterator_t>);
    static_assert(!std::random_access_iterator<regular_chunk_iterator_t>);

    auto data_view = std::views::all(data);
    using data_view_t = decltype(data_view);
    using regular_chunk_view_t = core::regular_chunk_view<data_view_t>;
    static_assert(std::ranges::forward_range<regular_chunk_view_t>);
    static_assert(std::ranges::bidirectional_range<regular_chunk_view_t>);
    static_assert(!std::ranges::random_access_range<regular_chunk_view_t>);
    static_assert(!std::ranges::sized_range<regular_chunk_view_t>);

    const std::size_t chunk_size = 6;
    core::regular_chunk_view chunks(data_view, chunk_size);

    auto iter = data.cbegin();
    for (const auto& chunk : chunks)
    {
        const std::size_t chunk_sz = std::distance(chunk.begin(), chunk.end());
        ASSERT_EQ(chunk_sz, chunk_size);
        for (int value : chunk)
            ASSERT_EQ(*iter++, value);
    }

    chunks = core::views::regular_chunk(data, chunk_size);

    iter = data.cbegin();
    for (const auto& chunk : chunks)
    {
        const std::size_t chunk_sz = std::distance(chunk.begin(), chunk.end());
        ASSERT_EQ(chunk_sz, chunk_size);
        for (int value : chunk)
            ASSERT_EQ(*iter++, value);
    }

    std::size_t nb_chunks = std::distance(chunks.begin(), chunks.end());
    ASSERT_EQ(nb_chunks, 4);
    auto first_iter = chunks.begin();
    *std::next(first_iter->begin(), 0) = 42;
    ASSERT_EQ(*std::next(data.begin(), 0), 42);
    *std::next(first_iter->begin(), 1) = 42;
    ASSERT_EQ(*std::next(data.begin(), 1), 42);
    *std::next(first_iter->begin(), 2) = 42;
    ASSERT_EQ(*std::next(data.begin(), 2), 42);
}

TEST(regular_chunk_view_tests, regular_chunk_range__list__ok)
{
    std::list<int> data = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28};

    auto data_view = std::views::all(data);

    const std::size_t chunk_size = 6;
    core::regular_chunk_view chunks(data_view, chunk_size);

    auto iter = data.cbegin();
    for (const auto& chunk : chunks)
    {
        const std::size_t chunk_sz = std::distance(chunk.begin(), chunk.end());
        ASSERT_EQ(chunk_sz, chunk_size);
        for (int value : chunk)
            ASSERT_EQ(*iter++, value);
    }

    std::size_t nb_chunks = std::distance(chunks.begin(), chunks.end());
    ASSERT_EQ(nb_chunks, 4);
    auto first_iter = chunks.begin();
    *std::next(first_iter->begin(), 0) = 42;
    ASSERT_EQ(*std::next(data.begin(), 0), 42);
    *std::next(first_iter->begin(), 1) = 42;
    ASSERT_EQ(*std::next(data.begin(), 1), 42);
    *std::next(first_iter->begin(), 2) = 42;
    ASSERT_EQ(*std::next(data.begin(), 2), 42);
}
