#pragma once

#include <array>
#include <span>
#include <vector>

template<typename T>
struct View {
    T* _begin = 0;
    T* _end = 0;
    using span_t = std::span<T>;
    using const_span_t = const span_t;
    using iterator = T*;
    using const_iterator = const T*;

    template<>
    void Init(std::vector<T>& vec) {
        _begin = &(*vec.begin());
        _end = &(*vec.end());
    }
    template<size_t len>
    void Init(std::array<T, len>& arr) {
        _begin = arr.begin();
        _end = arr.end();
    }

    View(T* begin, T* end) {
        _begin = begin;
        _end = end;
    }
    template<size_t len>
    View(std::array<T, len>& arr) {
        Init(arr);
    }
    View() {
        _begin = 0; _end = 0;
    }
    //OOB is a thing
    T& operator[](size_t index) {
        return *(_begin + index);
    }
    const T& operator[](size_t index) const {
        return *(_begin + index);
    }
    const std::span<T> GetConst() const {
        return std::span(_begin, _end);
    }
    std::span<T> Get() {
        return std::span(_begin, _end);
    }
    operator const_span_t() const {
        return GetConst();
    }
    operator span_t() {
        return Get();
    }
    size_t size() const {
        return _end - _begin;
    }
    iterator begin() { return _begin; }
    const_iterator begin() const { return _begin; }
    iterator end() { return _end; }
    const_iterator end() const { return _end; }
};