#include <arba/core/template/request_type.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>

class serializable_tag { ~serializable_tag() = delete; };

ARBA_CORE_DEFINE_REQUESTED_TYPE(serializable_tag, std::string, std::u8string)

TEST(type_helper_tests, test_requested_type)
{
    using serializable_string_type = core::requested_type_t<serializable_tag, std::string>;
    static_assert(std::is_same_v<serializable_string_type, std::u8string>);
}

TEST(type_helper_tests, test_requested_type_or_defined)
{
    using serializable_string_type = core::requested_type_or_t<serializable_tag, std::string, std::u32string>;
    static_assert(std::is_same_v<serializable_string_type, std::u8string>);
    static_assert(!std::is_same_v<serializable_string_type, std::u32string>);
}

TEST(type_helper_tests, test_requested_type_or_undefined)
{
    using serializable_string_type = core::requested_type_or_t<serializable_tag, const char*, std::u32string>;
    static_assert(!std::is_same_v<serializable_string_type, std::u8string>);
    static_assert(std::is_same_v<serializable_string_type, std::u32string>);
}
