#pragma once

#include <functional>
#include <optional>

inline namespace arba
{
namespace core
{

// Scope-Bound Resource Manager
// https://en.cppreference.com/w/cpp/language/raii
template <class functor_type>
class sbrm
{
public:
    using deleter_type = functor_type;

    sbrm(const deleter_type& deleter) : deleter_(deleter) {}

    sbrm(deleter_type&& deleter) : deleter_(std::forward<deleter_type>(deleter)) {}

    ~sbrm()
    {
        if (deleter_.has_value())
            (*deleter_)();
    }

    void disable() { deleter_ = std::nullopt; }

private:
    std::optional<deleter_type> deleter_;
};

} // namespace core
} // namespace arba
