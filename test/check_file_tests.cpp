#include <core/io/check_file.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::filesystem::path create_resource()
{
    std::filesystem::path rsc_dpath = std::filesystem::temp_directory_path()/"arba/core/rsc";
    std::filesystem::path story_fpath = rsc_dpath/"check_file.txt";

    if (!std::filesystem::exists(story_fpath))
    {
        std::filesystem::create_directories(rsc_dpath);
        std::ofstream rsc_fstream(story_fpath);
        rsc_fstream << "Once upon a time";
    }

    return story_fpath;
}

TEST(check_file_tests, test_check_input_file)
{
    try
    {
        core::check_input_file(create_resource());
        SUCCEED();
    }
    catch (...)
    {
        FAIL();
    }
}

TEST(check_file_tests, test_check_input_file_exception_file_not_found)
{
    try
    {
        core::check_input_file("/__does_not_exist__");
        FAIL();
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

TEST(check_file_tests, test_check_input_file_exception_not_regular_file)
{
    try
    {
        core::check_input_file(std::filesystem::temp_directory_path());
        FAIL();
    }
    catch (const std::filesystem::filesystem_error& err)
    {
        EXPECT_EQ(err.code().message(), "No such file or directory");
        EXPECT_STREQ(err.what(), "filesystem error: Input path is not a regular file: No such file or directory [/tmp]");
        SUCCEED();
    }
    catch (const std::iostream::failure& err)
    {
        FAIL();
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
