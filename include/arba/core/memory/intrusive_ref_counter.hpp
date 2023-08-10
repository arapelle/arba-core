#pragma once

#include <atomic>
#include <type_traits>

inline namespace arba
{
namespace core
{

class intrusive_ref_counter_base
{
public:
    intrusive_ref_counter_base() noexcept
        : shared_ref_counter_(0)
    {}

    explicit intrusive_ref_counter_base(const intrusive_ref_counter_base&) noexcept
        : shared_ref_counter_(0)
    {}

    intrusive_ref_counter_base& operator=(const intrusive_ref_counter_base&) noexcept
    {
        return *this;
    }

    inline unsigned int use_count() const noexcept { return shared_ref_counter_; }

    friend void intrusive_shared_ptr_add_ref_(intrusive_ref_counter_base* ptr) noexcept;
    friend bool intrusive_shared_ptr_dec_ref_(intrusive_ref_counter_base* ptr) noexcept;

protected:
    ~intrusive_ref_counter_base() = default;

private:
    std::atomic_uint shared_ref_counter_ = 0;
};

inline void intrusive_shared_ptr_add_ref_(intrusive_ref_counter_base* ptr) noexcept
{
    ++(ptr->shared_ref_counter_);
}

inline bool intrusive_shared_ptr_dec_ref_(intrusive_ref_counter_base* ptr) noexcept
{
    return --(ptr->shared_ref_counter_) == 0;
}

template <class derived_type>
class intrusive_ref_counter : public intrusive_ref_counter_base
{
public:
    using intrusive_ref_counter_base::intrusive_ref_counter_base;
};

template <class value_type>
requires std::is_base_of_v<intrusive_ref_counter_base, value_type>
void intrusive_shared_ptr_add_ref(value_type* ptr) noexcept
{
    intrusive_shared_ptr_add_ref_(ptr);
}

template <class value_type>
requires std::is_base_of_v<intrusive_ref_counter_base, value_type>
void intrusive_shared_ptr_release(value_type* ptr) noexcept
{
    if (intrusive_shared_ptr_dec_ref_(ptr))
        delete ptr;
}

}
}
