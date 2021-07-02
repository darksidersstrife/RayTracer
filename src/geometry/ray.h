//
// Created by vvlla on 22.03.2021.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vector.h"
#include "point.h"

template<typename T, size_t size, size_t alignment = alignof(T)>
struct Ray
{
    Vector<T, size, alignment> direction;
    Point<T, size, alignment> origin;

    constexpr Ray() =default;
    constexpr Ray(const Point<T, size, alignment>& origin, const Vector<T, size, alignment>& direction) :origin(origin), direction(direction) {}
};


template<size_t alignment = alignof(float)>
using Ray3f = Ray<float, 3, alignment>;

#endif //RAYTRACER_RAY_H
