
#include <type_traits>

#include <gtest/gtest.h>

template <class>
class no_rebind_and_rebase;

template <class SelfType = void, template <class> class CrtpTemplate = no_rebind_and_rebase>
class crtp_base;

template <>
class crtp_base<>
{
};

template <template <class> class CrtpTemplate>
class crtp_base<void, CrtpTemplate> : public crtp_base<void>
{
public:
    template <class OtherType>
    using rebind_t = CrtpTemplate<OtherType>;
};

template <class SelfType>
class crtp_base<SelfType> : public crtp_base<>
{
public:
    using self_type = SelfType;

    [[nodiscard]] inline const self_type& self() const noexcept { return static_cast<const self_type&>(*this); }
    [[nodiscard]] inline self_type& self() noexcept { return static_cast<self_type&>(*this); }
};

template <class SelfType, template <class> class CrtpTemplate>
class crtp_base : public crtp_base<SelfType, no_rebind_and_rebase>
{
public:
    template <class OtherType>
    using rebind_t = CrtpTemplate<OtherType>;
};

template <class Base, class Concrete>
struct concrete_crtp_base
{
    using type = std::conditional_t<requires (Base& base, const Base& cbase)
                                    {
                                        typename Base::self_type;
                                        { base.self() } -> std::same_as<typename Base::self_type&>;
                                        { cbase.self() } -> std::same_as<const typename Base::self_type&>;
                                    },
                                    Base,
                                    typename Base::template rebind_t<Concrete>
                                    >;
};

template <class Base, class Concrete>
using concrete_crtp_base_t = typename concrete_crtp_base<Base, Concrete>::type;

template <class Base, class OtherType>
using rebind_t = typename Base::template rebind_t<OtherType>;

class decorator
{
public:
    template <class>
    using rebind_t = decorator;

    template <class>
    using rebase_t = decorator;
};

template <class Base, class OtherBaseType>
using rebase_t = typename Base::template rebase_t<OtherBaseType>;

// ------

template <class SelfType = void>
class computer : public crtp_base<SelfType>
{
public:
    template <class OtherType>
    using rebind_t = computer<OtherType>;

    unsigned generate() const
    {
        return this->self().scale_value() * 1 + this->self().offset_value();
    }

    unsigned scale_value() const { return 1; }

    unsigned offset_value() const { return 0; }
};

template <class Base, int Scale>
class scale : public concrete_crtp_base_t<Base, scale<Base, Scale>>
{
public:
    template <class OtherType>
    using rebind_t = scale<rebind_t<Base, OtherType>, Scale>;

    unsigned scale_value() const
    {
        return (Scale << this->self().scale_bit_factor());
    }

    unsigned scale_bit_factor() const { return 0; }
};

template <class Base, int Offset>
class offset : public concrete_crtp_base_t<Base, offset<Base, Offset>>
{
public:
    template <class OtherType>
    using rebind_t = offset<rebind_t<Base, OtherType>, Offset>;

    unsigned offset_value() const
    {
        return (Offset << this->self().offset_bit_factor());
    }

    unsigned offset_bit_factor() const { return 0; }
};

TEST(crtp_tests, test_crtp_using)
{
    using mytype = offset<scale<computer<>, 6>, 1000>;
    mytype var;
    unsigned value = var.generate();
    ASSERT_EQ(value, 1006);
}

class myclass : public scale<offset<computer<myclass>, 1000>, 6>
{
    using base_ = scale<offset<computer<myclass>, 1000>, 6>;

public:
    unsigned scale_value() const
    {
        return 1 + this->base_::scale_value();
    }

    unsigned scale_bit_factor() const { return 1; }

    unsigned offset_bit_factor() const { return 1; }
};

TEST(crtp_tests, test_crtp_class)
{
    myclass var;
    unsigned value = var.generate();
    ASSERT_EQ(value, 2013);
}
