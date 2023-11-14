#pragma once

#include <string_view>

inline namespace arba
{
namespace core
{
namespace private_
{

[[nodiscard]] inline constexpr bool is_digit_(char ch) noexcept { return ch >= '0' && ch <= '9'; }
[[nodiscard]] inline constexpr bool is_pos_digit_(char ch) noexcept { return ch >= '1' && ch <= '9'; }
[[nodiscard]] inline constexpr bool is_alpha_(char ch) noexcept
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '-';
}
[[nodiscard]] inline constexpr bool is_alphanum_(char ch) noexcept
{
    return is_alpha_(ch) || is_digit_(ch);
}

[[nodiscard]] constexpr bool
extract_version_number_(std::string_view::const_iterator& iter, std::string_view::const_iterator end_iter,
                        std::string_view& num_sv, std::string_view sep, bool end_is_ok)
{
    std::string_view::const_iterator begin = iter;

    const auto check_end = [&](std::string_view::const_iterator& iter)
    {
        if (iter != end_iter)
            return sep.find(*iter) != std::string_view::npos;
        return end_is_ok;
    };

    if (iter == end_iter)
        return false;

    char ch = *iter++;
    if (ch == '0')
    {
        num_sv = std::string_view(&*begin, 1);
        return check_end(iter);
    }

    if (!private_::is_pos_digit_(ch))
        return false;

    for (; iter != end_iter; ++iter)
    {
        if (!private_::is_digit_(*iter))
            break;
    }

    if (check_end(iter))
    {
        num_sv = std::string_view(begin, iter);
        return true;
    }

    return false;
}

[[nodiscard]] constexpr bool
extract_simple_version_(std::string_view::const_iterator& iter, std::string_view::const_iterator end_iter,
                        std::string_view& major, std::string_view& minor, std::string_view& patch)
{
    return extract_version_number_(iter, end_iter, major, ".", false)
           && extract_version_number_(++iter, end_iter, minor, ".", false)
           && extract_version_number_(++iter, end_iter, patch, "-+", true);
}

[[nodiscard]] constexpr bool
extract_simple_version_(std::string_view version,
                        std::string_view& major, std::string_view& minor, std::string_view& patch)
{
    auto iter = version.cbegin();
    bool res = extract_simple_version_(iter, version.cend(), major, minor, patch);
    return res;
}

}
}
}
