//
// Created by vvlla on 30.04.2021.
//

#ifndef RAYTRACER_RENDER_POOL_H
#define RAYTRACER_RENDER_POOL_H


#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "framebuffer.h"
#include "../material/spectral.h"
#include "ray_sampler.h"
#include "ray_generator.h"

template<typename SceneType>
class RenderThreadPool
{
public:
    RenderThreadPool(size_t thread_count, size_t x, size_t y, const RaySampler<SceneType>* raySampler, const RayGenerator* rayGenerator) :
    raySampler(raySampler), rayGenerator(rayGenerator), image(x, y) {
        for (size_t i = 0; i < thread_count; ++i) {
            threads.emplace_back([this, thread_counter = size_t(0), i]() mutable{
                while (true) {
                    {
                        std::unique_lock ul(m);
                        if (thread_counter >= task_counter) {
                            ++ready_counter;
                            if (ready_counter == threads.size())
                                producer_cv.notify_one();
                        }
                        cv.wait(ul, [this, thread_counter]() { return !pause && thread_counter < task_counter || end; });
                        if (end)
                            return;
                    }

                    size_t y_begin = image.size().y / threads.size() * i;
                    size_t y_end = std::min(y_begin + image.size().y / threads.size(), image.size().y);
                    for (size_t y = y_begin; y < y_end; y++)
                    {
                        for (size_t x = 0; x < image.size().x; x++)
                        {
                            image[y][x] = this->raySampler->sample(this->rayGenerator->get_ray(x, y));
                        }
                    }

                    ++thread_counter;

                }
            });
        }
    }

    void start(size_t num_tasks = 1) {
        std::unique_lock ul(m);
        pause = false;
        task_counter += num_tasks;
        ready_counter = 0;
        cv.notify_all();
    }

    void wait_completion() {
        std::unique_lock ul(m);
        producer_cv.wait(ul, [this](){return ready_counter == threads.size();});
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

    FrameBuffer<spectral> image;
private:
    size_t task_counter = 0;
    size_t ready_counter = 0;
    bool end = false;
    bool pause = true;

    std::mutex m;
    std::condition_variable cv;
    std::condition_variable producer_cv;

    std::vector<std::thread> threads;
    const RaySampler<SceneType>* raySampler;
    const RayGenerator* rayGenerator;
};

#endif //RAYTRACER_RENDER_POOL_H
