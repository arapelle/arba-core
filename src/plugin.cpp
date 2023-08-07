#include <arba/core/plugin.hpp>
#include <format>
#include <iostream>
#include <type_traits>
#include <cassert>
#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

inline namespace arba
{
namespace core
{

// Windows API:
//   https://learn.microsoft.com/fr-fr/windows/win32/dlls/using-run-time-dynamic-linking
//   https://learn.microsoft.com/fr-fr/windows/win32/dlls/creating-a-simple-dynamic-link-library
// UNIX API (dl):
//   https://linux.die.net/man/3/dlopen

plugin::plugin(const std::filesystem::path& plugin_path)
{
#ifdef WIN32
    static_assert(std::is_pointer_v<HINSTANCE>);
    static_assert(std::is_nothrow_convertible_v<HINSTANCE, void*>);

    HINSTANCE instance = LoadLibraryW(plugin_path.native().c_str());
    if (!instance) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw std::system_error(error_code,
                                std::format("Exception occurred while loading {}", plugin_path.generic_string()));
    }
    handle_ = static_cast<void*>(instance);
#else
    std::string plugin_path_string;
    if (plugin_path.has_extension() || std::filesystem::exists(plugin_path))
    {
        plugin_path_string = plugin_path.generic_string();
    }
    else
    {
        plugin_path_string = plugin_path.generic_string() + ".so";
    }
    void* handle = dlopen(plugin_path_string.c_str(), RTLD_LAZY);
    if (!handle) [[unlikely]]
    {
        std::string error_message(dlerror());
        throw std::runtime_error(std::format("Exception occurred while loading plugin: {}",
                                             error_message));
    }
    handle_ = handle;
#endif
}

plugin::~plugin()
{
#ifdef WIN32
    if (handle_)
    {
        int result = FreeLibrary(static_cast<HINSTANCE>(handle_));
        if (result == 0) [[unlikely]]
        {
            std::error_code error_code(GetLastError(), std::system_category());
            std::cerr << std::format("A problem occured while unloading plugin: {}", error_code.message()) << std::endl;
            std::exit(error_code.value());
        }
    }
#else
    int result = dlclose(handle_);
    if (result != 0)
    {
        std::string error_message(dlerror());
        std::cerr << std::format("A problem occured while unloading plugin: {}", error_message) << std::endl;
        std::exit(result);
    }
#endif
}

void* plugin::find_symbol_pointer_(const std::string& symbol_name)
{
#ifdef WIN32
        static_assert(std::is_pointer_v<FARPROC>);

        FARPROC pointer = GetProcAddress(static_cast<HINSTANCE>(handle_), symbol_name.c_str());
        if (!pointer) [[unlikely]]
        {
            std::error_code error_code(GetLastError(), std::system_category());
            throw std::system_error(error_code,
                                    std::format("Exception occurred while looking for address of {}", symbol_name));
        }
        return reinterpret_cast<void*>(pointer);
#else
        void* pointer = dlsym(handle_, symbol_name.c_str());
        if (!pointer) [[unlikely]]
        {
            std::string error_message(dlerror());
            throw std::runtime_error(std::format("Exception occurred while looking for address of symbol: {}",
                                                 error_message));
        }
        return pointer;
#endif
}

}
}