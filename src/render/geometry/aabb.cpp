#include "aabb.h"

const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

AABB::AABB() = default;

AABB::AABB(const Interval &x, const Interval &y, const Interval &z)
    : x(x), y(y), z(z)
{
    padToMinimum();
}

AABB::AABB(const glm::vec3 &point1, const glm::vec3 &point2)
{
    x = (point1[0] <= point2[0]) ? Interval(point1[0], point2[0]) : Interval(point2[0], point1[0]);
    y = (point1[1] <= point2[1]) ? Interval(point1[1], point2[1]) : Interval(point2[1], point1[1]);
    z = (point1[2] <= point2[2]) ? Interval(point1[2], point2[2]) : Interval(point2[2], point1[2]);
    padToMinimum();
}

AABB::AABB(const AABB &box0, const AABB &box1)
{
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}

const Interval &AABB::getAxisInterval(int axis) const
{
    switch (axis)
    {
    case 1:
        return y;
    case 2:
        return z;
    default:
        return x;
    }
}

int AABB::longestAxis() const
{
    if (x.size() > y.size())
        return x.size() > z.size() ? 0 : 2;
    else
        return y.size() > z.size() ? 1 : 2;
}

bool AABB::hit(const Ray &ray, Interval ray_interval) const
{
    const glm::vec3 &ray_origin = ray.origin();
    const glm::vec3 &ray_direction = ray.direction();

    for (int axis = 0; axis < 3; axis++)
    {
        const Interval &axis_interval = getAxisInterval(axis);
        const double inverse_direction = 1.0f / ray_direction[axis];

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

void AABB::padToMinimum()
{
    float min = 0.0001f;
    if (x.size() < min)
        x = x.expand(min);
    if (y.size() < min)
        y = y.expand(min);
    if (z.size() < min)
        z = z.expand(min);
}