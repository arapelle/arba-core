#include <core/hash.hpp>

namespace core
{
uint64_t murmur_hash_64(const void* data, int len, uint64_t seed)
{
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t* data64 = (const uint64_t*)data;
    const uint64_t* end = data64 + (len/8);

    while (data64 != end)
    {
        uint64_t k = *data64++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const uint8_t* data8 = (const unsigned char*)data64;

    switch(len & 7)
    {
    case 7: h ^= uint64_t(data8[6]) << 48; [[fallthrough]];
    case 6: h ^= uint64_t(data8[5]) << 40; [[fallthrough]];
    case 5: h ^= uint64_t(data8[4]) << 32; [[fallthrough]];
    case 4: h ^= uint64_t(data8[3]) << 24; [[fallthrough]];
    case 3: h ^= uint64_t(data8[2]) << 16; [[fallthrough]];
    case 2: h ^= uint64_t(data8[1]) << 8; [[fallthrough]];
    case 1: h ^= uint64_t(data8[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
}
