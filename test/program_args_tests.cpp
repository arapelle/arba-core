#include <arba/core/program_args.hpp>

#include <gtest/gtest.h>

using namespace std::string_literals;

std::array s_args = { "/root/dir/program_name.v2.run"s, "6"s, "-c"s, "Debug"s };
std::array cs_args = { s_args[0].data(), s_args[1].data(), s_args[2].data(), s_args[3].data() };
int argc = cs_args.size();
char** argv = cs_args.data();

TEST(program_args_tests, test_constructor_empty)
{
    core::program_args args;
    ASSERT_EQ(args.argc, 0);
    ASSERT_EQ(args.argv, nullptr);
    ASSERT_TRUE(args.empty());
}

TEST(program_args_tests, test_constructor)
{
    core::program_args args(argc, argv);
    ASSERT_EQ(args.argc, argc);
    ASSERT_EQ(args.argv, argv);
    ASSERT_FALSE(args.empty());
}

TEST(program_args_tests, test_size)
{
    core::program_args args(argc, argv);
    ASSERT_EQ(args.size(), argc);
}

TEST(program_args_tests, test_begin)
{
    core::program_args args(argc, argv);

    ASSERT_EQ(args.begin(), core::program_args::iterator(argv));
    ASSERT_EQ(args.begin() + 2, core::program_args::iterator(argv + 2));
    auto iter = args.begin();
    ASSERT_EQ(++iter, core::program_args::iterator(argv + 1));
    ASSERT_EQ(iter++, core::program_args::iterator(argv + 1));
    ASSERT_EQ(iter, core::program_args::iterator(argv + 2));
    ASSERT_EQ(--iter, core::program_args::iterator(argv + 1));
    ASSERT_EQ(iter--, core::program_args::iterator(argv + 1));
    ASSERT_EQ(iter, core::program_args::iterator(argv));

    ASSERT_EQ(args.cbegin(), core::program_args::const_iterator(argv));
    ASSERT_EQ(args.cbegin() + 2, core::program_args::const_iterator(argv + 2));
    auto citer = args.begin();
    ASSERT_EQ(++citer, core::program_args::const_iterator(argv + 1));
    ASSERT_EQ(citer++, core::program_args::const_iterator(argv + 1));
    ASSERT_EQ(citer, core::program_args::const_iterator(argv + 2));
    ASSERT_EQ(--citer, core::program_args::iterator(argv + 1));
    ASSERT_EQ(citer--, core::program_args::iterator(argv + 1));
    ASSERT_EQ(citer, core::program_args::iterator(argv));
}

TEST(program_args_tests, test_end)
{
    core::program_args args(argc, argv);
    ASSERT_EQ(args.end(), core::program_args::iterator(argv + 4));
    ASSERT_EQ(args.end() - 2, core::program_args::iterator(argv + 2));
    ASSERT_EQ(args.cend(), core::program_args::const_iterator(argv + 4));
    ASSERT_EQ(args.cend() - 2, core::program_args::const_iterator(argv + 2));
}

TEST(program_args_tests, test_operator_nth)
{
    core::program_args args(argc, argv);
    ASSERT_EQ(args.size(), 4);
    ASSERT_EQ(args[0], "/root/dir/program_name.v2.run");
    ASSERT_EQ(args[1], "6");
    ASSERT_EQ(args[2], "-c");
    ASSERT_EQ(args[3], "Debug");
}

TEST(program_args_tests, test_program_features)
{
    core::program_args args(argc, argv);
    ASSERT_GT(args.size(), 0);
    ASSERT_EQ(args.program_path(), std::filesystem::path("/root/dir/program_name.v2.run"));
    ASSERT_EQ(args.program_dir(), std::filesystem::path("/root/dir"));
    ASSERT_EQ(args.program_name(), std::filesystem::path("program_name.v2.run"));
    ASSERT_EQ(args.program_stem(), std::filesystem::path("program_name.v2"));
}
