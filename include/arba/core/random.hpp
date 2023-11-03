#pragma once

#include <random>

inline namespace arba
{
namespace core
{

template <typename IntType, typename UrngT>
    requires std::is_integral_v<IntType>
[[nodiscard]] inline IntType rand_int(UrngT& rng)
{
    if constexpr (std::is_same_v<IntType, typename UrngT::result_type>)
    {
        return rng();
    }
    else
    {
        return std::uniform_int_distribution<IntType>(std::numeric_limits<IntType>::min(),
                                                      std::numeric_limits<IntType>::max())(rng);
    }
}

template <typename IntType, typename UrngT>
    requires std::is_integral_v<IntType>
[[nodiscard]] inline IntType rand_int(UrngT& rng, IntType min, IntType max)
{
    return std::uniform_int_distribution<IntType>(min, max)(rng);
}

namespace private_
{

using rand_int_engine_type_ = std::mt19937_64;

rand_int_engine_type_& rand_int_engine_();

}

template <typename IntType>
    requires std::is_integral_v<IntType>
[[nodiscard]] inline IntType rand_int()
{
    if constexpr (std::is_same_v<IntType, typename private_::rand_int_engine_type_::result_type>)
    {
        return private_::rand_int_engine_()();
    }
    else
    {
        return std::uniform_int_distribution<IntType>(std::numeric_limits<IntType>::min(),
                                                      std::numeric_limits<IntType>::max())
            (private_::rand_int_engine_());
    }
}

template <typename IntType>
    requires std::is_integral_v<IntType>
[[nodiscard]] inline IntType rand_int(IntType min, IntType max)
{
    return std::uniform_int_distribution<IntType>(min, max)(private_::rand_int_engine_());
}

inline void reseed()
{
    private_::rand_int_engine_().seed(std::random_device{}());
}

inline void reseed(private_::rand_int_engine_type_::result_type value)
{
    private_::rand_int_engine_().seed(value);
}

[[nodiscard]] inline uint8_t rand_u8(auto& rng) { return rand_int<uint8_t>(rng); }
[[nodiscard]] inline uint16_t rand_u16(auto& rng) { return rand_int<uint16_t>(rng); }
[[nodiscard]] inline uint32_t rand_u32(auto& rng) { return rand_int<uint32_t>(rng); }
[[nodiscard]] inline uint64_t rand_u64(auto& rng) { return rand_int<uint64_t>(rng); }

[[nodiscard]] inline int8_t rand_i8(auto& rng) { return rand_int<int8_t>(rng); }
[[nodiscard]] inline int16_t rand_i16(auto& rng) { return rand_int<int16_t>(rng); }
[[nodiscard]] inline int32_t rand_i32(auto& rng) { return rand_int<int32_t>(rng); }
[[nodiscard]] inline int64_t rand_i64(auto& rng) { return rand_int<int64_t>(rng); }

[[nodiscard]] inline std::byte rand_byte(auto& rng) { return std::byte{rand_int<std::underlying_type_t<std::byte>>(rng)}; }

[[nodiscard]] inline uint8_t rand_u8(auto& rng, uint8_t min, uint8_t max) { return rand_int<uint8_t>(rng, min, max); }
[[nodiscard]] inline uint16_t rand_u16(auto& rng, uint16_t min, uint16_t max) { return rand_int<uint16_t>(rng, min, max); }
[[nodiscard]] inline uint32_t rand_u32(auto& rng, uint32_t min, uint32_t max) { return rand_int<uint32_t>(rng, min, max); }
[[nodiscard]] inline uint64_t rand_u64(auto& rng, uint64_t min, uint64_t max) { return rand_int<uint64_t>(rng, min, max); }

[[nodiscard]] inline int8_t rand_i8(auto& rng, int8_t min, int8_t max) { return rand_int<int8_t>(rng, min, max); }
[[nodiscard]] inline int16_t rand_i16(auto& rng, int16_t min, int16_t max) { return rand_int<int16_t>(rng, min, max); }
[[nodiscard]] inline int32_t rand_i32(auto& rng, int32_t min, int32_t max) { return rand_int<int32_t>(rng, min, max); }
[[nodiscard]] inline int64_t rand_i64(auto& rng, int64_t min, int64_t max) { return rand_int<int64_t>(rng, min, max); }

[[nodiscard]] inline std::byte rand_byte(auto& rng,
                                         std::underlying_type_t<std::byte> min, std::underlying_type_t<std::byte> max)
{
    return std::byte{rand_int<std::underlying_type_t<std::byte>>(rng, min, max)};
}
[[nodiscard]] inline std::byte rand_byte(auto& rng, std::byte min, std::byte max)
{
    using byte_int_t = std::underlying_type_t<std::byte>;
    return std::byte{rand_int<byte_int_t>(rng, static_cast<byte_int_t>(min), static_cast<byte_int_t>(max))};
}

[[nodiscard]] inline uint8_t rand_u8() { return rand_int<uint8_t>(); }
[[nodiscard]] inline uint16_t rand_u16() { return rand_int<uint16_t>(); }
[[nodiscard]] inline uint32_t rand_u32() { return rand_int<uint32_t>(); }
[[nodiscard]] inline uint64_t rand_u64() { return rand_int<uint64_t>(); }

[[nodiscard]] inline int8_t rand_i8() { return rand_int<int8_t>(); }
[[nodiscard]] inline int16_t rand_i16() { return rand_int<int16_t>(); }
[[nodiscard]] inline int32_t rand_i32() { return rand_int<int32_t>(); }
[[nodiscard]] inline int64_t rand_i64() { return rand_int<int64_t>(); }

[[nodiscard]] inline std::byte rand_byte() { return std::byte{rand_int<std::underlying_type_t<std::byte>>()}; }

[[nodiscard]] inline uint8_t rand_u8(uint8_t min, uint8_t max) { return rand_int<uint8_t>(min, max); }
[[nodiscard]] inline uint16_t rand_u16(uint16_t min, uint16_t max) { return rand_int<uint16_t>(min, max); }
[[nodiscard]] inline uint32_t rand_u32(uint32_t min, uint32_t max) { return rand_int<uint32_t>(min, max); }
[[nodiscard]] inline uint64_t rand_u64(uint64_t min, uint64_t max) { return rand_int<uint64_t>(min, max); }

[[nodiscard]] inline int8_t rand_i8(int8_t min, int8_t max) { return rand_int<int8_t>(min, max); }
[[nodiscard]] inline int16_t rand_i16(int16_t min, int16_t max) { return rand_int<int16_t>(min, max); }
[[nodiscard]] inline int32_t rand_i32(int32_t min, int32_t max) { return rand_int<int32_t>(min, max); }
[[nodiscard]] inline int64_t rand_i64(int64_t min, int64_t max) { return rand_int<int64_t>(min, max); }

[[nodiscard]] inline std::byte rand_byte(std::underlying_type_t<std::byte> min, std::underlying_type_t<std::byte> max)
{
    return std::byte{rand_int<std::underlying_type_t<std::byte>>(min, max)};
}
[[nodiscard]] inline std::byte rand_byte(std::byte min, std::byte max)
{
    using byte_int_t = std::underlying_type_t<std::byte>;
    return std::byte{rand_int<byte_int_t>(static_cast<byte_int_t>(min), static_cast<byte_int_t>(max))};
}

namespace private_
{

template <class RNG, class IntType, IntType... IntParams>
class uniform_engine_impl_;

template <class RNG, class IntType, IntType MinValue, IntType MaxValue>
class uniform_engine_impl_<RNG, IntType, MinValue, MaxValue> : public RNG
{
public:
    using result_type = IntType;
    using distribution_type = std::uniform_int_distribution<result_type>;

    explicit uniform_engine_impl_(typename RNG::result_type seed)
        : RNG(seed)
    {}

    uniform_engine_impl_()
        : uniform_engine_impl_(typename RNG::result_type(std::random_device{}()))
    {}

    result_type operator()()
    {
        if constexpr (std::is_same_v<result_type, typename RNG::result_type>
                      && min() == std::numeric_limits<result_type>::min()
                      && max() == std::numeric_limits<result_type>::max())
        {
            return static_cast<RNG&>(*this);
        }
        else
        {
            return std::uniform_int_distribution<result_type>(min(), max())(static_cast<RNG&>(*this));
        }
    }

    static constexpr result_type min() { return MinValue; }

    static constexpr result_type max() { return MaxValue; }
};

template <class RNG, class IntType>
class uniform_engine_impl_<RNG, IntType> : public RNG
{
public:
    using result_type = IntType;
    using distribution_type = std::uniform_int_distribution<result_type>;

    explicit uniform_engine_impl_(typename RNG::result_type seed, distribution_type dist = distribution_type())
        : RNG(seed), dist_(std::move(dist))
    {}

    inline uniform_engine_impl_()
        : uniform_engine_impl_(typename RNG::result_type(std::random_device{}()))
    {}

    uniform_engine_impl_(typename RNG::result_type seed, result_type min, result_type max)
        : uniform_engine_impl_(seed, distribution_type(min, max))
    {}

    explicit uniform_engine_impl_(distribution_type dist)
        : uniform_engine_impl_(std::random_device{}(), dist)
    {}

    uniform_engine_impl_(result_type min, result_type max)
        : uniform_engine_impl_(distribution_type(min, max))
    {}

    result_type operator()()
    {
        return dist_(static_cast<RNG&>(*this));
    }

    const distribution_type& distribution() const { return dist_; }
    distribution_type& distribution() { return dist_; }

private:
    distribution_type dist_;
};

}

template <class RNG, class IntType, IntType... IntParams>
class uniform_engine : public private_::uniform_engine_impl_<RNG, IntType, IntParams...>
{
    using base_ = private_::uniform_engine_impl_<RNG, IntType, IntParams...>;

public:
    inline uniform_engine()
        : base_()
    {}

    inline uniform_engine(typename RNG::result_type seed)
        : base_(seed)
    {}

    template <typename... _Up>
    inline explicit uniform_engine(_Up... args)
        : base_( args... )
    {}
};

template <std::uint_fast8_t... IntParams>
using urng_u8 = uniform_engine<std::mt19937, std::uint_fast8_t, IntParams...>;

template <std::int_fast8_t... IntParams>
using urng_i8 = uniform_engine<std::mt19937, std::int_fast8_t, IntParams...>;

template <std::uint_fast16_t... IntParams>
using urng_u16 = uniform_engine<std::mt19937, std::uint_fast16_t, IntParams...>;

template <std::int_fast16_t... IntParams>
using urng_i16 = uniform_engine<std::mt19937, std::int_fast16_t, IntParams...>;

template <std::uint_fast32_t... IntParams>
using urng_u32 = uniform_engine<std::mt19937, std::uint_fast32_t, IntParams...>;

template <std::int_fast32_t... IntParams>
using urng_i32 = uniform_engine<std::mt19937, std::int_fast32_t, IntParams...>;

template <std::uint_fast64_t... IntParams>
using urng_u64 = uniform_engine<std::mt19937_64, std::uint_fast64_t, IntParams...>;

template <std::int_fast64_t... IntParams>
using urng_i64 = uniform_engine<std::mt19937_64, std::int_fast64_t, IntParams...>;

}
}
