#pragma once

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

inline namespace arba
{
namespace core
{

// trim:

template <class StringType>
inline StringType& left_trim(StringType& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](const auto& ch) { return !std::isspace(ch); }));
    return str;
}

template <class StringType>
inline StringType& right_trim(StringType& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](const auto& ch) { return !std::isspace(ch); }).base(),
              str.end());
    return str;
}

template <class StringViewType>
    requires requires(StringViewType& str, std::size_t len) {
        { str.remove_prefix(len) };
    }
inline StringViewType& left_trim(StringViewType& str)
{
    str.remove_prefix(std::distance(
        str.begin(), std::find_if(str.begin(), str.end(), [](const auto& ch) { return !std::isspace(ch); })));
    return str;
}

template <class StringViewType>
    requires requires(StringViewType& str, std::size_t len) {
        { str.remove_suffix(len) };
    }
inline StringViewType& right_trim(StringViewType& str)
{
    str.remove_suffix(std::distance(
        str.rbegin(), std::find_if(str.rbegin(), str.rend(), [](const auto& ch) { return !std::isspace(ch); })));
    return str;
}

template <class StringType>
inline StringType& trim(StringType& str)
{
    return left_trim(right_trim(str));
}

// trim copy:

template <class StringType>
inline StringType left_trim_copy(StringType str)
{
    left_trim(str);
    return str;
}

template <class StringType>
inline StringType right_trim_copy(StringType str)
{
    right_trim(str);
    return str;
}

template <class StringType>
inline StringType trim_copy(StringType str)
{
    return trim(str);
}

// trim_view:

inline std::string_view left_trim_view(std::string_view str)
{
    return left_trim(str);
}
inline std::string_view right_trim_view(std::string_view str)
{
    return right_trim(str);
}
inline std::string_view trim_view(std::string_view str)
{
    return trim(str);
}

} // namespace core
} // namespace arba
