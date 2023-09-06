#pragma once

#include <string>
#include <regex>
#include <format>

inline namespace arba
{
namespace core
{
namespace private_
{

template <uint64_t... values>
class trinum_version_impl_;

template <uint64_t Major, uint64_t Minor, uint64_t Patch>
    requires (Minor < (1LL<<32) && Patch < (1LL<<32))
class trinum_version_impl_<Major, Minor, Patch>
{
public:
    constexpr trinum_version_impl_() = default;
    inline static constexpr uint64_t major() noexcept { return Major; }
    inline static constexpr uint32_t minor() noexcept { return Minor; }
    inline static constexpr uint32_t patch() noexcept { return Patch; }
    inline static constexpr std::tuple<uint64_t, uint32_t, uint32_t> to_tuple() noexcept
    {
        return std::tuple<uint64_t, uint32_t, uint32_t>{ Major, Minor, Patch };
    }
};

template <>
class trinum_version_impl_<> : public std::tuple<uint64_t, uint32_t, uint32_t>
{
    using base_ = std::tuple<uint64_t, uint32_t, uint32_t>;

public:
    static constexpr std::string number_regex_str()
    {
        return R"(0|[1-9]\d*)";
    }

    static constexpr std::string trinum_regex_str()
    {
        const std::string num_re = number_regex_str();
        return std::string("(") + num_re + ")\\.(" + num_re + ")\\.(" + num_re + ")";
    }

    static constexpr std::string regex_str()
    {
        return std::string("^") + trinum_regex_str() + "$";
        //        return R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)$)";
    }

    static const std::regex regex;

    static constexpr uint8_t major_submatch_id = 1;
    static constexpr uint8_t minor_submatch_id = 2;
    static constexpr uint8_t patch_submatch_id = 3;

    constexpr explicit trinum_version_impl_(uint64_t major, uint32_t minor, uint32_t patch)
        : base_{ major, minor, patch }
    {}

    template <typename VersionT>
        requires requires(const VersionT& value)
    {
        { value.major() } -> std::convertible_to<uint64_t>;
        { value.minor() } -> std::convertible_to<uint32_t>;
        { value.patch() } -> std::convertible_to<uint32_t>;
    }

    constexpr explicit trinum_version_impl_(const VersionT& version)
        : base_{ version.major(), version.minor(), version.patch() }
    {}

    trinum_version_impl_(std::string_view version_str);

    inline constexpr uint64_t major() const noexcept { return std::get<0>(*this); }
    inline constexpr uint32_t minor() const noexcept { return std::get<1>(*this); }
    inline constexpr uint32_t patch() const noexcept { return std::get<2>(*this); }

    inline void set_major(uint64_t major) noexcept { std::get<0>(*this) = major; }
    inline void set_minor(uint32_t minor) noexcept { std::get<1>(*this) = minor; }
    inline void set_patch(uint32_t patch) noexcept { std::get<2>(*this) = patch; }

    inline void up_major() noexcept { ++std::get<0>(*this); std::get<1>(*this) = 0; std::get<2>(*this) = 0; }
    inline void up_minor() noexcept { ++std::get<1>(*this); std::get<2>(*this) = 0; }
    inline void up_patch() noexcept { ++std::get<2>(*this); }

    inline constexpr const std::tuple<uint64_t, uint32_t, uint32_t>& to_tuple() const noexcept
    {
        return *this;
    }
};

}

template <uint64_t... values>
class trinum_version : public private_::trinum_version_impl_<values...>
{
    using base_ = private_::trinum_version_impl_<values...>;

public:
    constexpr trinum_version()
        : base_()
    {}

    template <typename... _Up>
    constexpr explicit trinum_version(_Up... args)
        : base_( args... )
    {}

    trinum_version(std::string_view version_str) : base_(version_str) {}

    template <uint64_t... vals>
    constexpr bool is_major_compatible_with(trinum_version<vals...> const& rv) const noexcept
    {
        return this->major() == rv.major()
               && ((this->minor() == rv.minor() && this->patch() >= rv.patch())
                   || this->minor() > rv.minor());
    }

    template <uint64_t... vals>
    constexpr bool is_minor_compatible_with(trinum_version<vals...> const& rv) const noexcept
    {
        return this->major() == rv.major() && this->minor() == rv.minor() && this->patch() >= rv.patch();
    }

    template <uint64_t... vals>
    inline constexpr auto operator==(trinum_version<vals...> const& rv) const noexcept
    {
        return this->to_tuple() == rv.to_tuple();
    }
    template <uint64_t... vals>
    inline constexpr auto operator!=(trinum_version<vals...> const& rv) const noexcept { return !(*this == rv); }
    template <uint64_t... vals>
    inline constexpr auto operator<(trinum_version<vals...> const& rv) const noexcept
    {
        return this->to_tuple() < rv.to_tuple();
    }
    template <uint64_t... vals>
    inline constexpr auto operator>(trinum_version<vals...> const& rv) const noexcept { return rv < *this; }
    template <uint64_t... vals>
    inline constexpr auto operator<=(trinum_version<vals...> const& rv) const noexcept { return !(rv < *this); }
    template <uint64_t... vals>
    inline constexpr auto operator>=(trinum_version<vals...> const& rv) const noexcept { return !(*this < rv); }
};

}
}

template <class CharT, uint64_t... values>
struct std::formatter<::arba::core::trinum_version<values...>, CharT>
{
    template <class FormatParseContext>
    inline constexpr auto parse(FormatParseContext& ctx) { return ctx.begin(); }

    template <class FormatContext>
    auto format(const ::arba::core::trinum_version<values...>& version, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}.{}.{}",
                              version.major(), version.minor(), version.patch());
    }
};
