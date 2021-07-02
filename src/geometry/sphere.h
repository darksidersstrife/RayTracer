//
// Created by vvlla on 22.03.2021.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

template<typename T, size_t size, size_t alignment = alignof(T)>
struct Sphere
{
    Point<T, size, alignment> position;
    T r;

    constexpr Sphere() =default;
    constexpr Sphere(const Point<T, size, alignment>& p, const T& r) : position(p), r(r) {}

    constexpr T hit(const Ray<T, size, alignment>& ray)
    {
        auto v = (ray.origin - position);
        auto c =  v * v - r * r;
        auto b = ray.direction * v;

        auto d = b * b - c;
        if (d < T(0))
            return T(-1);

        using std::sqrt;
        return sqrt(d) - b;
    }
};

template<size_t alignment = alignof(float)>
using Sphere3f = Sphere<float, 3, alignment>;

#endif //RAYTRACER_SPHERE_H
