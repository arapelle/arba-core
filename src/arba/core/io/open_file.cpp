#include <arba/core/io/open_file.hpp>
#include <arba/core/io/check_file.hpp>
#include <fstream>

inline namespace arba
{
namespace core
{
std::ifstream& open_input_file(std::ifstream& input_fstream,
                               const std::filesystem::path& input_fpath,
                               std::ios_base::openmode mode)
{
    check_input_file(input_fpath);
    input_fstream.open(input_fpath, mode);
    input_fstream.exceptions(std::ifstream::failbit);
    return input_fstream;
}

[[nodiscard]] std::ifstream open_input_file(const std::filesystem::path& input_fpath,
                                            std::ios_base::openmode mode)
{
    std::ifstream ifs;
    open_input_file(ifs, input_fpath, mode);
    return std::ifstream(std::move(ifs));
}

}
}
