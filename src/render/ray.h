#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
    Ray() = default;
    Ray(const glm::vec3 &origin, const glm::vec3 &direction) : orig(origin), dir(direction) {}

    const glm::vec3 &origin() const { return orig; }
    const glm::vec3 &direction() const { return dir; }

    glm::vec3 at(float t) const
    {
        return orig + (dir * t);
    }

private:
    glm::vec3 orig;
    glm::vec3 dir;
};