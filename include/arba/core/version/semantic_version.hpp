#pragma once

#include "tri_version.hpp"
#include "_private/extract_semantic_version.hpp"
#include "concepts/semantic_version.hpp"

inline namespace arba
{
namespace core
{
// https://semver.org/spec/v2.0.0.html

class semantic_version : protected tri_version
{
public:
    constexpr semantic_version(uint64_t major, uint32_t minor, uint32_t patch,
                               std::string_view pre_release = std::string_view(),
                               std::string_view build_metadata = std::string_view());

    constexpr explicit semantic_version(const TriVersion auto& version_core,
                                        std::string_view pre_release = std::string_view(),
                                        std::string_view build_metadata = std::string_view());

    constexpr semantic_version(std::string_view version);

    constexpr const tri_version& core() const noexcept { return static_cast<const tri_version&>(*this); }
    constexpr tri_version& core() noexcept { return static_cast<tri_version&>(*this); }
    constexpr std::string_view pre_release() const noexcept { return pre_release_; }
    constexpr std::string_view build_metadata() const noexcept { return build_metadata_; }

    using tri_version::major;
    using tri_version::minor;
    using tri_version::patch;
    using tri_version::set_major;
    using tri_version::set_minor;
    using tri_version::set_patch;
    using tri_version::up_major;
    using tri_version::up_minor;
    using tri_version::up_patch;
    using tri_version::is_major_compatible_with;
    using tri_version::is_minor_compatible_with;
    using tri_version::is_patch_compatible_with;

    inline constexpr bool operator==(const semantic_version& other) const;
    inline constexpr bool operator<(const semantic_version& other) const;
    inline constexpr bool operator!=(const semantic_version& other) const { return !(other == *this); }
    inline constexpr bool operator>(const semantic_version& other) const { return other < *this; }
    inline constexpr bool operator<=(const semantic_version& other) const { return !(other < *this); }
    inline constexpr bool operator>=(const semantic_version& other) const { return !(*this < other); }

private:
    constexpr bool pre_release_is_less_than_(const std::string& right_pr) const;

    static constexpr semantic_version valid_semantic_version_(std::string_view semver);
    static constexpr std::string_view valid_pre_release_(std::string_view pre_release_version);
    static constexpr std::string_view valid_build_metadata_(std::string_view build_metadata);

private:
    std::string pre_release_;
    std::string build_metadata_;
};

constexpr semantic_version::semantic_version(uint64_t major, uint32_t minor, uint32_t patch,
                                             std::string_view pre_release,
                                             std::string_view build_metadata)
    : tri_version(major, minor, patch),
    pre_release_(valid_pre_release_(pre_release)),
    build_metadata_(valid_build_metadata_(build_metadata))
{}

constexpr semantic_version::semantic_version(const TriVersion auto& version_core,
                                             std::string_view pre_release, std::string_view build_metadata)
    : tri_version(version_core.major(), version_core.minor(), version_core.patch()),
    pre_release_(valid_pre_release_(pre_release)),
    build_metadata_(valid_build_metadata_(build_metadata))
{}

constexpr semantic_version::semantic_version(std::string_view version)
    : tri_version(),
    pre_release_(),
    build_metadata_()
{
    *this = valid_semantic_version_(version);
}

inline constexpr bool semantic_version::operator==(const semantic_version& other) const
{
    return static_cast<const tri_version&>(*this) == static_cast<const tri_version&>(other)
           && pre_release_ == other.pre_release_;
}

inline constexpr bool semantic_version::operator<(const semantic_version& other) const
{
    auto cmp_res = static_cast<const tri_version&>(*this) <=> static_cast<const tri_version&>(other);
    return cmp_res < 0 || (cmp_res == 0 && pre_release_is_less_than_(other.pre_release_));
}

constexpr bool semantic_version::pre_release_is_less_than_(const std::string& right_pr) const
{
    if (pre_release_.empty())
        return false;
    else if (right_pr.empty())
        return true;

    bool left_is_shortest = false;
    bool left_is_num = true;
    bool right_is_num = true;

    const auto left_end_iter = pre_release_.end();
    const auto right_end_iter = right_pr.end();
    auto left_iter = pre_release_.begin();
    auto right_iter = right_pr.begin();
    char left_ch = '\0';
    char right_ch = '\0';

    for (;; ++left_iter, ++right_iter)
    {
        if (left_iter == left_end_iter)
        {
            if (right_iter == right_end_iter)
                return false;
            left_is_shortest = true;
            break;
        }
        else if (right_iter == right_end_iter)
            break;

        left_ch = *left_iter;
        right_ch = *right_iter;
        if (left_ch != right_ch)
            break;

        if (left_ch == '.')
            left_is_num = true;
        else
            left_is_num = left_is_num && (left_ch >= '0' && left_ch <= '9');
    }

    right_is_num = left_is_num;
    bool left_is_lt = left_ch < right_ch;
    bool length_break = true;

    if (left_ch != right_ch)
    {
        length_break = false;
        left_is_num = left_is_num && (left_ch >= '0' && left_ch <= '9');
        right_is_num = right_is_num && (right_ch >= '0' && right_ch <= '9');
        if (!left_is_num && !right_is_num)
            return left_is_lt;
    }
    else if (!left_is_num && !right_is_num)
    {
        return left_is_shortest;
    }

    for (; left_iter != left_end_iter && left_is_num; ++left_iter)
    {
        left_ch = *left_iter;
        if (left_ch == '.')
            break;
        left_is_num = left_is_num && (left_ch >= '0' && left_ch <= '9');
    }

    for (; right_iter != right_end_iter && right_is_num; ++ right_iter)
    {
        right_ch = *right_iter;
        if (right_ch == '.')
            break;
        right_is_num = right_is_num && (right_ch >= '0' && right_ch <= '9');
    }

    if (left_is_num)
    {
        if (!right_is_num)
            return true;
        return left_is_lt || (length_break && left_is_shortest);
    }
    if (right_is_num)
        return false;
    return left_is_lt;
}

constexpr semantic_version semantic_version::valid_semantic_version_(std::string_view semver)
{
    std::string_view major, minor, patch, pr, bm;
    if (!private_::extract_semantic_version_(semver, major, minor, patch, pr, bm)) [[unlikely]]
    {
        if (std::is_constant_evaluated())
        {
            compile_time_error("'semver' is not a valid semantic version.");
        }
        throw std::invalid_argument(std::string(semver));
    }

    return semantic_version(stoi64(major), stoi64(minor), stoi64(patch), pr, bm);
}

constexpr std::string_view semantic_version::valid_pre_release_(std::string_view pre_release_version)
{
    if (!pre_release_version.empty())
    {
        if (!private_::extract_pre_release_(pre_release_version, pre_release_version)) [[unlikely]]
        {
            if (std::is_constant_evaluated())
            {
                compile_time_error("'pre_release_version' is not a valid pre-release version.");
            }
            throw std::invalid_argument(std::string(pre_release_version));
        }
    }

    return pre_release_version;
}

constexpr std::string_view semantic_version::valid_build_metadata_(std::string_view build_metadata)
{
    if (!build_metadata.empty())
    {
        if (!private_::check_build_metadata_(build_metadata)) [[unlikely]]
        {
            if (std::is_constant_evaluated())
            {
                compile_time_error("'build_metadata' is not a valid build metadata string.");
            }
            throw std::invalid_argument(std::string(build_metadata));
        }
    }

    return build_metadata;
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
                              version.core(), version.pre_release(), version.build_metadata());
    }
};
