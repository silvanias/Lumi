#pragma once

#include "geometry/hittable_list.h"
#include "geometry/sphere.h"
#include "material/material.h"
#include "geometry/bvh.h"

HittableList createWorld()
{
    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
    auto material_center = std::make_shared<Lambertian>(glm::vec3(0.76f, 0.13f, 0.89f));
    auto material_left = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f));
    auto material_right = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f));

    world.add(std::make_shared<Sphere>(glm::vec3(0, 0, -1.0f), 0.5f, material_center));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000.5f, -1), 1000.0f, material_ground));
    world.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0, -1.0f), 0.5f, material_left));
    world.add(std::make_shared<Sphere>(glm::vec3(1.0f, 0, -1.0f), 0.5f, material_right));

    // Accelerate
    world = HittableList(std::make_shared<BVHNode>(world));
    return world;
};