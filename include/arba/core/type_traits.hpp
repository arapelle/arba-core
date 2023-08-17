#pragma once

#include <type_traits>
#include <cstdint>
#include <atomic>
#include <arba/core/template/thread_policy.hpp>

inline namespace arba
{
namespace core
{

template <typename sign_type>
concept signedness = std::is_same_v<sign_type, signed> || std::is_same_v<sign_type, unsigned>;

template <unsigned bitsize, signedness = signed, thread_policy = thread_unsafe_t>
struct make_integer;

template <unsigned bitsize, signedness sign_type = signed, thread_policy th_policy = thread_unsafe_t>
using make_integer_t = typename make_integer<bitsize, sign_type, th_policy>::type;

template <> struct make_integer<8> { using type = std::int8_t; };
template <> struct make_integer<16> { using type = std::int16_t; };
template <> struct make_integer<32> { using type = std::int32_t; };
template <> struct make_integer<64> { using type = std::int64_t; };

template <unsigned bitsize>
struct make_integer<bitsize, unsigned>
{
    using type = std::make_unsigned_t<make_integer_t<bitsize>>;
};

template <unsigned bitsize, signedness sign_type>
struct make_integer<bitsize, sign_type, thread_safe_t>
{
    using type = std::atomic<make_integer_t<bitsize, sign_type>>;
};

}
}
