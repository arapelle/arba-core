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
        : ref_counter_(0)
    {}

    explicit intrusive_ref_counter_base(const intrusive_ref_counter_base&) noexcept
        : ref_counter_(0)
    {}

    intrusive_ref_counter_base& operator=(const intrusive_ref_counter_base&) noexcept
    {
        return *this;
    }

    inline unsigned int use_count() const noexcept { return ref_counter_.load() & 0x00000000ffffffff; }
    inline unsigned int latent_count() const noexcept { return (ref_counter_.load() & 0xffffffff00000000) >> 32; }

    friend void intrusive_shared_ptr_add_ref_(intrusive_ref_counter_base* ptr) noexcept;
    friend bool intrusive_shared_ptr_dec_ref_(intrusive_ref_counter_base* ptr) noexcept;
    friend void intrusive_weak_ptr_add_ref_(intrusive_ref_counter_base* ptr) noexcept;
    friend bool intrusive_weak_ptr_dec_ref_(intrusive_ref_counter_base* ptr) noexcept;

protected:
    ~intrusive_ref_counter_base() = default;

private:
    std::atomic_uint64_t ref_counter_ = 0;
};

inline void intrusive_shared_ptr_add_ref_(intrusive_ref_counter_base* ptr) noexcept
{
    ptr->ref_counter_.fetch_add(1);
}

inline bool intrusive_shared_ptr_dec_ref_(intrusive_ref_counter_base* ptr) noexcept
{
    return ptr->ref_counter_.fetch_sub(1) == 1;
}

inline void intrusive_weak_ptr_add_ref_(intrusive_ref_counter_base* ptr) noexcept
{
    ptr->ref_counter_.fetch_add(1LL<<32);
}

inline bool intrusive_weak_ptr_dec_ref_(intrusive_ref_counter_base* ptr) noexcept
{
    return (ptr->ref_counter_.fetch_sub(1LL<<32)) == 1LL<<32;
}

template <class derived_type>
class intrusive_ref_counter : public intrusive_ref_counter_base
{
public:
    using intrusive_ref_counter_base::intrusive_ref_counter_base;
};

template <class element_type>
    requires std::is_base_of_v<intrusive_ref_counter_base, element_type>
void intrusive_shared_ptr_add_ref(element_type* ptr) noexcept
{
    intrusive_shared_ptr_add_ref_(ptr);
}

template <class element_type>
    requires std::is_base_of_v<intrusive_ref_counter_base, element_type>
void intrusive_shared_ptr_release(element_type* ptr) noexcept
{
    if (intrusive_shared_ptr_dec_ref_(ptr))
        delete ptr;
}

template <class element_type>
    requires std::is_base_of_v<intrusive_ref_counter_base, element_type>
void intrusive_weak_ptr_add_ref(element_type* ptr) noexcept
{
    intrusive_weak_ptr_add_ref_(ptr);
}

template <class element_type>
    requires std::is_base_of_v<intrusive_ref_counter_base, element_type>
void intrusive_weak_ptr_release(element_type* ptr) noexcept
{
    if (intrusive_weak_ptr_dec_ref_(ptr))
        delete ptr;
}

template <class element_type>
    requires std::is_base_of_v<intrusive_ref_counter_base, element_type>
std::size_t intrusive_weak_ptr_use_count(element_type* ptr) noexcept
{
    return ptr->use_count();
}

}
}
