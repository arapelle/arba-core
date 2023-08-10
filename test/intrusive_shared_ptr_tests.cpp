#include <arba/core/intrusive_shared_ptr.hpp>
#include <unordered_set>
#include <set>
#include <gtest/gtest.h>

struct simple_data
{
    int counter = 0;
    std::string text;
    bool* valid = nullptr;

    explicit simple_data(bool& valid) : valid(&valid) { valid = true; }
    ~simple_data() { *valid = false; }
};

namespace core
{

template <>
void intrusive_shared_ptr_add_ref<simple_data>(simple_data* ptr) noexcept
{
    ++(ptr->counter);
}

template <>
void intrusive_shared_ptr_release<simple_data>(simple_data* ptr) noexcept
{
    if (--(ptr->counter); ptr->counter == 0)
        delete ptr;
}

}

TEST(intrusive_shared_ptr_simple_data_tests, test_iptr_empty_constructor)
{
    core::intrusive_shared_ptr<simple_data> iptr;
    ASSERT_EQ(iptr.get(), nullptr);
}

TEST(intrusive_shared_ptr_simple_data_tests, test_iptr_constructor)
{
    bool valid = false;
    {
        simple_data* data_ptr = new simple_data(valid);
        core::intrusive_shared_ptr<simple_data> iptr(data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->counter, 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_simple_data_tests, test_iptr_copy_constructor)
{
    bool valid = false;
    {
        simple_data* data_ptr = new simple_data(valid);
        core::intrusive_shared_ptr<simple_data> iptr(data_ptr);
        {
            core::intrusive_shared_ptr<simple_data> jptr(iptr);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->counter, 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->counter, 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->counter, 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_simple_data_tests, test_iptr_move_constructor)
{
    bool valid = false;
    simple_data* data_ptr = new simple_data(valid);
    core::intrusive_shared_ptr<simple_data> iptr(data_ptr);
    {
        core::intrusive_shared_ptr<simple_data> jptr(std::move(iptr));
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_EQ(jptr.get(), data_ptr);
        ASSERT_EQ(jptr->counter, 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

//-------------------------------------------------------

struct ircnt_data : public core::intrusive_ref_counter<ircnt_data>
{
    std::string text;
    bool* valid = nullptr;

    explicit ircnt_data(bool& valid, const std::string& text = "") : text(text), valid(&valid) { valid = true; }
    ~ircnt_data() { *valid = false; }
};

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_empty_constructor)
{
    core::intrusive_shared_ptr<ircnt_data> iptr;
    ASSERT_EQ(iptr.get(), nullptr);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_constructor)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_copy_constructor)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        {
            core::intrusive_shared_ptr<ircnt_data> jptr(iptr);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->use_count(), 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_move_constructor)
{
    bool valid = false;
    ircnt_data* data_ptr = new ircnt_data(valid);
    core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
    {
        core::intrusive_shared_ptr<ircnt_data> jptr(std::move(iptr));
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_EQ(jptr.get(), data_ptr);
        ASSERT_EQ(jptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_copy_assignment)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        {
            core::intrusive_shared_ptr<ircnt_data> jptr;
            jptr = iptr;
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->use_count(), 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_move_assignment)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        {
            core::intrusive_shared_ptr<ircnt_data> jptr;
            jptr = std::move(iptr);
            ASSERT_EQ(iptr.get(), nullptr);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 1);
            ASSERT_TRUE(valid);
        }
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_release)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        ircnt_data* d_ptr = iptr.release();
        ASSERT_EQ(d_ptr, data_ptr);
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_reset_nullptr)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        iptr.reset();
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_reset_other_ptr)
{
    bool valid = false;
    bool other_valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        ircnt_data* other_data_ptr = new ircnt_data(other_valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        iptr.reset(other_data_ptr);
        ASSERT_EQ(iptr.get(), other_data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(other_valid);
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
    ASSERT_FALSE(other_valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_deref)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        ircnt_data& data_ref = *data_ptr;
        ASSERT_EQ(&data_ref, data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_deref_const)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        const core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        const ircnt_data& data_ref = *data_ptr;
        ASSERT_EQ(&data_ref, data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_cast_to_ptr)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        ircnt_data* d_ptr = data_ptr;
        ASSERT_EQ(d_ptr, data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_cast_to_cptr)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        const core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        const ircnt_data* d_ptr = data_ptr;
        ASSERT_EQ(d_ptr, data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_arrow)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid, "arba::core");
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        ASSERT_EQ(iptr->text, "arba::core");
        iptr->text = "arba_core";
        ASSERT_EQ(&iptr->text, &data_ptr->text);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_arrow_const)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid, "arba::core");
        const core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        ASSERT_EQ(iptr->text, "arba::core");
        ASSERT_EQ(&iptr->text, &data_ptr->text);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_cast_to_bool)
{
    bool valid = false;
    {
        ircnt_data* data_ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
        bool test_bool = iptr;
        ASSERT_TRUE(test_bool);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
        iptr.reset();
        test_bool = iptr;
        ASSERT_FALSE(test_bool);
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_cast_swap)
{
    bool valid = false;
    ircnt_data* data_ptr = new ircnt_data(valid);
    core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
    ircnt_data* other_data_ptr = new ircnt_data(valid);
    core::intrusive_shared_ptr<ircnt_data> jptr(other_data_ptr);
    iptr.swap(jptr);
    ASSERT_EQ(iptr.get(), other_data_ptr);
    ASSERT_EQ(iptr->use_count(), 1);
    ASSERT_TRUE(valid);
    ASSERT_EQ(jptr.get(), data_ptr);
    ASSERT_EQ(jptr->use_count(), 1);
    ASSERT_TRUE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_eq)
{
    bool valid = false;
    ircnt_data* data_ptr = new ircnt_data(valid);
    core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
    core::intrusive_shared_ptr<ircnt_data> jptr(data_ptr);
    ASSERT_EQ(iptr, jptr);
    ASSERT_EQ(iptr->use_count(), 2);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_operator_ne)
{
    bool valid = false;
    ircnt_data* data_ptr = new ircnt_data(valid);
    core::intrusive_shared_ptr<ircnt_data> iptr(data_ptr);
    ircnt_data* other_data_ptr = new ircnt_data(valid);
    core::intrusive_shared_ptr<ircnt_data> jptr(other_data_ptr);
    ASSERT_NE(iptr, jptr);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_hash)
{
    bool valid = true;
    {
        ircnt_data* ptr = new ircnt_data(valid);
        core::intrusive_shared_ptr<ircnt_data> iptr(ptr);
        ASSERT_EQ(std::hash<core::intrusive_shared_ptr<ircnt_data>>{}(iptr), std::hash<core::intrusive_shared_ptr<ircnt_data>>{}(ptr));
    }
    ASSERT_FALSE(valid);
}

TEST(intrusive_shared_ptr_ircnt_data_tests, test_iptr_set_and_uset)
{
    bool b_set = true;
    bool b_uset = true;
    {
        std::set<core::intrusive_shared_ptr<ircnt_data>> iptr_set;
        iptr_set.insert(core::intrusive_shared_ptr<ircnt_data>(new ircnt_data(b_set)));
        std::unordered_set<core::intrusive_shared_ptr<ircnt_data>> iptr_uset;
        iptr_uset.insert(core::intrusive_shared_ptr<ircnt_data>(new ircnt_data(b_uset)));
    }
    ASSERT_FALSE(b_set);
    ASSERT_FALSE(b_uset);
}

//-------------------------------------------------------

TEST(intrusive_shared_ptr_ircnt_data_tests, test_make_isptr)
{
    bool valid = false;
    {
        core::intrusive_shared_ptr<ircnt_data> iptr = core::make_intrusive_shared_ptr<ircnt_data>(valid, "make_iptr");
        ASSERT_TRUE(valid);
        ASSERT_NE(iptr.get(), nullptr);
        ASSERT_EQ(iptr->text, "make_iptr");
    }
    ASSERT_FALSE(valid);
}

struct mkiptr_data : public core::intrusive_ref_counter<mkiptr_data>
{
    std::string text;
    bool* valid = nullptr;

    explicit mkiptr_data(bool& valid, std::string&& text) : text(std::forward<std::string>(text)), valid(&valid) { valid = true; }
    ~mkiptr_data() { *valid = false; }
};

TEST(intrusive_shared_ptr_ircnt_data_tests, test_make_isptr_2)
{
    bool valid = false;
    {
        std::string str("move!");
        core::intrusive_shared_ptr<mkiptr_data> iptr = core::make_intrusive_shared_ptr<mkiptr_data>(valid, std::move(str));
        ASSERT_TRUE(valid);
        ASSERT_NE(iptr.get(), nullptr);
        ASSERT_EQ(iptr->text, "move!");
        ASSERT_EQ(str, "");
    }
    ASSERT_FALSE(valid);
}
