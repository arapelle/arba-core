#include <arba/core/type_traits.hpp>
#include <gtest/gtest.h>

struct abstract_scope_bound_data
{
    virtual ~abstract_scope_bound_data() = default;
};

struct scope_bound_data : public abstract_scope_bound_data
{
    bool* valid = nullptr;

    constexpr explicit scope_bound_data(bool& valid) : valid(&valid) { valid = true; }
    virtual ~scope_bound_data() override
    {
        *valid = false;
    }
};

TEST(type_traits_tests, test_is_shared_ptr_move_assignable_to_itself)
{
    bool valid = false;
    {
        scope_bound_data* ptr = new scope_bound_data(valid);
        ASSERT_TRUE(valid);
        std::shared_ptr<scope_bound_data> smart_ptr(ptr);
        smart_ptr = std::move(smart_ptr);
        ASSERT_EQ(smart_ptr.get(), ptr);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(type_traits_tests, test_is_unique_ptr_move_assignable_to_itself)
{
    bool valid = false;
    {
        scope_bound_data* ptr = new scope_bound_data(valid);
        ASSERT_TRUE(valid);
        std::unique_ptr<scope_bound_data> smart_ptr(ptr);
        smart_ptr = std::move(smart_ptr);
        ASSERT_EQ(smart_ptr.get(), ptr);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

