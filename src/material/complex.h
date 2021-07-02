//
// Created by vvlla on 08.04.2021.
//

#ifndef RAYTRACER_COMPLEX_H
#define RAYTRACER_COMPLEX_H

#include "diffuse.h"
#include "specular.h"

struct complex
{
    auto BRDF(const Vec3f& wi, const Vec3f& wo, const Vec3f& normal) const
    {
        return s.BRDF(wi, wo, normal) + d.BRDF(wi, wo, normal);
    }

    diffuse d;
    specular s;
};

#endif //RAYTRACER_COMPLEX_H
