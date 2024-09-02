#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace Utils
{
    // Random number generation
    namespace Random
    {
        inline double randomDouble()
        {
            static std::mt19937 generator(std::random_device{}());
            static std::uniform_real_distribution<> distribution(0.0f, 1.0f);
            return distribution(generator);
        }

        inline double randomDouble(double min, double max)
        {
            static std::mt19937 generator(std::random_device{}());
            std::uniform_real_distribution<> distribution(min, max);
            return distribution(generator);
        }

        inline int randomInt(int min, int max)
        {
            return int(randomDouble(min, max + 1));
        }
    }

    // Color conversion utilities
    namespace Color
    {
        inline double linearToGamma(double linear_color)
        {
            return (linear_color > 0.0) ? sqrt(linear_color) : 0.0;
        }

        inline glm::vec3 linearToGamma(const glm::vec3 &linear_color)
        {
            return glm::vec3(
                linearToGamma(linear_color.x),
                linearToGamma(linear_color.y),
                linearToGamma(linear_color.z));
        }
    }

    // Sampling utilities
    namespace Sampling
    {
        inline glm::vec3 sampleUnitSquare()
        {
            return glm::vec3(Random::randomDouble() - 0.5f, Random::randomDouble() - 0.5f, 0.0f);
        }

        inline glm::vec3 sampleUnitSphere()
        {
            while (true)
            {
                glm::vec3 point(Random::randomDouble(-1.0f, 1.0f), Random::randomDouble(-1.0f, 1.0f), Random::randomDouble(-1.0f, 1.0f));
                if (glm::length2(point) < 1.0f)
                {
                    return glm::normalize(point);
                }
            }
        }

        inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
        {
            glm::vec3 on_unit_sphere = sampleUnitSphere();
            return (glm::dot(on_unit_sphere, normal) > 0.0f) ? on_unit_sphere : -on_unit_sphere;
        }
    }

    // Reflection utilities
    namespace Reflection
    {
        inline glm::vec3 reflect(const glm::vec3 &vec, const glm::vec3 &normal)
        {
            return vec - 2.0f * glm::dot(vec, normal) * normal;
        }
    }
}