#pragma once
#include <random>
#include <glm/glm.hpp>

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

glm::vec3 sampleUnitSquare()
{
    return glm::vec3(random_double() - 0.5f, random_double() - 0.5f, 0.0f);
}
