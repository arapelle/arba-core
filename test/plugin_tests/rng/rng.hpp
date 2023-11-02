#pragma once

#include <rng_interface.hpp>
#include <memory>

class Generator : public GeneratorInterface
{
public:
    Generator(int a = 0, int b = 1000);
    virtual ~Generator() = default;
    virtual int generate() const override;

private:
    int a_;
    int b_;
};
