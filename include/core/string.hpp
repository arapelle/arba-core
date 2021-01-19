#pragma once

#include <string>
#include <string_view>
#include <algorithm>
#include <ranges>

namespace core
{

// trim:

template <class string_type>
inline string_type& left_trim(string_type& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](const auto& ch) { return !std::isspace(ch); }));
    return str;
}

template <class string_type>
inline string_type& right_trim(string_type& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](const auto& ch) { return !std::isspace(ch); }).base(), str.end());
    return str;
}

template <class string_view_type>
requires requires(string_view_type& str, std::size_t len)
{
    { str.remove_prefix(len) };
}
inline string_view_type& left_trim(string_view_type& str)
{
    str.remove_prefix(std::distance(str.begin(), std::find_if(str.begin(), str.end(),
                                                              [](const auto& ch) { return !std::isspace(ch); })));
    return str;
}

template <class string_view_type>
requires requires(string_view_type& str, std::size_t len)
{
    { str.remove_suffix(len) };
}
inline string_view_type& right_trim(string_view_type& str)
{
    str.remove_suffix(std::distance(str.rbegin(), std::find_if(str.rbegin(), str.rend(),
                                                               [](const auto& ch) { return !std::isspace(ch); })));
    return str;
}

template <class string_type>
inline string_type trim(string_type& str)
{
    return left_trim(right_trim(str));
}

// trim copy:

template <class string_type>
inline string_type left_trim_copy(string_type str)
{
    left_trim(str);
    return str;
}

template <class string_type>
inline string_type right_trim_copy(string_type str)
{
    right_trim(str);
    return str;
}

template <class string_type>
inline string_type trim_copy(string_type str)
{
    return trim(str);
}

// trim_view:

inline std::string_view left_trim_view(std::string_view str) { return left_trim(str); }
inline std::string_view right_trim_view(std::string_view str) { return right_trim(str); }
inline std::string_view trim_view(std::string_view str) { return trim(str); }

}
