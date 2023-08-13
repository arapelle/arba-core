#pragma once

#include <type_traits>
#include <cstdint>
#include <atomic>

inline namespace arba
{
namespace core
{

template <uint8_t size, bool is_unsigned = false, bool is_atomic = false>
struct make_integer;

template <uint8_t size, bool is_unsigned = false, bool is_atomic = false>
using make_integer_t = typename make_integer<size, is_unsigned, is_atomic>::type;

template <> struct make_integer<8> { using type = std::int8_t; };
template <> struct make_integer<16> { using type = std::int16_t; };
template <> struct make_integer<32> { using type = std::int32_t; };
template <> struct make_integer<64> { using type = std::int64_t; };

template <uint8_t size>
struct make_integer<size, true>
{
    using type = std::make_unsigned_t<make_integer_t<size>>;
};

template <uint8_t size, bool is_unsigned>
struct make_integer<size, is_unsigned, true>
{
    using type = std::atomic<make_integer_t<size, is_unsigned>>;
};

}
}
