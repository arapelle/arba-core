#include <arba/core/io/check_file.hpp>

inline namespace arba
{
namespace core
{

void check_input_file(const std::filesystem::path& input_fpath)
{
    std::error_code err;
    std::filesystem::file_status f_status = std::filesystem::status(input_fpath, err);
    if (!std::filesystem::exists(f_status)) [[unlikely]]
        throw std::filesystem::filesystem_error("Input path", input_fpath, err);
    if (!std::filesystem::is_regular_file(f_status)) [[unlikely]]
    {
        err = std::error_code(2, std::generic_category());
        throw std::filesystem::filesystem_error("Input path is not a regular file", input_fpath, err);
    }
}

}
}
