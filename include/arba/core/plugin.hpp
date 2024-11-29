#pragma once

#include <filesystem>
#include <memory>
#include <type_traits>
#include <any>
#include <format>
#include <unordered_map>
#ifdef WIN32
#include <system_error>
#else
#include <stdexcept>
#endif


inline namespace arba
{
namespace core
{

class plugin_load_error : public
#ifdef WIN32
                     std::system_error
{
    using std::system_error::system_error;
#else
                     std::runtime_error
{
    using std::runtime_error::runtime_error;
#endif
};

class plugin_unload_error : public
#ifdef WIN32
                          std::system_error
{
    using std::system_error::system_error;
#else
                          std::runtime_error
{
    using std::runtime_error::runtime_error;
#endif
};

class plugin_find_symbol_error : public
#ifdef WIN32
                            std::system_error
{
    using std::system_error::system_error;
#else
                            std::runtime_error
{
    using std::runtime_error::runtime_error;
#endif
};

constexpr std::string_view plugin_file_extension =
#ifdef WIN32
    ".dll";
#else
    ".so";
#endif

/**
 * @brief The plugin_base class
 */
class plugin_base
{
protected:
    inline plugin_base() {}

    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::runtime_error If the file does not exist or if there is a problem during loading.
     */
    explicit plugin_base(const std::filesystem::path& plugin_path);

public:
    /**
     * @brief ~Plugin destructor which unloads the plugin.
     * @warning If any error occurs while unloading, an error message is written
     * on the error stream and the program exit with the error code.
     */
    ~plugin_base();

    /**
     * @brief plugin_base Move constructor.
     * @param other A r-value plugin instance.
     * The embedded handle of the other instance is set to nullptr.
     */
    plugin_base(plugin_base&& other);

    /**
     * @brief operator = Move assignment.
     * @param other A r-value plugin instance.
     * @return A reference to the current instance.
     * The embedded handle of the other instance is set to nullptr.
     */
    plugin_base& operator=(plugin_base&& other);

    /**
     * @brief load Load the plugin present at a given path.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::runtime_error If the file does not exist or if there is a problem during loading.
     * @warning If a plugin is already loaded by this instance, the behavior is undefined.
     */
    void load(const std::filesystem::path& plugin_path);

    /**
     * @brief unload Unload the plugin.
     * @warning If no plugin is loaded by this instance, the behavior is undefined.
     */
    void unload();

    /**
     * @brief is_loaded Indicate is this plugin is loaded or not.
     * @return true If a loaded plugin is held by this instance.
     */
    [[nodiscard]] inline bool is_loaded() const noexcept { return handle_; }

protected:
    void* find_symbol_pointer(const std::string& symbol_name);

private:
    plugin_base(const plugin_base&) = delete;
    plugin_base& operator=(const plugin_base&) = delete;

protected:
    void* handle_ = nullptr;
};

/**
 * @brief The plugin_impl class
 */
template <class PluginType>
class plugin_impl : public plugin_base
{
protected:
    inline plugin_impl() {}

    explicit plugin_impl(const std::filesystem::path& plugin_path) : plugin_base(plugin_path) {}

public:
    plugin_impl(plugin_impl&&) = default;
    plugin_impl& operator=(plugin_impl&&) = default;

    static constexpr std::string_view default_instance_ref_func_name = "instance_ref";

    /**
     * @brief instance_ref Find a function returning a reference to a global variable and call it to return the
     * reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin.
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    InstanceType& instance_ref(const std::string_view getter_function_name = default_instance_ref_func_name)
    {
        using MainObjectGetter = InstanceType& (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        MainObjectGetter getter = self.template find_function_ptr<MainObjectGetter>(getter_function_name);
        return getter();
    }

    static constexpr std::string_view default_instance_cref_func_name = "instance_cref";

    /**
     * @brief instance_cref Find a function returning a const reference to a global variable and call it to return the
     * const reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin.
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be const InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    const InstanceType& instance_cref(const std::string_view getter_function_name = default_instance_cref_func_name)
    {
        using MainObjectMaker = const InstanceType& (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        MainObjectMaker getter = self.template find_function_ptr<MainObjectMaker>(getter_function_name);
        return getter();
    }

    static constexpr std::string_view default_make_unique_func_name = "make_unique_instance";

    /**
     * @brief make_unique_instance Find a function making a new instance stored in a std::unique_ptr and call it to
     * return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_instance")
     * @return A std::unique_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType>
    requires std::has_virtual_destructor_v<ClassType>
        std::unique_ptr<ClassType>
        make_unique_instance(const std::string_view maker_function_name = default_make_unique_func_name)
    {
        using InstanceMaker = std::unique_ptr<ClassType> (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

    /**
     * @brief make_unique_instance Find a function making a new instance stored in a std::unique_ptr and call it to
     * return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam ArgsT... The types of the arguments to pass to the maker function.
     * @param maker_function_name The name of the maker function to find in the plugin.
     * @param args The arguments to pass to the maker function.
     * @return A std::unique_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     * @warning All args types must be explicitly provided. (make_unique_instance<InstanceType, Parameter1Type>(...))
     */
    template <typename ClassType, typename... ArgsT>
        requires std::has_virtual_destructor_v<ClassType> && (sizeof...(ArgsT) > 0)
        std::unique_ptr<ClassType> make_unique_instance(const std::string_view maker_function_name, ArgsT... args)
    {
        using InstanceMaker = std::unique_ptr<ClassType> (*)(ArgsT...);
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker(args...);
    }

    static constexpr std::string_view default_make_shared_func_name = "make_shared_instance";

    /**
     * @brief make_shared_instance Find a function making a new instance stored in a std::shared_ptr and call it to
     * return the std::shared_ptr.
     * @tparam ClassType The type of the made instance.
     * @param maker_function_name The name of the maker function to find in the plugin.
     * @return A std::shared_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::shared_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType>
    requires std::has_virtual_destructor_v<ClassType>
        std::shared_ptr<ClassType>
        make_shared_instance(const std::string_view maker_function_name = default_make_shared_func_name)
    {
        using InstanceMaker = std::shared_ptr<ClassType> (*)();
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

    /**
     * @brief make_shared_instance Find a function making a new instance stored in a std::shared_ptr and call it to
     * return the std::shared_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam ArgsT... The types of the arguments to pass to the maker function.
     * @param maker_function_name The name of the maker function to find in the plugin.
     * @param args The arguments to pass to the maker function.
     * @return A std::shared_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::shared_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     * @warning All args types must be explicitly provided. (make_shared_instance<InstanceType, Parameter1Type>(...))
     */
    template <typename ClassType, typename... ArgsT>
        requires std::has_virtual_destructor_v<ClassType> && (sizeof...(ArgsT) > 0)
        std::shared_ptr<ClassType> make_shared_instance(const std::string_view maker_function_name, ArgsT... args)
    {
        using InstanceMaker = std::shared_ptr<ClassType> (*)(ArgsT...);
        PluginType& self = static_cast<PluginType&>(*this);
        InstanceMaker maker = self.template find_function_ptr<InstanceMaker>(maker_function_name);
        return maker(args...);
    }
};

/**
 * @brief The unsafe_plugin class
 */
class unsafe_plugin : public plugin_impl<unsafe_plugin>
{
private:
    using base_ = plugin_impl<unsafe_plugin>;

public:
    inline unsafe_plugin() {}

    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     */
    explicit unsafe_plugin(const std::filesystem::path& plugin_path) : base_(plugin_path) {}

    unsafe_plugin(unsafe_plugin&&) = default;
    unsafe_plugin& operator=(unsafe_plugin&&) = default;

    /**
     * @brief find_function_ptr Find the address of the function with a given name.
     * @tparam PointerType Signature of the search function. (i.e. void(*)(int))
     * @param function_name The name of the searched function.
     * @return A function pointer to the found function symbol in the plugin.
     * @throw std::runtime_error If the symbol is not found or if a plugin is not loaded by this instance.
     * @warning There is no guarantee that the function has the wanted signature.
     */
    template <typename FunctionSignatureType>
    requires std::is_pointer_v<FunctionSignatureType>
        && std::is_function_v<std::remove_cvref_t<decltype(*std::declval<FunctionSignatureType>)>>
            FunctionSignatureType find_function_ptr(std::string_view function_name)
    {
        return reinterpret_cast<FunctionSignatureType>(this->find_symbol_pointer(std::string(function_name)));
    }
};

using plugin_function_register_type = std::any(*)(std::string_view);
static constexpr std::string_view plugin_function_register_fname = "arba_core_plugin_function_register_";

/**
 * @brief The plugin class
 */
class plugin : public plugin_impl<plugin>
{
private:
    using base_ = plugin_impl<plugin>;

public:
    inline plugin() {}

    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     */
    explicit plugin(const std::filesystem::path& plugin_path) : base_(plugin_path) {}

    plugin(plugin&&) = default;
    plugin& operator=(plugin&&) = default;

    /**
     * @brief find_function_ptr Find the address of the function with a given name and check the type of the function.
     * @tparam FunctionSignatureType Signature of the searched function. (i.e. void(*)(int))
     * @param function_name The name of the searched function.
     * @return A function pointer to the found function symbol in the plugin.
     * @throw std::runtime_error If the symbol is not found or if a plugin is not loaded by this instance.
     * @throw std::runtime_error If the type of the function is not the expected one.
     * @warning Only functions registered with ARBA_CORE_REGISTER_PLUGIN_FUNCTION() can be found (and checked).
     */
    template <typename FunctionSignatureType>
        requires std::is_pointer_v<FunctionSignatureType>
            && std::is_function_v<std::remove_cvref_t<decltype(*std::declval<FunctionSignatureType>)>>
    FunctionSignatureType find_function_ptr(std::string_view function_name)
    {
        const plugin_function_register_type find_function_ptr_as_any =
            reinterpret_cast<plugin_function_register_type>(this->find_symbol_pointer(std::string(plugin_function_register_fname)));

        std::any any_value = find_function_ptr_as_any(function_name);
        if (any_value.has_value()) [[likely]]
        {
            try
            {
                return std::any_cast<FunctionSignatureType>(any_value);
            }
            catch (const std::bad_any_cast& err)
            {
                throw std::runtime_error(std::format("Function type of '{}' is not the requested type function '{}'.",
                                                     function_name, typeid(FunctionSignatureType).name()));
            }
        }

        std::ignore = this->find_symbol_pointer(std::string(function_name));
        throw std::runtime_error(std::format("Function '{}' exists in plugin, but its type cannot be checked. "
                                             "Did you forget to use ARBA_CORE_REGISTER_PLUGIN_FUNCTION() ?", function_name));
    }
};

}
}

#define ARBA_CORE_BEGIN_PLUGIN_FUNCTION_REGISTER() \
extern "C" std::any arba_core_plugin_function_register_(std::string_view function_name) \
{ \
    static_assert(arba::core::plugin_function_register_fname == __func__); \
    static_assert(std::is_same_v<arba::core::plugin_function_register_type, decltype(&arba_core_plugin_function_register_)>); \
    static const std::unordered_map<std::string_view, std::any> function_register_{

#define ARBA_CORE_REGISTER_PLUGIN_FUNCTION(function_) \
            { #function_, &function_ },

#define ARBA_CORE_END_PLUGIN_FUNCTION_REGISTER() \
        }; \
    const auto iter = function_register_.find(function_name); \
        return iter != function_register_.cend() ? iter->second : std::any(); \
}
