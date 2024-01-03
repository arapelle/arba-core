#include <arba/core/plugin.hpp>
#include <cassert>
#include <format>
#include <iostream>
#include <type_traits>
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
    load_from_file(plugin_path);
}

plugin::~plugin()
{
    if (is_loaded())
    {
        try
        {
            unload();
        }
        catch (const std::runtime_error& err)
        {
            std::cerr << err.what() << std::endl;
            std::terminate();
        }
    }
}

void plugin::load_from_file(const std::filesystem::path& plugin_path)
{
    assert(!is_loaded());
#ifdef WIN32
    static_assert(std::is_pointer_v<HINSTANCE>);
    static_assert(std::is_nothrow_convertible_v<HINSTANCE, void*>);

    HINSTANCE instance = LoadLibraryW(plugin_path.native().c_str());
    if (!instance) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw plugin_error(error_code,
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
        throw plugin_error(std::format("Exception occurred while loading plugin: {}", error_message));
    }
    handle_ = handle;
#endif
}

void plugin::unload()
{
    assert(is_loaded());
#ifdef WIN32
    int result = FreeLibrary(static_cast<HINSTANCE>(handle_));
    if (result == 0) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw plugin_error(error_code,
                           std::format("A problem occured while unloading plugin: {}", error_code.message()));
    }
#else
    int result = dlclose(handle_);
    if (result != 0) [[unlikely]]
    {
        std::string error_message(dlerror());
        throw plugin_error(std::format("A problem occured while unloading plugin: {}", error_message));
    }
#endif
    handle_ = nullptr;
}

void* plugin::find_symbol_pointer_(const std::string& symbol_name)
{
    assert(is_loaded());
#ifdef WIN32
    static_assert(std::is_pointer_v<FARPROC>);

    FARPROC pointer = GetProcAddress(static_cast<HINSTANCE>(handle_), symbol_name.c_str());
    if (!pointer) [[unlikely]]
    {
        std::error_code error_code(GetLastError(), std::system_category());
        throw plugin_error(error_code, std::format("Exception occurred while looking for address of {}", symbol_name));
    }
    return reinterpret_cast<void*>(pointer);
#else
    void* pointer = dlsym(handle_, symbol_name.c_str());
    if (!pointer) [[unlikely]]
    {
        std::string error_message(dlerror());
        throw plugin_error(std::format("Exception occurred while looking for address of symbol: {}", error_message));
    }
    return pointer;
#endif
}

}
}
