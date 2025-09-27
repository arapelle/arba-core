#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>
#include <array>

namespace ut
{
inline std::filesystem::path create_binary_resource(std::string_view ut_dir_name, std::string_view rsc_file_name = "resource.bin")
{
    const std::filesystem::path root_dpath = std::filesystem::temp_directory_path() / "dev/arba/core" / ut_dir_name;
    const std::filesystem::path rsc_dpath = root_dpath / "rsc";
    const std::filesystem::path rsc_fpath = rsc_dpath / rsc_file_name;

    if (!std::filesystem::exists(rsc_dpath))
        std::filesystem::create_directories(rsc_dpath);

    if (!std::filesystem::exists(rsc_fpath))
    {
        std::ofstream rsc_fstream(rsc_fpath, std::ios::trunc|std::ios::binary);
        std::array<char, 16> bytes{ 0, 2, 4, 7, 11, 13, 17, 23, 66, 12, 14, 18, 25, 33, 127, 'A' };
        rsc_fstream.write(bytes.data(), bytes.size());
    }

    return rsc_fpath;
}
} // namespace ut
