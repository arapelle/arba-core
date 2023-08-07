#pragma once

#include <string_view>

class GeneratorInterface
{
public:
    virtual ~GeneratorInterface() = default;
    virtual int generate() const = 0;
};
