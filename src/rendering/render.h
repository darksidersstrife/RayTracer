//
// Created by vvlla on 30.04.2021.
//

#ifndef RAYTRACER_RENDER_H
#define RAYTRACER_RENDER_H


#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename SceneType>
class Renderer {

public:
    Renderer() {

    }

    template<typename RayType>
    auto sample(const RayType& ray)
    {
        return calculate_color(ray, min, limit);
    }

    template<typename RayType>
    auto sample(const RayType& ray, double min, size_t limit)
    {
        if (limit == 0)
            return spectral{};

        auto o_hit = trace_ray(ray, min);
        if (o_hit.g_hit.p.t < 0)
            return background(ray, min);


        auto n = o_hit.g_hit.p.normal;
        auto hit_point = ray.origin + o_hit.g_hit.p.t * ray.direction;

        spectral res = {};

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
        /*auto t = 0.5f * (ray.direction[0] + 1.0f);
        auto v = (1.0f - t) * Vec3f(1.0f, 1.0f, 1.0f) + t * Vec3f(0.5f, 0.7f, 1.0f);
        spectral s;
        s.spectre[0] = v[0];
        s.spectre[1] = v[1];
        s.spectre[2] = v[2];*/

        return spectral{};
    }

    const SceneType* scene;
    double min;
    size_t limit;
};

template<typename ObjectType, typename LightType>
class RenderThreadPool : public Renderer<ObjectType, LightType>
{
public:
    RenderThreadPool(size_t thread_count)
    {
        for (size_t i = 0; i < thread_count; ++i) {
            threads.emplace_back([this](){
                while (true) {
                    {
                        std::unique_lock ul(m);
                        cv.wait(ul, [pause, end]() { return !(!pause || end); });
                        if (end)
                            return;
                    }
                    this->
                }
            })
        }
    }

    ~RenderThreadPool() {
        {
            std::unique_lock ul(m);
            end = true;
        }
        cv.notify_all();
        for (auto& thread : threads)
            thread.join();
    }

private:
    bool end = false;
    bool pause = true;

    std::mutex m;
    std::condition_variable cv;
    std::vector<std::thread> threads;
};

#endif //RAYTRACER_RENDER_H
