#pragma once

inline namespace arba
{
namespace core
{

inline void compile_time_error(...)
{
    throw "This function must only be called in constant context.";
}

} // namespace core
} // namespace arba
