#pragma once

#include <arba/core/type_traits.hpp>

#include <algorithm>
#include <ranges>

inline namespace arba
{
namespace core
{

struct last_value_safe_t { explicit last_value_safe_t() = default; };
constexpr last_value_safe_t last_value_safe;

struct last_value_unsafe_t { explicit last_value_unsafe_t() = default; };
constexpr last_value_unsafe_t last_value_unsafe;

template <class tag_type>
concept last_value_policy = std::is_same_v<tag_type, last_value_safe_t>
                            || std::is_same_v<tag_type, last_value_unsafe_t>;

template <std::ranges::random_access_range range_type>
inline void unstable_erase(range_type& range, std::ranges::iterator_t<range_type> iter,
                    last_value_unsafe_t = last_value_unsafe)
{
    auto last_iter = --std::ranges::end(range);
    if constexpr (is_move_assignable_to_itself_v<std::ranges::range_value_t<range_type>>)
    {
        *iter = std::move(*last_iter);
    }
    else
    {
        if (iter != last_iter) [[likely]]
            *iter = std::move(*last_iter);
    }
    range.pop_back();
}

template <std::ranges::random_access_range range_type>
inline void unstable_erase(range_type& range, typename range_type::iterator iter, last_value_safe_t)
{
    *iter = std::move(*--std::ranges::end(range));
    range.pop_back();
}

}
}
