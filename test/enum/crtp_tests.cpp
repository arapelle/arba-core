
#include <type_traits>

#include <gtest/gtest.h>

template <class>
class no_rebind;

template <class SelfType, template <class> class CrtpTemplate = no_rebind>
class crtp_base;

template <template <class> class CrtpTemplate>
class crtp_base<void, CrtpTemplate>
{
protected:
    template <class>
    using rebase_t = crtp_base;
};

template <class SelfType>
    requires (std::is_class_v<SelfType>)
class crtp_base<SelfType> : public crtp_base<void>
{
public:
    using self_type = SelfType;

protected:
    [[nodiscard]] inline const self_type& self() const noexcept { return static_cast<const self_type&>(*this); }
    [[nodiscard]] inline self_type& self() noexcept { return static_cast<self_type&>(*this); }
};

template <class SelfType, template <class> class CrtpTemplate>
class crtp_base : public crtp_base<SelfType>
{
protected:
    template <class OtherType>
    using rebind_t = CrtpTemplate<OtherType>;
};

template <class Type>
concept CrtpBase = std::is_base_of_v<crtp_base<void>, Type>;

class crtp_deco
{
protected:
    template <class>
    using rebind_t = crtp_deco;

    template <class>
    using rebase_t = crtp_deco;
};

template <class Type>
concept CrtpDeco = std::is_base_of_v<crtp_deco, Type>;

template <class Type>
concept CrtpType = CrtpBase<Type> || CrtpDeco<Type>;

template <class Base, class OtherType>
using rebind_t = typename Base::template rebind_t<OtherType>;

template <class Base, class OtherBaseType>
using rebase_t = typename Base::template rebase_t<OtherBaseType>;

template <class SelfT, class ThisT>
using conditional_base_t = std::conditional_t<std::is_void_v<SelfT>, ThisT, SelfT>;

template <CrtpDeco Base, CrtpBase RebasedType, CrtpType RecRebasedType>
using conditional_rebase_t = std::conditional_t<std::is_same_v<Base, crtp_deco>, RebasedType, RecRebasedType>;

template <CrtpBase Base, CrtpDeco Decorator, class OtherSelf>
struct decorate
{
    using type = rebind_t<rebase_t<Decorator, Base>, OtherSelf>;
};

template <CrtpBase Base, class OtherSelf>
struct decorate<Base, crtp_deco, OtherSelf>
{
    using type = rebind_t<Base, OtherSelf>;
};

template <CrtpBase Base, CrtpDeco Decorator, class OtherSelf>
using decorate_t = typename decorate<Base, Decorator, OtherSelf>::type;

// ------

template <class SelfType>
class computer : public crtp_base<SelfType, computer>
{
public:
    int generate() const
    {
        return this->self().seed() + 1000 + this->self().offset_value();
    }

    int offset_value() const { return 0; }
};

template <class Base, int Seed>
class factor : public Base
{
public:
    template <class OtherSelf>
    using rebind_t = factor<rebind_t<Base, OtherSelf>, Seed>;

    template <CrtpBase OtherBase>
    using rebase_t = conditional_rebase_t<Base,
                                          factor<OtherBase, Seed>,
                                          factor<rebase_t<Base, OtherBase>, Seed>>;

    int seed() const
    {
        return Seed * this->self().seed_factor();
    }

    int seed_factor() const { return 6; }
};

template <class Base, int Offset>
class offset : public Base
{
public:
    template <class OtherSelf>
    using rebind_t = offset<rebind_t<Base, OtherSelf>, Offset>;

    template <CrtpBase OtherBase>
    using rebase_t = conditional_rebase_t<Base,
                                          offset<OtherBase, Offset>,
                                          offset<rebase_t<Base, OtherBase>, Offset>>;

    int offset_value() const
    {
        return Offset;
    }
};

class myclass : public factor<offset<computer<myclass>, 10000>, 6>
{
    using base_ = factor<offset<computer<myclass>, 10000>, 6>;

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
    ASSERT_EQ(value, 11142);
}

class myoffsetbase : public offset<computer<myoffsetbase>, 10000>
{
    using base_ = offset<computer<myoffsetbase>, 10000>;

public:
};

class myfinalbase : public decorate_t<myoffsetbase, factor<crtp_deco, 6>, myfinalbase>
{
    using base_ = decorate_t<myoffsetbase, factor<crtp_deco, 6>, myfinalbase>;

public:
    int seed() const
    {
        return 700 + this->base_::seed();
    }

    int seed_factor() const { return 8; }
};

TEST(crtp_tests, test_crtp_rebase)
{
    myfinalbase var;
    int value = var.generate();
    ASSERT_EQ(value, 11748);
}
