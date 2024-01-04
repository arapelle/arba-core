#include "rng.hpp"

#include <iostream>
#include <vector>

Generator::Generator(int a, int b) : a_(a), b_(b)
{
}

int Generator::generate() const
{
    return a_ + (rand() % (b_ - a_));
}

extern "C" std::unique_ptr<GeneratorInterface> make_unique_instance()
{
    return std::make_unique<Generator>();
}

extern "C" std::unique_ptr<GeneratorInterface> make_unique_instance_from_args(int a, int b)
{
    return std::make_unique<Generator>(a, b);
}

extern "C" std::shared_ptr<GeneratorInterface> make_shared_instance()
{
    return std::make_shared<Generator>();
}

extern "C" std::shared_ptr<GeneratorInterface> make_shared_instance_from_args(int a, int b)
{
    return std::make_shared<Generator>(a, b);
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
