#pragma once
#include <algorithm>

#include "../ray.h"
#include "../interval.h"

// Axis-Aligned Bounding Box class used to define bounding volumes for intersection tests.
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

    // Returns the index of the longest axis in the bounding box (0 = x, 1 = y, 2 = z).
    int longestAxis() const;

    bool hit(const Ray &ray, Interval rayInterval) const;

    // Returns a new AABB offset by a given vector.
    AABB operator+(const glm::vec3 &offset);

    static const AABB empty;
    static const AABB universe;

private:
    // Pads the AABB to a minimum size.
    void padToMinimum();
};