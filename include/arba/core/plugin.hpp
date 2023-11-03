#pragma once

#include <type_traits>
#include <memory>
#include <filesystem>
#ifdef WIN32
#include <system_error>
#else
#include <stdexcept>
#endif

inline namespace arba
{
namespace core
{

class plugin_error : public
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

/**
 * @brief The plugin class
 */
class plugin
{
public:
    static constexpr std::string_view file_extension =
#ifdef WIN32
        ".dll";
#else
        ".so";
#endif

    inline plugin() {}

    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::runtime_error If the file does not exist or if there is a problem during loading.
     */
    explicit plugin(const std::filesystem::path& plugin_path);

    plugin(plugin&&) = default;
    plugin& operator=(plugin&&) = default;

    /**
     * @brief ~Plugin destructor which unloads the plugin.
     * @warning If any error occurs while unloading, an error message is written
     * on the error stream and the program exit with the error code.
     */
    ~plugin();

    /**
     * @brief load_from_file Load the plugin present at a given path.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::runtime_error If the file does not exist or if there is a problem during loading.
     * @warning If a plugin is already loaded by this instance, the behavior is undefined.
     */
    void load_from_file(const std::filesystem::path& plugin_path);

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

    /**
     * @brief find_function_ptr Find the address of the function with a given name.
     * @tparam PointerType Signature of the search function. (i.e. void(*)(int))
     * @param function_name The name of the searched function.
     * @return A function pointer to the found function symbol in the plugin.
     * @throw std::runtime_error If the symbol is not found or if a plugin is not loaded by this instance.
     * @warning There is no guarantee that the function has the wanted signature.
     */
    template <typename PointerType>
        requires std::is_pointer_v<PointerType>
                 && std::is_function_v<std::remove_cvref_t<decltype(*std::declval<PointerType>)>>
    auto find_function_ptr(const std::string_view function_name)
    {
        return reinterpret_cast<PointerType>(this->find_symbol_pointer_(std::string(function_name)));
    }

    static constexpr std::string_view default_instance_ref_func_name = "instance_ref";

    /**
     * @brief instance_ref Find a function returning a reference to a global variable and call it to return the reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin. (default is "instance_ref")
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    InstanceType& instance_ref(const std::string_view getter_function_name =
                               default_instance_ref_func_name)
    {
        using MainObjectGetter = InstanceType&(*)();
        MainObjectGetter getter = this->find_function_ptr<MainObjectGetter>(getter_function_name);
        return getter();
    }

    static constexpr std::string_view default_instance_cref_func_name = "instance_cref";

    /**
     * @brief instance_cref Find a function returning a const reference to a global variable and call it to return the const reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin. (default is "instance_cref")
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be const InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    const InstanceType& instance_cref(const std::string_view getter_function_name =
                                      default_instance_cref_func_name)
    {
        using MainObjectMaker = const InstanceType&(*)();
        MainObjectMaker getter = this->find_function_ptr<MainObjectMaker>(getter_function_name);
        return getter();
    }

    static constexpr std::string_view default_make_unique_func_name = "make_unique_instance";

    /**
     * @brief make_unique_instance Find a function making a new instance stored in a std::unique_ptr and call it to return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam DeleterType The type of the deleter functor used by std::unique_ptr. (default is std::unique_ptr<ClassType>::deleter_type>)
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_instance")
     * @return A std::unique_ptr<ClassType[, DeleterType]> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType[, DeleterType]>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType, typename DeleterType = typename std::unique_ptr<ClassType>::deleter_type>
        requires std::has_virtual_destructor_v<ClassType>
    std::unique_ptr<ClassType, DeleterType> make_unique_instance(const std::string_view maker_function_name =
                                                              default_make_unique_func_name)
    {
        using InstanceMaker = std::unique_ptr<ClassType, DeleterType>(*)();
        InstanceMaker maker = this->find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

    /**
     * @brief make_unique_instance Find a function making a new instance stored in a std::unique_ptr and call it to return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam DeleterType The type of the deleter functor used by std::unique_ptr. (default is std::unique_ptr<ClassType>::deleter_type>)
     * @tparam ArgsT... The types of the arguments to pass to the maker function.
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_instance")
     * @return A std::unique_ptr<ClassType[, DeleterType]> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType[, DeleterType]>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType, typename DeleterType = typename std::unique_ptr<ClassType>::deleter_type, typename... ArgsT>
        requires std::has_virtual_destructor_v<ClassType> && (sizeof...(ArgsT) > 0)
    std::unique_ptr<ClassType, DeleterType> make_unique_instance(const std::string_view maker_function_name, ArgsT&&... args)
    {
        using InstanceMaker = std::unique_ptr<ClassType, DeleterType>(*)(ArgsT&&...);
        InstanceMaker maker = this->find_function_ptr<InstanceMaker>(maker_function_name);
        return maker(std::forward<ArgsT>(args)...);
    }

    static constexpr std::string_view default_make_shared_func_name = "make_shared_instance";

    /**
     * @brief make_shared_instance Find a function making a new instance stored in a std::shared_ptr and call it to return the std::shared_ptr.
     * @tparam ClassType The type of the made instance.
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_shared_instance")
     * @return A std::shared_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::shared_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType>
        requires std::has_virtual_destructor_v<ClassType>
    std::shared_ptr<ClassType> make_shared_instance(const std::string_view maker_function_name =
                                                    default_make_shared_func_name)
    {
        using InstanceMaker = std::shared_ptr<ClassType>(*)();
        InstanceMaker maker = this->find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

    /**
     * @brief make_shared_instance Find a function making a new instance stored in a std::shared_ptr and call it to return the std::shared_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam ArgsT... The types of the arguments to pass to the maker function.
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_shared_instance")
     * @return A std::shared_ptr<ClassType> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::shared_ptr<ClassType>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType, typename... ArgsT>
        requires std::has_virtual_destructor_v<ClassType> && (sizeof...(ArgsT) > 0)
    std::shared_ptr<ClassType> make_shared_instance(const std::string_view maker_function_name, ArgsT&&... args)
    {
        using InstanceMaker = std::shared_ptr<ClassType>(*)(ArgsT&&...);
        InstanceMaker maker = this->find_function_ptr<InstanceMaker>(maker_function_name);
        return maker(std::forward<ArgsT>(args)...);
    }

private:
    plugin(const plugin&) = delete;
    plugin& operator=(const plugin&) = delete;
    void* find_symbol_pointer_(const std::string& symbol_name);

private:
    void* handle_ = nullptr;
};

}
}
