# Concept #

C++ core tools (UUID class, hash functions, htow/wtoh...)

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-13)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.13 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-core --recurse-submodules
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-core
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-core/
cmake -P uninstall.cmake
```

# How to use
## Example - Print UUID
```c++
#include <arba/core/uuid.hpp>
#include <iostream>

int main()
{
    core::uuid id("f192c2c7-1e5e-4211-b540-b40ba0009624");
    std::cout << id << std::endl;
    return EXIT_SUCCESS;
}

```

## Example - Using *arba-core* in a CMake project
See *basic_cmake_project* in example, and more specifically the *CMakeLists.txt* to see how to use *arba-core* in your CMake projects.

# License

[MIT License](./LICENSE.md) © arba-core
