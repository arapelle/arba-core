#pragma once

#include "extract_tri_version.hpp"

inline namespace arba
{
namespace core
{
namespace private_
{

[[nodiscard]] constexpr bool advance_alphanum_str_(std::string_view::const_iterator& iter,
                                                   std::string_view::const_iterator end_iter)
{
    const auto begin = iter;

    const auto check_end = [&](std::string_view::const_iterator& iter)
    {
        if (iter != end_iter)
            return std::string_view(".+").find(*iter) != std::string_view::npos;
        return true;
    };

    if (check_end(iter))
        return false;

    char ch = *iter;
    bool digit_ok = private_::is_digit_(ch);
    bool alpha_ok = private_::is_alpha_(ch);
    bool expect_alpha = alpha_ok;

    ++iter;
    if (ch == '0')
    {
        if (check_end(iter))
            return true;
        expect_alpha = true;
    }

    if (digit_ok)
    {
        for (;; ++iter)
        {
            if (check_end(iter))
                return digit_ok && !expect_alpha;

            if (!private_::is_digit_(*iter))
            {
                expect_alpha = true;
                digit_ok = false;
                alpha_ok = private_::is_alpha_(*iter);
                break;
            }
        }
    }

    for (;; ++iter)
    {
        if (check_end(iter))
            return alpha_ok && expect_alpha;

        if (private_::is_alphanum_(*iter))
            continue;
        return false;
    }
}

[[nodiscard]] constexpr bool extract_pre_release_(std::string_view::const_iterator& iter,
                                                  std::string_view::const_iterator end_iter, std::string_view& pr_sv)
{
    const auto begin = iter;

    for (;; ++iter)
    {
        if (!advance_alphanum_str_(iter, end_iter))
            return false;
        if (iter == end_iter || *iter == '+')
            break;
    }

    pr_sv = std::string_view(begin, iter);
    return true;
}

[[nodiscard]] constexpr bool extract_pre_release_(std::string_view str, std::string_view& pr_sv)
{
    auto iter = str.cbegin();
    bool res = extract_pre_release_(iter, str.cend(), pr_sv);
    return res;
}

[[nodiscard]] constexpr bool check_build_metadata_(std::string_view str)
{
    auto iter = str.cbegin(), end_iter = str.cend();

    if (iter == end_iter || !private_::is_alphanum_(*iter))
        return false;

    for (++iter; iter != end_iter; ++iter)
    {
        if (private_::is_alphanum_(*iter))
            continue;
        if (*iter == '.')
        {
            ++iter;
            if (iter == end_iter || *iter == '.')
                return false;
        }
        else
            return false;
    }

    return true;
}

[[nodiscard]] constexpr bool extract_semantic_version_(std::string_view str, std::string_view& major,
                                                       std::string_view& minor, std::string_view& patch,
                                                       std::string_view& pre_release, std::string_view& build_metadata)
{
    pre_release = std::string_view();
    build_metadata = std::string_view();

    auto iter = str.cbegin();
    const auto end_iter = str.cend();

    if (!extract_tri_version_(iter, end_iter, major, minor, patch))
        return false;
    if (iter == end_iter)
        return true;

    if (*iter == '-')
    {
        if (!extract_pre_release_(++iter, end_iter, pre_release))
            return false;
        if (iter == end_iter)
            return true;
    }

    build_metadata = std::string_view(++iter, end_iter);
    return check_build_metadata_(build_metadata);
}

}
}
}
