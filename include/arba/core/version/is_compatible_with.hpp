#pragma once

#include "concepts/simple_version.hpp"

inline namespace arba
{
namespace core
{

inline constexpr bool is_major_compatible_with(const SimpleVersion auto& lv, const SimpleVersion auto& rv) noexcept
{
    return lv.major() == rv.major()
           && ((lv.minor() == rv.minor() && lv.patch() >= rv.patch())
               || lv.minor() > rv.minor());
}

inline constexpr bool is_minor_compatible_with(const SimpleVersion auto& lv, const SimpleVersion auto& rv) noexcept
{
    return lv.major() == rv.major() && lv.minor() == rv.minor() && lv.patch() >= rv.patch();
}

inline constexpr bool is_patch_compatible_with(const SimpleVersion auto& lv, const SimpleVersion auto& rv) noexcept
{
    return lv.major() == rv.major() && lv.minor() == rv.minor() && lv.patch() == rv.patch();
}

}
}
