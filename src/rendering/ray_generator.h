//
// Created by vvlla on 10.10.2021.
//

#ifndef RAYTRACER_RAY_GENERATOR_H
#define RAYTRACER_RAY_GENERATOR_H

#include "camera.h"
#include "../geometry/ray.h"

class RayGenerator {
public:
    RayGenerator(const Camera& camera, size_t x, size_t y, float aspect_ratio, float field_of_view_tan) {
        x_step = -(1.0f / x) * aspect_ratio * field_of_view_tan * camera.left;
        y_step = -(1.0f / y) * field_of_view_tan * camera.up;

        left_up_corner = camera.direction +
                               0.5f * aspect_ratio * field_of_view_tan * camera.left +
                               0.5f * field_of_view_tan * camera.up;

        position = camera.position;
    }

    auto get_ray(size_t x, size_t y) const {
        return  Ray3f<>(position, (left_up_corner + float(x) * x_step + float(y) * y_step).get_normalized());
    }

private:
    Vec3f x_step, y_step, left_up_corner;
    Point3f position;
};

#endif //RAYTRACER_RAY_GENERATOR_H
