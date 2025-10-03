#pragma once

#include <iterator>
#include <ranges>

inline namespace arba
{
namespace core
{

namespace private_
{
template <typename Iterator>
class regular_chunk_iterator_base_;

template <typename Iterator>
    requires(!std::ranges::sized_range<std::ranges::subrange<Iterator>>)
class regular_chunk_iterator_base_<Iterator>
{
public:
    using iterator_type = Iterator;

    explicit regular_chunk_iterator_base_()
        requires std::default_initializable<iterator_type>
    {
    }

    explicit regular_chunk_iterator_base_(iterator_type iter, std::size_t chunk_size)
        : chunk_size_(chunk_size), chunk_(std::move(iter), std::next(iter, chunk_size))
    {
    }

    [[nodiscard]] inline std::size_t chunk_size() const { return chunk_size_; }

private:
    std::size_t chunk_size_ = 0;

protected:
    std::ranges::subrange<Iterator> chunk_;
};

template <typename Iterator>
    requires std::ranges::sized_range<std::ranges::subrange<Iterator>>
class regular_chunk_iterator_base_<Iterator>
{
public:
    using iterator_type = Iterator;

    explicit regular_chunk_iterator_base_()
        requires std::default_initializable<iterator_type>
    {
    }

    explicit regular_chunk_iterator_base_(iterator_type iter, std::size_t chunk_size)
        : chunk_(std::move(iter), std::next(iter, chunk_size))
    {
    }

    [[nodiscard]] inline std::size_t chunk_size() const { return std::ranges::size(chunk_); }

protected:
    std::ranges::subrange<Iterator> chunk_;
};
} // namespace private_

template <std::forward_iterator Iterator>
class regular_chunk_iterator : public private_::regular_chunk_iterator_base_<Iterator>
{
private:
    using base_ = private_::regular_chunk_iterator_base_<Iterator>;

public:
    using iterator_type = Iterator;
    using difference_type = std::iter_difference_t<iterator_type>;
    using value_type = std::ranges::subrange<Iterator>;
    using pointer = std::add_pointer_t<value_type>;

    inline regular_chunk_iterator()
        requires std::default_initializable<iterator_type>
    {
    }

    explicit regular_chunk_iterator(iterator_type iter, std::size_t chunk_sz) : base_(std::move(iter), chunk_sz) {}

    using base_::chunk_size;

    regular_chunk_iterator& operator++()
    {
        iterator_type iter = this->chunk_.end();
        this->chunk_ = value_type(std::move(iter), std::next(iter, chunk_size()));
        return *this;
    }

    regular_chunk_iterator operator++(int)
    {
        regular_chunk_iterator aux(*this);
        ++(*this);
        return aux;
    }

    regular_chunk_iterator& operator--()
    {
        iterator_type iter = this->chunk_.end();
        this->chunk_ = value_type(std::move(iter), std::next(iter, -chunk_size()));
        return *this;
    }

    regular_chunk_iterator operator--(int)
    {
        regular_chunk_iterator aux(*this);
        --(*this);
        return aux;
    }

    [[nodiscard]] inline value_type operator*() { return this->chunk_; };
    [[nodiscard]] inline value_type operator*() const { return this->chunk_; };

    [[nodiscard]] inline pointer operator->() { return &this->chunk_; };
    [[nodiscard]] inline const pointer operator->() const { return &this->chunk_; };

    [[nodiscard]] inline bool operator==(const regular_chunk_iterator& rhs) const
    {
        return this->chunk_.begin() == rhs.chunk_.begin() && this->chunk_.end() == rhs.chunk_.end();
    }
    [[nodiscard]] inline bool operator!=(const regular_chunk_iterator& rhs) const { return !(*this == rhs); }

    [[nodiscard]] inline bool operator<(const regular_chunk_iterator& rhs) const
        requires std::random_access_iterator<iterator_type>
    {
        std::size_t chunk_sz = chunk_size();
        if (chunk_sz != rhs.chunk_sz) [[unlikely]]
            throw std::runtime_error("Problem when comparing regular_chunk_iterator: chunk sizes are different.");
        return this->chunk_.cbegin() < rhs.chunk_.cbegin();
    }
    [[nodiscard]] inline bool operator<=(const regular_chunk_iterator& rhs) const
        requires std::random_access_iterator<iterator_type>
    {
        return (*this < rhs) || (*this == rhs);
    }
    [[nodiscard]] inline bool operator>(const regular_chunk_iterator& rhs) const
        requires std::random_access_iterator<iterator_type>
    {
        return !(*this <= rhs);
    }
    [[nodiscard]] inline bool operator>=(const regular_chunk_iterator& rhs) const
        requires std::random_access_iterator<iterator_type>
    {
        return !(*this < rhs);
    }

    regular_chunk_iterator& operator+=(const difference_type& offset)
        requires std::random_access_iterator<iterator_type>
    {
        iterator_type iter = std::next(this->chunk_.begin(), (offset * chunk_size()));
        this->chunk_ = value_type(std::move(iter), std::next(iter, chunk_size()));
        return *this;
    }

    regular_chunk_iterator& operator-=(const difference_type& offset)
        requires std::random_access_iterator<iterator_type>
    {
        iterator_type iter = std::prev(this->chunk_.begin(), (offset * chunk_size()));
        this->chunk_ = value_type(std::move(iter), std::next(iter, chunk_size()));
        return *this;
    }

    [[nodiscard]] difference_type operator-(const regular_chunk_iterator& other) const
        requires std::random_access_iterator<iterator_type>
    {
        std::size_t chunk_sz = chunk_size();
        if (chunk_sz != other.chunk_size()) [[unlikely]]
            throw std::runtime_error(
                "Problem when computing regular_chunk_iterator difference: chunk sizes are different.");
        const difference_type diff = this->chunk_.begin() - other.chunk_.begin();
        if (diff % chunk_sz != 0) [[unlikely]]
            throw std::runtime_error(
                "Problem when computing regular_chunk_iterator difference: chunks are not aligned.");
        return diff / chunk_sz;
    }

    [[nodiscard]] decltype(auto) operator[](const difference_type& offset)
        requires std::random_access_iterator<iterator_type>
    {
        iterator_type iter = std::next(this->chunk_.end(), (offset * chunk_size()));
        return value_type(std::move(iter), std::next(iter, chunk_size()));
    }

    [[nodiscard]] decltype(auto) operator[](const difference_type& offset) const
        requires std::random_access_iterator<iterator_type>
    {
        iterator_type iter = std::next(this->chunk_.end(), (offset * chunk_size()));
        return value_type(std::move(iter), std::next(iter, chunk_size()));
    }
};

template <typename Iterator>
    requires requires(regular_chunk_iterator<Iterator> iter,
                      const typename regular_chunk_iterator<Iterator>::difference_type& offset) {
        { iter += offset } -> std::same_as<regular_chunk_iterator<Iterator>&>;
    }
[[nodiscard]] inline regular_chunk_iterator<Iterator>
operator+(regular_chunk_iterator<Iterator> iter,
          const typename regular_chunk_iterator<Iterator>::difference_type& offset)
{
    iter += offset;
    return iter;
}

template <typename Iterator>
    requires requires(const typename regular_chunk_iterator<Iterator>::difference_type& offset,
                      regular_chunk_iterator<Iterator> iter) {
        { iter += offset } -> std::same_as<regular_chunk_iterator<Iterator>&>;
    }
[[nodiscard]] inline regular_chunk_iterator<Iterator>
operator+(const typename regular_chunk_iterator<Iterator>::difference_type& offset,
          regular_chunk_iterator<Iterator> iter)
{
    iter += offset;
    return iter;
}

template <typename Iterator>
    requires requires(regular_chunk_iterator<Iterator> iter,
                      const typename regular_chunk_iterator<Iterator>::difference_type& offset) {
        { iter -= offset } -> std::same_as<regular_chunk_iterator<Iterator>&>;
    }
[[nodiscard]] inline regular_chunk_iterator<Iterator>
operator-(regular_chunk_iterator<Iterator> iter,
          const typename regular_chunk_iterator<Iterator>::difference_type& offset)
{
    iter -= offset;
    return iter;
}

template <std::ranges::view View>
    requires std::ranges::forward_range<View>
class regular_chunk_view : public std::ranges::view_interface<regular_chunk_view<View>>
{
public:
    using iterator = regular_chunk_iterator<std::ranges::iterator_t<View>>;

    regular_chunk_view()
        requires std::default_initializable<View>
    {
    }

    explicit regular_chunk_view(View view, std::ranges::range_difference_t<View> chunk_size)
        : view_(std::move(view)), chunk_size_(chunk_size)
    {
    }

    [[nodiscard]] inline iterator begin() const { return iterator(view_.begin(), chunk_size_); }

    [[nodiscard]] inline iterator end() const
    {
        std::size_t view_size = 0;
        if constexpr (std::ranges::sized_range<View>)
        {
            view_size = std::ranges::size(view_);
        }
        else
        {
            view_size = std::distance(view_.begin(), view_.end());
        }
        auto end_iter = std::next(view_.begin(), (view_size / chunk_size_) * chunk_size_);
        return iterator(std::move(end_iter), chunk_size_);
    }

private:
    View view_;
    std::ranges::range_difference_t<View> chunk_size_ = 0;
};

namespace views
{
template <std::ranges::viewable_range Range>
auto regular_chunk(Range&& range, std::ranges::range_difference_t<Range> chunk_size)
{
    if constexpr (std::ranges::view<std::remove_cv_t<Range>>)
        return regular_chunk_view(std::forward<Range>(range), chunk_size);
    else
        return regular_chunk_view(std::views::all(std::forward<Range>(range)), chunk_size);
}

} // namespace views
} // namespace core
} // namespace arba
