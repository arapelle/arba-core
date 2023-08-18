#pragma once

#include <arba/core/template/exception_policy.hpp>

#include <new>
#include <span>
#include <system_error>

inline namespace arba
{
namespace core
{

/**
 * @brief The span_size_error class
 */
class span_size_error : public std::invalid_argument
{
public:
    span_size_error(const std::span<std::byte>& bytes, std::size_t type_size);
    span_size_error(const std::span<const std::byte>& bytes, std::size_t type_size);
};

// as_span()

template <class Type>
inline std::span<const Type>
as_span(std::span<std::byte> bytes, maythrow_t)
{
    if (bytes.size() % sizeof(Type) != 0) [[unlikely]]
        throw span_size_error(bytes, sizeof(Type));
    return std::span<const Type>{reinterpret_cast<const Type*>(bytes.data()), bytes.size() / sizeof(Type)};
}

template <class Type, size_t Extent>
    requires (Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0))
inline std::span<const Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_span(std::span<std::byte, Extent> bytes, std::nothrow_t)
{
    constexpr auto extent = Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type);
    return std::span<const Type, extent>(reinterpret_cast<const Type*>(bytes.data()), bytes.size() / sizeof(Type));
}

template <class Type, size_t Extent>
inline std::span<const Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_span(std::span<std::byte, Extent> bytes)
{
    return as_span<Type>(bytes, std::nothrow);
}

template <class Type>
inline std::span<const Type>
as_span(std::span<const std::byte> bytes, maythrow_t)
{
    if (bytes.size() % sizeof(Type) != 0) [[unlikely]]
        throw span_size_error(bytes, sizeof(Type));
    return std::span<const Type>{reinterpret_cast<const Type*>(bytes.data()), bytes.size() / sizeof(Type)};
}

template <class Type, size_t Extent>
    requires (Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0))
inline std::span<const Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_span(std::span<const std::byte, Extent> bytes, std::nothrow_t)
{
    constexpr auto extent = Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type);
    return std::span<const Type, extent>(reinterpret_cast<const Type*>(bytes.data()), bytes.size() / sizeof(Type));
}

template <class Type, size_t Extent>
inline std::span<const Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_span(std::span<const std::byte, Extent> bytes)
{
    return as_span<Type>(bytes, std::nothrow);
}

// as_writable_span()

template <class Type>
    requires (!std::is_const_v<Type>)
inline std::span<Type> as_writable_span(std::span<std::byte> bytes, maythrow_t)
{
    if (bytes.size() % sizeof(Type) != 0) [[unlikely]]
        throw span_size_error(bytes, sizeof(Type));
    return std::span<Type>(reinterpret_cast<Type*>(bytes.data()), bytes.size() / sizeof(Type));
}

template <class Type, size_t Extent>
    requires ((!std::is_const_v<Type>) && (Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0)))
inline std::span<Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_writable_span(std::span<std::byte, Extent> bytes, std::nothrow_t)
{
    constexpr auto extent = Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type);
    return std::span<Type, extent>(reinterpret_cast<Type*>(bytes.data()), bytes.size() / sizeof(Type));
}

template <class Type, size_t Extent>
    requires (!std::is_const_v<Type>)
inline std::span<Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_writable_span(std::span<std::byte, Extent> bytes)
{
    return as_writable_span<Type>(bytes, std::nothrow);
}

}
}
