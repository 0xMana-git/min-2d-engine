#pragma once

#include <array>
#include <span>

template<typename T>
struct View {
    T* _begin = 0;
    T* _end = 0;

    template<size_t len>
    void Init(const std::array<T, len>& arr) {
        _begin = arr.begin();
        _end = arr.end();
    }
    View(T* begin, T* end) {
        _begin = begin;
        _end = end;
    }
    template<size_t len>
    View(const std::array<T, len>& arr) {
        Init(arr);
    }
    View() {
        _begin = 0; _end = 0;
    }
    const std::span<T> GetConst() const {
        return std::span(_begin, _end);
    }
    std::span<T> Get() {
        return std::span(_begin, _end);
    }
};