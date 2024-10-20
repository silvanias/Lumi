#include "hittable_list.h"

void HittableList::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
    bbox = AABB(bbox, object->boundingBox());
}

HittableList &HittableList::operator+=(const HittableList &other)
{
    for (const auto &obj : other.objects)
    {
        add(obj);
    }
    return *this;
}

HittableList operator+(HittableList lhs, const HittableList &rhs)
{
    lhs += rhs;
    return lhs;
}

bool HittableList::hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    HitRecord tempRec;
    bool hitAnything = false;
    auto closestSoFar = rayT.max;

    for (const auto &object : objects)
    {
        if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}

AABB HittableList::boundingBox() const
{
    return bbox;
}