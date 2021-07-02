//
// Created by vvlla on 07.04.2021.
//

#ifndef RAYTRACER_TRIANGLEMODEL_H
#define RAYTRACER_TRIANGLEMODEL_H

#include "../geometry/triangle.h"
#include "../material/complex.h"
#include <vector>
#include <algorithm>

template<typename TriangleType>
class TriangleModel
{
public:
//private:
    std::vector<TriangleType> triangles;
public:
    using T = decltype(triangles[0].p1[0] + triangles[0].p1[0]);



    template<typename RayType>
    auto trace_ray(const RayType& ray, float min)
    {
        struct hit_info
        {
            TriangleType* primitive;
            decltype(triangles[0].hit(ray)) p;
        };

        hit_info hit;
        hit.p.t = -1;

        for(auto& tr : triangles)
        {
            auto h = tr.hit(ray);
            if (h.t > min && (hit.p.t == -1 || hit.p.t > h.t))
                hit = {&tr, h};
        }
        return hit;
    }

    complex material;
};

#endif //RAYTRACER_TRIANGLEMODEL_H
