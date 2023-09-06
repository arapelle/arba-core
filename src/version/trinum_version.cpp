#include <arba/core/version/trinum_version.hpp>
#include <format>

inline namespace arba
{
namespace core
{
namespace private_
{

const std::regex trinum_version_impl_<>::regex(trinum_version_impl_<>::regex_str(),
                                               std::regex::ECMAScript|std::regex::optimize);

trinum_version_impl_<>::trinum_version_impl_(std::string_view version_str)
{
    std::match_results<std::string_view::const_iterator> sv_match;
    if (std::regex_match(version_str.cbegin(), version_str.cend(), sv_match, std::regex(regex_str())))
    {
        set_major(std::stoi(sv_match[major_submatch_id]));
        if (std::string minor_str = sv_match[minor_submatch_id]; !minor_str.empty())
            set_minor(std::stoi(minor_str));
        else
            set_minor(0);
        if (std::string patch_str = sv_match[patch_submatch_id]; !patch_str.empty())
            set_patch(std::stoi(patch_str));
        else
            set_patch(0);
    }
    else
    {
        throw std::runtime_error(std::format("Provided string is not a normal version number: {}", version_str));
    }
}

}
}
}
