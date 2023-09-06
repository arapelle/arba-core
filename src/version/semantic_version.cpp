#include <arba/core/version/semantic_version.hpp>
#include <format>

inline namespace arba
{
namespace core
{

const std::regex semantic_version::regex(semantic_version::regex_str(),
                                         std::regex::ECMAScript|std::regex::optimize);

semantic_version::semantic_version(uint64_t major, uint32_t minor, uint32_t patch,
                                   std::string_view pre_release_version, std::string_view build_metadata)
    : trinum_version(major, minor, patch), pre_release_version_(pre_release_version),
    build_metadata_(build_metadata)
{
    if (!pre_release_version.empty())
    {
        const std::regex pre_release_version_regex(pre_release_version_regex_str());
        if (!std::regex_match(pre_release_version.begin(),
                              pre_release_version.end(),
                              pre_release_version_regex))
        {
            throw std::runtime_error(std::format("Provided string is not a pre release version: {}",
                                                 pre_release_version));
        }
    }
    if (!build_metadata.empty())
    {
        const std::regex build_metadata_regex(build_metadata_regex_str());
        if (!std::regex_match(build_metadata.begin(),
                              build_metadata.end(),
                              build_metadata_regex))
        {
            throw std::runtime_error(std::format("Provided string is not a build metadata: {}",
                                                 build_metadata));
        }
    }
}

semantic_version::semantic_version(std::string_view version_str)
    : trinum_version<>(0, 0, 0)
{
    std::match_results<std::string_view::const_iterator> sv_match;
    if (std::regex_match(version_str.cbegin(), version_str.cend(), sv_match, core::semantic_version::regex))
    {
        set_major(std::stoi(sv_match[major_submatch_id]));
        set_minor(std::stoi(sv_match[minor_submatch_id]));
        set_patch(std::stoi(sv_match[patch_submatch_id]));
        pre_release_version_ = sv_match[pre_release_version_submatch_id];
        build_metadata_ = sv_match[build_metadata_submatch_id];
    }
    else
    {
        throw std::runtime_error(std::format("Provided string is not a semantic version: {}", version_str));
    }
}

bool semantic_version::operator<(const semantic_version& other) const
{
    return static_cast<const trinum_version&>(*this) < static_cast<const trinum_version&>(other)
           ||
           (static_cast<const trinum_version&>(*this) == static_cast<const trinum_version&>(other)
            && pre_release_version_is_less_than_(other.pre_release_version_)
            );
}

[[nodiscard]] int semantic_version::compare_submatchs_(std::sub_match<std::string::const_iterator> const& match,
                                                       std::sub_match<std::string::const_iterator> const& other_match)
{
    std::string_view str(match.first, match.second);
    std::string_view other_str(other_match.first, other_match.second);
    return str.compare(other_str);
}

bool semantic_version::pre_release_version_is_less_than_(const std::string& other_pre_release_version) const
{
    if (pre_release_version_.empty())
        return false;

    static std::regex field_regex(R"((0|[1-9]\d*)|(\d*[a-zA-Z-][0-9a-zA-Z-]*))", std::regex::optimize);

    std::string::const_iterator iter = pre_release_version_.cbegin();
    std::string::const_iterator other_iter = other_pre_release_version.cbegin();
    std::match_results<std::string::const_iterator> field_match, other_field_match;
    for (;;)
    {
        bool search_res = std::regex_search(iter, pre_release_version_.cend(), field_match, field_regex);
        bool other_search_res = std::regex_search(other_iter, other_pre_release_version.cend(), other_field_match, field_regex);
        if (search_res)
        {
            if (!other_search_res)
                return false;
        }
        else if (other_search_res)
            return true;
        else
            return false;

        const auto& match_1 = field_match[1];
        const auto& other_match_1 = other_field_match[1];
        if (match_1.length() > 0)
        {
            if (other_match_1.length() > 0)
            {
                int cmp_res = compare_submatchs_(match_1, other_match_1);
                if (cmp_res < 0)
                    return true;
                else if (cmp_res > 0)
                    return false;
                else
                {
                    iter = match_1.second;
                    other_iter = other_match_1.second;
                }
            }
            else
                return true;
        }
        else
        {
            const auto& match_2 = field_match[2];
            const auto& other_match_2 = other_field_match[2];
            if (match_2.length() > 0)
            {
                if (other_match_1.length() > 0)
                    return false;
                else // (other_match_2.length() > 0)
                {
                    int cmp_res = compare_submatchs_(match_2, other_match_2);
                    if (cmp_res < 0)
                        return true;
                    else if (cmp_res > 0)
                        return false;
                    else
                    {
                        iter = match_2.second;
                        other_iter = other_match_2.second;
                    }
                }
            }
        }
    }
}

}
}
