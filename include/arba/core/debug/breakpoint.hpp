#pragma once

#ifdef WIN32
#ifndef NDEBUG
#include <debugapi.h>
#endif
#endif

inline namespace arba
{
namespace core
{

#ifdef WIN32
#define ARBA_BREAKPOINT()  DebugBreak()
#elif __clang__ || __GNUC__
#define ARBA_BREAKPOINT()  __builtin_trap()
#else
#warning "Unknown platform and/or compiler: impossible to provide ARBA_BREAKPOINT() feature."
#define ARBA_BREAKPOINT() ((void)0)
#endif

}
}
