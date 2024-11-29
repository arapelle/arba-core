#include <gtest/gtest.h>

// class to test
#include <arba/core/plugin.hpp>

#include <concat_interface/concat_interface.hpp>
#include <format>

std::filesystem::path plugin_dir = PLUGIN_DIR;

// Constructors

TEST(UnsafePluginTest, ConstructorEmpty_NominalCase_ExpectNoException)
{
    try
    {
        core::unsafe_plugin plugin;
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(UnsafePluginTest, Constructor_ExistingLibrary_ExpectNoException)
{
    try
    {
        core::unsafe_plugin plugin(plugin_dir / "libconcat");
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(UnsafePluginTest, Constructor_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "concat/libunfound";

    try
    {
        core::unsafe_plugin plugin(std::filesystem::current_path() / "concat/libunfound");
        FAIL();
    }
    catch(const core::plugin_load_error& exception)
    {
        std::string expected_msg(std::format("Exception occurred while loading plugin: {}", lib_path.generic_string()));
        std::string err_msg(exception.what());
        ASSERT_EQ(err_msg.find(expected_msg), 0);
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// Load

TEST(UnsafePluginTest, Load_ExistingLibraryWithExtension_ExpectNoException)
{
    try
    {
        core::unsafe_plugin plugin;
        plugin.load(plugin_dir / std::format("libconcat{}", core::plugin_file_extension));
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(UnsafePluginTest, Load_ExistingLibraryNoExtension_ExpectNoException)
{
    try
    {
        core::unsafe_plugin plugin;
        plugin.load(plugin_dir / "libconcat");
        ASSERT_TRUE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

TEST(UnsafePluginTest, Load_UnfoundLibrary_ExpectException)
{
    std::filesystem::path lib_path = std::filesystem::current_path() / "concat/libunfound";
    try
    {
        core::unsafe_plugin plugin;
        plugin.load(lib_path);
        FAIL();
    }
    catch (const core::plugin_load_error& exception)
    {
        constexpr std::string_view expected_msg_fmt = "Exception occurred while loading plugin: {}";
        std::string expected_msg(std::format(expected_msg_fmt, lib_path.generic_string()));
        ASSERT_EQ(std::string(exception.what()).find(expected_msg), 0);
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// Unload

TEST(UnsafePluginTest, Unload_NomicalCase_ExpectNoException)
{
    try
    {
        core::unsafe_plugin plugin(plugin_dir / "libconcat");
        ASSERT_TRUE(plugin.is_loaded());
        plugin.unload();
        ASSERT_FALSE(plugin.is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// FindFunctionPtr

TEST(UnsafePluginTest, FindFunctionPtr_FunctionName_ReturnNotNullFunctionPtr)
{
    std::string res;
    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    auto execute = plugin.find_function_ptr<void(*)(std::string&, std::string_view, const std::string&)>("execute");
    ASSERT_NE(execute, nullptr);
    execute(res, "a", "b");
    ASSERT_EQ(res, "a-b");
}

TEST(UnsafePluginTest, FindFunctionPtr_FunctionName_ExpectException)
{
    std::string_view function_name("notFoundFunction");

    try
    {
        core::unsafe_plugin plugin(plugin_dir / "libconcat");
        plugin.find_function_ptr<void(*)(int&)>(function_name);
    }
    catch(const core::plugin_find_symbol_error& exception)
    {
        std::string msg(exception.what());
        ASSERT_EQ(msg.find("Exception occurred while looking for address of"), 0);
        ASSERT_NE(msg.find(function_name), std::string::npos);
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// MakeUniqueInstance

TEST(UnsafePluginTest, MakeUniqueInstance_FunctionExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance");

    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    std::unique_ptr<ConcatInterface> instance = plugin.make_unique_instance<ConcatInterface>(function_name);
    ASSERT_EQ(instance->concat("a", "b"), "a-b");
}

TEST(UnsafePluginTest, MakeUniqueInstance_FunctionTakingArgsExists_ReturnUniquePtr)
{
    std::string_view function_name("make_unique_instance_from_args");

    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    std::unique_ptr<ConcatInterface> instance;

    std::string a = "(";
    std::string b = "(";
    std::string z = "))";

    ASSERT_EQ(b, "(");
    instance = plugin.make_unique_instance<ConcatInterface,
                                           std::string_view, std::string&, const std::string&>(function_name, a, b, z);
    std::string str = instance->concat("aa", "bb");
    ASSERT_EQ(str, "((aa-bb))");
    ASSERT_EQ(b, "((");
}

// MakeSharedInstance

TEST(UnsafePluginTest, MakeSharedInstance_FunctionExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance");

    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    std::shared_ptr<ConcatInterface> instance = plugin.make_shared_instance<ConcatInterface>(function_name);
    ASSERT_EQ(instance->concat("a", "b"), "a-b");
}

TEST(UnsafePluginTest, MakeSharedInstance_FunctionTakingArgsExists_ReturnSharedPtr)
{
    std::string_view function_name("make_shared_instance_from_args");

    std::string a = "(";
    std::string b = "(";
    std::string z = "))";

    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    ASSERT_EQ(b, "(");
    std::shared_ptr<ConcatInterface> instance =
        plugin.make_shared_instance<ConcatInterface,
                                    std::string_view, std::string&, const std::string&>(function_name, a, b, z);
    std::string str = instance->concat("aa", "bb");
    ASSERT_EQ(str, "((aa-bb))");
    ASSERT_EQ(b, "((");
}

// InstanceRef & InstanceCref

TEST(UnsafePluginTest, InstanceRef_FunctionExists_ReturnTypeRef)
{
    std::string_view function_name("default_concat");

    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    ConcatInterface& instance = plugin.instance_ref<ConcatInterface>(function_name);
    ASSERT_EQ(instance.concat("a", "b"), "a-b");
}

TEST(UnsafePluginTest, InstanceCref_FunctionExists_ReturnTypeConstRef)
{
    std::string_view function_name("default_const_concat");

    core::unsafe_plugin plugin(plugin_dir / "libconcat");
    const ConcatInterface& instance = plugin.instance_cref<ConcatInterface>(function_name);
    ASSERT_EQ(instance.concat("a", "b"), "a-b");
}

// Move Constructor

TEST(UnsafePluginTest, MoveConstructor_ExistingLibrary_ExpectNoException)
{
    try
    {
        std::unique_ptr plugin_uptr = std::make_unique<core::unsafe_plugin>(plugin_dir / "libconcat");
        ASSERT_TRUE(plugin_uptr->is_loaded());
        core::unsafe_plugin other_plugin(std::move(*plugin_uptr));
        ASSERT_TRUE(other_plugin.is_loaded());
        ASSERT_FALSE(plugin_uptr->is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}

// Move Assignment

TEST(UnsafePluginTest, MoveAssignment_ExistingLibrary_ExpectNoException)
{
    try
    {
        std::unique_ptr plugin_uptr = std::make_unique<core::unsafe_plugin>(plugin_dir / "libconcat");
        ASSERT_TRUE(plugin_uptr->is_loaded());
        core::unsafe_plugin other_plugin;
        other_plugin = std::move(*plugin_uptr);
        ASSERT_TRUE(other_plugin.is_loaded());
        ASSERT_FALSE(plugin_uptr->is_loaded());
    }
    catch (const std::exception& exception)
    {
        FAIL() << exception.what();
    }
}