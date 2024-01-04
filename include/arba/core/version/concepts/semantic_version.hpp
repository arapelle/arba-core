#pragma once

#include "tri_version.hpp"

#include <string_view>

inline namespace arba
{
namespace core
{
inline namespace concepts
{

template <typename VersionT>
concept SemanticVersion = TriVersion<VersionT> && requires(const VersionT& value) {
    {
        value.pre_release_version()
    } -> std::convertible_to<std::string_view>;
    {
        value.build_metadata()
    } -> std::convertible_to<std::string_view>;
};

}
}
}
