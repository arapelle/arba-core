#pragma once 

#include "byte_swap.hpp"
#include <bit>

namespace core
{
// Host TO World:

template <byte_swappable T>
inline T htow(T value)
{
    if constexpr (std::endian::native == std::endian::little)
        return byte_swap(value);
    else
        return value;
}

// World TO Host:

template <byte_swappable T>
inline T wtoh(T value)
{
    if constexpr (std::endian::native == std::endian::little)
        return byte_swap(value);
    else
        return value;
}

}
