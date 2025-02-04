
#include <type_traits>

#include <gtest/gtest.h>

template <class>
class no_rebase;

template <class SelfType, template <class> class CrtpTemplate = no_rebase>
class crtp_base;

template <class SelfType>
class crtp_base<SelfType>
{
public:
    using self_type = SelfType;

    [[nodiscard]] inline const self_type& self() const noexcept { return static_cast<const self_type&>(*this); }
    [[nodiscard]] inline self_type& self() noexcept { return static_cast<self_type&>(*this); }
};

template <class SelfType, template <class> class CrtpTemplate>
class crtp_base : public crtp_base<SelfType>
{
public:
    template <class>
    using rebase_t = CrtpTemplate<SelfType>;
};

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

template <class Base, class ThenT, class ElseT>
using conditional_rebase_t = std::conditional<std::is_same_v<Base, decorator>, ThenT, ElseT>;

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
    template <class OtherBase>
    using rebase_t = conditional_rebase_t<Base, factor<OtherBase, Seed>, factor<rebase_t<Base, OtherBase>, Seed>>;

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
