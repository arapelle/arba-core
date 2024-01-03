#include <arba/core/span.hpp>
#include <array>
#include <gtest/gtest.h>

struct tri_char
{
    char first;
    char second;
    char third;
};

// as_span() tests

TEST(span_tests, test_as_span_N__nothrow__no_throw_span_size_2)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_span__nothrow__no_throw_span_size_2)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_span_const__nothrow__no_throw_span_size_2)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_span__nothrow__no_throw_span_size_1)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_span__maythrow__no_throw)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span, core::maythrow);
        ASSERT_EQ(reinterpret_cast<const void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_span__maythrow__throw_span_size_error)
{
    try
    {
        std::array<uint8_t, 5> bytes{ 'a', 'b', 'c', 'd', 'e' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        [[maybe_unused]] std::span<const tri_char> tri_char_span = core::as_span<tri_char>(bytes_span, core::maythrow);
        FAIL();
    }
    catch (const core::span_size_error& err)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}

// as_wriatable_span() tests

TEST(span_tests, test_as_writable_span_N__nothrow__no_throw_span_size_2)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_writable_span__nothrow__no_throw_span_size_2)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_writable_span__nothrow__no_throw_span_size_1)
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
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_writable_span__maythrow__no_throw_span_size_2)
{
    try
    {
        std::array<uint8_t, 6> bytes{ 'a', 'b', 'c', 'd', 'e', 'f' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        std::span<tri_char> tri_char_span = core::as_writable_span<tri_char>(bytes_span, core::maythrow);
        ASSERT_EQ(reinterpret_cast<void*>(tri_char_span.data()), reinterpret_cast<void*>(bytes.data()));
        ASSERT_EQ(tri_char_span.size(), 2);
        ASSERT_EQ(tri_char_span[0].first, 'a');
        ASSERT_EQ(tri_char_span[0].second, 'b');
        ASSERT_EQ(tri_char_span[0].third, 'c');
        ASSERT_EQ(tri_char_span[1].first, 'd');
        ASSERT_EQ(tri_char_span[1].second, 'e');
        ASSERT_EQ(tri_char_span[1].third, 'f');
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(span_tests, test_as_writable_span__maythrow__throw_span_size_error)
{
    try
    {
        std::array<uint8_t, 5> bytes{ 'a', 'b', 'c', 'd', 'e' };
        std::span<std::byte> bytes_span = std::as_writable_bytes(std::span(bytes));
        [[maybe_unused]] std::span<const tri_char> tri_char_span =
            core::as_writable_span<tri_char>(bytes_span, core::maythrow);
        FAIL();
    }
    catch (const core::span_size_error& err)
    {
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}
