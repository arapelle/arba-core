//#include <arba/core/enum/enum.hpp>
#include <array>

#include <gtest/gtest.h>

//class Status : public core::enumeration<int8_t, core::no_implicit, Status>
//{
//public:
//    using core::enumeration<int8_t, core::no_implicit, Status>::enumeration;

//    static constexpr enumerator ok{0};
//    static constexpr enumerator error{1};
//    static constexpr enumerator fatal_error{2};
//};

//class Color : public core::enumeration<int8_t, core::implicit_integral, Color>
//{
//public:
//    using core::enumeration<int8_t, core::implicit_integral, Color>::enumeration;

//    static constexpr enumerator white{0};
//    static constexpr enumerator red{1};
//    static constexpr enumerator green{1};
//    static constexpr enumerator blue{1};
//};

//class Shape : public core::enumeration<int8_t, core::implicit_integral, Shape>
//{
//public:
//    using core::enumeration<int8_t, core::implicit_integral, Shape>::enumeration;

//    static constexpr enumerator circle{0};
//    static constexpr enumerator triangle{1};
//    static constexpr enumerator square{2};

//    static constexpr std::array enumerators = { circle, triangle, square, };
//    static constexpr std::array enumerator_strings = { "circle", "triangle", "square", };
//};

//TEST(enum_tests, test_)
//{
//    Status status = Status::ok;
////    ASSERT_EQ(status, status);

//    [[maybe_unused]] constexpr Color::int_type impossible = 42;

//    static_assert(Color::white.to_underlying_type() == 0);

//    Color color = Color::white;
//    Color color_2;
//    color_2 = Color::white;
//    int color_index = color;
////    color_2.to_string();
////    Color color_3(impossible); // checked
////    ASSERT_EQ(Color::white, Color::white);
////    static_assert(Color::white == Color::white);
////    ASSERT_EQ(color, Color::black);
////    ASSERT_GT(color, Color::white);

//    Shape shape = Shape::circle;
//    Shape shape_2;
//    shape_2 = Shape::square;
//    ASSERT_EQ(shape_2.to_string(), "square");

////    static_assert(Shape::square == Shape::square);
////    static_assert(Shape::square < Shape::triangle);
////    static_assert(Shape::triangle < Shape::square);

////    shape_2 = Shape(Color::white); // checked
////    Shape shape_3(impossible); // checked
//}

template <class EnumerationType>
class EnumeratorBase;

template <class EnumeratorType, class SelfType>
class Enumeration;

template <class SelfType>
class EnumerationBase
{
protected:
    template <class... ArgsTypes>
    static consteval auto make_enumerator_(int val, ArgsTypes&&... args)
    {
        return SelfType::make_enumerator__(EnumeratorBase<SelfType>(val), std::forward<ArgsTypes>(args)...);
    }
};

template <class EnumerationType>
class EnumeratorBase
{
public:
    constexpr EnumeratorBase() {}

private:
    consteval EnumeratorBase(int val) : value(val) {}
    friend EnumerationBase<EnumerationType>;

protected:
    int value;
};

template <class EnumerationType, class SelfType>
class Enumerator : public EnumeratorBase<EnumerationType>
{
public:
    constexpr Enumerator() {}
    consteval Enumerator(const EnumeratorBase<EnumerationType>& val) : EnumeratorBase<EnumerationType>(val) {}

    constexpr std::string_view to_string() const noexcept
        requires (requires{ {SelfType::enumerator_strings[std::declval<int>()]} -> std::convertible_to<std::string_view>; })
    {
        return SelfType::enumerator_strings[this->value];
    }
};

template <class EnumeratorType, class SelfType>
class Enumeration : public EnumerationBase<SelfType>
{
public:
    using enumerator = EnumeratorType;

protected:
    template <class... ArgsTypes>
    static consteval EnumeratorType make_enumerator(int val, ArgsTypes&&... args)
    {
        return EnumerationBase<SelfType>::make_enumerator_(val, std::forward<ArgsTypes>(args)...);
    }

private:
    template <class... ArgsTypes>
    static consteval EnumeratorType make_enumerator__(EnumeratorBase<SelfType> val, ArgsTypes&&... args)
    {
        return EnumeratorType(val, std::forward<ArgsTypes>(args)...);
    }

    using EnumerationBase<SelfType>::make_enumerator_;

    friend class EnumerationBase<SelfType>;
};

// --------

class Cities;

class City : public Enumerator<Cities, City>
{
public:
    constexpr City() {}
    consteval City(const EnumeratorBase<Cities>& val) : Enumerator<Cities, City>(val) {}

    static constexpr std::array enumerator_strings = { "paris", "london" };
};

class Cities : public Enumeration<City, Cities>
{
public:
    static constexpr City paris = make_enumerator(0);
    static constexpr City london = make_enumerator(1);

    static constexpr std::size_t size() noexcept { return 2; }
};

TEST(enum_tests, test_Enum)
{
    City city;
    city = Cities::paris;
    ASSERT_EQ(city.to_string(), "paris");
}
