#pragma once
#include <algorithm>

#include "../ray.h"
#include "../interval.h"

class AABB
{
public:
    Interval x;
    Interval y;
    Interval z;

    AABB();
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const glm::vec3 &point1, const glm::vec3 &point2);
    AABB(const AABB &box0, const AABB &box1);

    const Interval &getAxisInterval(int axis) const;
    int longestAxis() const;
    bool hit(const Ray &ray, Interval ray_interval) const;

    AABB operator+(const glm::vec3 &offset);

    static const AABB empty;
    static const AABB universe;

private:
    void padToMinimum();
};