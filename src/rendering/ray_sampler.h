//
// Created by vvlla on 10.10.2021.
//

#ifndef RAYTRACER_RAY_SAMPLER_H
#define RAYTRACER_RAY_SAMPLER_H
template<typename SceneType>
class RaySampler {

public:
    RaySampler(const SceneType* scene, double min) : scene(scene), min(min) {};

    template<typename RayType>
    auto sample(const RayType& ray) const
    {
        auto o_hit = scene->trace_ray(ray, min);
        if (o_hit.g_hit.p.t < 0)
            return spectral{};


        auto n = o_hit.g_hit.p.normal;
        auto hit_point = ray.origin + o_hit.g_hit.p.t * ray.direction;

        spectral res = {};

        for(auto& l : scene->lights)
        {
            auto light_dir = hit_point - l.position;
            if (light_dir * n > 0)
                continue;
            auto len = light_dir.length();
            light_dir /= len;
            auto h = scene->trace_ray(decltype(ray){l.position, light_dir}, min);
            if (h.g_hit.primitive == o_hit.g_hit.primitive)
                res = res - o_hit.object->material.BRDF(-1.0f * ray.direction, light_dir, n) * l.spectral_intensity * (n * light_dir) / len / len;
        }

        return res;
    }

    const SceneType* scene;
    double min;
};
#endif //RAYTRACER_RAY_SAMPLER_H
