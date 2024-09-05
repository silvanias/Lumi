#pragma once

#include "utils.h"
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

HittableList createComplexWorld()
{
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = Utils::Random::randomDouble();
            glm::vec3 center(a + 0.9 * Utils::Random::randomDouble(), 0.2, b + 0.9 * Utils::Random::randomDouble());

            if (glm::length(center - glm::vec3(4, 0.2, 0)) > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // Albedo
                    auto albedo = glm::vec3(Utils::Random::randomDouble(), Utils::Random::randomDouble(), Utils::Random::randomDouble());
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // Metal
                    auto albedo = glm::vec3(Utils::Random::randomDouble(0.5, 1), Utils::Random::randomDouble(0.5, 1), Utils::Random::randomDouble(0.5, 1));
                    sphere_material = std::make_shared<Metal>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Lambertian>(glm::vec3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(glm::vec3(-4, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5));
    world.add(make_shared<Sphere>(glm::vec3(4, 1, 0), 1.0, material2));

    // Accelerate
    world = HittableList(std::make_shared<BVHNode>(world));
    return world;
};
