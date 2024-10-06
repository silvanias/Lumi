#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

// Utility functions for random number generation and sampling.
namespace Utils
{
    namespace Random
    {
        // Generates a random double between 0.0 and 1.0.
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
        // Converts linear color to gamma-corrected color.
        inline double linearToGamma(double linearColor)
        {
            return (linearColor > 0.0) ? sqrt(linearColor) : 0.0;
        }

        inline glm::vec3 linearToGamma(const glm::vec3 &linearColor)
        {
            return glm::vec3(
                linearToGamma(linearColor.x),
                linearToGamma(linearColor.y),
                linearToGamma(linearColor.z));
        }
    }

    namespace Sampling
    {
        // Samples a point within a unit square (0f z component).
        inline glm::vec3 sampleUnitSquare()
        {
            return glm::vec3(Random::randomDouble() - 0.5f, Random::randomDouble() - 0.5f, 0.0f);
        }

        // Samples a point within a unit sphere.
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

        // Samples a point on the hemisphere oriented by the normal.
        inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
        {
            glm::vec3 onUnitSphere = sampleUnitSphere();
            return (glm::dot(onUnitSphere, normal) > 0.0f) ? onUnitSphere : -onUnitSphere;
        }
    }

    namespace Reflection
    {
        // Reflects a vector against a normal.
        inline glm::vec3 reflect(const glm::vec3 &vec, const glm::vec3 &normal)
        {
            return vec - 2.0f * glm::dot(vec, normal) * normal;
        }
    }
}