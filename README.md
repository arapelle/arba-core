# Concept #

C++ core tools (UUID class, hash function, ...)

See [task board](https://app.gitkraken.com/glo/board/X0_o9z2bBQARuity) for future updates and features.

# Install #
## Requirements ##
Binaries:
- A C++20 compiler (ex: g++-10)
- CMake 3.16 or later

Libraries:
- [Google Test](https://github.com/google/googletest) 1.10 or later (only for testing)

## Clone

```
git clone https://github.com/arapelle/core --recurse-submodules
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/core
cmake -P cmake_quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -DCMAKE_BUILD_TYPE=Debug -P cmake_quick_install.cmake
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-core/
cmake -P cmake_uninstall.cmake
```

# How to use
## Example - Print UUID
```c++
#include <core/uuid.hpp>
#include <iostream>

int main()
{
    core::uuid id("f192c2c7-1e5e-4211-b540-b40ba0009624");
    std::cout << id << std::endl;
    return EXIT_SUCCESS;
}

```

## Example - Using *strn* in a CMake project
See the [basic cmake project](https://github.com/arapelle/core/tree/master/example/basic_cmake_project) example, and more specifically the [CMakeLists.txt](https://github.com/arapelle/core/tree/master/example/basic_cmake_project/CMakeLists.txt) to see how to use *core* in your CMake projects.

# License

[MIT License](./LICENSE.md) © core