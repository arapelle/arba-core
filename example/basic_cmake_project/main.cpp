#include <core/version.hpp>
#include <core/uuid.hpp>
#include <iostream>
#include <cstdlib>

int main()
{
    std::cout << "core-" << CORE_VERSION << std::endl;
    core::uuid id("03b0d57c-a6bd-40de-bbb7-d26534801bb0");
    std::cout << "uuid: " << id << std::endl;
    return EXIT_SUCCESS;
}
