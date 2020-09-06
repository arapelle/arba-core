#pragma once

#include <iostream>
#include <type_traits>
#include <concepts>
#include <cstdint>
#if defined(_MSC_VER) && !defined(_DEBUG)
#include <cstdlib>
#endif

namespace core
{
inline uint16_t byte_swap(uint16_t value)
{
#if (defined(__GNUC__) && !defined(__ICC))
    return __builtin_bswap16(value);
#elif defined(_MSC_VER) && !defined(_DEBUG)
    return _byteswap_ushort(value);
#else
    return uint16_t(value << 8) | uint16_t(value >> 8);
#endif
}

inline uint32_t byte_swap(uint32_t value)
{
#if (defined(__GNUC__) && !defined(__ICC))
    return __builtin_bswap32(value);
#elif defined(_MSC_VER) && !defined(_DEBUG)
    return _byteswap_ulong(value);
#else
    return uint32_t((value & 0x000000ff) << 24)
         | uint32_t((value & 0x0000ff00) << 8)
         | uint32_t((value & 0x00ff0000) >> 8)
         | uint32_t((value & 0xff000000) >> 24);
#endif
}

inline uint64_t byte_swap(uint64_t value)
{
#if (defined(__GNUC__) && !defined(__ICC))
    return __builtin_bswap64(value);
#elif defined(_MSC_VER) && !defined(_DEBUG)
    return _byteswap_uint64(value);
#else
    return uint64_t((value & 0x00000000000000ff) << 56)
         | uint64_t((value & 0x000000000000ff00) << 40)
         | uint64_t((value & 0x0000000000ff0000) << 24)
         | uint64_t((value & 0x00000000ff000000) << 8)
         | uint64_t((value & 0x000000ff00000000) >> 8)
         | uint64_t((value & 0x0000ff0000000000) >> 24)
         | uint64_t((value & 0x00ff000000000000) >> 40)
         | uint64_t((value & 0xff00000000000000) >> 56);
#endif
}

inline char16_t byte_swap(char16_t value)
{
    static_assert(sizeof(char16_t) == sizeof(uint16_t));
    uint16_t result = byte_swap(*reinterpret_cast<uint16_t*>(&value));
    return *reinterpret_cast<char16_t*>(&result);
}

inline char32_t byte_swap(char32_t value)
{
    static_assert(sizeof(char32_t) == sizeof(uint32_t));
    uint32_t result = byte_swap(*reinterpret_cast<uint32_t*>(&value));
    return *reinterpret_cast<char32_t*>(&result);
}

inline float byte_swap(float value)
{
    static_assert(sizeof(float) == sizeof(uint32_t));
    uint32_t result = byte_swap(*reinterpret_cast<uint32_t*>(&value));
    return *reinterpret_cast<float*>(&result);
}

inline double byte_swap(double value)
{
    static_assert(sizeof(double) == sizeof(uint64_t));
    uint64_t result = byte_swap(*reinterpret_cast<uint64_t*>(&value));
    return *reinterpret_cast<double*>(&result);
}

template <typename T>
requires std::is_integral_v<T> && std::is_signed_v<T> && (sizeof(T) >= 2)
inline T byte_swap(T value)
{
    return static_cast<T>(byte_swap(static_cast<std::make_unsigned_t<T>>(value)));
}

template <typename T>
requires std::is_enum_v<T> && (sizeof(T) >= 2)
inline T byte_swap(T value)
{
    return static_cast<T>(byte_swap(static_cast<std::underlying_type_t<T>>(value)));
}

template <typename T>
concept byte_swappable = requires(T value)
{
    { byte_swap(value) } -> std::same_as<T>;
};

}
