//
// Created by vvlla on 22.03.2021.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "../geometry/vector.h"
#include "../geometry/point.h"

//template<typename T, size_t size, size_t alignment = alignof(T)>
struct Camera
{
    Vec3f up, direction, left ;
    Point3f position;

    Camera() : direction(0, 0, 1), up(0, 1, 0), left(cross(direction, up)), position(0, 0, 0) {}


    Camera& rotate(float toLeft, float toUp)
    {
        rotateAgainstVector(toLeft, up);
        rotateAgainstVector(toUp, left);
        return *this;
    }
private:
    void rotateAgainstVector(float angle, const Vec3f& vec)
    {
        auto c = cos(angle);
        auto s = sin(angle);
        float r[3][3];

        r[0][0] = c + (1 - c) * vec[0] * vec[0];
        r[0][1] = (1 - c) * vec[0] * vec[1] - s * vec[2];
        r[0][2] = (1 - c) * vec[0] * vec[2] + s * vec[1];
        r[1][0] = (1 - c) * vec[0] * vec[1] + s * vec[2];
        r[1][1] = c + (1 - c) * vec[1] * vec[1];
        r[1][2] = (1 - c) * vec[1] * vec[2] - s * vec[0];
        r[2][0] = (1 - c) * vec[0] * vec[2] - s * vec[1];
        r[2][1] = (1 - c) * vec[1] * vec[2] + s * vec[0];
        r[2][2] = c + (1 - c) * vec[2] * vec[2];

        multiplyVector(up, r);
        multiplyVector(left, r);
        multiplyVector(direction, r);
    }

    void multiplyVector(Vec3f& vec, float r[3][3])
    {
        float x = 0.0, y = 0.0, z = 0.0;

        x = r[0][0] * vec[0] + r[0][1] * vec[1] + r[0][2] * vec[2];
        y = r[1][0] * vec[0] + r[1][1] * vec[1] + r[1][2] * vec[2];
        z = r[2][0] * vec[0] + r[2][1] * vec[1] + r[2][2] * vec[2];

        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }
};

#endif //RAYTRACER_CAMERA_H
