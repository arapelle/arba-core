#include <arba/core/sbrm/sb_file_remover.hpp>
#include <fstream>
#include <gtest/gtest.h>

std::filesystem::path resource_dir()
{
    std::filesystem::path root_dpath = std::filesystem::temp_directory_path() / "arba/core" / "sbrm_tests";
    if (std::filesystem::exists(root_dpath))
        std::filesystem::remove_all(root_dpath);
    std::filesystem::create_directories(root_dpath);

    std::filesystem::path rsc_dpath = root_dpath / "rsc";
    std::filesystem::create_directories(rsc_dpath);

    return rsc_dpath;
}

TEST(sb_file_remover, test_sb_file_remover)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path output_fpath = rsc_dir / "output_file.txt";
    std::ofstream().open(output_fpath);

    {
        core::sbrm sentry = core::make_sb_file_remover(output_fpath);
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
}

TEST(sb_file_remover, test_sb_file_remover_rvalue)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path output_fpath = rsc_dir / "output_file.txt";
    std::ofstream().open(output_fpath);

    {
        core::sbrm sentry = core::make_sb_file_remover(std::filesystem::path(output_fpath));
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
}

TEST(sb_file_remover, test_sb_file_remover_ec)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path output_fpath = rsc_dir / "output_file.txt";
    std::ofstream().open(output_fpath);

    std::error_code ec;
    {
        core::sbrm sentry = core::make_sb_file_remover(output_fpath, ec);
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
    ASSERT_EQ(ec.value(), 0);
}

TEST(sb_file_remover, test_sb_file_remover_ec_rvalue)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path output_fpath = rsc_dir / "output_file.txt";
    std::ofstream().open(output_fpath);

    std::error_code ec;
    {
        core::sbrm sentry = core::make_sb_file_remover(std::filesystem::path(output_fpath), ec);
        ASSERT_TRUE(std::filesystem::exists(output_fpath));
    }
    ASSERT_FALSE(std::filesystem::exists(output_fpath));
    ASSERT_EQ(ec.value(), 0);
}

TEST(sb_file_remover, test_sb_all_files_remover)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path jpg_dpath = rsc_dir / "jpg/";
    std::filesystem::path png_dpath = rsc_dir / "png/";
    std::filesystem::path output_fpath = png_dpath / "output_file.txt";
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

TEST(sb_file_remover, test_sb_all_files_remover_rvalue)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path jpg_dpath = rsc_dir / "jpg/";
    std::filesystem::path png_dpath = rsc_dir / "png/";
    std::filesystem::path output_fpath = png_dpath / "output_file.txt";
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

TEST(sb_file_remover, test_sb_all_files_remover_ec)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path jpg_dpath = rsc_dir / "jpg/";
    std::filesystem::path png_dpath = rsc_dir / "png/";
    std::filesystem::path output_fpath = png_dpath / "output_file.txt";
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

TEST(sb_file_remover, test_sb_all_files_remover_ec_rvalue)
{
    std::filesystem::path rsc_dir = resource_dir();
    std::filesystem::path jpg_dpath = rsc_dir / "jpg/";
    std::filesystem::path png_dpath = rsc_dir / "png/";
    std::filesystem::path output_fpath = png_dpath / "output_file.txt";
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
