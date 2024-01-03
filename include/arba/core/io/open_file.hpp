#pragma once

#include <filesystem>
#include <fstream>

inline namespace arba
{
namespace core
{

std::ifstream& open_input_file(std::ifstream& input_fstream, const std::filesystem::path& input_fpath,
                               std::ios_base::openmode mode = std::ios_base::in);

[[nodiscard]] std::ifstream open_input_file(const std::filesystem::path& input_fpath,
                                            std::ios_base::openmode mode = std::ios_base::in);

}
}
