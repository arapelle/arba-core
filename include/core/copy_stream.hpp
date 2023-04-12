#pragma once

#include <istream>
#include <ostream>
#include <filesystem>

namespace core
{
bool copy_stream(std::istream& input_stream, std::size_t number_of_bytes, std::ostream& output_stream);
bool copy_fstream(std::ifstream& input_fstream, std::ostream& output_stream);
bool copy_file(const std::filesystem::path& input_fpath, std::ostream& output_stream);
}
