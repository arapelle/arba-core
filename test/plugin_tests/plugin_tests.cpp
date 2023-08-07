#include <gtest/gtest.h>
#include <arba/core/plugin.hpp>
#include <rng_interface.hpp>
#include <format>

TEST(PluginTest, Constructor_ExistingLibraryNoExtension_ExpectNoException)
{
    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        SUCCEED();
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, Constructor_ExistingLibraryWithExtension_ExpectNoException)
{
    try
    {
        constexpr std::string_view ext =
#ifdef WIN32
            "dll";
#else
            "so";
#endif
        core::plugin plugin(std::filesystem::current_path() / "rng" / std::format("librng.{}", ext));
        SUCCEED();
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, Constructor_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "rng/libunfound";
    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng/libunfound");
        FAIL();
    }
    catch(std::runtime_error exception)
    {
        constexpr std::string_view expected_msg_fmt =
#ifdef WIN32
            "Exception occurred while loading {}";
#else
            "Exception occurred while loading plugin: {}";
#endif
        std::string expected_msg(std::format(expected_msg_fmt, lib_path.generic_string()));
        ASSERT_EQ(std::string(exception.what()).find(expected_msg), 0);
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, FindFunctionPtr_FunctionName_ExpectNoException)
{
    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        auto execute = plugin.find_function_ptr<void(*)(int&)>("execute");
        ASSERT_NE(execute, nullptr);
        int value = -1;
        execute(value);
        ASSERT_GE(value, 0);
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, FindFunctionPtr_FunctionName_ExpectException)
{
    std::string_view function_name("not_found_function");

    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        plugin.find_function_ptr<void(*)(int&)>(function_name);
    }
    catch(std::runtime_error exception)
    {
        constexpr std::string_view expected_msg_fmt =
#ifdef WIN32
            "Exception occurred while looking for address of {}";
#else
            "Exception occurred while looking for address of symbol: {}";
#endif
        std::string expected_msg(std::format("", function_name));
        ASSERT_EQ(std::string(exception.what()).find(expected_msg), 0);
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, MakeInstance_FunctionExists_ReturnUniquePtr)
{
    std::string_view function_name("make_instance");

    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        std::unique_ptr<GeneratorInterface> generator = plugin.make_instance<GeneratorInterface>(function_name);
        std::cout << generator->generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, MakeSharedInstance_FunctionExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance");

    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        std::shared_ptr<GeneratorInterface> generator = plugin.make_shared_instance<GeneratorInterface>(function_name);
        std::cout << generator->generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, InstanceRef_FunctionExists_ReturnTypeRef)
{
    std::string_view function_name("default_generator");

    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        GeneratorInterface& generator = plugin.instance_ref<GeneratorInterface>(function_name);
        std::cout << generator.generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        FAIL();
    }
}

TEST(PluginTest, InstanceCref_FunctionExists_ReturnTypeConstRef)
{
    std::string_view function_name("default_const_generator");

    try
    {
        core::plugin plugin(std::filesystem::current_path() / "rng" / "librng");
        const GeneratorInterface& generator = plugin.instance_cref<GeneratorInterface>(function_name);
        std::cout << generator.generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        FAIL();
    }
}