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

    template<typename RayType>
    auto calculate_color(const RayType& ray, float min)
    {
        return calculate_color(ray, min, limit);
    }

    template<typename RayType>
    auto calculate_color(const RayType& ray, float min, size_t limit)
    {
        if (limit == 0)
            return spectral{};

        auto o_hit = trace_ray(ray, min);
        if (o_hit.g_hit.p.t < 0)
            return background(ray, min);


        auto n = o_hit.g_hit.p.normal;
        auto hit_point = ray.origin + o_hit.g_hit.p.t * ray.direction;

        spectral res;

        for(auto& l : lights)
        {
            auto light_dir = hit_point - l.position;
            if (light_dir * n > 0)
                continue;
            auto len = light_dir.length();
            light_dir /= len;
            auto h = trace_ray(decltype(ray){l.position, light_dir}, min);
            if (h.g_hit.primitive == o_hit.g_hit.primitive)
                res = res - o_hit.object->material.BRDF(-1.0f * ray.direction, light_dir, n) * l.spectral_intensity * (n * light_dir) / len / len;
        }

        auto new_direction = -2 * (n * ray.direction) * n + ray.direction;

        return res + o_hit.object->material.BRDF(-1.0f * ray.direction, new_direction, n) * (n * new_direction) *
        calculate_color(decltype(ray){hit_point, new_direction}, min, limit - 1);
    }

    template<typename RayType>
    auto calculate_color_debug(const RayType& ray, float min, size_t limit)
    {
        if (limit == 0)
            return spectral{};

        if (limit == 4)
            std::cout << 4;

        auto o_hit = trace_ray(ray, min);
        if (o_hit.g_hit.p.t < 0)
            return background(ray, min);


        auto n = o_hit.g_hit.p.normal;
        auto hit_point = ray.origin + o_hit.g_hit.p.t * ray.direction;

        spectral res;

        for(auto& l : lights)
        {
            auto light_dir = hit_point - l.position;
            if (light_dir * n > 0)
                continue;
            auto len = light_dir.length();
            light_dir /= len;
            auto h = trace_ray(decltype(ray){l.position, light_dir}, min);
            if (h.g_hit.primitive == o_hit.g_hit.primitive)
                res = res - o_hit.object->material.BRDF(-1.0f * ray.direction, light_dir, n) * l.spectral_intensity * (n * light_dir) / len / len;
        }

        auto new_direction = -2 * (n * ray.direction) * n + ray.direction;

        return res + o_hit.object->material.BRDF(-1.0f * ray.direction, new_direction, n) * (n * new_direction) *
                     calculate_color_debug(decltype(ray){hit_point, new_direction}, min, limit - 1);
    }

    template<typename RayType>
    auto background(const RayType& ray, T min)
    {
        auto t = 0.5f * (ray.direction[0] + 1.0f);
        auto v = (1.0f - t) * Vec3f(1.0f, 1.0f, 1.0f) + t * Vec3f(0.5f, 0.7f, 1.0f);
        spectral s;
        s.spectre[0] = v[0];
        s.spectre[1] = v[1];
        s.spectre[2] = v[2];

        return spectral{};
    }

    std::vector<ObjectType> objects;
    std::vector<LightType> lights;
    size_t limit = 3;
};

#endif //RAYTRACER_SCENE_H
