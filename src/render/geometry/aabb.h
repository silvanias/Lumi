#pragma once
#include <algorithm>

#include "ray.h"
#include "interval.h"

class AABB
{
public:
    Interval x_interval;
    Interval y_interval;
    Interval z_interval;

    AABB() {}

    AABB(const Interval &x, const Interval &y, const Interval &z)
        : x_interval(x), y_interval(y), z_interval(z) {}

    AABB(const glm::vec3 &point1, const glm::vec3 &point2)
    {
        x_interval = (point1[0] <= point2[0]) ? Interval(point1[0], point2[0]) : Interval(point2[0], point1[0]);
        y_interval = (point1[1] <= point2[1]) ? Interval(point1[1], point2[1]) : Interval(point2[1], point1[1]);
        z_interval = (point1[2] <= point2[2]) ? Interval(point1[2], point2[2]) : Interval(point2[2], point1[2]);
    }

    const Interval &getAxisInterval(int axis) const
    {
        switch (axis)
        {
        case 1:
            return y_interval;
        case 2:
            return z_interval;
        default:
            return x_interval;
        }
    }

    bool hit(const Ray &ray, Interval ray_interval) const
    {
        const glm::vec3 &ray_origin = ray.origin();
        const glm::vec3 &ray_direction = ray.direction();

        for (int axis = 0; axis < 3; axis++)
        {
            const Interval &axis_interval = getAxisInterval(axis);
            const double inverse_direction = 1.0 / ray_direction[axis];

            auto t0 = (axis_interval.min - ray_origin[axis]) * inverse_direction;
            auto t1 = (axis_interval.max - ray_origin[axis]) * inverse_direction;

            if (t0 > t1)
                std::swap(t0, t1);

            ray_interval.min = std::max(ray_interval.min, t0);
            ray_interval.max = std::min(ray_interval.max, t1);

            if (ray_interval.max <= ray_interval.min)
                return false;
        }
        return true;
    }
};