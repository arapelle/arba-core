#pragma once

#include "htow.hpp"

#include <arba/meta/policy/endianness_policy.hpp>

inline namespace arba
{
namespace core
{
// Host TO World:

template <ByteSwappable T>
inline T htow_when(T value, meta::endianness_neutral_t)
{
    return htow(value);
}

template <ByteSwappable T>
inline T htow_when(T value, meta::endianness_specific_t)
{
    return value;
}

// World TO Host:

template <ByteSwappable T>
inline T wtoh_when(T value, meta::endianness_neutral_t)
{
    return wtoh(value);
}

template <ByteSwappable T>
inline T wtoh_when(T value, meta::endianness_specific_t)
{
    return value;
}

} // namespace core
} // namespace arba
