//
// Created by vvlla on 08.04.2021.
//

#ifndef RAYTRACER_DIFFUSE_H
#define RAYTRACER_DIFFUSE_H

#include "spectral.h"
#include "../geometry/vector.h"

struct diffuse
{
    auto BRDF(const Vec3f& wi, const Vec3f& wo, const Vec3f& normal) const
    {
        return kd / pi;
    }

    spectral kd;
};

#endif //RAYTRACER_DIFFUSE_H
