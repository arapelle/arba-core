#pragma once

#include "byte.hpp"

#include <arba/meta/type_traits/kwargs.hpp>

#include <cstdint>
#include <span>
#include <string>
#include <fstream>
#include <sstream>
#include <optional>
#include <format>

inline namespace arba
{
namespace core
{
namespace bytes_formatter_kwargs
{

ARBA_META_KWARG(unit_format, std::string);
ARBA_META_KWARG(unit_sep, std::string);
ARBA_META_KWARG(seq_beginning, std::string);
ARBA_META_KWARG(seq_end, std::string);
ARBA_META_KWARG(chunk_beginning, std::string);
ARBA_META_KWARG(chunk_end, std::string);
ARBA_META_KWARG(nb_units_per_chunk, uint32_t);
ARBA_META_KWARG(force_chunk_end, bool);

}

template <class T>
concept BytesFormatterKwarg = meta::Kwarg<T,
                            bytes_formatter_kwargs::unit_format,
                            bytes_formatter_kwargs::unit_sep,
                            bytes_formatter_kwargs::seq_beginning,
                            bytes_formatter_kwargs::seq_end,
                            bytes_formatter_kwargs::chunk_beginning,
                            bytes_formatter_kwargs::chunk_end,
                            bytes_formatter_kwargs::nb_units_per_chunk,
                            bytes_formatter_kwargs::force_chunk_end
                            >;

class bytes_formatter
{
    static constexpr std::size_t buffer_size = 64 * 1024;

public:
    template <typename... Kwargs>
        requires (BytesFormatterKwarg<Kwargs> && ...)
    bytes_formatter(Kwargs&&... kwargs)
    {
        meta::kwargs_parser<Kwargs...> k_parser(std::forward<Kwargs>(kwargs)...);
        unit_format_ = k_parser.template arg_or_default<bytes_formatter_kwargs::unit_format>("{:x}");
        unit_sep_ = k_parser.template arg_or_default<bytes_formatter_kwargs::unit_sep>(", ");
        seq_beginning_ = k_parser.template arg_or_default<bytes_formatter_kwargs::seq_beginning>("[");
        seq_end_ = k_parser.template arg_or_default<bytes_formatter_kwargs::seq_end>("]");
        chunk_beginning_ = k_parser.template arg_or_default<bytes_formatter_kwargs::chunk_beginning>("");
        chunk_end_ = k_parser.template arg_or_default<bytes_formatter_kwargs::chunk_end>("\n");
        nb_units_per_chunk_ = k_parser.template arg_or_default<bytes_formatter_kwargs::nb_units_per_chunk>(32);
        force_chunk_end_ = k_parser.template arg_or_default<bytes_formatter_kwargs::force_chunk_end>(!chunk_beginning_.empty());
    }

    [[nodiscard]] inline const std::string& unit_format() const { return unit_format_; }
    inline void set_unit_format(const std::string& unit_format) { unit_format_ = unit_format; }

    [[nodiscard]] inline const std::string& unit_sep() const { return unit_sep_; }
    inline void set_unit_sep(const std::string& unit_sep) { unit_sep_ = unit_sep; }

    [[nodiscard]] inline const std::string& seq_beginning() const { return seq_beginning_; }
    inline void set_seq_beginning(const std::string& seq_beginning) { seq_beginning_ = seq_beginning; }

    [[nodiscard]] inline const std::string& seq_end() const { return seq_end_; }
    inline void set_seq_end(const std::string& seq_end) { seq_end_ = seq_end; }

    [[nodiscard]] inline const std::string& chunk_beginning() const { return chunk_beginning_; }
    inline void set_chunk_beginning(const std::string& chunk_beginning) { chunk_beginning_ = chunk_beginning; }

    [[nodiscard]] inline const std::string& chunk_end() const { return chunk_end_; }
    inline void set_chunk_end(const std::string& chunk_end) { chunk_end_ = chunk_end; }

    [[nodiscard]] inline uint32_t nb_units_per_chunk() const { return nb_units_per_chunk_; }
    inline void set_nb_units_per_chunk(uint32_t nb_units_per_chunk) { nb_units_per_chunk_ = nb_units_per_chunk; }

    [[nodiscard]] inline bool force_chunk_end() const { return force_chunk_end_; }
    inline void set_force_chunk_end(bool force_chunk_end) { force_chunk_end_ = force_chunk_end; }

public:
    void format_binary_stream_to(std::istream &input_stream, std::ostream& output_stream, std::size_t first_unit_index = 0) const;
    void format_binary_cstream_to(FILE* input_stream, std::ostream& output_stream, std::size_t first_unit_index = 0) const;
    void format_bytes_to(std::span<const std::byte> bytes, std::ostream& output_stream, std::size_t first_unit_index = 0) const;

    [[nodiscard]] std::string format_binary_stream(std::istream &input_stream, std::size_t first_unit_index = 0) const;
    [[nodiscard]] std::string format_binary_cstream(FILE* input_stream, std::size_t first_unit_index = 0) const;
    [[nodiscard]] std::string format_bytes(std::span<const std::byte> bytes, std::size_t first_byte_index = 0) const;

private:
    [[nodiscard]] inline bool uses_simple_format_() const { return unit_format_ == "{}"; }
    void format_bytes_to_(std::span<const std::byte> bytes, std::ostream& output_stream,
                          std::size_t& unit_counter, std::size_t& unit_index) const;
    bool format_byte_to_(std::byte byte, std::ostream& output_stream,
                         std::size_t& unit_counter, std::size_t& unit_index,
                         std::string_view unit_sep, bool use_simple_format) const;
    inline void format_last_byte_to_(std::byte byte, std::ostream& output_stream,
                                     std::size_t& unit_counter, std::size_t& unit_index) const
    {
        if (!format_byte_to_(byte, output_stream, unit_counter, unit_index, "", uses_simple_format_()) && force_chunk_end_)
            output_stream << chunk_end_;
    }

private:
    std::string unit_format_;
    std::string unit_sep_;
    std::string seq_beginning_;
    std::string seq_end_;
    std::string chunk_beginning_;
    std::string chunk_end_;
    uint32_t nb_units_per_chunk_;
    bool force_chunk_end_;
};

inline void bytes_formatter::format_binary_stream_to(std::istream& input_stream, std::ostream& output_stream, std::size_t unit_index) const
{
    const std::size_t current_pos = input_stream.tellg();
    input_stream.seekg(0, std::ios::end);
    std::size_t input_file_size = static_cast<std::size_t>(input_stream.tellg()) - current_pos;
    input_stream.seekg(current_pos, std::ios::beg);

    output_stream << seq_beginning_;

    std::size_t nb_full_iterations = input_file_size / buffer_size;
    std::size_t nb_last_bytes = input_file_size % buffer_size;
    if (nb_last_bytes == 0 && nb_full_iterations > 0) [[unlikely]]
    {
        --nb_full_iterations;
        nb_last_bytes = buffer_size;
    }

    std::size_t unit_counter = 0;
    std::array<std::byte, buffer_size> bytes;
    for (; nb_full_iterations > 0; --nb_full_iterations)
    {
        input_stream.read(reinterpret_cast<char*>(bytes.data()), buffer_size);
        format_bytes_to_(bytes, output_stream, unit_counter, unit_index);
    }

    if (nb_last_bytes > 0) [[likely]]
    {
        input_stream.read(reinterpret_cast<char*>(bytes.data()), nb_last_bytes);
        format_bytes_to_(std::span(bytes.data(), nb_last_bytes - 1), output_stream, unit_counter, unit_index);
        format_last_byte_to_(bytes[nb_last_bytes - 1], output_stream, unit_counter, unit_index);
    }

    output_stream << seq_end_;
}

inline void bytes_formatter::format_binary_cstream_to(FILE *input_stream, std::ostream &output_stream, std::size_t unit_index) const
{
    output_stream << seq_beginning_;

    std::size_t unit_counter = 0;
    std::array<std::byte, buffer_size> bytes;
    while (!std::feof(input_stream)) [[likely]]
    {
        const size_t nb_bytes = std::fread(bytes.data(), 1, buffer_size, input_stream);
        int ch = std::fgetc(input_stream);
        ungetc(ch, input_stream);
        if (ch != EOF)
            format_bytes_to_(std::span(bytes), output_stream, unit_counter, unit_index);
        else
        {
            format_bytes_to_(std::span(bytes.data(), nb_bytes - 1), output_stream, unit_counter, unit_index);
            format_last_byte_to_(bytes[nb_bytes - 1], output_stream, unit_counter, unit_index);
            break;
        }
    }

    output_stream << seq_end_;
}

inline void bytes_formatter::format_bytes_to(std::span<const std::byte> bytes, std::ostream &output_stream, std::size_t unit_index) const
{
    output_stream << seq_beginning_;
    if (bytes.size() > 0) [[likely]]
    {
        std::size_t unit_counter = 0;
        format_bytes_to_(bytes.subspan(0, bytes.size() - 1), output_stream, unit_counter, unit_index);
        format_last_byte_to_(bytes.back(), output_stream, unit_counter, unit_index);
    }
    output_stream << seq_end_;
}

inline std::string bytes_formatter::format_binary_stream(std::istream &input_stream, std::size_t first_unit_index) const
{
    std::ostringstream stream;
    format_binary_stream_to(input_stream, stream, first_unit_index);
    return stream.str();
}

inline std::string bytes_formatter::format_binary_cstream(FILE *input_stream, std::size_t first_unit_index) const
{
    std::ostringstream stream;
    format_binary_cstream_to(input_stream, stream, first_unit_index);
    return stream.str();
}

inline std::string bytes_formatter::format_bytes(std::span<const std::byte> bytes, std::size_t first_byte_index) const
{
    std::ostringstream stream;
    format_bytes_to(bytes, stream, first_byte_index);
    return stream.str();
}

inline void bytes_formatter::format_bytes_to_(std::span<const std::byte> bytes, std::ostream& output_stream,
                                              std::size_t& unit_counter, std::size_t& unit_index) const
{
    const bool uses_simple_format = uses_simple_format_();
    for (auto& byte : bytes)
        format_byte_to_(byte, output_stream, unit_counter, unit_index, unit_sep_, uses_simple_format);
}

inline bool bytes_formatter::format_byte_to_(std::byte byte, std::ostream& output_stream, std::size_t& unit_counter,
                                             std::size_t &unit_index, std::string_view unit_separator, bool use_simple_format) const
{
    if (unit_counter == 0) [[unlikely]]
        output_stream << chunk_beginning_;
    if (use_simple_format)
        output_stream << static_cast<uint16_t>(byte);
    else
        output_stream << std::vformat(unit_format_, std::make_format_args(byte, unit_index));
    ++unit_index;
    if (++unit_counter %= nb_units_per_chunk_; unit_counter == 0) [[unlikely]]
    {
        output_stream << chunk_end_;
        return true;
    }
    else
        output_stream << unit_separator;
    return false;
}

template <typename... Kwargs>
    requires (BytesFormatterKwarg<Kwargs> && ...)
[[nodiscard]] inline std::string format_bytes(std::span<const std::byte> bytes, Kwargs&&... kwargs)
{
    const bytes_formatter formatter(std::forward<Kwargs>(kwargs)...);
    return formatter.format_bytes(bytes);
}

template <typename... Kwargs>
    requires (BytesFormatterKwarg<Kwargs> && ...)
[[nodiscard]] inline std::string format_bytes(std::span<const std::byte> bytes, std::size_t first_byte_index, Kwargs&&... kwargs)
{
    const bytes_formatter formatter(std::forward<Kwargs>(kwargs)...);
    return formatter.format_bytes(bytes, first_byte_index);
}

}
}
