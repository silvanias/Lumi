#pragma once

#include <memory>
#include <vector>
#include "hittable.h"
#include "geometry/bounding/aabb.h"
#include "geometry/interval.h"
#include "geometry/ray.h"

class HittableList : public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() = default;
    explicit HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object);
    bool hit(const Ray &r, Interval ray_t, HitRecord &rec) const override;
    AABB boundingBox() const override;

private:
    AABB bbox;
};