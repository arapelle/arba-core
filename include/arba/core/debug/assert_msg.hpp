#pragma once

#include "breakpoint.hpp"

#include <format>
#include <string_view>

inline namespace arba
{
namespace core
{
namespace private_
{

void print_assert_failed_(const char* expr_str, const char* file, int line, std::string_view msg);

template <class... Args>
inline void print_assert_failed_(const char* expr_str, const char* file, int line, std::format_string<Args...> fmt,
                                 Args&&... args)
{
    const std::string msg = std::format(std::move(fmt), std::forward<Args>(args)...);
    print_assert_failed_(expr_str, file, line, std::string_view(msg));
}

}

#ifdef NDEBUG
#define ARBA_ASSERT_MSG(bexpr__, msg__, ...) ((void)0)
#else

#define ARBA_ASSERT_MSG(bexpr__, msg__, ...)                                                                           \
    (void)(static_cast<bool>(bexpr__)                                                                                  \
           || (::arba::core::private_::print_assert_failed_((#bexpr__), (__FILE__), (__LINE__),                        \
                                                            (msg__)__VA_OPT__(, ) __VA_ARGS__),                        \
               ARBA_BREAKPOINT(), std::abort(), 0))

#endif

}
}
