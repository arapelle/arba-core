#include <arba/core/bit/htow_when.hpp>

#include <gtest/gtest.h>

#include <cstdlib>

TEST(htow_if_tests, htow_if__endianness_specific__ok)
{
    const uint16_t value = 0x1122;
    const uint16_t swapped_value = core::htow_when(value, meta::endianness_specific);
    const uint16_t expected_value = value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_if_tests, htow_if__endianness_neutral__ok)
{
    const uint16_t value = 0x1122;
    const uint16_t swapped_value = core::htow_when(value, meta::endianness_neutral);
    uint16_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = 0x2211;
    else
        expected_value = 0x1122;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_if_tests, wtoh_if__endianness_specific__ok)
{
    const uint16_t value = 0x2211;
    const uint16_t swapped_value = core::htow_when(value, meta::endianness_specific);
    const uint16_t expected_value = value;
    ASSERT_EQ(swapped_value, expected_value);
}

TEST(htow_if_tests, wtoh_if__endianness_neutral__ok)
{
    const uint16_t value = 0x2211;
    const uint16_t swapped_value = core::wtoh_when(value, meta::endianness_neutral);
    uint16_t expected_value = 0;
    if constexpr (std::endian::native == std::endian::little)
        expected_value = 0x1122;
    else
        expected_value = 0x2211;
    ASSERT_EQ(swapped_value, expected_value);
}
