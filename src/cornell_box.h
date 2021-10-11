//
// Created by vvlla on 11.10.2021.
//

#ifndef RAYTRACER_CORNELL_BOX_H
#define RAYTRACER_CORNELL_BOX_H

#include <sstream>
#include "scene/TriangleModel.h"
#include "scene/scene.h"
#include "light/point.h"

const char* fl = "552.8 0.0   0.0   \n"
                    "  0.0 0.0   0.0\n"
                    "  0.0 0.0 559.2\n"
                    "549.6 0.0 559.2";

const char* ceiling = "556.0 548.8 0.0   \n"
                      "556.0 548.8 559.2\n"
                      "  0.0 548.8 559.2\n"
                      "  0.0 548.8   0.0";

const char* back = "549.6   0.0 559.2 \n"
                   "  0.0   0.0 559.2\n"
                   "  0.0 548.8 559.2\n"
                   "556.0 548.8 559.2";

const char* right = "0.0   0.0 559.2   \n"
                    "0.0   0.0   0.0\n"
                    "0.0 548.8   0.0\n"
                    "0.0 548.8 559.2";

const char* left = "552.8   0.0   0.0 \n"
                   "549.6   0.0 559.2\n"
                   "556.0 548.8 559.2\n"
                   "556.0 548.8   0.0";

const char* short_block = "130.0 165.0  65.0 \n"
                          " 82.0 165.0 225.0\n"
                          "240.0 165.0 272.0\n"
                          "290.0 165.0 114.0\n"
                          "\n"
                          "290.0   0.0 114.0\n"
                          "290.0 165.0 114.0\n"
                          "240.0 165.0 272.0\n"
                          "240.0   0.0 272.0\n"
                          "\n"
                          "130.0   0.0  65.0\n"
                          "130.0 165.0  65.0\n"
                          "290.0 165.0 114.0\n"
                          "290.0   0.0 114.0\n"
                          "\n"
                          " 82.0   0.0 225.0\n"
                          " 82.0 165.0 225.0\n"
                          "130.0 165.0  65.0\n"
                          "130.0   0.0  65.0\n"
                          "\n"
                          "240.0   0.0 272.0\n"
                          "240.0 165.0 272.0\n"
                          " 82.0 165.0 225.0\n"
                          " 82.0   0.0 225.0";

const char* tall = "423.0 330.0 247.0\n"
                   "265.0 330.0 296.0\n"
                   "314.0 330.0 456.0\n"
                   "472.0 330.0 406.0\n"
                   "\n"
                   "423.0   0.0 247.0\n"
                   "423.0 330.0 247.0\n"
                   "472.0 330.0 406.0\n"
                   "472.0   0.0 406.0\n"
                   "\n"
                   "472.0   0.0 406.0\n"
                   "472.0 330.0 406.0\n"
                   "314.0 330.0 456.0\n"
                   "314.0   0.0 456.0\n"
                   "\n"
                   "314.0   0.0 456.0\n"
                   "314.0 330.0 456.0\n"
                   "265.0 330.0 296.0\n"
                   "265.0   0.0 296.0\n"
                   "\n"
                   "265.0   0.0 296.0\n"
                   "265.0 330.0 296.0\n"
                   "423.0 330.0 247.0\n"
                   "423.0   0.0 247.0";

auto white() {
    complex c;
    c.d.kd = spectral {{0.736, 0.736, 0.737}};
    //c.d.kd.spectre.normalize();
    c.s.ks = spectral{};
    return c;
}

auto red() {
    complex c;
    c.d.kd = spectral {{0.492, 0.049, 0.051}};
    //c.d.kd.spectre.normalize();
    c.s.ks = spectral{};
    return c;
}

auto green() {
    complex c;
    c.d.kd = spectral {{0.068, 0.398, 0.086}};
    //c.d.kd.spectre.normalize();
    c.s.ks = spectral{};
    return c;
}

Point3f get_point(std::stringstream& ss) {
    Point3f p;
    ss >> p[0] >> p [1] >> p[2];
    return p;
}

TriangleModel<Triangle<float, 3>> load_rectangle(std::stringstream& ss) {
    Point3f p[4];
    for (int i = 0; i < 4; ++i) {
        p[i] = get_point(ss);
    }
    TriangleModel<Triangle<float, 3>> t;
    t.triangles.emplace_back(p[0], p[1], p[2]);
    t.triangles.emplace_back(p[0], p[2], p[3]);
    t.material = white();
    return t;
}

void load_side(Scene<TriangleModel<Triangle<float, 3>>, PointLight>& scene, std::stringstream& ss) {
    scene.objects.push_back(load_rectangle(ss));
}

void load_side(Scene<TriangleModel<Triangle<float, 3>>, PointLight>& scene, const char* str) {

    std::stringstream ss(str);
    load_side(scene, ss);
}

void load_box(Scene<TriangleModel<Triangle<float, 3>>, PointLight>& scene, const char* str) {

    std::stringstream ss(str);
    for (int i = 0; i < 6; ++i) {
        load_side(scene, ss);
    }
}

auto load_scene() {
    Scene<TriangleModel<Triangle<float, 3>>, PointLight> scene;

    load_side(scene, fl);
    load_side(scene, ceiling);
    load_side(scene, left);
    scene.objects.back().material = red();
    load_side(scene, right);
    scene.objects.back().material = green();
    load_side(scene, back);
    load_box(scene, tall);
    load_box(scene, short_block);

    return scene;
}

#endif //RAYTRACER_CORNELL_BOX_H
