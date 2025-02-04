
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

    template <class OtherBaseType>
    using rebase_t = CrtpTemplate<void>;
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

    template <class OtherBaseType>
    using rebase_t = CrtpTemplate<SelfType>;
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
class computer : public crtp_base<SelfType, computer>
{
public:
    int generate() const
    {
        return this->self().seed() + 1000;
    }
};

template <class Base, int Seed>
class factor : public concrete_crtp_base_t<Base, factor<Base, Seed>>
{
public:
    template <class OtherType>
    using rebind_t = factor<rebind_t<Base, OtherType>, Seed>;

    int seed() const
    {
        return Seed * this->self().seed_factor();
    }

    int seed_factor() const { return 6; }
};

TEST(crtp_tests, test_crtp_using)
{
    using mytype = factor<computer<>, 6>;
    mytype var;
    int value = var.generate();
    ASSERT_EQ(value, 1036);
}

class myclass : public factor<computer<myclass>, 6>
{
    using base_ = factor<computer<myclass>, 6>;

public:
    int seed() const
    {
        return 100 + this->base_::seed();
    }

    int seed_factor() const { return 7; }
};

TEST(crtp_tests, test_crtp_class)
{
    myclass var;
    int value = var.generate();
    ASSERT_EQ(value, 1142);
}
