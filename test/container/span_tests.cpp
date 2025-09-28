#include <arba/core/container/span.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>

struct tri_char
{
    char first;
    char second;
    char third;
};

// as_span() tests

TEST(span_tests, as_span_N__nothrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte, 6> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<const tri_char, 2> tri_char_span = core::as_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<const void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_span__nothrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<const void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_span_const__nothrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<const std::byte> bytes_span = std::as_bytes(std::span(bytes));
        std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<const void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_span__nothrow__no_throw_span_size_1)
{
    try
    {
        std::array<uint8_t, 5> bytes{ 'a', 'b', 'c', 'd', 'e' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<const void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 1);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_span__maythrow__no_throw)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span, meta::maythrow);
        ASSERT_EQ(reinterpret_cast<const void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_span__maythrow__throw_span_size_error)
{
    try
    {
        std::array<uint8_t, 5> bytes{ 'a', 'b', 'c', 'd', 'e' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        [[maybe_unused]] std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span, meta::maythrow);
        FAIL();
    }
    catch (const core::span_size_error& err)
    {
        SUCCEED();
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

// as_writable_span() tests

TEST(span_tests, as_writable_span_N__nothrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte, 6> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<tri_char, 2> tri_char_span = core::as_writable_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_writable_span__nothrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<tri_char> tri_char_span = core::as_writable_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_writable_span__nothrow__no_throw_span_size_1)
{
    try
    {
        std::array<uint8_t, 5> bytes{ 'a', 'b', 'c', 'd', 'e' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<tri_char> tri_char_span = core::as_writable_span<tri_char>(bytes_span);
        ASSERT_EQ(reinterpret_cast<void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 1);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_writable_span__maythrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<tri_char> tri_char_span = core::as_writable_span<tri_char>(bytes_span, meta::maythrow);
        ASSERT_EQ(reinterpret_cast<void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

TEST(span_tests, as_writable_span__maythrow__throw_span_size_error)
{
    try
    {
        std::array<uint8_t, 5> bytes{ 'a', 'b', 'c', 'd', 'e' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        [[maybe_unused]] std::span<const tri_char> tri_char_span =
            core::as_writable_span<tri_char>(bytes_span, meta::maythrow);
        FAIL();
    }
    catch (const core::span_size_error& err)
    {
        SUCCEED();
    }
    catch (const std::exception& err)
    {
        FAIL() << err.what();
    }
}

// as_bytes() tests

template <std::integral Type>
unsigned count_bytes(const Type& value)
{
    unsigned counter = 0;
    std::span<const std::byte, sizeof(value)> bytes = core::as_bytes(value);
    std::ranges::for_each(bytes, [&](const std::byte& b){ if (b == std::byte{0x01}) ++counter; });
    return counter;
}

TEST(span_tests, as_bytes__uint_t__ok)
{
    uint8_t u8value = 0x01;
    ASSERT_EQ(count_bytes(u8value), 1);
    uint16_t u16value = 0x0101;
    ASSERT_EQ(count_bytes(u16value), 2);
    uint32_t u32value = 0x01010101;
    ASSERT_EQ(count_bytes(u32value), 4);
    uint64_t u64value = 0x0101010101010101;
    ASSERT_EQ(count_bytes(u64value), 8);
}

TEST(span_tests, as_bytes__int_t__ok)
{
    int8_t i8value = 0x01;
    ASSERT_EQ(count_bytes(i8value), 1);
    int16_t i16value = 0x0101;
    ASSERT_EQ(count_bytes(i16value), 2);
    int32_t i32value = 0x01010101;
    ASSERT_EQ(count_bytes(i32value), 4);
    int64_t i64value = 0x0101010101010101;
    ASSERT_EQ(count_bytes(i64value), 8);
}

// as_writable_bytes() tests

template <std::integral Type>
Type& transform_integer(Type& value)
{
    std::span<std::byte, sizeof(value)> bytes = core::as_writable_bytes(value);
    std::ranges::for_each(bytes, [](std::byte& b){ b = std::byte{0x02}; });
    return value;
}

TEST(span_tests, as_writable_bytes__uint_t__ok)
{
    uint8_t u8value = 0x01;
    ASSERT_EQ(transform_integer(u8value), 0x02);
    uint16_t u16value = 0x0101;
    ASSERT_EQ(transform_integer(u16value), 0x0202);
    uint32_t u32value = 0x01010101;
    ASSERT_EQ(transform_integer(u32value), 0x02020202);
    uint64_t u64value = 0x0101010101010101;
    ASSERT_EQ(transform_integer(u64value), 0x0202020202020202);
}

TEST(span_tests, as_writable_bytes__int_t__ok)
{
    int8_t i8value = 0x01;
    ASSERT_EQ(transform_integer(i8value), 0x02);
    int16_t i16value = 0x0101;
    ASSERT_EQ(transform_integer(i16value), 0x0202);
    int32_t i32value = 0x01010101;
    ASSERT_EQ(transform_integer(i32value), 0x02020202);
    int64_t i64value = 0x0101010101010101;
    ASSERT_EQ(transform_integer(i64value), 0x0202020202020202);
}
