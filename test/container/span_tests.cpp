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
        std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span, cppx::maythrow);
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
        [[maybe_unused]] std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span, cppx::maythrow);
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
        std::span<tri_char> tri_char_span = core::as_writable_span<tri_char>(bytes_span, cppx::maythrow);
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
            core::as_writable_span<tri_char>(bytes_span, cppx::maythrow);
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
unsigned count_bytes()
{
    Type value = 0;
    unsigned counter = 0;
    std::span<const std::byte, sizeof(value)> bytes = core::as_bytes(value);
    std::ranges::for_each(bytes,
                          [&](const std::byte& b)
                          {
                              if (b == std::byte{ 0x00 })
                                  ++counter;
                          });
    return counter;
}

TEST(span_tests, as_bytes__uint_t__ok)
{
    ASSERT_EQ(count_bytes<uint8_t>(), 1);
    ASSERT_EQ(count_bytes<uint16_t>(), 2);
    ASSERT_EQ(count_bytes<uint32_t>(), 4);
    ASSERT_EQ(count_bytes<uint64_t>(), 8);
}

TEST(span_tests, as_bytes__int_t__ok)
{
    ASSERT_EQ(count_bytes<int8_t>(), 1);
    ASSERT_EQ(count_bytes<int16_t>(), 2);
    ASSERT_EQ(count_bytes<int32_t>(), 4);
    ASSERT_EQ(count_bytes<int64_t>(), 8);
}

// as_writable_bytes() tests

template <std::integral Type>
Type transform_integer()
{
    Type value;
    std::span<std::byte, sizeof(value)> bytes = core::as_writable_bytes(value);
    std::ranges::for_each(bytes, [](std::byte& b) { b = std::byte{ 0x02 }; });
    return value;
}

