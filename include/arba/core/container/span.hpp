#pragma once

#include <arba/cppx/policy/exception_policy.hpp>
#include <arba/meta/type_traits/integer_n.hpp>

#include <format>
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
    span_size_error(const std::span<std::byte>& bytes, std::size_t type_size)
        : std::invalid_argument(
              std::format("Bytes size ({}) does not match N size of Type ({}).", bytes.size(), type_size))
    {
    }

    span_size_error(const std::span<const std::byte>& bytes, std::size_t type_size)
        : std::invalid_argument(
              std::format("Bytes size ({}) does not match N size of Type ({}).", bytes.size(), type_size))
    {
    }
};

// as_span()

template <class Type, size_t Extent>
    requires(Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0))
inline std::span<const Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_span(std::span<std::byte, Extent> bytes, cppx::maythrow_t)
{
    if constexpr (Extent == std::dynamic_extent)
    {
        if (bytes.size() % sizeof(Type) != 0) [[unlikely]]
            throw span_size_error(bytes, sizeof(Type));
    }
    constexpr auto extent = Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type);
    return std::span<const Type, extent>{ reinterpret_cast<const Type*>(bytes.data()), bytes.size() / sizeof(Type) };
}

template <class Type, size_t Extent>
    requires(Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0))
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
inline std::span<const Type> as_span(std::span<const std::byte> bytes, cppx::maythrow_t)
{
    if (bytes.size() % sizeof(Type) != 0) [[unlikely]]
        throw span_size_error(bytes, sizeof(Type));
    return std::span<const Type>{ reinterpret_cast<const Type*>(bytes.data()), bytes.size() / sizeof(Type) };
}

template <class Type, size_t Extent>
    requires(Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0))
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
    requires(!std::is_const_v<Type>)
inline std::span<Type> as_writable_span(std::span<std::byte> bytes, cppx::maythrow_t)
{
    if (bytes.size() % sizeof(Type) != 0) [[unlikely]]
        throw span_size_error(bytes, sizeof(Type));
    return std::span<Type>(reinterpret_cast<Type*>(bytes.data()), bytes.size() / sizeof(Type));
}

template <class Type, size_t Extent>
    requires((!std::is_const_v<Type>) && (Extent == std::dynamic_extent || (Extent % sizeof(Type) == 0)))
inline std::span<Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_writable_span(std::span<std::byte, Extent> bytes, std::nothrow_t)
{
    constexpr auto extent = Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type);
    return std::span<Type, extent>(reinterpret_cast<Type*>(bytes.data()), bytes.size() / sizeof(Type));
}

template <class Type, size_t Extent>
    requires(!std::is_const_v<Type>)
inline std::span<Type, Extent == std::dynamic_extent ? std::dynamic_extent : Extent / sizeof(Type)>
as_writable_span(std::span<std::byte, Extent> bytes)
{
    return as_writable_span<Type>(bytes, std::nothrow);
}

// as_bytes()

template <std::integral Type>
std::span<const std::byte, sizeof(Type)> as_bytes(const Type& value)
{
    return std::span<const std::byte, sizeof(Type)>(reinterpret_cast<const std::byte*>(&value),
                                                    reinterpret_cast<const std::byte*>(&value) + sizeof(Type));
}

template <std::integral Type>
std::span<const std::byte, sizeof(Type)> as_bytes(const Type&& value) = delete;

// as_uint(), as_int(), as_integer()

template <std::integral Type, typename ByteType, size_t Extent>
    requires std::is_same_v<std::remove_const_t<ByteType>, std::byte>
             && (Extent == std::dynamic_extent || Extent == sizeof(Type))
const Type& as_integer(std::span<ByteType, Extent> bytes)
{
    return *reinterpret_cast<const Type*>(bytes.data());
}

template <unsigned BitSize, typename ByteType, std::size_t Extent>
    requires std::is_same_v<std::remove_const_t<ByteType>, std::byte>
             && (Extent == std::dynamic_extent || ((Extent * 8) == BitSize))
const meta::uint_n_t<BitSize>& as_uint(std::span<ByteType, Extent> bytes)
{
    return *reinterpret_cast<const meta::uint_n_t<BitSize>*>(bytes.data());
}

template <unsigned BitSize, typename ByteType, std::size_t Extent>
    requires std::is_same_v<std::remove_const_t<ByteType>, std::byte>
             && (Extent == std::dynamic_extent || ((Extent * 8) == BitSize))
const meta::int_n_t<BitSize>& as_int(std::span<ByteType, Extent> bytes)
{
    return *reinterpret_cast<const meta::int_n_t<BitSize>*>(bytes.data());
}

// as_writable_bytes()

template <std::integral Type>
std::span<std::byte, sizeof(Type)> as_writable_bytes(Type& value)
{
    return std::span<std::byte, sizeof(Type)>(reinterpret_cast<std::byte*>(&value),
                                              reinterpret_cast<std::byte*>(&value) + sizeof(Type));
}

// as_writable_uint(), as_writable_int(), as_writable_integer()

template <std::integral Type, size_t Extent>
    requires(!std::is_const_v<Type>) && (Extent == std::dynamic_extent || Extent == sizeof(Type))
Type& as_writable_integer(std::span<std::byte, Extent> bytes)
{
    return *reinterpret_cast<Type*>(bytes.data());
}

template <unsigned BitSize, size_t Extent>
    requires(Extent == std::dynamic_extent || ((Extent * 8) == BitSize))
meta::uint_n_t<BitSize>& as_writable_uint(std::span<std::byte, Extent> bytes)
{
    return *reinterpret_cast<meta::uint_n_t<BitSize>*>(bytes.data());
}

template <unsigned BitSize, std::size_t Extent>
    requires(Extent == std::dynamic_extent || ((Extent * 8) == BitSize))
meta::int_n_t<BitSize>& as_writable_int(std::span<std::byte, Extent> bytes)
{
    return *reinterpret_cast<meta::int_n_t<BitSize>*>(bytes.data());
}

} // namespace core
} // namespace arba
