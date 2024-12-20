#pragma once

#include "plugin_impl.hpp"

#include <any>
#include <format>
#include <unordered_map>

inline namespace arba
{
namespace core
{

namespace private_
{
using plugin_function_register_type = std::any (*)(std::string_view);
static constexpr std::string_view plugin_function_register_fname = "arba_core_plugin_function_register_";
}

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
        const auto find_function_ptr = this->find_symbol_pointer(std::string(private_::plugin_function_register_fname));
        const private_::plugin_function_register_type find_function_ptr_as_any =
            reinterpret_cast<private_::plugin_function_register_type>(find_function_ptr);

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
                                             "Did you forget to use ARBA_CORE_REGISTER_PLUGIN_FUNCTION() ?",
                                             function_name));
    }
};

}
}

#define ARBA_CORE_BEGIN_PLUGIN_FUNCTION_REGISTER()                                                                     \
    extern "C" std::any arba_core_plugin_function_register_(std::string_view function_name)                            \
    {                                                                                                                  \
        static_assert(arba::core::private_::plugin_function_register_fname == __func__);                               \
        static_assert(std::is_same_v<arba::core::private_::plugin_function_register_type,                              \
                                     decltype(&arba_core_plugin_function_register_)>);                                 \
        static const std::unordered_map<std::string_view, std::any> function_register_                                 \
        {

#define ARBA_CORE_REGISTER_PLUGIN_FUNCTION(function_) { #function_, &function_ },

#define ARBA_CORE_END_PLUGIN_FUNCTION_REGISTER()                                                                       \
    }                                                                                                                  \
    ;                                                                                                                  \
    const auto iter = function_register_.find(function_name);                                                          \
    return iter != function_register_.cend() ? iter->second : std::any();                                              \
    }
