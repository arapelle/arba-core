#pragma once

#include <cstdint>
#include <concepts>

inline namespace arba
{
namespace core
{
inline namespace concepts
{

template <typename VersionT>
concept SimpleVersion = requires(const VersionT& value)
{
    { value.major() } -> std::convertible_to<uint64_t>;
    { value.minor() } -> std::convertible_to<uint32_t>;
    { value.patch() } -> std::convertible_to<uint32_t>;
};

}
}
}
