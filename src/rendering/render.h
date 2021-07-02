//
// Created by vvlla on 30.04.2021.
//

#ifndef RAYTRACER_RENDER_H
#define RAYTRACER_RENDER_H


#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class RenderThreadPool
{
public:
    RenderThreadPool(size_t thread_count)
    {
        for (size_t i = 0; i < thread_count; ++i) {
            threads.emplace_back([this](){
                std::unique_lock ul(m);
                cv.wait(ul, )
            })
        }
    }



private:
    std::mutex m;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::vector<std::pa
};

using FrameBufferType = std::vector<uint8_t>;

class Renderer
{
public:
    Renderer() = default;

    FrameBufferType framebuffer;
private:


};

#endif //RAYTRACER_RENDER_H
