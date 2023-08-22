#include <arba/core/algorithm.hpp>
#include <gtest/gtest.h>
#include <cstdlib>
#include <memory>
#include <list>

TEST(algorithm_tests, test_unstable_erase_string_unsafe_body_item)
{
    std::vector<std::string> vec = { "0", "1", "2", "3", "4", "5", "6", "10" };
    auto iter = vec.begin() + 2;
    core::unstable_erase(vec, iter);
    std::vector<std::string> expected_vec = { "0", "1", "10", "3", "4", "5", "6" };
    ASSERT_EQ(vec, expected_vec);
}

TEST(algorithm_tests, test_unstable_erase_string_unsafe_last_item)
{
    std::vector<std::string> vec = { "0", "1", "2", "3", "4", "5", "6", "10" };
    auto iter = --vec.end();
    core::unstable_erase(vec, iter);
    std::vector<std::string> expected_vec = { "0", "1", "2", "3", "4", "5", "6" };
    ASSERT_EQ(vec, expected_vec);
}

TEST(algorithm_tests, test_unstable_erase_int_unsafe_body_item)
{
    std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 10 };
    auto iter = vec.begin() + 2;
    core::unstable_erase(vec, iter);
    std::vector<int> expected_vec = { 0, 1, 10, 3, 4, 5, 6 };
    ASSERT_EQ(vec, expected_vec);
}

TEST(algorithm_tests, test_unstable_erase_int_unsafe_last_item)
{
    std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 10 };
    auto iter = --vec.end();
    core::unstable_erase(vec, iter);
    std::vector<int> expected_vec = { 0, 1, 2, 3, 4, 5, 6 };
    ASSERT_EQ(vec, expected_vec);
}

TEST(algorithm_tests, test_unstable_erase_safe)
{
    std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 10 };
    auto iter = vec.begin() + 2;
    core::unstable_erase(vec, iter, core::not_last_value);
    std::vector<int> expected_vec = { 0, 1, 10, 3, 4, 5, 6 };
    ASSERT_EQ(vec, expected_vec);
}


TEST(algorithm_tests, test_quick_erase_unsafe)
{
    std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 10 };
    auto iter = --vec.end();
    core::quick_erase(vec, iter);
    std::vector<int> expected_vec = { 0, 1, 2, 3, 4, 5, 6 };
    ASSERT_EQ(vec, expected_vec);
}

TEST(algorithm_tests, test_quick_erase_safe)
{
    std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 10 };
    auto iter = vec.begin();
    core::quick_erase(vec, iter, core::not_last_value);
    std::vector<int> expected_vec = { 10, 1, 2, 3, 4, 5, 6 };
    ASSERT_EQ(vec, expected_vec);
}

TEST(algorithm_tests, test_quick_erase_not_random_range)
{
    std::list<int> collection = { 0, 1, 2, 3, 4, 5, 6, 10 };
    auto iter = collection.begin();
    core::quick_erase(collection, iter);
    std::list<int> expected_collection = { 1, 2, 3, 4, 5, 6, 10 };
    ASSERT_EQ(collection, expected_collection);
}
