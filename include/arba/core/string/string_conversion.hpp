#pragma once

#include <string_view>
#include <cstdint>
#include <stdexcept>

inline namespace arba
{
namespace core
{

static constexpr int64_t stoi64(std::string_view str)
{
    constexpr std::string_view spaces = " \f\n\r\t\v";
    std::size_t pos = str.find_first_not_of(spaces);
    if (pos == std::string_view::npos)
        throw std::invalid_argument{ "Input string is empty or full of space characters." };
    str.remove_prefix(pos);
    str.remove_suffix(str.size() - str.find_last_not_of(spaces) - 1);

    int64_t sign = 1;
    char ch = str[0];
    if (ch == '-')
    {
        sign = -1;
        str.remove_prefix(1);
    }
    else if (ch == '+')
    {
        str.remove_prefix(1);
    }

    int64_t result = 0;
    int64_t multiplier = 1;
    for (std::ptrdiff_t i = str.size() - 1; i >= 0; --i)
    {
        ch = str[i];
        if (ch < '0' || ch > '9') [[unlikely]]
            throw std::invalid_argument{ "Invalid character found in input string." };
        int64_t number = ch - '0';
        result += number * multiplier;
        multiplier *= 10;
        // TODO? https://stackoverflow.com/questions/1815367/catch-and-compute-overflow-during-multiplication-of-two-large-integers
    }

    return sign * result;
}

}
}
