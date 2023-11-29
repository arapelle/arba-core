#include <arba/core/debug/assert.hpp>
#include <iostream>

inline namespace arba
{
namespace core
{
namespace private_
{

void print_assert_failed_(const char* expr_str, const char* file, int line)
{
    std::cerr << "Assertion failed: " << expr_str << ", file: " << file << ", line: " << line << std::endl;
}

}
}
}
