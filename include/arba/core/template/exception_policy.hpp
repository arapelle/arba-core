#pragma once

#include <new>
#include <type_traits>

inline namespace arba
{
namespace core
{

struct maythrow_t
{
    explicit maythrow_t() = default;
};
constexpr maythrow_t maythrow;

template <class tag_type>
concept exception_policy = std::is_same_v<tag_type, std::nothrow_t> || std::is_same_v<tag_type, maythrow_t>;

}
}
