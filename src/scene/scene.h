//
// Created by vvlla on 22.03.2021.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include "../geometry/vector.h"
#include "../material/spectral.h"

template<typename ObjectType, typename LightType>
class Scene
{
public:
    using T = typename ObjectType::T;

    template<typename RayType>
    auto trace_ray(const RayType& ray, T min)
    {
        struct object_hit
        {
            decltype(objects[0].trace_ray(ray, 0)) g_hit;
            ObjectType* object;
        } o_hit;
        o_hit.g_hit.p.t = -1;

        for(auto& o : objects)
        {
            auto g_hit = o.trace_ray(ray, min);
            if (g_hit.p.t > min && (o_hit.g_hit.p.t == -1 || o_hit.g_hit.p.t > g_hit.p.t))
                o_hit.g_hit = g_hit, o_hit.object = &o;
        }
        return o_hit;
    }

    std::vector<ObjectType> objects;
    std::vector<LightType> lights;
    size_t limit = 1;
};

#endif //RAYTRACER_SCENE_H
