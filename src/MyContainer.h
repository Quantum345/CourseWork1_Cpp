#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <string>
#include <exception>

template <typename T>
class MyContainer {
private:
    T* data = nullptr;
    std::size_t _size = 0;
    std::size_t _capacity = 0;

    void _reallocate() {
        if (data == nullptr) {
            data = new T[++_capacity];
            return;
        }
        size_t newCapacity = _capacity * 2;
        T* tmp = new T[newCapacity];
        std::move(data, data + _size, tmp);
        delete[] data;
        data = tmp;
        _capacity = newCapacity;
    }

public:
    MyContainer() = default;
    MyContainer(const T* input, std::size_t inputSize)
        : _size(inputSize), _capacity(inputSize) {
        data = new T[_capacity]{};
        std::copy(input, input + inputSize, data);
    }
    MyContainer(const MyContainer& other)
        : _size(other._size), _capacity(other._capacity) {
        data = new T[_capacity]{};
        std::copy(other.data, other.data + other._size, data);
    }
    MyContainer(MyContainer&& other) noexcept
        : data(other.data), _size(other._size), _capacity(other._capacity) {
        other.data = nullptr;
        other._size = other._capacity = 0;
    }
    ~MyContainer() {
        delete[] data;
    }

    MyContainer& operator=(const MyContainer& other) {
        if (this != &other) {
            MyContainer tmp(other);
            swap(tmp);
        }
        return *this;
    }
    MyContainer& operator=(MyContainer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            _size = other._size;
            _capacity = other._capacity;
            other.data = nullptr;
            other._size = other._capacity = 0;
        }
        return *this;
    }

    template <bool IsConst>
    class MyIterator {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        constexpr MyIterator() noexcept : m_ptr(nullptr) {}
        constexpr explicit MyIterator(pointer ptr) noexcept : m_ptr(ptr) {}

        constexpr reference operator*() const noexcept {
            return *m_ptr;
        }
        constexpr pointer operator->() const noexcept {
            return m_ptr;
        }

        constexpr MyIterator& operator++() noexcept {
            ++m_ptr;
            return *this;
        }
        constexpr MyIterator& operator--() noexcept {
            --m_ptr;
            return *this;
        }

        constexpr MyIterator operator++(int) noexcept {
            MyIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        constexpr MyIterator operator--(int) noexcept {
            MyIterator tmp = *this;
            --(*this);
            return tmp;
        }

        constexpr MyIterator operator+(difference_type n) const noexcept { return MyIterator(m_ptr + n); }
        constexpr MyIterator operator-(difference_type n) const noexcept { return MyIterator(m_ptr - n); }
        constexpr difference_type operator-(const MyIterator& other) const noexcept { return m_ptr - other.m_ptr; }
        constexpr MyIterator& operator+=(difference_type n) noexcept { m_ptr += n; return *this; }
        constexpr MyIterator& operator-=(difference_type n) noexcept { m_ptr -= n; return *this; }
        constexpr reference operator[](difference_type n) const noexcept { return m_ptr[n]; }

        friend constexpr auto operator<=>(const MyIterator& a, const MyIterator& b) noexcept = default;

    private:
        pointer m_ptr;
    };

    using iterator = MyIterator<false>;
    using const_iterator = MyIterator<true>;

    constexpr iterator begin() noexcept { return iterator(data); }
    constexpr iterator end() noexcept { return iterator(data + _size); }

    constexpr const_iterator begin() const noexcept { return const_iterator(data); }
    constexpr const_iterator end() const noexcept { return const_iterator(data + _size); }

    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }

    constexpr std::size_t getSize() const noexcept { return _size; }
    constexpr std::size_t getCapacity() const noexcept { return _capacity; }
    constexpr bool isEmpty() const noexcept { return _size == 0; }

    constexpr T* data_ptr() noexcept { return data; }
    constexpr const T* data_ptr() const noexcept { return data; }

    // Without boundary check
    constexpr T& operator[](std::size_t i) noexcept { return data[i]; }
    // Without boundary check
    constexpr const T& operator[](std::size_t i) const noexcept { return data[i]; }

    // With boundary check
    constexpr T& at(std::size_t i) {
        if (i >= _size)
            throw std::out_of_range("Out of Range Error: index extends beyond the container's size");
        return data[i];
    }
    // With boundary check
    constexpr const T& at(std::size_t i) const {
        if (i >= _size)
            throw std::out_of_range("Out of Range Error: index extends beyond the container's size");
        return data[i];
    }

    void add(const T& element) {
        if (_size == _capacity)
            _reallocate();
        data[_size++] = element;
    }
    void add(T&& element) {
        if (_size == _capacity)
            _reallocate();
        data[_size++] = std::move(element);
    }

    void swap(MyContainer& other) noexcept {
        std::swap(data, other.data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    friend std::istream& operator>>(std::istream& is, MyContainer& c) {
        std::string classTag;
        size_t capacity = 0;
        size_t size = 0;

        if (!(is >> classTag >> capacity >> size)
            || classTag != "MyContainer"
            || capacity < size
            ) {
            is.setstate(std::ios::failbit);
            return is;
        }

        MyContainer<T> tmp;
        tmp._capacity = capacity;
        tmp._size = size;
        if (capacity) tmp.data = new T[capacity];
        for (size_t i = 0; i < size; ++i)
            if (!(is >> tmp.data[i])) {
                is.setstate(std::ios::failbit);
                return is;
            }
        c.swap(tmp);
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyContainer& c) {
        os << "MyContainer "
            << c._capacity << ' '
            << c._size;
        if (c._size == 0 || c.data == nullptr)
            return os;
        for (auto const& i : c)
            os << ' ' << i;
        return os;
    }
};
