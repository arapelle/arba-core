#include <arba/core/byte/bytes_formatter.hpp>
#include <arba/core/version.hpp>

#include <cstdlib>
#include <iostream>

int main()
{
    core::bytes_formatter bformatter;
    bformatter.format_binary_cstream_to(stdin, std::cout);
    return EXIT_SUCCESS;
}
