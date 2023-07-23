#include <arba/core/random.hpp>

inline namespace arba
{
namespace core
{

void reseed()
{
    private_::rand_int_engine_().seed(std::random_device{}());
}

void reseed(private_::rand_int_engine_type_::result_type value)
{
    private_::rand_int_engine_().seed(value);
}

}
}
