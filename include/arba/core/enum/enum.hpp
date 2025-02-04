#pragma once

#include <concepts>
#include <cstdint>
#include <string_view>
#include <type_traits>

inline namespace arba
{
namespace core
{

enum enumeration_type
{
    no_implicit = 0,
    implicit_integral = 1,
};

template <std::integral IntType, enumeration_type EnumType, class SelfType>
class enumeration;

template <std::integral IntType, enumeration_type EnumType, class SelfType>
class enumerator
{
public:
    using int_type = std::remove_cvref_t<IntType>;
    using underlying_type = int_type;

private:
    constexpr explicit enumerator(int_type ival) noexcept : value(ival) {}
    friend SelfType;
    friend enumeration<IntType, EnumType, SelfType>;

public:
    constexpr operator int_type () const noexcept
        requires (EnumType == enumeration_type::implicit_integral)
    {
        return value;
    }

    constexpr underlying_type to_underlying_type() const noexcept
    {
        return value;
    }

    constexpr std::string_view to_string() const noexcept
        requires (requires{ {SelfType::enumerator_strings[std::declval<int_type>()]} -> std::convertible_to<std::string_view>; })
    {
        return SelfType::enumerator_strings[value];
    }

    constexpr auto operator<=>(const enumerator& other) const noexcept = default;

protected:
    int_type value;
};

template <std::integral IntType, enumeration_type EnumType, class SelfType>
class enumeration : public enumerator<IntType, EnumType, SelfType>
{
    using base_ = enumerator<IntType, EnumType, SelfType>;

protected:
    using enumerator = base_;

public:
    constexpr enumeration(enumerator enum_value) noexcept
        : base_(enum_value)
    {}

    constexpr enumeration()
        : enumeration(SelfType::default_enumerator())
    {}

    static constexpr enumerator default_enumerator() noexcept
    {
        if constexpr (requires{ {SelfType::enumerators[std::declval<IntType>()]} -> std::same_as<enumerator&>; })
        {
            return SelfType::enumerators[0];
        }
        else
        {
            return enumerator(0);
        }
    }
};


} // namespace core
} // namespace arba
