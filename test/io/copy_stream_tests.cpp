#include <arba/core/io/copy_stream.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::filesystem::path create_resource()
{
    std::filesystem::path root_dpath = std::filesystem::temp_directory_path() / "arba/core" / "copy_stream_tests";
    if (std::filesystem::exists(root_dpath))
        std::filesystem::remove_all(root_dpath);
    std::filesystem::create_directories(root_dpath);

    std::filesystem::path rsc_dpath = root_dpath / "rsc";
    std::filesystem::create_directories(rsc_dpath);

    std::filesystem::path story_fpath = rsc_dpath/"open_file.txt";
    std::ofstream rsc_fstream(story_fpath);
    rsc_fstream << "Once upon a time";

    return story_fpath;
}

TEST(copy_stream_tests, test_copy_stream)
{
    std::istringstream iss("hello world", std::ios::binary);
    std::ostringstream oss(std::ios::binary);
    core::copy_stream(iss, 4, oss);
    ASSERT_EQ(oss.str(), "hell");
}

TEST(copy_stream_tests, test_copy_fstream)
{
    std::ifstream ifs(create_resource(), std::ios::binary);
    std::ostringstream oss(std::ios::binary);
    core::copy_fstream(ifs, oss);
    ASSERT_EQ(oss.str(), "Once upon a time");
}

TEST(copy_stream_tests, test_copy_fstream_2)
{
    std::ifstream ifs(create_resource(), std::ios::binary);
    ifs.get();
    std::ostringstream oss(std::ios::binary);
    core::copy_fstream(ifs, oss);
    ASSERT_EQ(oss.str(), "nce upon a time");
}

TEST(copy_stream_tests, test_copy_file)
{
    std::ostringstream oss(std::ios::binary);
    core::copy_file(create_resource(), oss);
    ASSERT_EQ(oss.str(), "Once upon a time");
}

TEST(copy_stream_tests, test_copy_file_exception_file_not_found)
{
    std::ostringstream oss(std::ios::binary);
    try
    {
        core::copy_file("/__does_not_exists__", oss);
        FAIL() << "core::copy_file(\"/__does_not_exists__\", oss) should throw an error\n";
    }
    catch (const std::filesystem::filesystem_error& err)
    {
        EXPECT_EQ(err.code().message(), "No such file or directory");
        SUCCEED();
    }
    catch (const std::iostream::failure& err)
    {
        FAIL();
    }
}

TEST(copy_stream_tests, test_copy_file_exception_not_regular_file)
{
    std::ostringstream oss(std::ios::binary);
    try
    {
        core::copy_file(std::filesystem::temp_directory_path(), oss);
        FAIL() << "core::copy_file(std::filesystem::temp_directory_path(), oss) should throw an error\n";
    }
    catch (const std::filesystem::filesystem_error& err)
    {
        EXPECT_EQ(err.code().message(), "No such file or directory");
        SUCCEED();
    }
    catch (const std::iostream::failure& err)
    {
        FAIL();
    }
}
