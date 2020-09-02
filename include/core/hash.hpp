#pragma once

#include <cstdint>

namespace core
{
/**
 * @brief murmur_hash_64
 * @param data  Array of bytes
 * @param len  Length of data
 * @param seed
 * @return hash
 *
 * The implementation is taken from the function MurmurHash64A written in the following file:
 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
 */
uint64_t murmur_hash_64(const void* data, int len, uint64_t seed = 6364136223846793005ULL);
}
