#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <format>

template <class CharT>
struct std::formatter<std::byte, CharT>
{
    template <class FormatParseContext>
    inline constexpr auto parse(FormatParseContext& ctx)
    {
        auto iter = ctx.begin();
        if (iter == ctx.end())
            return iter;
        constexpr std::array<CharT, 7> chs = { 'u', 'i', 'x', 'b', 'o', 'c', '0' };
        constexpr uint8_t zero_ch_index = 6;
        auto find_iter = std::ranges::find(chs, *iter);
        if (find_iter == chs.end()) [[unlikely]]
            return iter;
        const uint8_t index = static_cast<uint8_t>(find_iter - chs.begin());
        if (index < zero_ch_index)
        {
            fmt_ = index;
            return ++iter;
        }
        ++iter;
        constexpr uint8_t nb_zero_reps = 5;
        const auto last_iter = chs.begin() + nb_zero_reps;
        find_iter = std::find(chs.begin(), last_iter, *iter);
        if (find_iter != last_iter)
        {
            fmt_ = static_cast<uint8_t>(find_iter - chs.begin()) + zero_ch_index;
            ++iter;
        }
        return iter;
    }

    template <class FormatContext>
    auto format(const std::byte& arg, FormatContext& ctx) const
    {
        switch (fmt_)
        {
        case 0: // u
            return std::format_to(ctx.out(), "{}", static_cast<uint8_t>(arg));
        case 1: // i
            return std::format_to(ctx.out(), "{}", static_cast<int8_t>(arg));
        case 2: // x
            return std::format_to(ctx.out(), "{:02x}", static_cast<uint8_t>(arg));
        case 3: // b
            return std::format_to(ctx.out(), "{:08b}", static_cast<uint8_t>(arg));
        case 4: // o
            return std::format_to(ctx.out(), "{:03o}", static_cast<uint8_t>(arg));
        case 5: // c
            return std::format_to(ctx.out(), "{}", static_cast<char>(arg));
        case 6: // 0u
            return std::format_to(ctx.out(), "{:03d}", static_cast<uint8_t>(arg));
        case 7: // 0i
            return std::format_to(ctx.out(), "{:04d}", static_cast<int8_t>(arg));
        case 8: // 0x
            return std::format_to(ctx.out(), "0x{:02x}", static_cast<uint8_t>(arg));
        case 9: // 0b
            return std::format_to(ctx.out(), "0b{:08b}", static_cast<uint8_t>(arg));
        case 10: // 0o
            return std::format_to(ctx.out(), "0o{:03o}", static_cast<uint8_t>(arg));
        }
        throw std::runtime_error(std::format("Unrecognized byte formatting: {}", fmt_));
    }

private:
    uint8_t fmt_ = 0;
};
