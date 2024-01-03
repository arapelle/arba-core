#pragma once

#include <concepts>
#include <cstdint>

inline namespace arba
{
namespace core
{
inline namespace concepts
{

template <typename VersionT>
concept TriVersion = requires(const VersionT& value) {
    {
        value.major()
    } -> std::convertible_to<uint64_t>;
    {
        value.minor()
    } -> std::convertible_to<uint32_t>;
    {
        value.patch()
    } -> std::convertible_to<uint32_t>;
};

}
}
}
