#pragma once

inline namespace arba
{
namespace core
{

// define_requested_type

template <class request_type_tag, class observed_type, class response_type>
struct define_requested_type
{
    using type = response_type;
};

// requested_type

template <class request_type_tag, class observed_type>
struct requested_type;

template <class request_type_tag, class observed_type>
using requested_type_t = typename requested_type<request_type_tag, observed_type>::type;

// requested_type_or

template <class request_type_tag, class observed_type, class or_type>
struct requested_type_or { using type = or_type; };

template <class request_type_tag, class observed_type, class or_type>
requires requires
{
    typename requested_type_t<request_type_tag, observed_type>;
}
struct requested_type_or<request_type_tag, observed_type, or_type> { using type = requested_type_t<request_type_tag, observed_type>; };

template <class request_type_tag, class observed_type, class or_type>
using requested_type_or_t = typename requested_type_or<request_type_tag, observed_type, or_type>::type;
}
}

// macro define helper

#define ARBA_CORE_DEFINE_REQUESTED_TYPE(request_type_tag, observed_type, response_type) \
    template <> struct arba::core::requested_type<request_type_tag, observed_type> \
        : public define_requested_type<request_type_tag, observed_type, response_type> {};
