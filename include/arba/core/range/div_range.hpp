#pragma once

#include "regular_chunk_view.hpp"

inline namespace arba
{
namespace core
{

template <std::ranges::forward_range RangeType>
    requires std::ranges::sized_range<RangeType>
[[nodiscard]] inline auto div_range_(RangeType&& range, std::size_t chunk_size)
{
    const std::size_t range_size = std::ranges::size(range);
    auto remain_begin = std::next(std::ranges::begin(range), (range_size / chunk_size) * chunk_size);
    auto remain_end = std::ranges::end(range);
    std::ranges::subrange remain_chunk(std::move(remain_begin), std::move(remain_end));
    return std::make_pair(views::regular_chunk(range, chunk_size), std::move(remain_chunk));
}

template <std::ranges::forward_range RangeType>
    requires std::ranges::sized_range<RangeType> && (!std::ranges::view<RangeType>)
[[nodiscard]] inline auto div_range(RangeType& range, std::size_t chunk_size)
{
    return div_range_(std::forward<RangeType>(range), chunk_size);
}

template <std::ranges::view ViewType>
[[nodiscard]] inline auto div_range(ViewType view, std::size_t chunk_size)
{
    return div_range_(std::move(view), chunk_size);
}

}
}
