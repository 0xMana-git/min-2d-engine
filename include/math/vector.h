#pragma once

#include "misc/typedefs.h"
#include <cmath>

namespace Engine {
    template <typename T>
    struct _Vec2 {
        T x = 0, y = 0;

        _Vec2(){};
        _Vec2(T x, T y){
            this->x = x;
            this->y = y;
        }
        
        T GetLengthSqr() {
            return x * x + y * y;
        }

        T GetLength() {
            return std::sqrt(x * x + y * y);
        }


        void AddFrom(const _Vec2& other){
            x += other.x;
            y += other.y;
        }

        void SubFrom(const _Vec2& other) {
            x -= other.x;
            y -= other.y;
        }

        void ScaleFrom(T v) {
            x *= v;
            y *= v;
        }

        void DivideFrom(T v) {
            x /= v;
            y /= v;
        }
        
        void Normalize() {
            //Assert len != 0 here
            DivideFrom(GetLength());
        }

        _Vec2 Add(const _Vec2& other) const {
            return _Vec2(x + other.x, y + other.y);
        }
        _Vec2 Sub(const _Vec2& other) const {
            return _Vec2(x - other.x, y - other.y);
        }
        _Vec2 GetNegated() const {
            return _Vec2(-x, -y);
        }

        _Vec2 Scale(T v) const {
            return _Vec2(x * v, y * v);
        }

        _Vec2 Divide(T v) const {
            return _Vec2(x / v, y / v);
        }

        _Vec2 GetNormalized() const {
            //Assert len != 0 here
            return Divide(GetLength());
        }        

        void operator+=(const _Vec2& other) {
            AddFrom(other);
        }
        
        void operator-=(const _Vec2& other) {
            SubFrom(other);
        }

        void operator*=(T v) {
            ScaleFrom(v);
        }

        void operator/=(T v) {
            DivideFrom(v);
        }

        _Vec2 operator+(const _Vec2& other) const {
            return Add(other);
        }
        
        _Vec2 operator-(const _Vec2& other) const {
            return Sub(other);
        }
        _Vec2 operator-() const {
            return GetNegated();
        }

        _Vec2 operator*(T v) const {
            return Scale(v);
        }

        _Vec2 operator/(T v) const {
            return Divide(v);
        }
        
        T Dot(const _Vec2& other) const{
            return x * other.x + y * other.y;
        }

        T operator*(const _Vec2& other) const {
            return Dot(other);
        }
        

        

    };
    using Vec2d = _Vec2<double>;
    using Vec2i = _Vec2<int>;
    using Vec2l = _Vec2<int64>;
    using Vec2 = _Vec2<num_t>;
}

