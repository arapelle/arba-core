#pragma once

#include <array>
#include <cstdint>
#include <ranges>

inline namespace arba
{
namespace core
{
/**
 * @brief murmur_hash_64
 * @param data  Array of bytes
 * @param len  Length of data
 * @param seed
 * @return hash 64
 *
 * The implementation is taken from the function MurmurHash64A written in the following file:
 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
 */
uint64_t murmur_hash_64(const void* data, std::size_t len, uint64_t seed = 6'364'136'223'846'793'005ULL);

template <std::ranges::contiguous_range crange>
    requires std::is_convertible_v<std::ranges::range_value_t<crange>, uint8_t>
inline uint64_t murmur_hash_64(const crange& bytes, uint64_t seed = 6'364'136'223'846'793'005ULL)
{
    return murmur_hash_64(std::ranges::data(bytes), std::ranges::size(bytes), seed);
}

/**
 * @param data  Array of bytes
 * @param len  Length of data
 * @param seed
 * @return hash 64
 *
 * The implementation is inspired from the function MurmurHashNeutral2 written in the following file:
 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
 */
uint64_t neutral_murmur_hash_64(const void* data, std::size_t len, uint64_t seed = 6'364'136'223'846'793'005ULL);

template <std::ranges::contiguous_range crange>
    requires std::is_convertible_v<std::ranges::range_value_t<crange>, uint8_t>
inline uint64_t neutral_murmur_hash_64(const crange& bytes, uint64_t seed = 6'364'136'223'846'793'005ULL)
{
    return neutral_murmur_hash_64(std::ranges::data(bytes), std::ranges::size(bytes), seed);
}

/**
 * @brief neutral_murmur_hash_array_16
 * @param data  Array of bytes
 * @param len  Length of data
 * @param seed
 * @return hash 128 as 16 uint8_t in a std::array
 */
std::array<uint8_t, 16> neutral_murmur_hash_array_16(const void* data, std::size_t len,
                                                     uint64_t seed = 6'364'136'223'846'793'005ULL);

template <std::ranges::contiguous_range crange>
    requires std::is_convertible_v<std::ranges::range_value_t<crange>, uint8_t>
inline std::array<uint8_t, 16> neutral_murmur_hash_array_16(const crange& bytes,
                                                            uint64_t seed = 6'364'136'223'846'793'005ULL)
{
    return neutral_murmur_hash_array_16(std::ranges::data(bytes), std::ranges::size(bytes), seed);
}

}
}
