#pragma once

#include "_private/extract_simple_version.hpp"
#include "concepts/simple_version.hpp"
#include "is_compatible_with.hpp"
#include <arba/core/string/string_conversion.hpp>
#include <arba/core/compile_time_error.hpp>
#include <tuple>
#include <cstdint>
#include <format>

inline namespace arba
{
namespace core
{

class simple_version : public std::tuple<uint64_t, uint32_t, uint32_t>
{
public:
    using tuple_type = std::tuple<uint64_t, uint32_t, uint32_t>;

    constexpr simple_version();
    constexpr explicit simple_version(uint64_t major, uint32_t minor, uint32_t patch);
    constexpr explicit simple_version(const SimpleVersion auto& version);
    constexpr explicit simple_version(std::string_view version_str);

    constexpr simple_version(const simple_version&) = default;
    constexpr simple_version& operator=(const simple_version&) = default;

    inline constexpr uint64_t major() const noexcept { return std::get<0>(*this); }
    inline constexpr uint32_t minor() const noexcept { return std::get<1>(*this); }
    inline constexpr uint32_t patch() const noexcept { return std::get<2>(*this); }

    inline constexpr void set_major(uint64_t major) noexcept { std::get<0>(*this) = major; }
    inline constexpr void set_minor(uint32_t minor) noexcept { std::get<1>(*this) = minor; }
    inline constexpr void set_patch(uint32_t patch) noexcept { std::get<2>(*this) = patch; }

    inline constexpr void up_major() noexcept { ++std::get<0>(*this); std::get<1>(*this) = 0; std::get<2>(*this) = 0; }
    inline constexpr void up_minor() noexcept { ++std::get<1>(*this); std::get<2>(*this) = 0; }
    inline constexpr void up_patch() noexcept { ++std::get<2>(*this); }

    inline constexpr bool is_major_compatible_with(const SimpleVersion auto& rv) const noexcept
    {
        return core::is_major_compatible_with(*this, rv);
    }

    inline constexpr bool is_minor_compatible_with(const SimpleVersion auto& rv) const noexcept
    {
        return core::is_minor_compatible_with(*this, rv);
    }

    inline constexpr bool is_patch_compatible_with(const SimpleVersion auto& rv) const noexcept
    {
        return core::is_patch_compatible_with(*this, rv);
    }

private:
    static constexpr simple_version make_instance_(std::string_view version_str);
};

constexpr simple_version::simple_version()
    : tuple_type{ 0, 1, 0 }
{}

constexpr simple_version::simple_version(uint64_t major, uint32_t minor, uint32_t patch)
    : tuple_type{ major, minor, patch }
{}

constexpr simple_version::simple_version(const SimpleVersion auto& version)
    : tuple_type{ static_cast<uint64_t>(version.major()),
                 static_cast<uint32_t>(version.minor()),
                 static_cast<uint32_t>(version.patch()) }
{}

constexpr simple_version::simple_version(std::string_view version_str)
    : simple_version(make_instance_(version_str))
{}

constexpr simple_version simple_version::make_instance_(std::string_view version_str)
{
    std::string_view major, minor, patch;
    if (!private_::extract_simple_version_(version_str, major, minor, patch)) [[unlikely]]
    {
        if (std::is_constant_evaluated())
        {
            compile_time_error("'version_str' is not a valid version."
                               R"(regex to match: ^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)$)");
        }
        throw std::invalid_argument(std::string(version_str));
    }
    return simple_version(stoi64(major), stoi64(minor), stoi64(patch));
}

}
}

template <class CharT>
struct std::formatter<::arba::core::simple_version, CharT>
{
    template <class FormatParseContext>
    inline constexpr auto parse(FormatParseContext& ctx) { return ctx.begin(); }

    template <class FormatContext>
    auto format(const ::arba::core::simple_version& version, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}.{}.{}",
                              version.major(), version.minor(), version.patch());
    }
};
