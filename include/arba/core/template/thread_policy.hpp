#pragma once

#include <type_traits>

inline namespace arba
{
namespace core
{

struct thread_safe_t
{
    explicit thread_safe_t() = default;
};
constexpr thread_safe_t thread_safe;

struct thread_unsafe_t
{
    explicit thread_unsafe_t() = default;
};
constexpr thread_unsafe_t thread_unsafe;

template <class tag_type>
concept thread_policy = std::is_same_v<tag_type, thread_safe_t> || std::is_same_v<tag_type, thread_unsafe_t>;

}
}
