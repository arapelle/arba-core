#pragma once

#include <random>

inline namespace arba
{
namespace core
{
namespace private_
{

using rand_int_engine_type_ = std::mt19937_64;

inline auto& rand_int_engine_()
{
    static thread_local rand_int_engine_type_ rand_engine{std::random_device{}()};
    return rand_engine;
}

}

template <typename IntType>
    requires std::is_integral_v<IntType>
[[nodiscard]] inline IntType rand_int(IntType min, IntType max)
{
    return std::uniform_int_distribution<IntType>(min, max)(private_::rand_int_engine_());
}

template <typename IntType>
    requires std::is_integral_v<IntType>
[[nodiscard]] inline IntType rand_int(IntType max)
{
    return std::uniform_int_distribution<IntType>(0, max)(private_::rand_int_engine_());
}

void reseed();

void reseed(private_::rand_int_engine_type_::result_type value);


template <class RNG, class IntType, IntType... IntParams>
class uniform_engine;

template <class RNG, class IntType, IntType MinValue, IntType MaxValue>
class uniform_engine<RNG, IntType, MinValue, MaxValue> : public RNG
{
public:
    using result_type = IntType;
    using distribution_type = std::uniform_int_distribution<result_type>;

    explicit uniform_engine(typename RNG::result_type seed)
        : RNG(seed)
    {}

    uniform_engine()
        : uniform_engine(typename RNG::result_type(std::random_device{}()))
    {}

    result_type operator()()
    {
        return std::uniform_int_distribution<result_type>(min(), max())(static_cast<RNG&>(*this));
    }

    static constexpr result_type min() { return MinValue; }

    static constexpr result_type max() { return MaxValue; }
};

template <class RNG, class IntType, IntType MaxValue>
class uniform_engine<RNG, IntType, MaxValue> : public uniform_engine<RNG, IntType, 0, MaxValue>
{
public:
    using uniform_engine<RNG, IntType, 0, MaxValue>::uniform_engine;
};

template <class RNG, class IntType>
class uniform_engine<RNG, IntType> : public RNG
{
public:
    using result_type = IntType;
    using distribution_type = std::uniform_int_distribution<result_type>;

    explicit uniform_engine(typename RNG::result_type seed, distribution_type dist = distribution_type())
        : RNG(seed), dist_(std::move(dist))
    {}

    uniform_engine()
        : uniform_engine(typename RNG::result_type(std::random_device{}()))
    {}

    uniform_engine(typename RNG::result_type seed, result_type min, result_type max)
        : uniform_engine(seed, distribution_type(min, max))
    {}

    explicit uniform_engine(distribution_type dist)
        : uniform_engine(std::random_device{}(), dist)
    {}

    uniform_engine(result_type min, result_type max)
        : uniform_engine(distribution_type(min, max))
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
