#pragma once

#include "trinum_version.hpp"

inline namespace arba
{
namespace core
{

// https://semver.org/spec/v2.0.0.html

class semantic_version : protected trinum_version<>
{
public:
    static constexpr std::string pre_release_version_regex_str()
    {
        return R"((0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(\.(0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*)";
    }

    static constexpr std::string build_metadata_regex_str()
    {
        return R"([0-9a-zA-Z-]+(\.[0-9a-zA-Z-]+)*)";
    }

    static constexpr std::string regex_str()
    {
        std::string regex_str = std::string("^") + trinum_regex_str() + R"''(
    (-(
        (0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)
        (\.(0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*
    ))?
    (\+(
        [0-9a-zA-Z-]+(\.[0-9a-zA-Z-]+)*
    ))?
    $
)''";
        std::erase_if(regex_str, isspace);
        return regex_str;
    }

    static const std::regex regex;

    static constexpr uint8_t pre_release_version_submatch_id = 5;
    static constexpr uint8_t build_metadata_submatch_id = 10;

public:
    constexpr semantic_version(uint64_t major, uint32_t minor, uint32_t patch)
        : trinum_version(major, minor, patch)
    {}

    semantic_version(uint64_t major, uint32_t minor, uint32_t patch,
                     std::string_view pre_release_version, std::string_view build_metadata = std::string_view());

    template <class VersionCoreT>
    semantic_version(const VersionCoreT& version_core,
                     std::string_view pre_release_version, std::string_view build_metadata = std::string_view())
        : semantic_version(version_core.major(), version_core.minor(), version_core.patch(),
                           pre_release_version, build_metadata)
    {}

    semantic_version(std::string_view version_str);

    constexpr const trinum_version& version_core() const noexcept { return static_cast<const trinum_version&>(*this); }
    constexpr trinum_version& version_core() noexcept { return static_cast<trinum_version&>(*this); }
    inline std::string_view pre_release_version() const noexcept { return pre_release_version_; }
    inline std::string_view build_metadata() const noexcept { return build_metadata_; }

    using trinum_version::major;
    using trinum_version::minor;
    using trinum_version::patch;
    using trinum_version::set_major;
    using trinum_version::set_minor;
    using trinum_version::set_patch;
    using trinum_version::up_major;
    using trinum_version::up_minor;
    using trinum_version::up_patch;

    using trinum_version::is_major_compatible_with;
    using trinum_version::is_minor_compatible_with;

    constexpr bool operator==(const semantic_version& other) const;
    bool operator<(const semantic_version& other) const;
    inline constexpr bool operator!=(const semantic_version& other) const { return !(other == *this); }
    inline bool operator>(const semantic_version& other) const { return other < *this; }
    inline bool operator<=(const semantic_version& other) const { return !(other < *this); }
    inline bool operator>=(const semantic_version& other) const { return !(*this < other); }

private:
    bool pre_release_version_is_less_than_(const std::string& other_pre_release_version) const;
    [[nodiscard]] static int compare_submatchs_(std::sub_match<std::string::const_iterator> const& match,
                                                std::sub_match<std::string::const_iterator> const& other_match);

private:
    std::string pre_release_version_;
    std::string build_metadata_;
};

constexpr bool semantic_version::operator==(const semantic_version& other) const
{
    return static_cast<const trinum_version&>(*this) == static_cast<const trinum_version&>(other)
           && pre_release_version_ == other.pre_release_version_;
}

}
}

template <class CharT>
struct std::formatter<::arba::core::semantic_version, CharT>
{
    template <class FormatParseContext>
    inline constexpr auto parse(FormatParseContext& ctx) { return ctx.begin(); }

    template <class FormatContext>
    auto format(const ::arba::core::semantic_version& version, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}-{}+{}",
                              version.version_core(), version.pre_release_version(), version.build_metadata());
    }
};
