#include <arba/core/hash.hpp>
#include <bit>

inline namespace arba
{
namespace core
{

uint64_t murmur_hash_64(const void* data, std::size_t len, uint64_t seed)
{
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t* data64 = (const uint64_t*)data;
    const uint64_t* end = data64 + (len / 8);

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

    switch (len & 7)
    {
    case 7:
        h ^= uint64_t(data8[6]) << 48;
        [[fallthrough]];
    case 6:
        h ^= uint64_t(data8[5]) << 40;
        [[fallthrough]];
    case 5:
        h ^= uint64_t(data8[4]) << 32;
        [[fallthrough]];
    case 4:
        h ^= uint64_t(data8[3]) << 24;
        [[fallthrough]];
    case 3:
        h ^= uint64_t(data8[2]) << 16;
        [[fallthrough]];
    case 2:
        h ^= uint64_t(data8[1]) << 8;
        [[fallthrough]];
    case 1:
        h ^= uint64_t(data8[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

uint64_t neutral_murmur_hash_64(const void* data, std::size_t len, uint64_t seed)
{
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ len;

    const uint8_t* data8 = reinterpret_cast<const unsigned char*>(data);

    while (len >= sizeof(h))
    {
        uint64_t k;

        if constexpr (std::endian::native == std::endian::little)
        {
            k = static_cast<uint64_t>(data8[0]);
            k |= static_cast<uint64_t>(data8[1]) << 8;
            k |= static_cast<uint64_t>(data8[2]) << 16;
            k |= static_cast<uint64_t>(data8[3]) << 24;
            k |= static_cast<uint64_t>(data8[4]) << 32;
            k |= static_cast<uint64_t>(data8[5]) << 40;
            k |= static_cast<uint64_t>(data8[6]) << 48;
            k |= static_cast<uint64_t>(data8[7]) << 56;
        }
        else
        {
            k = static_cast<uint64_t>(data8[7]);
            k |= static_cast<uint64_t>(data8[6]) << 8;
            k |= static_cast<uint64_t>(data8[5]) << 16;
            k |= static_cast<uint64_t>(data8[4]) << 24;
            k |= static_cast<uint64_t>(data8[3]) << 32;
            k |= static_cast<uint64_t>(data8[2]) << 40;
            k |= static_cast<uint64_t>(data8[1]) << 48;
            k |= static_cast<uint64_t>(data8[0]) << 56;
        }

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data8 += sizeof(h);
        len -= sizeof(h);
    }

    if constexpr (std::endian::native == std::endian::little)
    {
        switch (len)
        {
        case 7:
            h ^= static_cast<uint64_t>(data8[6]) << 48;
            [[fallthrough]];
        case 6:
            h ^= static_cast<uint64_t>(data8[5]) << 40;
            [[fallthrough]];
        case 5:
            h ^= static_cast<uint64_t>(data8[4]) << 32;
            [[fallthrough]];
        case 4:
            h ^= static_cast<uint64_t>(data8[3]) << 24;
            [[fallthrough]];
        case 3:
            h ^= static_cast<uint64_t>(data8[2]) << 16;
            [[fallthrough]];
        case 2:
            h ^= static_cast<uint64_t>(data8[1]) << 8;
            [[fallthrough]];
        case 1:
            h ^= static_cast<uint64_t>(data8[0]);
            h *= m;
        };
    }
    else
    {
        switch (len)
        {
        case 7:
            h ^= static_cast<uint64_t>(data8[0]) << 48;
            [[fallthrough]];
        case 6:
            h ^= static_cast<uint64_t>(data8[1]) << 40;
            [[fallthrough]];
        case 5:
            h ^= static_cast<uint64_t>(data8[2]) << 32;
            [[fallthrough]];
        case 4:
            h ^= static_cast<uint64_t>(data8[3]) << 24;
            [[fallthrough]];
        case 3:
            h ^= static_cast<uint64_t>(data8[4]) << 16;
            [[fallthrough]];
        case 2:
            h ^= static_cast<uint64_t>(data8[5]) << 8;
        case 1:
            h ^= static_cast<uint64_t>(data8[6]);
            h *= m;
        };
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

std::array<uint8_t, 16> neutral_murmur_hash_array_16(const void* data, std::size_t len, uint64_t seed)
{
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    std::array<uint8_t, 16> res;
    uint64_t h = seed ^ len;
    uint64_t h2 = (~seed) ^ len;

    const uint8_t* data8 = reinterpret_cast<const unsigned char*>(data);

    while (len >= sizeof(h))
    {
        uint64_t k;

        if constexpr (std::endian::native == std::endian::little)
        {
            k = static_cast<uint64_t>(data8[0]);
            k |= static_cast<uint64_t>(data8[1]) << 8;
            k |= static_cast<uint64_t>(data8[2]) << 16;
            k |= static_cast<uint64_t>(data8[3]) << 24;
            k |= static_cast<uint64_t>(data8[4]) << 32;
            k |= static_cast<uint64_t>(data8[5]) << 40;
            k |= static_cast<uint64_t>(data8[6]) << 48;
            k |= static_cast<uint64_t>(data8[7]) << 56;
        }
        else
        {
            k = static_cast<uint64_t>(data8[7]);
            k |= static_cast<uint64_t>(data8[6]) << 8;
            k |= static_cast<uint64_t>(data8[5]) << 16;
            k |= static_cast<uint64_t>(data8[4]) << 24;
            k |= static_cast<uint64_t>(data8[3]) << 32;
            k |= static_cast<uint64_t>(data8[2]) << 40;
            k |= static_cast<uint64_t>(data8[1]) << 48;
            k |= static_cast<uint64_t>(data8[0]) << 56;
        }

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;
        h2 *= m;
        h2 ^= k;

        data8 += sizeof(h);
        len -= sizeof(h);
    }

    uint64_t mask = 0;
    if constexpr (std::endian::native == std::endian::little)
    {
        switch (len)
        {
        case 7:
            mask = static_cast<uint64_t>(data8[6]) << 48;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 6:
            mask = static_cast<uint64_t>(data8[5]) << 40;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 5:
            mask = static_cast<uint64_t>(data8[4]) << 32;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 4:
            mask = static_cast<uint64_t>(data8[3]) << 24;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 3:
            mask = static_cast<uint64_t>(data8[2]) << 16;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 2:
            mask = static_cast<uint64_t>(data8[1]) << 8;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 1:
            mask = static_cast<uint64_t>(data8[0]);
            h ^= mask;
            h2 ^= mask;
            h *= m;
            h2 *= m;
        };
    }
    else
    {
        switch (len)
        {
        case 7:
            mask = static_cast<uint64_t>(data8[0]) << 48;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 6:
            mask = static_cast<uint64_t>(data8[1]) << 40;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 5:
            mask = static_cast<uint64_t>(data8[2]) << 32;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 4:
            mask = static_cast<uint64_t>(data8[3]) << 24;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 3:
            mask = static_cast<uint64_t>(data8[4]) << 16;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 2:
            mask = static_cast<uint64_t>(data8[5]) << 8;
            h ^= mask;
            h2 ^= mask;
            [[fallthrough]];
        case 1:
            mask = static_cast<uint64_t>(data8[6]);
            h ^= mask;
            h2 ^= mask;
            h *= m;
            h2 *= m;
        };
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    h2 ^= h2 >> r;
    h2 *= m;
    h2 ^= h2 >> r;

    auto iter = res.begin(), end_iter = iter + sizeof(h);
    for (; iter != end_iter; ++iter)
    {
        *iter = h % 256;
        h /= 256;
    }
    for (iter = end_iter, end_iter = iter + sizeof(h2); iter != end_iter; ++iter)
    {
        *iter = h2 % 256;
        h2 /= 256;
    }

    return res;
}

}
}
