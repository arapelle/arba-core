#include <core/hash.hpp>
#include <gtest/gtest.h>
#include <cstdlib>

// murmur_hash_64:

TEST(core_tests, test_murmur_hash_64_eq)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::murmur_hash_64(bytes.data(), bytes.size());
    uint64_t hash2 = core::murmur_hash_64(bytes.data(), bytes.size());
    ASSERT_EQ(hash, hash2);
}

TEST(core_tests, test_murmur_hash_64_neq_data)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::murmur_hash_64(bytes.data(), bytes.size());
    std::array<std::uint8_t, 8> other_bytes{ 103, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash2 = core::murmur_hash_64(other_bytes.data(), other_bytes.size());
    ASSERT_NE(hash, hash2);
}

TEST(core_tests, test_murmur_hash_64_neq_seed)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::murmur_hash_64(bytes.data(), bytes.size());
    uint64_t hash2 = core::murmur_hash_64(bytes.data(), bytes.size(), 8467930056364136223ULL);
    ASSERT_NE(hash, hash2);
}

TEST(core_tests, test_murmur_hash_64_eq_from_range)
{
    std::array<std::uint8_t, 8> a_bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::vector<std::uint8_t> v_bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::murmur_hash_64(a_bytes);
    uint64_t hash2 = core::murmur_hash_64(v_bytes);
    ASSERT_EQ(hash, hash2);
}

// neutral_murmur_hash_64:

TEST(core_tests, test_neutral_murmur_hash_64_eq)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::neutral_murmur_hash_64(bytes.data(), bytes.size());
    uint64_t hash2 = core::neutral_murmur_hash_64(bytes.data(), bytes.size());
    ASSERT_EQ(hash, hash2);
}

TEST(core_tests, test_neutral_murmur_hash_64_neq_data)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::neutral_murmur_hash_64(bytes.data(), bytes.size());
    std::array<std::uint8_t, 8> other_bytes{ 103, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash2 = core::murmur_hash_64(other_bytes.data(), other_bytes.size());
    ASSERT_NE(hash, hash2);
}

TEST(core_tests, test_neutral_murmur_hash_64_neq_seed)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::neutral_murmur_hash_64(bytes.data(), bytes.size());
    uint64_t hash2 = core::neutral_murmur_hash_64(bytes.data(), bytes.size(), 8467930056364136223ULL);
    ASSERT_NE(hash, hash2);
}

TEST(core_tests, test_neutral_murmur_hash_64_eq_from_range)
{
    std::array<std::uint8_t, 8> a_bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::vector<std::uint8_t> v_bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    uint64_t hash = core::neutral_murmur_hash_64(a_bytes);
    uint64_t hash2 = core::neutral_murmur_hash_64(v_bytes);
    ASSERT_EQ(hash, hash2);
}

// neutral_murmur_hash_array_16:

TEST(core_tests, test_neutral_murmur_hash_array_16_eq)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::array<std::uint8_t, 16> hash = core::neutral_murmur_hash_array_16(bytes.data(), bytes.size());
    std::array<std::uint8_t, 16> hash2 = core::neutral_murmur_hash_array_16(bytes.data(), bytes.size());
    ASSERT_EQ(hash, hash2);
}

TEST(core_tests, test_neutral_murmur_hash_array_16_neq_data)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::array<std::uint8_t, 16> hash = core::neutral_murmur_hash_array_16(bytes.data(), bytes.size());
    std::array<std::uint8_t, 8> other_bytes{ 103, 26, 64, 25, 55, 224, 146, 246 };
    std::array<std::uint8_t, 16> hash2 = core::neutral_murmur_hash_array_16(other_bytes.data(), bytes.size());
    ASSERT_NE(hash, hash2);
}

TEST(core_tests, test_neutral_murmur_hash_array_16_neq_seed)
{
    std::array<std::uint8_t, 8> bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::array<std::uint8_t, 16> hash = core::neutral_murmur_hash_array_16(bytes.data(), bytes.size());
    std::array<std::uint8_t, 16> hash2 = core::neutral_murmur_hash_array_16(bytes.data(), bytes.size(), 8467930056364136223ULL);
    ASSERT_NE(hash, hash2);
}

TEST(core_tests, test_neutral_murmur_hash_array_16_neq_seed_from_range)
{
    std::array<std::uint8_t, 8> a_bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::vector<std::uint8_t> v_bytes{ 102, 26, 64, 25, 55, 224, 146, 246 };
    std::array<std::uint8_t, 16> hash = core::neutral_murmur_hash_array_16(a_bytes);
    std::array<std::uint8_t, 16> hash2 = core::neutral_murmur_hash_array_16(v_bytes);
    ASSERT_EQ(hash, hash2);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
