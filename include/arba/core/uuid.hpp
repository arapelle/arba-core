#pragma once

#include <string_view>
#include <array>
#include <string>
#include <cstdint>

inline namespace arba
{
namespace core
{

/**
 * @brief The uuid class
 *
 * Implementation is taken from stduuid: https://github.com/mariusbancila/stduuid/blob/master/include/uuid.h
 */
class uuid
{
public:
    uuid();
    uuid(const std::string_view& str);
    bool is_nil() const;
    std::string to_string() const;
    inline const std::array<uint8_t, 16>& data() const { return data_; }
    inline std::array<uint8_t, 16>& data() { return data_; }

    auto operator<=>(const uuid&) const = default;
    friend std::ostream& operator<<(std::ostream& stream, const uuid& uuid);

private:
    std::array<uint8_t, 16> data_;
};

}
}

namespace std
{

template <>
struct hash< ::arba::core::uuid>
{
    std::size_t operator()(const ::arba::core::uuid& uuid) const;
};

}
