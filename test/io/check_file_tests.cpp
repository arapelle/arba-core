#include <arba/core/io/check_file.hpp>
#include <chrono>
#include <cstdlib>
#include <format>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

std::filesystem::path create_resource()
{
    std::filesystem::path root_dpath = std::filesystem::temp_directory_path() / "arba/core" / "check_file_tests";
    if (std::filesystem::exists(root_dpath))
        std::filesystem::remove_all(root_dpath);
    std::filesystem::create_directories(root_dpath);

    std::filesystem::path rsc_dpath = root_dpath / "rsc";
    std::filesystem::create_directories(rsc_dpath);

    std::filesystem::path story_fpath = rsc_dpath / "check_file_1.2.3.txt";
    std::ofstream rsc_fstream(story_fpath, std::ios::trunc);
    rsc_fstream << std::chrono::system_clock::now() << std::endl;

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
        throw;
    }
}

#if linux || __APPLE__
TEST(check_file_tests, test_check_input_file__symlink__ok)
{
    std::filesystem::path rsc_file = create_resource();
    try
    {
        std::filesystem::path rsc_slpath = rsc_file.parent_path() / "check_file.txt";
        std::filesystem::create_symlink(rsc_file, rsc_slpath);
        core::check_input_file(rsc_slpath);
        SUCCEED();
    }
    catch (...)
    {
        throw;
    }
}
#endif

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
        throw;
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
        std::string_view err_message(err.what());
        std::string expected_err_string =
            std::format("filesystem error: Input path is not a regular file: No such file or directory [{}]",
                        std::filesystem::temp_directory_path().string());
        EXPECT_EQ(err_message, expected_err_string);
        SUCCEED();
    }
    catch (const std::iostream::failure& err)
    {
        throw;
    }
}
