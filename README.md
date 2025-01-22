# Concept #

C++ core tools
- Host TO World / World TO Host (htow/wtoh)
- Scope-Bound Resource Management (sbrm)
- Trim string
...

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-14)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.14 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-core
```

## Use with `conan`

Create the conan package.
```
conan create . --build=missing -c
```
Add a requirement in your conanfile project file.
```python
    def requirements(self):
        self.requires("arba-core/0.29.0")
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
## Example - Trim string
```c++
#include <arba/core/string/trim.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << core::trim_view("  __example__  ") << std::endl;
    std::cout << "EXAMPLE SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
```

# License

[MIT License](./LICENSE.md) © arba-core
