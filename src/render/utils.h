#pragma once
#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

inline double randomDouble()
{
    static auto distribution = std::uniform_real_distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max)
{
    static auto distribution = std::uniform_real_distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double linearToGamma(double linear_color)
{
    if (linear_color > 0)
        return sqrt(linear_color);
    return 0;
}

inline glm::vec3 linearToGamma(glm::vec3 linear_color)
{
    return glm::vec3(
        linearToGamma(linear_color.x),
        linearToGamma(linear_color.y),
        linearToGamma(linear_color.z));
}

inline glm::vec3 sampleUnitSquare()
{
    return glm::vec3(randomDouble() - 0.5f, randomDouble() - 0.5f, 0.0f);
}

inline glm::vec3 sampleUnitSphere()
{
    // TODO: Convert to analytical method
    while (true)
    {
        auto point = glm::vec3(randomDouble(-1.0f, 1.0f), randomDouble(-1.0f, 1.0f), randomDouble(-1.0f, 1.0f));
        if (glm::length2(point) < 1)
            return glm::normalize(point);
    }
}

inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
{
    glm::vec3 on_unit_sphere = sampleUnitSphere();
    if (dot(on_unit_sphere, normal) > 0.0f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline glm::vec3 reflect(const glm::vec3 &vec, const glm::vec3 &normal)
{
    return vec - 2 * dot(vec, normal) * normal;
}