#pragma once

#include "engine/math/vector.h"
#include <cmath>
#include <array>


namespace Engine {
    //Too lazy to make a generalized impl sorry
    template <typename T>
    struct _Matrix3x3 {
        constexpr static size_t row_n = 3;
        constexpr static size_t col_n = 3;
        using Row_t = std::array<T, row_n>;
        using Mat_t = std::array<Row_t, col_n>;

        using Vec2_t = _Vec2<T>;
        //column is [], row is [][]
        Mat_t _data;

        
        
        _Matrix3x3(Mat_t data) {
            _data = data;
        }
        _Matrix3x3() {
            for(Row_t& v : _data) {
                v.fill(0);
            }
        }
        static _Matrix3x3 FromRotation(num_t rotation) {
            T cosv = std::cos(rotation), sinv = std::sin(rotation);
            return _Matrix3x3({
                Row_t{cosv, -sinv, 0},
                Row_t{sinv, cosv,  0},
                Row_t{0,    0,     1}
            });
        };
        
        static _Matrix3x3 FromTranslationVec(const Vec2_t& vec) {
            //meme
            return _Matrix3x3({
                Row_t{1, 0, vec.x},
                Row_t{0, 1, vec.y},
                Row_t{0, 0, 1}
            });
        }

        Vec2_t Transform(const Vec2_t& orig) {
            std::array<T, 3> orig_v3 {orig.x, orig.y, 1}, res_v3{0, 0, 0};
            for(size_t i = 0; i < row_n; i++){
                for(size_t j = 0; j < col_n; j++){
                    res_v3[i] += orig_v3[j] * _data[i][j];
                }
            }
            return Vec2_t(res_v3[0], res_v3[1]);
        }

        //Credits: https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
        _Matrix3x3 Multiply(const _Matrix3x3& other) const {
            _Matrix3x3 res;
            for (int i = 0; i < row_n; i++) {
                for (int j = 0; j < other.col_n; j++) {
                    res._data[i][j] = 0;
                    for (int k = 0; k < other.row_n; k++) {
                        res._data[i][j] += _data[i][k] * other._data[k][j];
                    }

                }
            }
            return res;
        }

        _Matrix3x3 operator*(const _Matrix3x3& other) const {
            return Multiply(other);
        }
        
        

    };
        //Too lazy to make a generalized impl sorry
    template <typename T>
    struct _Matrix2x2 {
        constexpr static size_t row_n = 2;
        constexpr static size_t col_n = 2;
        using Row_t = std::array<T, row_n>;
        using Mat_t = std::array<Row_t, col_n>;
        //column is [], row is [][]
        Mat_t _data;

        //Credits: https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
        _Matrix2x2 Multiply(const _Matrix2x2& other) const {
            _Matrix2x2 res;
            for (int i = 0; i < row_n; i++) {
                for (int j = 0; j < other.col_n; j++) {
                    res._data[i][j] = 0;
                    for (int k = 0; k < other.row_n; k++) {
                        res._data[i][j] += _data[i][k] * other._data[k][j];
                    }

                }
            }
            return res;
        }
        
        _Matrix2x2(Mat_t data) {
            _data = data;
        }
        _Matrix2x2() {
            for(Row_t& v : _data) {
                v.fill(0);
            }
        }
        static _Matrix2x2 FromRotation(num_t rotation) {
            T cosv = std::cos(rotation), sinv = std::sin(rotation);
            return _Matrix2x2({
                Row_t{cosv, -sinv},
                Row_t{sinv, cosv}
            });
        };


    };

    using Matrix3x3d = _Matrix3x3<double>;
    using Matrix3x3 = _Matrix3x3<num_t>;
    using Matrix2x2 = _Matrix2x2<num_t>;
}