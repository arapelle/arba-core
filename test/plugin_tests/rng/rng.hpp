#pragma once

#include <rng_interface.hpp>
#include <memory>

class Generator : public GeneratorInterface
{
public:
    virtual ~Generator() = default;
    virtual int generate() const override;
};
