#include <arba/core/uuid.hpp>
#include <arba/core/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << std::format("arba-core-{}", arba::core::version) << std::endl;
    arba::core::uuid id("03b0d57c-a6bd-40de-bbb7-d26534801bb0");
    std::cout << "uuid: " << id << std::endl;
    core::uuid id2("78fdfce9-76f0-4088-ba95-713cf711781a");
    std::cout << "uuid: " << id2 << std::endl;
    return EXIT_SUCCESS;
}
