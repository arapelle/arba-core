#include "rng.hpp"
#include <vector>
#include <iostream>

int Generator::generate() const
{
    return rand() % 1000;
}


extern "C" std::unique_ptr<GeneratorInterface> make_instance()
{
    return std::make_unique<Generator>();
}

extern "C" std::shared_ptr<GeneratorInterface> make_shared_instance()
{
    return std::make_shared<Generator>();
}

extern "C" void execute(int& value)
{
    value = Generator().generate();
}

extern "C" GeneratorInterface& default_generator()
{
    static Generator generator;
    return generator;
}

extern "C" const Generator& default_const_generator()
{
    static Generator generator;
    return generator;
}
