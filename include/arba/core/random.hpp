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

}
}
