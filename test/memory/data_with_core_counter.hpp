#pragma once

#include <string>
#include <arba/core/memory/intrusive_ref_counter.hpp>

struct data_with_ircnt : public core::intrusive_ref_counter<data_with_ircnt>
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnt(bool& valid, const std::string& text = "") : text(text), valid(&valid) { valid = true; }
    ~data_with_ircnt() { *valid = false; }
};

struct data_with_ircnt_mc : public core::intrusive_ref_counter<data_with_ircnt_mc>
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnt_mc(bool& valid, std::string&& text) : text(std::forward<std::string>(text)), valid(&valid) { valid = true; }
    ~data_with_ircnt_mc() { *valid = false; }
};
