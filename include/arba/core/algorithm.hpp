#pragma once

#include <algorithm>
#include <arba/core/type_traits.hpp>
#include <ranges>

inline namespace arba
{
namespace core
{

// unstable_erase()

struct any_value_t
{
    explicit any_value_t() = default;
};
constexpr any_value_t any_value;

struct not_last_value_t
{
    explicit not_last_value_t() = default;
};
constexpr not_last_value_t not_last_value;

template <class tag_type>
concept erase_value_policy = std::is_same_v<tag_type, any_value_t> || std::is_same_v<tag_type, not_last_value_t>;

template <std::ranges::random_access_range range_type>
inline void unstable_erase(range_type& range, std::ranges::iterator_t<range_type> iter, any_value_t = any_value)
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
inline void unstable_erase(range_type& range, typename range_type::iterator iter, not_last_value_t)
{
    *iter = std::move(*--std::ranges::end(range));
    range.pop_back();
}

// quick_erase()

template <std::ranges::random_access_range range_type, erase_value_policy erase_value_policy>
inline void quick_erase(range_type& range, std::ranges::iterator_t<range_type> iter, erase_value_policy policy)
{
    unstable_erase(range, iter, policy);
}

template <std::ranges::range range_type, erase_value_policy erase_value_policy>
inline void quick_erase(range_type& range, std::ranges::iterator_t<range_type> iter, erase_value_policy)
{
    range.erase(iter);
}

template <std::ranges::range range_type>
inline void quick_erase(range_type& range, typename range_type::iterator iter)
{
    quick_erase(range, iter, any_value);
}

}
}
