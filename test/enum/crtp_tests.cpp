
#include <type_traits>

#include <gtest/gtest.h>


template <class SelfType = void>
class crtp_base;

template <>
class crtp_base<void>
{
public:
    static constexpr bool is_concrete = false;

    crtp_base()
    {
        std::string func = __PRETTY_FUNCTION__;
        std::cout << func << " " << is_concrete << std::endl << std::flush;
    }

    template <class OtherType>
    using rebind_t = crtp_base<OtherType>;
};

template <class SelfType>
class crtp_base //: public crtp_base<void>
{
public:
    static constexpr bool is_concrete = true;

    crtp_base()
    {
        std::string func = __PRETTY_FUNCTION__;
        std::cout << func << " " << is_concrete << std::endl << std::flush;
    }

    template <class OtherType>
    using rebind_t = crtp_base<OtherType>;

    using self_type = SelfType;

    [[nodiscard]] inline const self_type& self() const noexcept { return static_cast<const self_type&>(*this); }
    [[nodiscard]] inline self_type& self() noexcept { return static_cast<self_type&>(*this); }
};

template <class Base, class Concrete>
struct concrete_crtp_base
{
    using type = std::conditional_t<Base::is_concrete
//                                    requires (Base& base, const Base& cbase)
//                                    {
//                                        typename Base::self_type;
//                                        { base.self() } -> std::same_as<typename Base::self_type&>;
//                                        { cbase.self() } -> std::same_as<const typename Base::self_type&>;
//                                    }
                                    ,
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
    using base_ = crtp_base<SelfType>;

public:
    template <class OtherType>
    using rebind_t = computer<OtherType>;

    computer()
    {
        std::string func = __PRETTY_FUNCTION__;
        std::cout << func << std::endl << std::flush;
    }

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

    scale()
    {
        std::string func = __PRETTY_FUNCTION__;
        std::cout << func << " " << this->is_concrete << std::endl << std::flush;
    }

    unsigned scale_value() const
    {
        return (Scale << this->self().scale_bit_factor());
    }

    unsigned scale_bit_factor() const { return 0; }
};

template <class Base, int Offset>
class offset : public //concrete_crtp_base_t<Base, offset<Base, Offset>>
                   std::conditional_t<Base::is_concrete,
                                      Base,
                                      typename Base::template rebind_t<offset<Base, Offset>>
                                      >
{
public:
    template <class OtherType>
    using rebind_t = offset<rebind_t<Base, OtherType>, Offset>;

    offset()
    {
        std::string func = __PRETTY_FUNCTION__;
        std::cout << func << " " << Base::is_concrete << std::endl << std::flush;
    }

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
