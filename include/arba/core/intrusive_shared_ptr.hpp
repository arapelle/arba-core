#pragma once

#include <atomic>
#include <functional>
#include <type_traits>

inline namespace arba
{
namespace core
{

template <class value_type>
void intrusive_shared_ptr_add_ref(value_type* ptr) noexcept;

template <class value_type>
void intrusive_shared_ptr_release(value_type* ptr) noexcept;

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

template <class val_type>
class intrusive_shared_ptr
{
public:
    using value_type = val_type;
    using pointer_type = std::add_pointer_t<value_type>;
    using reference_type = std::add_lvalue_reference_t<value_type>;

    intrusive_shared_ptr(std::nullptr_t = nullptr) {}
    explicit intrusive_shared_ptr(value_type* node);
    intrusive_shared_ptr(const intrusive_shared_ptr& node);
    intrusive_shared_ptr(intrusive_shared_ptr&& node);

    template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<value_type>>
    intrusive_shared_ptr(const intrusive_shared_ptr<Up>& node)
        : intrusive_shared_ptr(static_cast<value_type*>(node.get()))
    {}

    ~intrusive_shared_ptr();

    intrusive_shared_ptr& operator=(intrusive_shared_ptr node);

    template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<value_type>>
    intrusive_shared_ptr& operator=(intrusive_shared_ptr<Up> node)
    {
        this->swap(intrusive_shared_ptr(node));
        return *this;
    }

    value_type* release() noexcept;
    void reset(value_type* pointer = nullptr) noexcept;

    inline value_type& operator*() const noexcept { return *get(); }
    inline operator value_type*() const noexcept { return get(); }
    inline value_type* operator->() const noexcept { return get(); }
    inline operator bool() const { return get() != nullptr; }

    inline value_type* get() const { return pointer_; }

    inline void swap(intrusive_shared_ptr& other) { std::swap(pointer_, other.pointer_); }
    inline auto operator<=>(const intrusive_shared_ptr&) const = default;

private:
    value_type* pointer_ = nullptr;
};

template <class Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(Type* node)
    : pointer_(node)
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <class Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(const intrusive_shared_ptr& node)
    : pointer_(node.pointer_)
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <class Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(intrusive_shared_ptr&& node)
    : pointer_(node.get())
{
    node.pointer_ = nullptr;
}

template <class Type>
inline intrusive_shared_ptr<Type>::~intrusive_shared_ptr()
{
    if (pointer_)
        intrusive_shared_ptr_release(pointer_);
}

template <class Type>
inline intrusive_shared_ptr<Type>& intrusive_shared_ptr<Type>::operator=(intrusive_shared_ptr<Type> node)
{
    this->swap(node);
    return *this;
}

template <class Type>
inline Type* intrusive_shared_ptr<Type>::release() noexcept
{
    if (pointer_)
    {
        Type* ptr = pointer_;
        intrusive_shared_ptr_release(pointer_);
        pointer_ = nullptr;
        return ptr;
    }
    return nullptr;
}

template <class Type>
inline void intrusive_shared_ptr<Type>::reset(Type* pointer) noexcept
{
    if (pointer_)
        intrusive_shared_ptr_release(pointer_);
    if (pointer_ = pointer; pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}


template <class val_type, class... args_types>
intrusive_shared_ptr<val_type> make_intrusive_shared_ptr(args_types&&... args)
{
    return intrusive_shared_ptr<val_type>(new val_type(std::forward<args_types>(args)...));
}

}
}

namespace std
{

template <class value_type>
class hash< ::arba::core::intrusive_shared_ptr<value_type>> : public hash<value_type*>
{
};

}
