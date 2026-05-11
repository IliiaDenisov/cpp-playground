#pragma once
#include <utility>

template<typename T>
class unique_ptr {
    T* m_ptr = nullptr;
public:
    constexpr unique_ptr() = default;
    constexpr unique_ptr(T* ptr) : m_ptr{ptr} {}
    constexpr ~unique_ptr() noexcept {
        delete m_ptr;
    }
    constexpr unique_ptr(unique_ptr<T>&& other) {
        delete m_ptr;
        m_ptr = std::exchange(other.m_ptr, nullptr);
    }
    constexpr unique_ptr& operator=(unique_ptr<T>&& other) {
        delete m_ptr;
        m_ptr = std::exchange(other.m_ptr, nullptr);
        return *this;
    }

    constexpr explicit operator bool() const {
        return m_ptr != nullptr;
    }
};