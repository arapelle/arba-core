#pragma once

#include "byte_swap.hpp"

#include <bit>

inline namespace arba
{
namespace core
{
// Host TO World:

template <ByteSwappable T>
inline T htow(T value)
{
    if constexpr (std::endian::native == std::endian::little)
        return byte_swap(value);
    else
        return value;
}

// World TO Host:

template <ByteSwappable T>
inline T wtoh(T value)
{
    if constexpr (std::endian::native == std::endian::little)
        return byte_swap(value);
    else
        return value;
}

}
}
