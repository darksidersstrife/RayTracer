//
// Created by vvlla on 08.04.2021.
//

#ifndef RAYTRACER_SPECTRAL_H
#define RAYTRACER_SPECTRAL_H

#include <map>
#include "../geometry/vector.h"

struct spectral
{
    decltype(auto) operator[](size_t i) const
    {
        return spectre[i];
    }

    auto operator+(const spectral& other) const
    {
        auto res = *this;

        for(size_t i = 0; i < 3; ++i)
            res[i] += other[i];
        return res;
    }

    auto operator-(const spectral& other) const
    {
        auto res = *this;

        for(size_t i = 0; i < 3; ++i)
            res[i] -= other[i];
        return res;
    }

    auto operator*(const spectral& other) const
    {
        spectral res = *this;

        for(size_t i = 0; i < 3; ++i)
            res[i] *=  other[i];
        return res;
    }

    auto operator*(double v) const
    {
        auto res = *this;

        for(size_t i = 0; i < 3; ++i)
            res[i] *= v;
        return res;
    }

    auto operator/(double v) const
    {
        auto res = *this;

        for(size_t i = 0; i < 3; ++i)
            res[i] /= v;
        return res;
    }
    
    mutable Vec3f spectre;
};

#endif //RAYTRACER_SPECTRAL_H
