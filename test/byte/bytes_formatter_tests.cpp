#include <arba/core/byte/bytes_formatter.hpp>

#include "create_resource.hpp"

#include <gtest/gtest.h>

#include <cstdlib>
#include <span>

class bytes_formatter_tests : public testing::Test
{
protected:
    bytes_formatter_tests()
    {
        input_fpath = ut::create_binary_resource("bytes_formatter_tests");
    }

    ~bytes_formatter_tests() override {}

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    std::filesystem::path input_fpath;
};

TEST_F(bytes_formatter_tests, format_bytes__empty_span__ok)
{
    std::array<const std::uint8_t, 0> bytes = {};
    core::bytes_formatter bformatter;
    const std::string result = bformatter.format_bytes(std::as_bytes(std::span(bytes)));
    ASSERT_EQ(result, "[]");
}

TEST_F(bytes_formatter_tests, format_bytes__non_empty_span__ok)
{
    const std::vector<std::uint8_t> bytes = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                                             27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45};
    core::bytes_formatter bformatter;
    const std::string result = bformatter.format_bytes(std::as_bytes(std::span(bytes)));
    std::string_view expected_res = "[0a, 0b, 0c, 0d, 0e, 0f, 10, 11, 12, 13, 14, 15, 16, 17, 18, "
                                    "19, 1a, 1b, 1c, 1d, 1e, 1f, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29\n2a, 2b, 2c, 2d]";
    ASSERT_EQ(result, expected_res);
}

TEST_F(bytes_formatter_tests, format_bytes__non_empty_span_and_custom_parameters__ok)
{
    using namespace core::bytes_formatter_kwargs;

    const std::vector<std::uint8_t> bytes = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
    core::bytes_formatter bformatter(unit_format("{1}:{0:0u}"), unit_sep(","), seq_beginning("["), seq_end("]"),
                                     chunk_beginning("("), chunk_end(")"), nb_units_per_chunk(10));
    std::string result = bformatter.format_bytes(std::as_bytes(std::span(bytes)));
    std::string expected_res = "[(0:010,1:011,2:012,3:013,4:014,5:015,6:016,7:017,8:018,9:019)(10:020,11:021,12:022,13:023,14:024,15:025)]";
    ASSERT_EQ(result, expected_res);

    bformatter = core::bytes_formatter(unit_format("{1}:{0:0u}"), unit_sep(","), seq_beginning("["), seq_end("]"),
                                       chunk_beginning("("), chunk_end(")"), nb_units_per_chunk(10), force_chunk_end(true));
    result = bformatter.format_bytes(std::as_bytes(std::span(bytes)), 10);
    expected_res = "[(10:010,11:011,12:012,13:013,14:014,15:015,16:016,17:017,18:018,19:019)(20:020,21:021,22:022,23:023,24:024,25:025)]";
    ASSERT_EQ(result, expected_res);

    bformatter = core::bytes_formatter(unit_format("{:0u}"), unit_sep(","), seq_beginning("["), seq_end("]"),
                                       chunk_beginning("("), chunk_end(")"), nb_units_per_chunk(10), force_chunk_end(false));
    result = bformatter.format_bytes(std::as_bytes(std::span(bytes)));
    expected_res = "[(010,011,012,013,014,015,016,017,018,019)(020,021,022,023,024,025]";
    ASSERT_EQ(result, expected_res);
}

TEST_F(bytes_formatter_tests, format_binary_stream__valid_fstream__ok)
{
    core::bytes_formatter bformatter;
    std::ifstream input_file_stream(input_fpath);
    const std::string result = bformatter.format_binary_stream(input_file_stream);
    std::string_view expected_res = "[00, 02, 04, 07, 0b, 0d, 11, 17, 42, 0c, 0e, 12, 19, 21, 7f, 41]";
    ASSERT_EQ(result, expected_res);
}

TEST_F(bytes_formatter_tests, format_binary_stream__valid_sstream__ok)
{
    core::bytes_formatter bformatter;
    std::istringstream input_stream("ABCDEFGH");
    const std::string result = bformatter.format_binary_stream(input_stream);
    std::string_view expected_res = "[41, 42, 43, 44, 45, 46, 47, 48]";
    ASSERT_EQ(result, expected_res);
}

TEST_F(bytes_formatter_tests, format_binary_cstream__valid_fcstream__ok)
{
    core::bytes_formatter bformatter;
    FILE* input_file = std::fopen(input_fpath.generic_string().c_str(), "rb");
    const std::string result = bformatter.format_binary_cstream(input_file);
    std::string_view expected_res = "[00, 02, 04, 07, 0b, 0d, 11, 17, 42, 0c, 0e, 12, 19, 21, 7f, 41]";
    ASSERT_EQ(result, expected_res);
    std::fclose(input_file);
}
