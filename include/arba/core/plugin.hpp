#pragma once

#include <type_traits>
#include <memory>
#include <filesystem>

inline namespace arba
{
namespace core
{

/**
 * @brief The plugin class
 */
class plugin
{
public:
    /**
     * @brief Plugin constructor which takes the path to the plugin to load.
     * @param plugin_path The path to the plugin to load (extension of the file is optional).
     * @throw std::system_error If the file does not exist or if there is a problem during loading.
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
     * @brief find_function_ptr Find the address of the function with a given name.
     * @tparam PointerType Signature of the search function. (i.e. void(*)(int))
     * @param function_name The name of the searched function.
     * @return A function pointer to the found function symbol in the plugin.
     * @throw std::system_error If the symbol is not found.
     * @warning There is no guarantee that the function has the wanted signature.
     */
    template <typename PointerType>
        requires std::is_pointer_v<PointerType>
                 && std::is_function_v<std::remove_cvref_t<decltype(*std::declval<PointerType>)>>
    auto find_function_ptr(const std::string_view function_name)
    {
        return reinterpret_cast<PointerType>(this->find_symbol_pointer_(std::string(function_name)));
    }

    /**
     * @brief instance_ref Find a function returning a reference to a global variable and call it to return the reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin. (default is "instance_ref")
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    InstanceType& instance_ref(const std::string_view getter_function_name = "instance_ref")
    {
        using MainObjectGetter = InstanceType&(*)();
        MainObjectGetter getter = this->find_function_ptr<MainObjectGetter>(getter_function_name);
        return getter();
    }

    /**
     * @brief instance_cref Find a function returning a const reference to a global variable and call it to return the const reference.
     * @tparam InstanceType The type of the global variable.
     * @param getter_function_name The name of the global variable getter function to find in the plugin. (default is "instance_cref")
     * @return The reference to the global instance defined in the plugin.
     * @details The signature of the global variable getter function is expected to be const InstanceType&(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename InstanceType>
    const InstanceType& instance_cref(const std::string_view getter_function_name = "instance_cref")
    {
        using MainObjectMaker = const InstanceType&(*)();
        MainObjectMaker getter = this->find_function_ptr<MainObjectMaker>(getter_function_name);
        return getter();
    }

    /**
     * @brief make_instance Find a function making a new instance stored in a std::unique_ptr and call it to return the std::unique_ptr.
     * @tparam ClassType The type of the made instance.
     * @tparam DeleterType The type of the deleter functor used by std::unique_ptr. (default is std::unique_ptr<ClassType>::deleter_type>)
     * @param maker_function_name The name of the maker function to find in the plugin. (default is "make_instance")
     * @return A std::unique_ptr<ClassType[, DeleterType]> holding the pointer to the made instance.
     * @details The signature of the maker function is expected to be std::unique_ptr<ClassType[, DeleterType]>(*)().
     * @warning There is no guarantee that the global variable getter function returns the wanted type.
     */
    template <typename ClassType, typename DeleterType = typename std::unique_ptr<ClassType>::deleter_type>
        requires std::has_virtual_destructor_v<ClassType>
    std::unique_ptr<ClassType> make_instance(const std::string_view maker_function_name = "make_instance")
    {
        using InstanceMaker = std::unique_ptr<ClassType, DeleterType>(*)();
        InstanceMaker maker = this->find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
    }

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
    std::shared_ptr<ClassType> make_shared_instance(const std::string_view maker_function_name = "make_shared_instance")
    {
        using InstanceMaker = std::shared_ptr<ClassType>(*)();
        InstanceMaker maker = this->find_function_ptr<InstanceMaker>(maker_function_name);
        return maker();
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
