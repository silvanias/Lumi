#include "world.h"

const CamPos CAM_POS_SPHERES{
    glm::vec3(-3.0f, 3.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    1.2f};

const CamPos CAM_POS_LIT{
    glm::vec3(26, 3, 6),
    glm::vec3(0, 2, 0),
    glm::vec3(0.0f, 1.0f, 0.0f),
    0.35f};

const CamPos CAM_POS_QUAD{
    glm::vec3(0.0f, 0.0f, 9.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    1.4f};

const CamPos CAM_POS_CORNELL_BOX{
    glm::vec3(278, 278, -800),
    glm::vec3(278, 278, 0),
    glm::vec3(0, 1, 0),
    0.7f};

HittableList sphereWorld()
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

    return world;
}

HittableList complexSphereWorld()
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
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // Metal
                    auto albedo = glm::vec3(Utils::Random::randomDouble(0.5, 1), Utils::Random::randomDouble(0.5, 1), Utils::Random::randomDouble(0.5, 1));
                    sphere_material = std::make_shared<Metal>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Lambertian>(glm::vec3(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(glm::vec3(-4, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5));
    world.add(std::make_shared<Sphere>(glm::vec3(4, 1, 0), 1.0, material2));

    // Accelerate
    world = HittableList(std::make_shared<BVHNode>(world));
    return world;
}

HittableList litWorld()
{
    HittableList world;

    auto material = std::make_shared<Lambertian>(glm::vec3(0.4, 0.2, 0.1));
    auto metal = std::make_shared<Metal>(glm::vec3(0.7, 0.7, 0.7));
    auto difflight = std::make_shared<DiffuseLight>(glm::vec3(4, 4, 4));
    auto bluelight = std::make_shared<DiffuseLight>(glm::vec3(0.5f, 0.5f, 6));

    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, material));
    world.add(std::make_shared<Sphere>(glm::vec3(0, 2, 0), 2, metal));
    world.add(std::make_shared<Quad>(glm::vec3(3, 1, -2), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), bluelight));
    world.add(std::make_shared<Sphere>(glm::vec3(0, 7, 0), 2, difflight));

    return world;
}

HittableList quadWorld()
{
    HittableList world;

    // Materials
    auto left_red = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.2f, 0.2f));
    auto back_green = std::make_shared<Lambertian>(glm::vec3(0.2f, 1.0f, 0.2f));
    auto right_blue = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.2f, 1.0f));
    auto upper_orange = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.5f, 0.0f));
    auto lower_teal = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.8f, 0.8f));

    // Quads
    world.add(std::make_shared<Quad>(glm::vec3(-3, -2, 5), glm::vec3(0, 0, -4), glm::vec3(0, 4, 0), left_red));
    world.add(std::make_shared<Quad>(glm::vec3(-2, -2, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), back_green));
    world.add(std::make_shared<Quad>(glm::vec3(3, -2, 1), glm::vec3(0, 0, 4), glm::vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<Quad>(glm::vec3(-2, 3, 1), glm::vec3(4, 0, 0), glm::vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<Quad>(glm::vec3(-2, -3, 5), glm::vec3(4, 0, 0), glm::vec3(0, 0, -4), lower_teal));

    return world;
}

HittableList cornellBox()
{
    HittableList world;

    auto red = std::make_shared<Lambertian>(glm::vec3(0.65f, 0.05f, 0.05f));
    auto white = std::make_shared<Lambertian>(glm::vec3(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(glm::vec3(.12, .45, .15));

    world.add(std::make_shared<Quad>(glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(glm::vec3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(glm::vec3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), white));
    world.add(std::make_shared<Quad>(glm::vec3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));

    world = HittableList(std::make_shared<BVHNode>(world));
    return world;
}