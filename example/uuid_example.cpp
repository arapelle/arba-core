#include <arba/core/version.hpp>
#include <arba/core/uuid.hpp>
#include <iostream>

int main()
{
    core::uuid id("f192c2c7-1e5e-4211-b540-b40ba0009624");
    std::cout << id << std::endl;
    return EXIT_SUCCESS;
}
