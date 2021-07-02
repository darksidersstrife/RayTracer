//
// Created by vvlla on 21.03.2021.
//

#ifndef RAYTRACER_POINT_H
#define RAYTRACER_POINT_H

#include "arraybase.h"
#include "vector.h"

template<typename T, size_t size, size_t alignment>
class Vector;

template<typename T, size_t size, size_t alignment = alignof(T)>
class Point : protected ArrayBase<T, size, alignment>
{
public:
    using ArrayType = ArrayBase<T, size, alignment>;
    using PointType = Point<T, size, alignment>;
    using VectorType = Vector<T, size, alignment>;

    using ArrayBase<T, size, alignment>::ArrayBase;
    using ArrayType::operator[];

    constexpr PointType operator+(const VectorType& v) const
    {
        return PointType(*this) += v;
    }

    constexpr PointType& operator+=(const VectorType& v)
    {
        Base()+=v;
        return *this;
    }

    constexpr PointType operator-(const VectorType& v) const
    {
        return PointType(*this) -= v;
    }

    constexpr PointType& operator-=(const VectorType& v)
    {
        Base()-=v;
        return *this;
    }

    constexpr VectorType operator-(const PointType& p) const
    {
        VectorType res;
        //_mm_store_ps(&res[0], _mm_sub_ps(_mm_load_ps(&this->arr[0]), _mm_load_ps(&p[0])));
        for (int i = 0; i < size; ++i)
        {
            res[i] = this->arr[i] - p[i];
        }
        return res;
    }

private:
    using ArrayType::Base;
};

using Point3f = Point<float, 3>;

#endif //RAYTRACER_POINT_H
