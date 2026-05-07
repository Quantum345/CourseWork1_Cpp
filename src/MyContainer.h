#pragma once
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <string>

template <typename T>
class MyContainer {
private:
    T* data = nullptr;
    std::size_t size = 0;
    std::size_t capacity = 0;

    constexpr void _reallocate() {
        if (data == nullptr) {
            data = new T[++capacity];
            return;
        }
        size_t newCapacity = capacity * 2;
        auto* tmp = new T[newCapacity]{};
        std::move(data, data + size, tmp);
        delete[] data;
        data = tmp;
        capacity = newCapacity;
        tmp = nullptr;
    }
public:
    MyContainer() = default;
    MyContainer(const T* input, std::size_t inputSize) : size(inputSize), capacity(inputSize) {
        data = new T[capacity]{};
        std::copy(input, input + inputSize, this->data);
    }
    MyContainer(const MyContainer& container) : size(container.size), capacity(container.capacity) {
        data = new T[capacity]{};
        std::copy(container.data, container.data + container.size, this->data);
    }
    MyContainer(MyContainer&& container) noexcept : data(container.data), size(container.size),  capacity(container.capacity) {
        container.data = nullptr;
        container.size = 0;
        container.capacity = 0;
    }
    ~MyContainer() {
        delete[] data;
    }

    MyContainer& operator=(const MyContainer& container) {
        if (this != &container) {
            delete[] data;
            size = container.size;
            capacity = container.capacity;
            data = new T[capacity]{};
            std::copy(container.data, container.data + container.size, this->data);
        }
        return *this;
    }
    MyContainer& operator=(MyContainer&& container) noexcept {
        if (this != &container) {
            delete[] data;
            data = container.data;
            size = container.size;
            capacity = container.capacity;
            container.data = nullptr;
            container.size = 0;
            container.capacity = 0;
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

        friend constexpr bool operator==(const MyIterator& a, const MyIterator& b) noexcept { return a.m_ptr == b.m_ptr; }
        friend constexpr bool operator!=(const MyIterator& a, const MyIterator& b) noexcept { return a.m_ptr != b.m_ptr; }
        friend constexpr bool operator<(const MyIterator& a, const MyIterator& b) noexcept { return a.m_ptr < b.m_ptr; }
        friend constexpr bool operator<=(const MyIterator& a, const MyIterator& b) noexcept { return a.m_ptr <= b.m_ptr; }
        friend constexpr bool operator>(const MyIterator& a, const MyIterator& b) noexcept { return a.m_ptr > b.m_ptr; }
        friend constexpr bool operator>=(const MyIterator& a, const MyIterator& b) noexcept { return a.m_ptr >= b.m_ptr; }

    private:
        pointer m_ptr;
    };

    using iterator = MyIterator<false>;
    using const_iterator = MyIterator<true>;

    constexpr iterator begin() noexcept { return iterator(data); }
    constexpr iterator end() noexcept { return iterator(data + size); }

    constexpr const_iterator begin() const noexcept { return const_iterator(data); }
    constexpr const_iterator end() const noexcept { return const_iterator(data + size); }

    constexpr const_iterator cbegin() const noexcept { return begin(); }
    constexpr const_iterator cend() const noexcept { return end(); }

    constexpr std::size_t getSize() const noexcept { return size; }
    constexpr std::size_t getCapacity() const noexcept { return capacity; }
    constexpr bool isEmpty() const noexcept { return size == 0; }

    constexpr T* data_ptr() noexcept { return data; }
    constexpr const T* data_ptr() const noexcept { return data; }

    constexpr T& operator[](std::size_t i) noexcept { return data[i]; }
    constexpr const T& operator[](std::size_t i) const noexcept { return data[i]; }

    constexpr T& at(std::size_t i) {
        if (i >= size)
            throw std::out_of_range("Out of Range Error: index extends beyond the container's size");
        return data[i];
    }
    constexpr const T& at(std::size_t i) const {
        if (i >= size)
            throw std::out_of_range("Out of Range Error: index extends beyond the container's size");
        return data[i];
    }

    constexpr void add(const T& element) {
        if (size == capacity)
            _reallocate();
        data[size++] = element;
    }
    constexpr void add(T&& element) {
        if (size == capacity)
            _reallocate();
        data[size++] = std::move(element);
    }

    friend std::istream& operator>>(std::istream& is, MyContainer& c) {
        std::string classTag;
        size_t capacity = 0;
        size_t size = 0;

        if (!(is >> classTag)
            || classTag != "MyContainer"
            || !(is >> capacity)
            || !(is >> size)
            || (capacity < size)) {
            is.setstate(std::ios::failbit);
            return is;
        }

        T* newData = nullptr;
        if (capacity > 0 && size > 0)
            newData = new T[capacity];

        for (size_t i = 0; i < size; ++i)
            if (!(is >> newData[i])) {
                delete[] newData;
                is.setstate(std::ios::failbit);
                return is;
            }

        delete[] c.data;
        c.capacity = capacity;
        c.size = size;
        c.data = newData;
        newData = nullptr;
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyContainer& c) {
        os << "MyContainer "
            << c.capacity << ' '
            << c.size;
        if (c.size == 0 || c.data == nullptr)
            return os;
        for (auto const& i : c)
            os << ' ' << i;
        return os;
    }
};
