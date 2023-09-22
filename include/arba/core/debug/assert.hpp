#pragma once

#include "breakpoint.hpp"

inline namespace arba
{
namespace core
{
namespace private_
{

void print_assert_failed_(const char* expr_str, const char* file, int line);

}

#ifdef NDEBUG
#define ARBA_ASSERT(bexpr__) ((void)0)
#else

#define ARBA_ASSERT(bexpr__) \
(void) \
    (static_cast<bool>(bexpr__) || \
     (::arba::core::private_::print_assert_failed_((#bexpr__),(__FILE__),(__LINE__)),ARBA_BREAKPOINT(),std::abort(),0))

#endif

}
}
