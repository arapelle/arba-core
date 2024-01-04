#pragma once

#include <memory>
#include <rng_interface.hpp>

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
