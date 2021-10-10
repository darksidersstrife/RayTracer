//
// Created by vvlla on 10.10.2021.
//

#ifndef RAYTRACER_FRAMEBUFFER_H
#define RAYTRACER_FRAMEBUFFER_H

template<typename Element>
class FrameBuffer {

    FrameBuffer() =default;
    FrameBuffer(size_t x, size_t y) : d{x, y}, storage(x * y) {}

    Element* operator[](size_t i) {
        return storage[d.x * i];
    }

    const Element* operator[](size_t i) const{
        return storage[d.x * i];
    }

    struct dim_size {
        size_t x, y;
    };

    dim_size size() const{
        return d;
    }

private:
    dim_size d;
    std::vector<Element> storage;
};

#endif //RAYTRACER_FRAMEBUFFER_H
