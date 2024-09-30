#include "hittable_list.h"

void HittableList::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
    bbox = AABB(bbox, object->boundingBox());
}

bool HittableList::hit(const Ray &r, Interval ray_t, HitRecord &rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto &object : objects)
    {
        if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

AABB HittableList::boundingBox() const
{
    return bbox;
}