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

bool AABB::hit(const Ray &ray, Interval rayInterval) const
{
    const glm::vec3 &rayOrigin = ray.origin();
    const glm::vec3 &rayDirection = ray.direction();

    for (int axis = 0; axis < 3; axis++)
    {
        const Interval &axisInterval = getAxisInterval(axis);
        const double inverseDirection = 1.0f / rayDirection[axis];

        auto t0 = (axisInterval.min - rayOrigin[axis]) * inverseDirection;
        auto t1 = (axisInterval.max - rayOrigin[axis]) * inverseDirection;

        if (t0 > t1)
            std::swap(t0, t1);

        rayInterval.min = std::max(rayInterval.min, t0);
        rayInterval.max = std::min(rayInterval.max, t1);

        if (rayInterval.max <= rayInterval.min)
            return false;
    }
    return true;
}

AABB AABB::operator+(const glm::vec3 &offset)
{
    return AABB(x + offset.x, y + offset.y, z + offset.z);
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