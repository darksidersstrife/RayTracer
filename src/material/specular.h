//
// Created by vvlla on 08.04.2021.
//

#ifndef RAYTRACER_SPECULAR_H
#define RAYTRACER_SPECULAR_H

#include <cmath>

struct specular
{
    auto BRDF(const Vec3f& wi, const Vec3f& wo, const Vec3f& normal) const
    {
        return ks * (n + 1) / pi * std::pow(std::max((2 * (wi * normal) * normal - wi) * wo, 0.0f), n);
    }

    spectral ks;
    unsigned int n = 1;
};

#endif //RAYTRACER_SPECULAR_H
