#include <arba/core/span.hpp>
#include <format>

inline namespace arba
{
namespace core
{

span_size_error::span_size_error(const std::span<std::byte>& bytes, std::size_t type_size)
    : std::invalid_argument(std::format("Bytes size ({}) does not match N size of Type ({}).", bytes.size(), type_size))
{
}

span_size_error::span_size_error(const std::span<const std::byte>& bytes, std::size_t type_size)
    : std::invalid_argument(std::format("Bytes size ({}) does not match N size of Type ({}).", bytes.size(), type_size))
{
}

}
}
