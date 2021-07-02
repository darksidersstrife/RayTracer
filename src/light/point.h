//
// Created by vvlla on 08.04.2021.
//

#ifndef RAYTRACER_POINT_LIGHT_H
#define RAYTRACER_POINT_LIGHT_H

#include "../material/spectral.h"
#include "../geometry/point.h"

struct PointLight
{
    spectral spectral_intensity;
    Point3f position;
};

#endif //RAYTRACER_POINT_LIGHT_H
