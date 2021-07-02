//
// Created by vvlla on 21.03.2021.
//

#ifndef RAYTRACER_VECTOR_H
#define RAYTRACER_VECTOR_H

#include <array>
#include <intrin.h>
#include "point.h"

template<typename T, size_t size, size_t alignment>
class Point;

template<typename T, size_t size, size_t alignment = alignof(T)>
class Vector : protected ArrayBase<T, size,  alignment>
{
public:
    using ArrayType = ArrayBase<T, size, alignment>;
    using PointType = Point<T, size, alignment>;
    using VectorType = Vector<T, size, alignment>;

    using ArrayBase<T, size, alignment>::ArrayBase;
    using ArrayType::operator[];

    constexpr Vector(const PointType& p1, const PointType& p2) : Vector(p1, p2, std::make_index_sequence<size>()) {};


    constexpr VectorType operator+(const VectorType& v) const
    {
        return VectorType(*this) += v;
    }

    constexpr VectorType& operator+=(const VectorType& v)
    {
        Base()+=v;
        return *this;
    }

    constexpr VectorType operator-(const VectorType& v) const
    {
        return VectorType(*this) -= v;
    }

    constexpr VectorType& operator-=(const VectorType& v)
    {
        Base()-=v;
        return *this;
    }

    constexpr VectorType operator*(const T& val) const
    {
        return VectorType(*this) *= val;
    }

    constexpr VectorType& operator*=(const T& val)
    {
        for (size_t i = 0; i < size; ++i)
        {
            this->arr[i] *= val;
        }
        return *this;
    }

    constexpr VectorType operator/(const T& val) const
    {
        return VectorType(*this) /= val;
    }

    constexpr VectorType& operator/=(const T& val)
    {
        for (size_t i = 0; i < size; ++i)
        {
            this->arr[i] /= val;
        }
        return *this;
    }

    constexpr T operator*(const VectorType & v) const
    {
        T res{};
        //_mm_store_ps1(&res, _mm_dp_ps(_mm_load_ps(&this->arr[0]), _mm_load_ps(&v[0]), (7 << 4) + 1));
        for (size_t i = 0; i < size; ++i)
        {
            res += (*this)[i] * v[i];
        }
        return res;
    }

    constexpr VectorType operator+(const PointType& p) const
    {
        return p + *this;
    }

    constexpr T squared_length() const
    {
        return *this * *this;
    }

    constexpr T length() const
    {
        return sqrt(squared_length());
    }

    constexpr VectorType& normalize()
    {
        auto len = length();
        if (len != 0)
            *this /= len;

        return *this;
    }

    constexpr VectorType get_normalized() const
    {
        return VectorType(*this).normalize();
    }

private:
    using ArrayType::Base;

    template<size_t... index>
    constexpr Vector(const PointType& p1, const PointType& p2, std::index_sequence<index...>) : ArrayBase<T, size, alignment>((p1[index ]- p2[index])...) {};
};

template<typename T, size_t size, size_t alignment>
constexpr Vector<T, size, alignment> operator*(const T& a, const Vector<T, size, alignment>& vec)
{
    return vec * a;
}

template<typename T>
constexpr T cross(const T& v1, const T& v2)
{
    return T{v1[1] * v2[2] - v1[2] * v2[1],
                 v1[2] * v2[0] - v1[0] * v2[2],
                 v1[0] * v2[1] - v1[1] * v2[0]};
}

using Vec3f = Vector<float, 3>;

#endif //RAYTRACER_VECTOR_H
