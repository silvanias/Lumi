#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

// Function to generate a random double between 0 and 1
inline double randomDouble()
{
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_real_distribution<> distribution(0.0f, 1.0f);
    return distribution(generator);
}

// Function to generate a random double between [min, max]
inline double randomDouble(double min, double max)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<> distribution(min, max);
    return distribution(generator);
}

// Function to convert linear color to gamma corrected color
inline double linearToGamma(double linear_color)
{
    return (linear_color > 0.0) ? sqrt(linear_color) : 0.0;
}

// Function to convert a linear color vector to gamma corrected color vector
inline glm::vec3 linearToGamma(const glm::vec3 &linear_color)
{
    return glm::vec3(
        linearToGamma(linear_color.x),
        linearToGamma(linear_color.y),
        linearToGamma(linear_color.z));
}

// Function to sample a point within a unit square
inline glm::vec3 sampleUnitSquare()
{
    return glm::vec3(randomDouble() - 0.5f, randomDouble() - 0.5f, 0.0f);
}

// Function to sample a point within a unit sphere
inline glm::vec3 sampleUnitSphere()
{
    while (true)
    {
        glm::vec3 point(randomDouble(-1.0f, 1.0f), randomDouble(-1.0f, 1.0f), randomDouble(-1.0f, 1.0f));
        if (glm::length2(point) < 1.0f)
        {
            return glm::normalize(point);
        }
    }
}

// Function to get a random point on a hemisphere defined by a normal
inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
{
    glm::vec3 on_unit_sphere = sampleUnitSphere();
    return (glm::dot(on_unit_sphere, normal) > 0.0f) ? on_unit_sphere : -on_unit_sphere;
}

// Function to reflect a vector given a normal
inline glm::vec3 reflect(const glm::vec3 &vec, const glm::vec3 &normal)
{
    return vec - 2.0f * glm::dot(vec, normal) * normal;
}