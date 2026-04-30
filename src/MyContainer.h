#pragma once
#include <iterator>
#include <type_traits>
#include <stdexcept>

template <typename T>
class MyContainer {
private:
    T* data = nullptr;
    std::size_t capacity = 0;
public:
    // Non-owning: caller retains ownership of the memory pointed by 'input'.
    MyContainer(T* input, std::size_t size) noexcept : data(input), capacity(size) {}

    template <bool IsConst>
    class MyIteratorImpl {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        constexpr MyIteratorImpl() noexcept : m_ptr(nullptr) {}
        constexpr explicit MyIteratorImpl(pointer ptr) noexcept : m_ptr(ptr) {}

        constexpr reference operator*() const noexcept {
            return *m_ptr;
        }
        constexpr pointer operator->() const noexcept {
            return m_ptr;
        }

        constexpr MyIteratorImpl& operator++() noexcept {
            ++m_ptr;
            return *this;
        }
        constexpr MyIteratorImpl& operator--() noexcept {
            --m_ptr;
            return *this;
        }

        constexpr MyIteratorImpl operator++(int) noexcept {
            MyIteratorImpl tmp = *this;
            ++(*this);
            return tmp;
        }
        constexpr MyIteratorImpl operator--(int) noexcept {
            MyIteratorImpl tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr MyIteratorImpl operator+(difference_type n) const noexcept { return MyIteratorImpl(m_ptr + n); }
        constexpr MyIteratorImpl operator-(difference_type n) const noexcept { return MyIteratorImpl(m_ptr - n); }
        constexpr difference_type operator-(const MyIteratorImpl& other) const noexcept { return m_ptr - other.m_ptr; }
        constexpr MyIteratorImpl& operator+=(difference_type n) noexcept { m_ptr += n; return *this; }
        constexpr MyIteratorImpl& operator-=(difference_type n) noexcept { m_ptr -= n; return *this; }
        constexpr reference operator[](difference_type n) const noexcept { return m_ptr[n]; }

        friend constexpr bool operator==(const MyIteratorImpl& a, const MyIteratorImpl& b) noexcept { return a.m_ptr == b.m_ptr; }
        friend constexpr bool operator!=(const MyIteratorImpl& a, const MyIteratorImpl& b) noexcept { return a.m_ptr != b.m_ptr; }
        friend constexpr bool operator<(const MyIteratorImpl& a, const MyIteratorImpl& b) noexcept { return a.m_ptr < b.m_ptr; }
        friend constexpr bool operator<=(const MyIteratorImpl& a, const MyIteratorImpl& b) noexcept { return a.m_ptr <= b.m_ptr; }
        friend constexpr bool operator>(const MyIteratorImpl& a, const MyIteratorImpl& b) noexcept { return a.m_ptr > b.m_ptr; }
        friend constexpr bool operator>=(const MyIteratorImpl& a, const MyIteratorImpl& b) noexcept { return a.m_ptr >= b.m_ptr; }

    private:
        pointer m_ptr;
    };

    using iterator = MyIteratorImpl<false>;
    using const_iterator = MyIteratorImpl<true>;

    constexpr iterator begin() noexcept { return iterator(data); }
    constexpr iterator end() noexcept { return iterator(data + capacity); }

    constexpr const_iterator begin() const noexcept { return const_iterator(data); }
    constexpr const_iterator end() const noexcept { return const_iterator(data + capacity); }

    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }

    constexpr std::size_t size() const noexcept { return capacity; }
    constexpr bool empty() const noexcept { return capacity == 0; }

    // Non-owning: returns the raw pointer (do not delete from caller)
    constexpr T* data_ptr() noexcept { return data; }
    // Non-owning: returns the raw pointer (do not delete from caller)
    constexpr const T* data_ptr() const noexcept { return data; }

    constexpr T& operator[](std::size_t i) noexcept { return data[i]; }
    constexpr const T& operator[](std::size_t i) const noexcept { return data[i]; }

    constexpr T& at(std::size_t i) {
        if (i >= capacity)
            throw std::out_of_range("Out of Range Error: index extends beyond the container's size");
        return data[i];
    }
    constexpr const T& at(std::size_t i) const {
        if (i >= capacity)
            throw std::out_of_range("Out of Range Error: index extends beyond the container's size");
        return data[i];
    }
};
