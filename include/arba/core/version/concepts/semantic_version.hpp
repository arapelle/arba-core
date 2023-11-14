#pragma once

#include <string_view>
#include "simple_version.hpp"

inline namespace arba
{
namespace core
{
inline namespace concepts
{

template <typename VersionT>
concept SemanticVersion = SimpleVersion<VersionT> && requires(const VersionT& value)
{
    { value.pre_release_version() } -> std::convertible_to<std::string_view>;
    { value.build_metadata() } -> std::convertible_to<std::string_view>;
};

}
}
}
