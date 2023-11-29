#include <arba/core/random.hpp>

inline namespace arba
{
namespace core
{
namespace private_
{

rand_int_engine_type_& rand_int_engine_()
{
    static thread_local rand_int_engine_type_ instance;
    return instance;
}

}
}
}
