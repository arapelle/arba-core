#include <core/copy_stream.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdlib>


std::filesystem::path create_resource()
{
    std::filesystem::path rsc_dpath = std::filesystem::temp_directory_path()/"arba/core/rsc";
    std::filesystem::path story_fpath = rsc_dpath/"story.txt";

    if (!std::filesystem::exists(story_fpath))
    {
        std::filesystem::create_directories(rsc_dpath);
        std::ofstream rsc_fstream(story_fpath);
        rsc_fstream << "Once upon a time";
    }

    return story_fpath;
}

TEST(core_tests, test_copy_stream)
{
    std::istringstream iss("hello world", std::ios::binary);
    std::ostringstream oss(std::ios::binary);
    core::copy_stream(iss, 4, oss);
    ASSERT_EQ(oss.str(), "hell");
}

TEST(core_tests, test_copy_fstream)
{
    std::ifstream ifs(create_resource(), std::ios::binary);
    std::ostringstream oss(std::ios::binary);
    core::copy_fstream(ifs, oss);
    ASSERT_EQ(oss.str(), "Once upon a time");
}

TEST(core_tests, test_copy_fstream_2)
{
    std::ifstream ifs(create_resource(), std::ios::binary);
    ifs.get();
    std::ostringstream oss(std::ios::binary);
    core::copy_fstream(ifs, oss);
    ASSERT_EQ(oss.str(), "nce upon a time");
}

TEST(core_tests, test_copy_file)
{
    std::ostringstream oss(std::ios::binary);
    core::copy_file(create_resource(), oss);
    ASSERT_EQ(oss.str(), "Once upon a time");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
