#pragma once

inline namespace arba
{
namespace core
{

// default_t

struct default_t
{
    explicit default_t() = default;
};
constexpr default_t default_type;

// parameter_type_or_t

template <class parameter_type, class or_type>
struct parameter_type_or
{
    using type = parameter_type;
    parameter_type_or() = delete;
};

template <class or_type>
struct parameter_type_or<default_t, or_type>
{
    using type = or_type;
    parameter_type_or() = delete;
};

template <class parameter_type, class or_type>
using parameter_type_or_t = typename parameter_type_or<parameter_type, or_type>::type;

}
}
