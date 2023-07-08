#include <arba/core/version.hpp>
#include <arba/core/uuid.hpp>
#include <iostream>
#include <cstdlib>

int main()
{
    std::cout << "arba-core-" << ARBA_CORE_VERSION << std::endl;
    core::uuid id("03b0d57c-a6bd-40de-bbb7-d26534801bb0");
    std::cout << "uuid: " << id << std::endl;
    return EXIT_SUCCESS;
}
