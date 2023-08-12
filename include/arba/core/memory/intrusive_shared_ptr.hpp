#pragma once

#include <functional>
#include <cassert>

inline namespace arba
{
namespace core
{

template <class element_type>
void intrusive_shared_ptr_add_ref(element_type* ptr) noexcept;

template <class element_type>
void intrusive_shared_ptr_release(element_type* ptr) noexcept;

template <class element_type>
concept intrusive_sharable = requires(element_type* ptr)
{
    intrusive_shared_ptr_add_ref(ptr);
    intrusive_shared_ptr_release(ptr);
};

template <intrusive_sharable elem_type>
class intrusive_shared_ptr
{
public:
    using element_type = elem_type;
    using pointer_type = std::add_pointer_t<element_type>;
    using reference_type = std::add_lvalue_reference_t<element_type>;

    intrusive_shared_ptr(std::nullptr_t = nullptr) {}

    explicit intrusive_shared_ptr(element_type* ptr);

    intrusive_shared_ptr(const intrusive_shared_ptr& isptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_shared_ptr(const intrusive_shared_ptr<Up>& isptr);

    intrusive_shared_ptr(intrusive_shared_ptr&& isptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_shared_ptr(intrusive_shared_ptr<Up>&& isptr);

//    template <typename Up>
//    explicit intrusive_shared_ptr(const intrusive_weak_ptr<Up>& iwptr);

    ~intrusive_shared_ptr();

    intrusive_shared_ptr& operator=(intrusive_shared_ptr isptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_shared_ptr& operator=(intrusive_shared_ptr<Up> isptr);

    element_type* release() noexcept;
    void reset(element_type* pointer = nullptr) noexcept;

    inline element_type* get() const noexcept { return pointer_; }
    inline element_type& operator*() const noexcept { assert(pointer_); return *pointer_; }
    inline element_type* operator->() const noexcept { assert(pointer_); return pointer_; }
    inline operator bool() const { return pointer_ != nullptr; }

    inline void swap(intrusive_shared_ptr& other) { std::swap(pointer_, other.pointer_); }
    inline auto operator<=>(const intrusive_shared_ptr&) const = default;

private:
    element_type* pointer_ = nullptr;

    template <intrusive_sharable Up>
    friend class intrusive_shared_ptr;
};

template <intrusive_sharable Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(Type* ptr)
    : pointer_(ptr)
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <intrusive_sharable Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(const intrusive_shared_ptr& isptr)
    : pointer_(isptr.pointer_)
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <intrusive_sharable Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(const intrusive_shared_ptr<Up>& isptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(isptr.get()))
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <intrusive_sharable Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(intrusive_shared_ptr&& isptr)
    : pointer_(isptr.pointer_)
{
    isptr.pointer_ = nullptr;
}

template <intrusive_sharable Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(intrusive_shared_ptr<Up>&& isptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(isptr.get()))
{
    isptr.pointer_ = nullptr;
}

template <intrusive_sharable Type>
inline intrusive_shared_ptr<Type>::~intrusive_shared_ptr()
{
    if (pointer_)
        intrusive_shared_ptr_release(pointer_);
}

template <intrusive_sharable Type>
inline intrusive_shared_ptr<Type>& intrusive_shared_ptr<Type>::operator=(intrusive_shared_ptr<Type> isptr)
{
    std::swap(pointer_, isptr.pointer_);
    return *this;
}

template <intrusive_sharable Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_shared_ptr<Type>& intrusive_shared_ptr<Type>::operator=(intrusive_shared_ptr<Up> isptr)
{
    intrusive_shared_ptr<Type> aux(std::move(isptr));
    std::swap(pointer_, aux.pointer_);
    return *this;
}


template <intrusive_sharable Type>
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

template <intrusive_sharable Type>
inline void intrusive_shared_ptr<Type>::reset(Type* pointer) noexcept
{
    if (pointer_)
        intrusive_shared_ptr_release(pointer_);
    if (pointer_ = pointer; pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}


template <intrusive_sharable val_type, class... args_types>
inline intrusive_shared_ptr<val_type> make_intrusive_shared_ptr(args_types&&... args)
{
    return intrusive_shared_ptr<val_type>(new val_type(std::forward<args_types>(args)...));
}

}
}

template <class value_type>
struct std::hash< ::arba::core::intrusive_shared_ptr<value_type>> : private std::hash<value_type*>
{
    std::size_t operator()(const ::arba::core::intrusive_shared_ptr<value_type>& ptr) const noexcept
    {
        return this->std::hash<value_type*>::operator()(ptr.get());
    }
};
