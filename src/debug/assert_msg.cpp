#include <arba/core/debug/assert_msg.hpp>
#include <iostream>

inline namespace arba
{
namespace core
{
namespace private_
{

void print_assert_failed_(const char* expr_str, const char* file, int line, std::string_view msg)
{
    std::cerr << "Assertion failed: " << expr_str << ", file: " << file << ", line: " << line;
    if (!msg.empty())
        std::cerr << ", msg: " << msg;
    std::cerr << std::endl;
}

}
}
}
