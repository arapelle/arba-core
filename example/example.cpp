#include <arba/core/string/trim.hpp>
#include <arba/core/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << core::trim_view("  __example__  ") << std::endl;
    std::cout << "EXAMPLE SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
