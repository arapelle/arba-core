#pragma once

#include <optional>
#include <filesystem>
#include <functional>

namespace core
{

// Scoped-Bound Resource Manager
// https://en.cppreference.com/w/cpp/language/raii
template <class functor_type>
class sbrm
{
public:
    using deleter_type = functor_type;

    sbrm(const deleter_type& deleter)
        : deleter_(deleter)
    {}

    sbrm(deleter_type&& deleter)
        : deleter_(std::forward<deleter_type>(deleter))
    {}

    ~sbrm()
    {
        if (deleter_.has_value())
            (*deleter_)();
    }

    void disable()
    {
        deleter_ = std::nullopt;
    }

private:
    std::optional<deleter_type> deleter_;
};

inline auto make_sb_file_remover(const std::filesystem::path& path)
{
    using function_type = bool(*)(const std::filesystem::path&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove), path));
}

inline auto make_sb_file_remover(const std::filesystem::path& path, std::error_code& ec)
{
    using function_type = bool(*)(const std::filesystem::path&, std::error_code&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove), path, std::ref(ec)));
}

inline auto make_sb_all_files_remover(const std::filesystem::path& path)
{
    using function_type = uintmax_t(*)(const std::filesystem::path&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove_all), path));
}

inline auto make_sb_all_files_remover(const std::filesystem::path& path, std::error_code& ec)
{
    using function_type = uintmax_t(*)(const std::filesystem::path&, std::error_code&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove_all), path, std::ref(ec)));
}

}
