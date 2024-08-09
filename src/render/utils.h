#pragma once
#include <random>
#include <glm/glm.hpp>

inline double random_double()
{
    static auto distribution = std::uniform_real_distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    static auto distribution = std::uniform_real_distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double linear_to_gamma(double linear_color)
{
    if (linear_color > 0)
        return sqrt(linear_color);
    return 0;
}

inline glm::vec3 linear_to_gamma(glm::vec3 linear_color)
{
    return glm::vec3(
        linear_to_gamma(linear_color.x),
        linear_to_gamma(linear_color.y),
        linear_to_gamma(linear_color.z));
}

glm::vec3 sampleUnitSquare()
{
    return glm::vec3(random_double() - 0.5f, random_double() - 0.5f, 0.0f);
}

glm::vec3 sampleUnitSphere()
{
    // TODO: Convert to analytical method
    while (true)
    {
        auto p = glm::vec3(random_double(-1.0f, 1.0f), random_double(-1.0f, 1.0f), random_double(-1.0f, 1.0f));
        if (glm::length2(p) < 1)
            return glm::normalize(p);
    }
}

glm::vec3 random_on_hemisphere(const glm::vec3 &normal)
{
    glm::vec3 on_unit_sphere = sampleUnitSphere();
    if (dot(on_unit_sphere, normal) > 0.0f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}