TEST(span_tests, as_writable_bytes__uint_t__ok)
{
    ASSERT_EQ(transform_integer<uint8_t>(), 0x02);
    ASSERT_EQ(transform_integer<uint16_t>(), 0x0202);
    ASSERT_EQ(transform_integer<uint32_t>(), 0x02020202);
    ASSERT_EQ(transform_integer<uint64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_writable_bytes__int_t__ok)
{
    ASSERT_EQ(transform_integer<int8_t>(), 0x02);
    ASSERT_EQ(transform_integer<int16_t>(), 0x0202);
    ASSERT_EQ(transform_integer<int32_t>(), 0x02020202);
    ASSERT_EQ(transform_integer<int64_t>(), 0x0202020202020202);
}

// as_uint(), as_int(), as_integer()

template <std::integral Type>
Type const_bytes_to_const_integer()
{
    std::array<std::byte, sizeof(Type)> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<const std::byte, sizeof(Type)> bytes_span(bytes);
    const Type& cref = core::as_integer<Type>(bytes_span);
    return cref;
}

template <std::size_t BitSize>
auto const_bytes_to_const_uint()
{
    std::array<std::byte, BitSize / 8> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<const std::byte, sizeof(bytes)> bytes_span(bytes);
    const meta::uint_n_t<BitSize>& cref = core::as_uint<BitSize>(bytes_span);
    return cref;
}

template <std::size_t BitSize>
auto const_bytes_to_const_int()
{
    std::array<std::byte, BitSize / 8> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<const std::byte, sizeof(bytes)> bytes_span(bytes);
    const meta::int_n_t<BitSize>& cref = core::as_int<BitSize>(bytes_span);
    return cref;
}

template <std::integral Type>
Type bytes_to_const_integer()
{
    std::array<std::byte, sizeof(Type)> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<std::byte, sizeof(Type)> bytes_span(bytes);
    const Type& cref = core::as_integer<Type>(bytes_span);
    return cref;
}

template <std::size_t BitSize>
auto bytes_to_const_uint()
{
    std::array<std::byte, BitSize / 8> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<std::byte, sizeof(bytes)> bytes_span(bytes);
    const meta::uint_n_t<BitSize>& cref = core::as_uint<BitSize>(bytes_span);
    return cref;
}

template <std::size_t BitSize>
auto bytes_to_const_int()
{
    std::array<std::byte, BitSize / 8> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<std::byte, sizeof(bytes)> bytes_span(bytes);
    const meta::int_n_t<BitSize>& cref = core::as_int<BitSize>(bytes_span);
    return cref;
}

TEST(span_tests, as_integer__from_const_bytes_to_uint_t__ok)
{
    ASSERT_EQ(const_bytes_to_const_integer<uint8_t>(), 0x02);
    ASSERT_EQ(const_bytes_to_const_integer<uint16_t>(), 0x0202);
    ASSERT_EQ(const_bytes_to_const_integer<uint32_t>(), 0x02020202);
    ASSERT_EQ(const_bytes_to_const_integer<uint64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_integer__from_const_bytes_to_int_t__ok)
{
    ASSERT_EQ(const_bytes_to_const_integer<int8_t>(), 0x02);
    ASSERT_EQ(const_bytes_to_const_integer<int16_t>(), 0x0202);
    ASSERT_EQ(const_bytes_to_const_integer<int32_t>(), 0x02020202);
    ASSERT_EQ(const_bytes_to_const_integer<int64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_uint__from_const_bytes_to_u8_to_u64__ok)
{
    ASSERT_EQ(const_bytes_to_const_uint<8>(), 0x02);
    ASSERT_EQ(const_bytes_to_const_uint<16>(), 0x0202);
    ASSERT_EQ(const_bytes_to_const_uint<32>(), 0x02020202);
    ASSERT_EQ(const_bytes_to_const_uint<64>(), 0x0202020202020202);
}

TEST(span_tests, as_int__from_const_bytes_to_i8_to_i64__ok)
{
    ASSERT_EQ(const_bytes_to_const_int<8>(), 0x02);
    ASSERT_EQ(const_bytes_to_const_int<16>(), 0x0202);
    ASSERT_EQ(const_bytes_to_const_int<32>(), 0x02020202);
    ASSERT_EQ(const_bytes_to_const_int<64>(), 0x0202020202020202);
}

TEST(span_tests, as_integer__from_bytes_to_uint_t__ok)
{
    ASSERT_EQ(bytes_to_const_integer<uint8_t>(), 0x02);
    ASSERT_EQ(bytes_to_const_integer<uint16_t>(), 0x0202);
    ASSERT_EQ(bytes_to_const_integer<uint32_t>(), 0x02020202);
    ASSERT_EQ(bytes_to_const_integer<uint64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_integer__from_bytes_to_int_t__ok)
{
    ASSERT_EQ(bytes_to_const_integer<int8_t>(), 0x02);
    ASSERT_EQ(bytes_to_const_integer<int16_t>(), 0x0202);
    ASSERT_EQ(bytes_to_const_integer<int32_t>(), 0x02020202);
    ASSERT_EQ(bytes_to_const_integer<int64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_uint__from_bytes_to_u8_to_u64__ok)
{
    ASSERT_EQ(bytes_to_const_uint<8>(), 0x02);
    ASSERT_EQ(bytes_to_const_uint<16>(), 0x0202);
    ASSERT_EQ(bytes_to_const_uint<32>(), 0x02020202);
    ASSERT_EQ(bytes_to_const_uint<64>(), 0x0202020202020202);
}

TEST(span_tests, as_int__from_bytes_to_i8_to_i64__ok)
{
    ASSERT_EQ(bytes_to_const_int<8>(), 0x02);
    ASSERT_EQ(bytes_to_const_int<16>(), 0x0202);
    ASSERT_EQ(bytes_to_const_int<32>(), 0x02020202);
    ASSERT_EQ(bytes_to_const_int<64>(), 0x0202020202020202);
}

//

template <std::integral Type>
Type bytes_to_integer()
{
    std::array<std::byte, sizeof(Type)> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<std::byte, sizeof(Type)> bytes_span(bytes);
    Type& ref = core::as_writable_integer<Type>(bytes_span);
    return ref;
}

template <std::size_t BitSize>
auto bytes_to_uint()
{
    std::array<std::byte, BitSize / 8> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<std::byte, sizeof(bytes)> bytes_span(bytes);
    meta::uint_n_t<BitSize>& ref = core::as_writable_uint<BitSize>(bytes_span);
    return ref;
}

template <std::size_t BitSize>
auto bytes_to_int()
{
    std::array<std::byte, BitSize / 8> bytes;
    std::ranges::for_each(bytes, [](std::byte& value) { value = std::byte{ 0x02 }; });
    std::span<std::byte, sizeof(bytes)> bytes_span(bytes);
    meta::int_n_t<BitSize>& ref = core::as_writable_int<BitSize>(bytes_span);
    return ref;
}

TEST(span_tests, as_writable_integer__from_bytes_to_uint_t__ok)
{
    ASSERT_EQ(bytes_to_integer<uint8_t>(), 0x02);
    ASSERT_EQ(bytes_to_integer<uint16_t>(), 0x0202);
    ASSERT_EQ(bytes_to_integer<uint32_t>(), 0x02020202);
    ASSERT_EQ(bytes_to_integer<uint64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_writable_integer__from_bytes_to_int_t__ok)
{
    ASSERT_EQ(bytes_to_integer<int8_t>(), 0x02);
    ASSERT_EQ(bytes_to_integer<int16_t>(), 0x0202);
    ASSERT_EQ(bytes_to_integer<int32_t>(), 0x02020202);
    ASSERT_EQ(bytes_to_integer<int64_t>(), 0x0202020202020202);
}

TEST(span_tests, as_writable_uint__from_bytes_to_u8_to_u64__ok)
{
    ASSERT_EQ(bytes_to_uint<8>(), 0x02);
    ASSERT_EQ(bytes_to_uint<16>(), 0x0202);
    ASSERT_EQ(bytes_to_uint<32>(), 0x02020202);
    ASSERT_EQ(bytes_to_uint<64>(), 0x0202020202020202);
}

TEST(span_tests, as_writable_int__from_bytes_to_i8_to_i64__ok)
{
    ASSERT_EQ(bytes_to_int<8>(), 0x02);
    ASSERT_EQ(bytes_to_int<16>(), 0x0202);
    ASSERT_EQ(bytes_to_int<32>(), 0x02020202);
    ASSERT_EQ(bytes_to_int<64>(), 0x0202020202020202);
}
