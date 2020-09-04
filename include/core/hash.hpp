#pragma once

#include <array>
#include <cstdint>

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
uint64_t murmur_hash_64(const void* data, std::size_t len, uint64_t seed = 6364136223846793005ULL);

/**
 * @param data  Array of bytes
 * @param len  Length of data
 * @param seed
 * @return hash 64
 *
 * The implementation is inspired from the function MurmurHashNeutral2 written in the following file:
 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
 */
uint64_t neutral_murmur_hash_64(const void* data, std::size_t len, uint64_t seed = 6364136223846793005ULL);

/**
 * @brief neutral_murmur_hash_array_16
 * @param data  Array of bytes
 * @param len  Length of data
 * @param seed
 * @return hash 128 as 16 uint8_t in a std::array
 */
std::array<uint8_t, 16> neutral_murmur_hash_array_16(const void* data, std::size_t len, uint64_t seed = 6364136223846793005ULL);
}
