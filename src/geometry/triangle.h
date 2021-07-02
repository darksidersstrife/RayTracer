//
// Created by vvlla on 07.04.2021.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

template<typename T, size_t size, size_t alignment = alignof(T)>
struct Triangle
{
    using PointType = Point<T, size, alignment>;
    using VectorType = Vector<T, size, alignment>;

    PointType p1;
    PointType p2;
    PointType p3;

    VectorType n1;
    VectorType n2;
    VectorType n3;

    constexpr Triangle() =default;
    constexpr Triangle(const PointType& _p1, const PointType& _p2, const PointType& _p3)
            : p1(_p1), p2(_p2), p3(_p3), n1(cross(p2 - p1, p3 - p1)) {}
    constexpr Triangle(const PointType& _p1, const PointType& _p2, const PointType& _p3,
                       const VectorType& _n1, const VectorType& _n2, const VectorType& _n3)
            : p1(_p1), p2(_p2), p3(_p3), n1(n1), n2(_n2), n3(_n3) {};

    struct hit_info
    {
        T t;
        VectorType normal;
    };

    constexpr hit_info hit(const Ray<T, size, alignment>& ray) const
    {
        auto v1 = p2 - p1, v2 = p3 - p1;
        auto n = cross(v1, v2);
        if (n * ray.direction > 0)
            return {-1};

        auto d = (p1 - PointType(0, 0, 0))*n;

        auto t = (d - n * (ray.origin - PointType(0, 0, 0))) / (n * ray.direction);
        if (t < 0)
            return {-1};

        auto p = ray.origin + t * ray.direction;
        v1 = p - p1;
        v2 = p - p2;
        auto v3 = p - p3;

        if (cross(v1, v2) * n < 0 || cross(v2, v3) * n < 0 || cross(v3, v1) * n < 0)
            return {-1};

        return hit_info{t, n};
    }
};

#endif //RAYTRACER_TRIANGLE_H
