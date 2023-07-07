#include <core/sbrm.hpp>
#include <gtest/gtest.h>
#include <fstream>

TEST(sbrm_tests, test_sbrm)
{
    bool active = true;
    {
        core::sbrm sentry([&active]{ active = false; });
        ASSERT_EQ(active, true);
    }
    ASSERT_EQ(active, false);
}

TEST(sbrm_tests, test_sb_file_remover)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path output_fpath = rsc_dir/"output_file.txt";
    std::filesystem::create_directories(rsc_dir);
    std::ofstream().open(output_fpath);

    {
        core::sbrm sentry = core::make_sb_file_remover(output_fpath);
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
}

TEST(sbrm_tests, test_sb_file_remover_rvalue)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path output_fpath = rsc_dir/"output_file.txt";
    std::filesystem::create_directories(rsc_dir);
    std::ofstream().open(output_fpath);

    {
        core::sbrm sentry = core::make_sb_file_remover(std::filesystem::path(output_fpath));
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
}

TEST(sbrm_tests, test_sb_file_remover_ec)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path output_fpath = rsc_dir/"output_file.txt";
    std::filesystem::create_directories(rsc_dir);
    std::ofstream().open(output_fpath);

    std::error_code ec;
    {
        core::sbrm sentry = core::make_sb_file_remover(output_fpath, ec);
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
    ASSERT_EQ(ec.value(), 0);
}

TEST(sbrm_tests, test_sb_file_remover_ec_rvalue)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path output_fpath = rsc_dir/"output_file.txt";
    std::filesystem::create_directories(rsc_dir);
    std::ofstream().open(output_fpath);

    std::error_code ec;
    {
        core::sbrm sentry = core::make_sb_file_remover(std::filesystem::path(output_fpath), ec);
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
    ASSERT_EQ(ec.value(), 0);
}

TEST(sbrm_tests, test_sb_all_files_remover)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path jpg_dpath = rsc_dir/"jpg/";
    std::filesystem::path png_dpath = rsc_dir/"png/";
    std::filesystem::path output_fpath = png_dpath/"output_file.txt";
    std::filesystem::create_directories(jpg_dpath);
    std::filesystem::create_directories(png_dpath);
    std::ofstream().open(output_fpath);

    {
        core::sbrm sentry = core::make_sb_all_files_remover(rsc_dir);
        ASSERT_TRUE(std::filesystem::exists(rsc_dir));
        ASSERT_TRUE(std::filesystem::exists(jpg_dpath));
        ASSERT_TRUE(std::filesystem::exists(png_dpath));
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(rsc_dir));
    ASSERT_FALSE(std::filesystem::exists(jpg_dpath));
    ASSERT_FALSE(std::filesystem::exists(png_dpath));
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
}

TEST(sbrm_tests, test_sb_all_files_remover_rvalue)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path jpg_dpath = rsc_dir/"jpg/";
    std::filesystem::path png_dpath = rsc_dir/"png/";
    std::filesystem::path output_fpath = png_dpath/"output_file.txt";
    std::filesystem::create_directories(jpg_dpath);
    std::filesystem::create_directories(png_dpath);
    std::ofstream().open(output_fpath);

    {
        core::sbrm sentry = core::make_sb_all_files_remover(std::filesystem::path(rsc_dir));
        ASSERT_TRUE(std::filesystem::exists(rsc_dir));
        ASSERT_TRUE(std::filesystem::exists(jpg_dpath));
        ASSERT_TRUE(std::filesystem::exists(png_dpath));
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(rsc_dir));
    ASSERT_FALSE(std::filesystem::exists(jpg_dpath));
    ASSERT_FALSE(std::filesystem::exists(png_dpath));
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
}

TEST(sbrm_tests, test_sb_all_files_remover_ec)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path jpg_dpath = rsc_dir/"jpg/";
    std::filesystem::path png_dpath = rsc_dir/"png/";
    std::filesystem::path output_fpath = png_dpath/"output_file.txt";
    std::filesystem::create_directories(jpg_dpath);
    std::filesystem::create_directories(png_dpath);
    std::ofstream().open(output_fpath);

    std::error_code ec;
    {
        core::sbrm sentry = core::make_sb_all_files_remover(rsc_dir, ec);
        ASSERT_TRUE(std::filesystem::exists(rsc_dir));
        ASSERT_TRUE(std::filesystem::exists(jpg_dpath));
        ASSERT_TRUE(std::filesystem::exists(png_dpath));
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(rsc_dir));
    ASSERT_FALSE(std::filesystem::exists(jpg_dpath));
    ASSERT_FALSE(std::filesystem::exists(png_dpath));
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
    ASSERT_EQ(ec.value(), 0);
}

TEST(sbrm_tests, test_sb_all_files_remover_ec_rvalue)
{
    std::filesystem::path rsc_dir = std::filesystem::temp_directory_path()/"arba/core/rsc/";
    std::filesystem::path jpg_dpath = rsc_dir/"jpg/";
    std::filesystem::path png_dpath = rsc_dir/"png/";
    std::filesystem::path output_fpath = png_dpath/"output_file.txt";
    std::filesystem::create_directories(jpg_dpath);
    std::filesystem::create_directories(png_dpath);
    std::ofstream().open(output_fpath);

    std::error_code ec;
    {
        core::sbrm sentry = core::make_sb_all_files_remover(std::filesystem::path(rsc_dir), ec);
        ASSERT_TRUE(std::filesystem::exists(rsc_dir));
        ASSERT_TRUE(std::filesystem::exists(jpg_dpath));
        ASSERT_TRUE(std::filesystem::exists(png_dpath));
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(rsc_dir));
    ASSERT_FALSE(std::filesystem::exists(jpg_dpath));
    ASSERT_FALSE(std::filesystem::exists(png_dpath));
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
    ASSERT_EQ(ec.value(), 0);
}
