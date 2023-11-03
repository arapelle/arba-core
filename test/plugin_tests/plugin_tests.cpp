#include <gtest/gtest.h>
#include <arba/core/plugin.hpp>
#include <rng_interface.hpp>
#include <format>
#include <regex>

std::filesystem::path plugin_dir = PLUGIN_DIR;

TEST(PluginTest, CheckDefaultFuncNames_Eq_Ok)
{
    ASSERT_EQ(core::plugin::default_instance_ref_func_name, "instance_ref");
    ASSERT_EQ(core::plugin::default_instance_cref_func_name, "instance_cref");
    ASSERT_EQ(core::plugin::default_make_unique_func_name, "make_unique_instance");
    ASSERT_EQ(core::plugin::default_make_shared_func_name, "make_shared_instance");
}

TEST(PluginTest, ConstructorEmpty_NominalCase_ExpectNoException)
{
    try
    {
        core::plugin plugin;
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, PluginFileExtension_NoArg_ExpectNoException)
{
    constexpr std::string_view ext =
#ifdef WIN32
        ".dll";
#else
        ".so";
#endif
    ASSERT_EQ(core::plugin::file_extension, ext);
}

TEST(PluginTest, LoadFromFile_ExistingLibraryWithExtension_ExpectNoException)
{
    try
    {
        core::plugin plugin;
        plugin.load_from_file(plugin_dir / std::format("librng{}", core::plugin::file_extension));
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, LoadFromFile_ExistingLibraryNoExtension_ExpectNoException)
{
    try
    {
        core::plugin plugin;
        plugin.load_from_file(plugin_dir / "librng");
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, LoadFromFile_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "rng/libunfound";
    try
    {
        core::plugin plugin;
        plugin.load_from_file(lib_path);
        FAIL();
    }
    catch(const core::plugin_error& exception)
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
        throw;
    }
}

TEST(PluginTest, Constructor_ExistingLibraryNoExtension_ExpectNoException)
{
    try
    {
        core::plugin plugin(plugin_dir / "librng");
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, Constructor_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "rng/libunfound";
    try
    {
        core::plugin plugin(lib_path);
        FAIL();
    }
    catch(const core::plugin_error& exception)
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
        throw;
    }
}

TEST(PluginTest, Unload_NomicalCase_ExpectNoException)
{
    try
    {
        core::plugin plugin(plugin_dir / "librng");
        ASSERT_TRUE(plugin.is_loaded());
        plugin.unload();
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, FindFunctionPtr_FunctionName_ExpectNoException)
{
    try
    {
        core::plugin plugin(plugin_dir / "librng");
        auto execute = plugin.find_function_ptr<void(*)(int&)>("execute");
        ASSERT_NE(execute, nullptr);
        int value = -1;
        execute(value);
        ASSERT_GE(value, 0);
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, FindFunctionPtr_FunctionName_ExpectException)
{
    std::filesystem::path lib_path = plugin_dir / "librng";
    std::string_view function_name("not_found_function");

    try
    {
        core::plugin plugin(lib_path);
        plugin.find_function_ptr<void(*)(int&)>(function_name);
    }
    catch(const core::plugin_error& exception)
    {
        constexpr std::string_view expected_msg_fmt =
#ifdef WIN32
            "Exception occurred while looking for address of {1}:.*";
#else
            "Exception occurred while looking for address of symbol: {}.so: .*: {}";
#endif
        std::string exception_what(exception.what());
        std::string expected_msg(std::format(expected_msg_fmt, lib_path.generic_string(), function_name));
        ASSERT_TRUE(std::regex_match(exception_what, std::regex(expected_msg)));
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, MakeInstance_FunctionExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance");

    try
    {
        core::plugin plugin(plugin_dir / "librng");
        std::unique_ptr<GeneratorInterface> generator = plugin.make_unique_instance<GeneratorInterface>(function_name);
        std::cout << generator->generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, MakeInstance_FunctionTakingArgsExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance_from_args");

    try
    {
        core::plugin plugin(plugin_dir / "librng");
        std::unique_ptr<GeneratorInterface> generator;

        generator = plugin.make_unique_instance<GeneratorInterface>(function_name, 100, 200);
        std::cout << generator->generate() << std::endl;

        using generator_deleter_type = std::unique_ptr<GeneratorInterface>::deleter_type;
        generator = plugin.make_unique_instance<GeneratorInterface, generator_deleter_type>(function_name, 100, 200);
        std::cout << generator->generate() << std::endl;

        SUCCEED();
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, MakeSharedInstance_FunctionExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance");

    try
    {
        core::plugin plugin(plugin_dir / "librng");
        std::shared_ptr<GeneratorInterface> generator = plugin.make_shared_instance<GeneratorInterface>(function_name);
        std::cout << generator->generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, MakeSharedInstance_FunctionTakingArgsExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance_from_args");

    try
    {
        core::plugin plugin(plugin_dir / "librng");
        std::shared_ptr<GeneratorInterface> generator = plugin.make_shared_instance<GeneratorInterface>(function_name, 100, 200);
        std::cout << generator->generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, InstanceRef_FunctionExists_ReturnTypeRef)
{
    std::string_view function_name("default_generator");

    try
    {
        core::plugin plugin(plugin_dir / "librng");
        GeneratorInterface& generator = plugin.instance_ref<GeneratorInterface>(function_name);
        std::cout << generator.generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        throw;
    }
}

TEST(PluginTest, InstanceCref_FunctionExists_ReturnTypeConstRef)
{
    std::string_view function_name("default_const_generator");

    try
    {
        core::plugin plugin(plugin_dir / "librng");
        const GeneratorInterface& generator = plugin.instance_cref<GeneratorInterface>(function_name);
        std::cout << generator.generate() << std::endl;
        SUCCEED();
    }
    catch(...)
    {
        throw;
    }
}
