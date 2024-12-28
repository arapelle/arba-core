#pragma once

#include "sbrm.hpp"

#include <filesystem>

inline namespace arba
{
namespace core
{

inline auto make_sb_file_remover(const std::filesystem::path& path)
{
    return sbrm([&path] { std::filesystem::remove(path); });
}

inline auto make_sb_file_remover(std::filesystem::path&& path)
{
    using function_type = bool (*)(const std::filesystem::path&);
    return sbrm(
        std::bind(static_cast<function_type>(&std::filesystem::remove), std::forward<std::filesystem::path>(path)));
}

inline auto make_sb_file_remover(const std::filesystem::path& path, std::error_code& ec)
{
    return sbrm([&path, &ec] { std::filesystem::remove(path, ec); });
}

inline auto make_sb_file_remover(std::filesystem::path&& path, std::error_code& ec)
{
    using function_type = bool (*)(const std::filesystem::path&, std::error_code&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove), path, std::ref(ec)));
}

inline auto make_sb_all_files_remover(const std::filesystem::path& path)
{
    return sbrm([&path] { std::filesystem::remove_all(path); });
}

inline auto make_sb_all_files_remover(std::filesystem::path&& path)
{
    using function_type = uintmax_t (*)(const std::filesystem::path&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove_all), path));
}

inline auto make_sb_all_files_remover(const std::filesystem::path& path, std::error_code& ec)
{
    return sbrm([&path, &ec] { std::filesystem::remove_all(path, ec); });
}

inline auto make_sb_all_files_remover(std::filesystem::path&& path, std::error_code& ec)
{
    using function_type = uintmax_t (*)(const std::filesystem::path&, std::error_code&);
    return sbrm(std::bind(static_cast<function_type>(&std::filesystem::remove_all), path, std::ref(ec)));
}

} // namespace core
} // namespace arba
